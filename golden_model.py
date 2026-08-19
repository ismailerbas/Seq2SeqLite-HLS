#!/usr/bin/env python3
"""
golden_model.py -- Float64 numpy reference implementation of the Seq2SeqLite
student QGRU encoder-decoder, using the EXACT int8 weight codes parsed
directly from src/student_weights_int8.h, run on the same raw_pixel_k.csv
inputs the HLS testbench uses, and compared against BOTH the real model's
output (expected_out_k.csv) and the HLS csim output (hls_out_pixel_k.csv).

Modes:
  (default)       run the verified semantics, report error vs expected and
                  vs the HLS dumps.
  --sweep         try all activation/state-quantization variants, rank by
                  error vs expected_out.
  --sweep-preproc keep the verified semantics fixed and vary the input
                  preprocessing (baseline bins 0..20), rank by error vs
                  expected_out -- tests whether generate_expected_out_from_
                  model.py normalized its inputs differently than the
                  testbench does.

Verified default semantics (QKeras 0.9.0 source + Seq2SeqLite.py):
  state quantizer : quantized_bits(8,0,symmetric=1,alpha=1.0)
                    -> clip(round_half_even(h*128), -127, 127)/128,
                    applied ON ENTRY; matmuls AND blend use qh.
  gates           : hard_sigmoid, clip(0.5*x + 0.5, 0, 1), float output.
  candidate       : quantized_tanh(8, symmetric=True), "hard" sigmoid
                    -> clip(round_half_even(clip(x,-1,1)*128), -127, 127)/128.
  reset product   : r * qh, float, unquantized.
  carried state   : float blend h = z*qh + (1-z)*hh; QDense reads this blend.
  decoder         : initial_state = encoder final float blend, inputs all 0.
  dense           : linear, y = h @ (Wd/128) + bd/128, float.

Requires: numpy.
"""

import argparse
import itertools
import os
import re
import sys

import numpy as np

SEQ_LEN = 135
N_OUT = 3
GRU_UNITS = 32
BASELINE_BINS = 10


def parse_weight_header(path):
    text = open(path).read()
    pattern = re.compile(
        r"static\s+const\s+int8_t\s+(\w+)\s*((?:\[\d+\])+)\s*=\s*\{(.*?)\};",
        re.DOTALL)
    arrays = {}
    for name, dims_txt, body in pattern.findall(text):
        dims = [int(d) for d in re.findall(r"\[(\d+)\]", dims_txt)]
        values = [int(v) for v in re.findall(r"-?\d+", body)]
        expected = 1
        for d in dims:
            expected *= d
        if len(values) != expected:
            print(f"ERROR: {name}: parsed {len(values)} values, "
                  f"expected {expected} for dims {dims}")
            sys.exit(1)
        arrays[name] = np.array(values, dtype=np.float64).reshape(dims)
    required = [
        "sencgru_kernel_z", "sencgru_kernel_r", "sencgru_kernel_h",
        "sencgru_recurrent_kernel_z", "sencgru_recurrent_kernel_r",
        "sencgru_recurrent_kernel_h",
        "sencgru_bias_z", "sencgru_bias_r", "sencgru_bias_h",
        "sdecgru_kernel_z", "sdecgru_kernel_r", "sdecgru_kernel_h",
        "sdecgru_recurrent_kernel_z", "sdecgru_recurrent_kernel_r",
        "sdecgru_recurrent_kernel_h",
        "sdecgru_bias_z", "sdecgru_bias_r", "sdecgru_bias_h",
        "sdec_dense_kernel", "sdec_dense_bias",
    ]
    missing = [n for n in required if n not in arrays]
    if missing:
        print(f"ERROR: arrays missing from {path}: {missing}")
        sys.exit(1)
    return {n: arrays[n] / 128.0 for n in required}


def preprocess_pixel(raw, baseline_bins):
    n = min(baseline_bins, len(raw))
    baseline = raw[:n].mean() if n > 0 else 0.0
    corrected = np.clip(raw - baseline, 0.0, None)
    max_val = corrected.max()
    return corrected / max_val if max_val > 0 else corrected


def q128_symmetric(x):
    """quantized_bits(8,0,symmetric=1,alpha=1.0): round half-to-even onto the
    1/128 grid, clip codes to [-127, +127]. np.round is half-to-even, matching
    tf.round exactly."""
    return np.clip(np.round(x * 128.0), -127.0, 127.0) / 128.0


def gate_fn(x, mode):
    if mode == "hard05":
        return np.clip(0.5 * x + 0.5, 0.0, 1.0)
    if mode == "hard02":
        return np.clip(0.2 * x + 0.5, 0.0, 1.0)
    if mode == "sigmoid":
        return 1.0 / (1.0 + np.exp(-x))
    raise ValueError(mode)


