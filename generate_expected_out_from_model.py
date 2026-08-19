#!/usr/bin/env python3
"""
generate_expected_out_from_model.py -- Regenerate expected_out_k.csv from
the ACTUAL trained QKeras model (real TensorFlow + real QKeras) using the
CANONICAL preprocessing pipeline (baseline subtract over the first 10 bins,
clip negatives, per-pixel max normalize) -- the same pipeline used by
eval_experimental.py, the HLS testbench (student_tb.cpp), and
prepare_frame_lanes.py. This makes the verification reference exercise the
exact input pipeline the FPGA deployment uses.

SERVER VERSION -- run on the GPFS server in the tf210_qkeras conda
environment, where TensorFlow + QKeras are installed. Single-pixel
inference; completes in seconds.

For each pixel k:
  1. loads raw_pixel_k.csv
  2. preprocesses with BASELINE_BINS = 10
  3. runs the real model (encoder input = normalized trace, decoder input
     = zeros, matching training/inference convention)
  4. reports the max abs diff between the NEW prediction and the EXISTING
     expected_out_k.csv (this diff is the diagnostic verdict: ~0.05-0.17
     means the old reference was generated WITHOUT baseline subtraction
     and is being correctly replaced; ~0.000x means the old reference was
     already bins=10 and regeneration changes nothing)
  5. renames the existing expected_out_k.csv to expected_out_k.backup.csv
     (only if no backup exists yet -- never overwrites a backup)
  6. writes the new expected_out_k.csv with full float precision

Requires: tensorflow (2.10.x), qkeras, numpy.
"""

import numpy as np
import os
import sys
import tensorflow as tf
import qkeras
from qkeras import QDense, QGRU, quantized_bits, quantized_tanh

# ============================================================================
# SERVER PATHS -- same layout as predict_5_pixels_real_model_server.py
# ============================================================================
EXP_DIR = (
    "/gpfs/u/home/HBNN/HBNNrbss/scratch/nmi/exptests/"
    "vanilla_kd_T4.0_a0.6_b8k8r8a8_gru32x1_dense3_effbs1024_microbs1024_lr1e-04bu"
)
WEIGHTS_H5_PATH = os.path.join(EXP_DIR, "student_final.weights.h5")
CSV_DIR = os.path.join(EXP_DIR, "vitis")
NUM_PIXELS = 5
# ============================================================================

UNITS = 32
BITS = 8
BASELINE_BINS = 10
SEQ_LEN = 135


def qwk(): return quantized_bits(BITS, 0, 1, alpha=1.0)
def qwr(): return quantized_bits(BITS, 0, 1, alpha=1.0)
def qwb(): return quantized_bits(BITS, 0, 1, alpha=1.0)
def qa(): return quantized_tanh(bits=BITS, symmetric=True)
def qs(): return quantized_bits(BITS, 0, 1, alpha=1.0)
def qd(): return quantized_bits(BITS, 0)


def build_model():
    enc_in = tf.keras.Input(shape=(None, 1), name="senc_input")
    dec_in = tf.keras.Input(shape=(None, 1), name="sdec_input")
    senc_out, senc_state = QGRU(
        units=UNITS, activation=qa(), kernel_quantizer=qwk(),
        recurrent_quantizer=qwr(), bias_quantizer=qwb(), state_quantizer=qs(),
        return_state=True, name="sencgru",
    )(enc_in)
    sdec_hid, _ = QGRU(
        units=UNITS, activation=qa(), kernel_quantizer=qwk(),
        recurrent_quantizer=qwr(), bias_quantizer=qwb(), state_quantizer=qs(),
        return_sequences=True, return_state=True, name="sdecgru",
    )(dec_in, initial_state=senc_state)
    out = QDense(
        3, kernel_quantizer=qd(), bias_quantizer=qd(), activation="linear",
        name="sdec_dense",
    )(sdec_hid)
    return tf.keras.Model(inputs=[enc_in, dec_in], outputs=out)


