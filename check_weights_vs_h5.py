#!/usr/bin/env python3
"""
check_weights_vs_h5.py -- Verify that src/student_weights_int8.h was
extracted from the SAME checkpoint that generate_expected_out_from_model.py
used to produce expected_out_k.csv.

Loads the Keras .h5 / .weights.h5 file with h5py (no TensorFlow needed),
locates the encoder GRU, decoder GRU, and dense layer, applies the exact
QKeras fake-quantization the model applies at inference
(quantized_bits(8,0,symmetric=1,alpha=1.0):
codes = clip(round_half_even(w*128), -127, 127)), splits the fused GRU
tensors into the z|r|h gate columns exactly as QGRUCell.call() slices them
(kernel[:, :32]=z, [:, 32:64]=r, [:, 64:96]=h), and diffs every code against
the header.

LAYER MAPPING: by layer NAME in the dataset path -- paths containing
'sencgru' are the encoder, 'sdecgru' the decoder, 'dense' the output head.
This matches the explicit layer names in this project's checkpoints. If a
checkpoint has no such names, the fallback uses Keras's cell naming order:
'qgru_cell' (no suffix) = first built = encoder, 'qgru_cell_1' = decoder.
(The previous version of this script sorted dataset paths alphabetically,
which swapped encoder and decoder for this project because 'sdecgru' sorts
before 'sencgru' -- producing mirrored false mismatches on every GRU tensor
while the dense layer matched.)

Verdicts:
  all zero diffs            -> header matches the checkpoint.
  diffs of magnitude 1      -> extraction rounding tie differences;
                               regenerate the header to be safe.
  large or widespread diffs -> the header came from a DIFFERENT checkpoint.

Usage:
  python check_weights_vs_h5.py --h5 <path_to_checkpoint.h5>

Requires: numpy, h5py.
"""

import argparse
import re
import sys

import numpy as np

try:
    import h5py
except ImportError:
    print("ERROR: h5py is required. Install with: pip install h5py")
    sys.exit(1)

GRU_UNITS = 32


def parse_weight_header_codes(path):
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
        arrays[name] = np.array(values, dtype=np.int64).reshape(dims)
    return arrays


def fake_quant_codes(w):
    """quantized_bits(8, 0, symmetric=1, alpha=1.0) inference behavior,
    expressed as integer codes: clip(round_half_even(w*128), -127, 127).
    np.round is half-to-even, matching tf.round exactly."""
    return np.clip(np.round(np.asarray(w, dtype=np.float64) * 128.0),
                   -127, 127).astype(np.int64)


def collect_datasets(h5file):
    found = []

    def visit(name, obj):
        if isinstance(obj, h5py.Dataset):
            found.append((name, tuple(obj.shape)))

    h5file.visititems(visit)
    return found


def classify_role(path_lower):
    if "sencgru" in path_lower:
        return "encoder"
    if "sdecgru" in path_lower:
        return "decoder"
    if "dense" in path_lower:
        return "dense"
    if "qgru_cell_1" in path_lower:
        return "decoder"
    if "qgru_cell" in path_lower:
        return "encoder"
    return None


def find_layers(h5file):
    datasets = collect_datasets(h5file)
    print("=== Datasets found in the h5 file ===")
    for name, shape in datasets:
        print(f"  {shape}  {name}")
    print()

    layers = {
        "encoder": {"kernel": None, "recurrent": None, "bias": None},
        "decoder": {"kernel": None, "recurrent": None, "bias": None},
        "dense": {"kernel": None, "bias": None},
    }

    for name, shape in datasets:
        role = classify_role(name.lower())
        if role is None:
            continue
        if role in ("encoder", "decoder"):
            if shape == (1, 3 * GRU_UNITS):
                slot = "kernel"
            elif shape == (GRU_UNITS, 3 * GRU_UNITS):
                slot = "recurrent"
            elif shape == (3 * GRU_UNITS,):
                slot = "bias"
            else:
                print(f"ERROR: {name} classified as {role} but has shape "
                      f"{shape}, which does not match a GRU_UNITS={GRU_UNITS} "
                      "QGRU (expected (1,96)/(32,96)/(96,)). Wrong-architecture "
                      "checkpoint.")
                sys.exit(1)
        else:
            if shape == (GRU_UNITS, 3):
                slot = "kernel"
            elif shape == (3,):
                slot = "bias"
            else:
                print(f"ERROR: {name} classified as dense but has shape "
                      f"{shape}, expected (32,3) or (3,).")
                sys.exit(1)
        if layers[role][slot] is not None:
            print(f"ERROR: duplicate {role} {slot}: {name} -- ambiguous "
                  "checkpoint, inspect the dataset list above.")
            sys.exit(1)
        layers[role][slot] = name

    for role, slots in layers.items():
        for slot, name in slots.items():
            if name is None:
                print(f"ERROR: could not locate {role} {slot} in the h5 file. "
                      "Inspect the dataset list above.")
                sys.exit(1)

    print("Layer mapping by layer NAME in dataset path:")
    print(f"  encoder: kernel={layers['encoder']['kernel']}")
    print(f"           recurrent={layers['encoder']['recurrent']}")
    print(f"           bias={layers['encoder']['bias']}")
    print(f"  decoder: kernel={layers['decoder']['kernel']}")
    print(f"           recurrent={layers['decoder']['recurrent']}")
    print(f"           bias={layers['decoder']['bias']}")
    print(f"  dense:   kernel={layers['dense']['kernel']}  "
          f"bias={layers['dense']['bias']}")
    print()

    def d(name):
        return np.array(h5file[name])

    return {
        "sencgru": (d(layers["encoder"]["kernel"]),
                    d(layers["encoder"]["recurrent"]),
                    d(layers["encoder"]["bias"])),
        "sdecgru": (d(layers["decoder"]["kernel"]),
                    d(layers["decoder"]["recurrent"]),
                    d(layers["decoder"]["bias"])),
        "dense": (d(layers["dense"]["kernel"]),
                  d(layers["dense"]["bias"])),
    }