def act_fn(x, mode):
    if mode == "qtanh_hard":
        return q128_symmetric(np.clip(x, -1.0, 1.0))
    if mode == "qtanh_smooth":
        return q128_symmetric(np.clip(0.375 * x, -1.0, 1.0))
    if mode == "qtanh_real":
        return q128_symmetric(np.tanh(x))
    if mode == "tanh":
        return np.tanh(x)
    raise ValueError(mode)


def run_model(w, x_seq, gate_mode, act_mode, state_quant):
    def cell(prefix, x, h):
        qh = q128_symmetric(h) if state_quant else h
        z = gate_fn(x * w[prefix + "_kernel_z"][0] +
                    qh @ w[prefix + "_recurrent_kernel_z"] +
                    w[prefix + "_bias_z"], gate_mode)
        r = gate_fn(x * w[prefix + "_kernel_r"][0] +
                    qh @ w[prefix + "_recurrent_kernel_r"] +
                    w[prefix + "_bias_r"], gate_mode)
        hh = act_fn(x * w[prefix + "_kernel_h"][0] +
                    (r * qh) @ w[prefix + "_recurrent_kernel_h"] +
                    w[prefix + "_bias_h"], act_mode)
        return z * qh + (1.0 - z) * hh

    h = np.zeros(GRU_UNITS, dtype=np.float64)
    for t in range(SEQ_LEN):
        h = cell("sencgru", x_seq[t], h)

    out = np.zeros((SEQ_LEN, N_OUT), dtype=np.float64)
    for t in range(SEQ_LEN):
        h = cell("sdecgru", 0.0, h)
        out[t] = h @ w["sdec_dense_kernel"] + w["sdec_dense_bias"]
    return out


def load_matrix_csv(path, expect_cols):
    rows = []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            rows.append([float(c) for c in line.split(",")])
    arr = np.array(rows, dtype=np.float64)
    if arr.shape != (SEQ_LEN, expect_cols):
        print(f"ERROR: {path} has shape {arr.shape}, expected "
              f"({SEQ_LEN}, {expect_cols})")
        sys.exit(1)
    return arr


def load_hls_dump(path):
    if not os.path.isfile(path):
        return None
    rows = []
    with open(path) as f:
        f.readline()
        for line in f:
            line = line.strip()
            if not line:
                continue
            cells = [float(c) for c in line.split(",")]
            rows.append(cells[1:4])
    arr = np.array(rows, dtype=np.float64)
    if arr.shape != (SEQ_LEN, N_OUT):
        print(f"WARNING: {path} has shape {arr.shape}, expected "
              f"({SEQ_LEN}, {N_OUT}); ignoring it")
        return None
    return arr