def preprocess_pixel(raw):
    baseline = raw[:BASELINE_BINS].mean()
    corrected = np.clip(raw - baseline, 0.0, None)
    max_val = corrected.max()
    norm = corrected / max_val if max_val > 0 else corrected
    return norm.astype(np.float32), baseline, max_val


def main():
    print(f"tensorflow version: {tf.__version__}")
    print(f"qkeras version:     {getattr(qkeras, '__version__', 'unknown')}")
    print(f"numpy version:      {np.__version__}")

    if not os.path.isfile(WEIGHTS_H5_PATH):
        print(f"ERROR: WEIGHTS_H5_PATH not found: {WEIGHTS_H5_PATH}")
        sys.exit(1)
    if not os.path.isdir(CSV_DIR):
        print(f"ERROR: CSV_DIR not found: {CSV_DIR}")
        sys.exit(1)

    print("Building model architecture...")
    model = build_model()
    print(f"Loading weights from {WEIGHTS_H5_PATH}")
    model.load_weights(WEIGHTS_H5_PATH)
    print("Weights loaded OK.")

    max_old_vs_new = 0.0
    for idx in range(1, NUM_PIXELS + 1):
        raw_path = os.path.join(CSV_DIR, f"raw_pixel_{idx}.csv")
        out_path = os.path.join(CSV_DIR, f"expected_out_{idx}.csv")
        backup_path = os.path.join(CSV_DIR, f"expected_out_{idx}.backup.csv")

        if not os.path.isfile(raw_path):
            print(f"ERROR: {raw_path} not found")
            sys.exit(1)

        raw = np.loadtxt(raw_path, delimiter=",")
        raw = np.atleast_1d(raw).reshape(-1)
        if raw.shape[0] != SEQ_LEN:
            print(f"ERROR: {raw_path} has {raw.shape[0]} values, expected {SEQ_LEN}")
            sys.exit(1)

        norm, baseline, max_val = preprocess_pixel(raw)
        enc_input = norm.reshape(1, -1, 1)
        dec_input = np.zeros_like(enc_input)

        pred = model({"senc_input": enc_input, "sdec_input": dec_input},
                     training=False)
        pred = pred.numpy()[0]
        if pred.shape != (SEQ_LEN, 3):
            print(f"ERROR: prediction shape {pred.shape}, expected ({SEQ_LEN}, 3)")
            sys.exit(1)

        print(f"\n[pixel_{idx}] baseline={baseline:.6f}  max_val={max_val:.6f}")

        if os.path.isfile(out_path):
            old = np.loadtxt(out_path, delimiter=",")
            if old.shape == (SEQ_LEN, 3):
                diff = float(np.abs(pred - old).max())
                max_old_vs_new = max(max_old_vs_new, diff)
                print(f"[pixel_{idx}] old expected_out vs NEW bins=10 model "
                      f"output: max_abs_diff={diff:.6f}")
            else:
                print(f"[pixel_{idx}] old {out_path} has shape {old.shape}; "
                      "skipping diff")
            if not os.path.isfile(backup_path):
                os.rename(out_path, backup_path)
                print(f"[pixel_{idx}] backed up old reference to {backup_path}")
            else:
                os.remove(out_path)
                print(f"[pixel_{idx}] backup already exists at {backup_path}; "
                      "old file removed")

        np.savetxt(out_path, pred, delimiter=",", fmt="%.10f")
        print(f"[pixel_{idx}] wrote NEW reference: {out_path}")

    print("\n========================================")
    print(f"Max old-vs-new diff across all pixels: {max_old_vs_new:.6f}")
    if max_old_vs_new > 0.02:
        print("VERDICT: the old expected_out files were NOT generated with the "
              "canonical bins=10 pipeline -- they have been replaced. Copy the "
              "new expected_out_*.csv files back to the Vitis csim data "
              "directory and rerun csim.")
    else:
        print("VERDICT: the old expected_out files already matched the bins=10 "
              "pipeline (regeneration changed nothing material). Do NOT expect "
              "csim to improve -- paste this output back for the semantic "
              "investigation, including the qkeras version printed above.")


if __name__ == "__main__":
    main()