def compare(name, h5_codes, header_codes, failures):
    if h5_codes.shape != header_codes.shape:
        print(f"[{name}] SHAPE MISMATCH: h5 {h5_codes.shape} vs header "
              f"{header_codes.shape}")
        failures.append(name)
        return
    diff = h5_codes - header_codes
    n_bad = int(np.count_nonzero(diff))
    if n_bad == 0:
        print(f"[{name}] OK: all {h5_codes.size} codes identical")
        return
    max_bad = int(np.abs(diff).max())
    print(f"[{name}] MISMATCH: {n_bad}/{h5_codes.size} codes differ, "
          f"max |code diff| = {max_bad}")
    idx = np.argwhere(diff != 0)
    for i in idx[:5]:
        i = tuple(int(v) for v in i)
        print(f"    at {i}: h5 code {int(h5_codes[i])} vs header code "
              f"{int(header_codes[i])}")
    failures.append(name)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--h5", required=True,
                    help="path to the checkpoint to compare against")
    ap.add_argument("--weights", default="src/student_weights_int8.h")
    args = ap.parse_args()

    header = parse_weight_header_codes(args.weights)

    with h5py.File(args.h5, "r") as f:
        layers = find_layers(f)

    failures = []
    for prefix in ("sencgru", "sdecgru"):
        kernel, recurrent, bias = layers[prefix]
        k_codes = fake_quant_codes(kernel)
        r_codes = fake_quant_codes(recurrent)
        b_codes = fake_quant_codes(bias)
        u = GRU_UNITS
        compare(prefix + "_kernel_z", k_codes[:, :u], header[prefix + "_kernel_z"], failures)
        compare(prefix + "_kernel_r", k_codes[:, u:2 * u], header[prefix + "_kernel_r"], failures)
        compare(prefix + "_kernel_h", k_codes[:, 2 * u:], header[prefix + "_kernel_h"], failures)
        compare(prefix + "_recurrent_kernel_z", r_codes[:, :u], header[prefix + "_recurrent_kernel_z"], failures)
        compare(prefix + "_recurrent_kernel_r", r_codes[:, u:2 * u], header[prefix + "_recurrent_kernel_r"], failures)
        compare(prefix + "_recurrent_kernel_h", r_codes[:, 2 * u:], header[prefix + "_recurrent_kernel_h"], failures)
        compare(prefix + "_bias_z", b_codes[:u], header[prefix + "_bias_z"], failures)
        compare(prefix + "_bias_r", b_codes[u:2 * u], header[prefix + "_bias_r"], failures)
        compare(prefix + "_bias_h", b_codes[2 * u:], header[prefix + "_bias_h"], failures)

    dk, db = layers["dense"]
    compare("sdec_dense_kernel", fake_quant_codes(dk), header["sdec_dense_kernel"], failures)
    compare("sdec_dense_bias", fake_quant_codes(db), header["sdec_dense_bias"], failures)

    print()
    if failures:
        print(f"VERDICT: {len(failures)} tensor(s) DIFFER from the checkpoint: "
              f"{failures}")
        print("The header was extracted from a different checkpoint or with "
              "different rounding/splitting.")
        sys.exit(1)
    print("VERDICT: header matches the checkpoint exactly.")
    sys.exit(0)


if __name__ == "__main__":
    main()