def load_pixels(args):
    pixels = []
    for k in range(1, args.num_pixels + 1):
        raw_path = os.path.join(args.data_dir, f"raw_pixel_{k}.csv")
        exp_path = os.path.join(args.data_dir, f"expected_out_{k}.csv")
        if not os.path.isfile(raw_path) or not os.path.isfile(exp_path):
            print(f"ERROR: missing {raw_path} or {exp_path}")
            sys.exit(1)
        raw = np.loadtxt(raw_path, delimiter=",", dtype=np.float64)
        raw = np.atleast_1d(raw).reshape(-1)
        if raw.size != SEQ_LEN:
            print(f"ERROR: {raw_path} has {raw.size} values, expected {SEQ_LEN}")
            sys.exit(1)
        expected = load_matrix_csv(exp_path, N_OUT)
        hls = load_hls_dump(os.path.join(args.data_dir, f"hls_out_pixel_{k}.csv"))
        pixels.append((k, raw, expected, hls))
    return pixels


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--weights", default="src/student_weights_int8.h")
    ap.add_argument("--h5", default=None,
                    help="load weights from this Keras checkpoint instead of "
                         "the header -- used to identify which checkpoint "
                         "generated expected_out")
    ap.add_argument("--data-dir", default=".",
                    help="directory containing raw_pixel_k.csv, "
                         "expected_out_k.csv, and optionally "
                         "hls_out_pixel_k.csv")
    ap.add_argument("--num-pixels", type=int, default=5)
    ap.add_argument("--baseline-bins", type=int, default=BASELINE_BINS)
    ap.add_argument("--sweep", action="store_true",
                    help="try all semantic variants and rank them by error "
                         "vs expected_out")
    ap.add_argument("--sweep-preproc", action="store_true",
                    help="keep verified semantics, vary baseline bins 0..20, "
                         "rank by error vs expected_out")
    args = ap.parse_args()

    if args.h5 is not None:
        import check_weights_vs_h5 as cw
        import h5py
        with h5py.File(args.h5, "r") as f:
            layers = cw.find_layers(f)
        w = {}
        u = GRU_UNITS
        for prefix in ("sencgru", "sdecgru"):
            kernel, recurrent, bias = layers[prefix]
            k = cw.fake_quant_codes(kernel).astype(np.float64) / 128.0
            r = cw.fake_quant_codes(recurrent).astype(np.float64) / 128.0
            b = cw.fake_quant_codes(bias).astype(np.float64) / 128.0
            w[prefix + "_kernel_z"] = k[:, :u]
            w[prefix + "_kernel_r"] = k[:, u:2 * u]
            w[prefix + "_kernel_h"] = k[:, 2 * u:]
            w[prefix + "_recurrent_kernel_z"] = r[:, :u]
            w[prefix + "_recurrent_kernel_r"] = r[:, u:2 * u]
            w[prefix + "_recurrent_kernel_h"] = r[:, 2 * u:]
            w[prefix + "_bias_z"] = b[:u]
            w[prefix + "_bias_r"] = b[u:2 * u]
            w[prefix + "_bias_h"] = b[2 * u:]
        dk, db = layers["dense"]
        w["sdec_dense_kernel"] = cw.fake_quant_codes(dk).astype(np.float64) / 128.0
        w["sdec_dense_bias"] = cw.fake_quant_codes(db).astype(np.float64) / 128.0
        print(f"Weights loaded from checkpoint: {args.h5}")
    else:
        w = parse_weight_header(args.weights)
    pixels = load_pixels(args)

    if args.sweep:
        combos = list(itertools.product(
            ["hard05", "hard02", "sigmoid"],
            ["qtanh_hard", "qtanh_smooth", "qtanh_real", "tanh"],
            [True, False]))
        results = []
        for gate_mode, act_mode, state_quant in combos:
            worst = 0.0
            for k, raw, expected, hls in pixels:
                x_seq = preprocess_pixel(raw, args.baseline_bins)
                out = run_model(w, x_seq, gate_mode, act_mode, state_quant)
                err = np.abs(out - expected).max()
                worst = max(worst, err)
            results.append((worst, gate_mode, act_mode, state_quant))
        results.sort()
        print("=== SWEEP: worst-pixel max abs error vs expected_out, best first ===")
        print(f"{'max_err':>10}  {'gate':>8}  {'activation':>12}  {'state_quant':>11}")
        for worst, gate_mode, act_mode, state_quant in results:
            print(f"{worst:10.6f}  {gate_mode:>8}  {act_mode:>12}  {str(state_quant):>11}")
        best = results[0]
        print(f"BEST: gate={best[1]} activation={best[2]} state_quant={best[3]} "
              f"max_err={best[0]:.6f}")
        return

    if args.sweep_preproc:
        results = []
        for b in range(0, 21):
            worst = 0.0
            per_pixel = []
            for k, raw, expected, hls in pixels:
                x_seq = preprocess_pixel(raw, b)
                out = run_model(w, x_seq, "hard05", "qtanh_hard", True)
                err = np.abs(out - expected).max()
                per_pixel.append(err)
                worst = max(worst, err)
            results.append((worst, b, per_pixel))
        results.sort()
        print("=== PREPROC SWEEP: verified semantics fixed, baseline bins varied ===")
        print(f"{'max_err':>10}  {'baseline_bins':>13}  per-pixel errors")
        for worst, b, per_pixel in results:
            pp = "  ".join(f"{e:.6f}" for e in per_pixel)
            print(f"{worst:10.6f}  {b:13d}  {pp}")
        best = results[0]
        print(f"BEST: baseline_bins={best[1]} max_err={best[0]:.6f}")
        print("If no row drops near zero, preprocessing is NOT the mismatch -- "
              "run check_weights_vs_h5.py to compare the extracted int8 codes "
              "against the checkpoint that generated expected_out.")
        return

    print("=== DEFAULT verified semantics: gate=hard05, activation=qtanh_hard, "
          "state_quant=True ===")
    overall_exp = 0.0
    overall_hls = 0.0
    for k, raw, expected, hls in pixels:
        x_seq = preprocess_pixel(raw, args.baseline_bins)
        out = run_model(w, x_seq, "hard05", "qtanh_hard", True)
        err_exp = np.abs(out - expected).max()
        overall_exp = max(overall_exp, err_exp)
        line = f"[pixel_{k}] golden vs expected: max_abs_err={err_exp:.6f}"
        if hls is not None:
            err_hls = np.abs(out - hls).max()
            overall_hls = max(overall_hls, err_hls)
            line += f"   golden vs HLS: max_abs_err={err_hls:.6f}"
        print(line)
    print(f"OVERALL golden vs expected: {overall_exp:.6f}")
    if overall_hls > 0.0:
        print(f"OVERALL golden vs HLS:      {overall_hls:.6f}")


if __name__ == "__main__":
    main()