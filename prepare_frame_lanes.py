#!/usr/bin/env python3
"""
prepare_frame_lanes.py -- Load a real 500x500-pixel experimental TPSF frame
from a .mat file (same "tp4d" key and loading convention used in
eval_experimental.py::load_and_preprocess_mat), preprocess every pixel with
the exact same baseline-subtract / clip / max-normalize pipeline already
validated against the real model, split the 250,000 pixels into 4 disjoint
lane buffers matching student_infer_frame_streaming's exact expected memory
layout, and write each lane's input buffer to a binary file ready to be
loaded into the FPGA's DDR via the host driver.

DATA FORMAT (must match the HLS kernel bit-exactly):
The kernel's m_axi input ports carry input_t = ap_fixed<16, 2, AP_TRN,
AP_WRAP> (see src/student_hls_types.h): 16-bit two's-complement fixed-point
codes with 2 integer bits and 14 fractional bits, LSB = 2^-14. The files
written here are therefore flat little-endian int16 arrays, NOT float32.
Each code is computed as floor(value * 2^14), which is exactly what the
AP_TRN conversion in "out[t] = input_t(n)" performs in student_tb.cpp's
preprocess_pixel, so DDR contents are bit-identical to what csim feeds the
verified kernel. Normalized values lie in [0, 1], so codes lie in
[0, 16384] and always fit in int16 with no wrap.

Requires: numpy, scipy.
"""

import numpy as np
import os
import sys
from scipy.io import loadmat

N_ROWS = 500
N_COLS = 500
SEQ_LEN = 135
BASELINE_BINS = 10
NUM_LANES = 4
TOTAL_PIXELS = N_ROWS * N_COLS
PIXELS_PER_LANE = TOTAL_PIXELS // NUM_LANES

# input_t = ap_fixed<16, 2, AP_TRN, AP_WRAP>: 14 fractional bits.
INPUT_T_FRAC_BITS = 14
INPUT_T_SCALE = 1 << INPUT_T_FRAC_BITS
INPUT_T_MIN_CODE = -(1 << 15)
INPUT_T_MAX_CODE = (1 << 15) - 1


def load_tp4d(mat_path):
    print(f"[MAT] Loading: {mat_path}")
    mat_data = loadmat(mat_path)
    if "tp4d" not in mat_data:
        available = [k for k in mat_data.keys() if not k.startswith("_")]
        print(f"ERROR: 'tp4d' not found in {mat_path}. Available keys: {available}")
        sys.exit(1)
    tp4d = mat_data["tp4d"]
    if tp4d.shape[0] != N_ROWS or tp4d.shape[1] != N_COLS or tp4d.shape[2] != SEQ_LEN:
        print(f"ERROR: tp4d has shape {tp4d.shape}, expected "
              f"({N_ROWS}, {N_COLS}, {SEQ_LEN})")
        sys.exit(1)
    return tp4d


def preprocess_pixel(raw):
    baseline = raw[:BASELINE_BINS].mean()
    corrected = np.clip(raw - baseline, 0.0, None)
    max_val = corrected.max()
    norm = corrected / max_val if max_val > 0 else corrected
    return norm


def quantize_to_input_t(norm):
    """Convert normalized float values to input_t codes, bit-exactly matching
    the AP_TRN (truncate toward negative infinity) conversion performed by
    "out[t] = input_t(n)" in student_tb.cpp. floor(v * 2^14) as int16."""
    codes = np.floor(norm * INPUT_T_SCALE)
    codes = np.clip(codes, INPUT_T_MIN_CODE, INPUT_T_MAX_CODE)
    return codes.astype(np.int16)


def main():
    if len(sys.argv) != 3:
        print("Usage: python prepare_frame_lanes.py <path_to_frame.mat> <output_dir>")
        sys.exit(1)

    mat_path = sys.argv[1]
    output_dir = sys.argv[2]
    os.makedirs(output_dir, exist_ok=True)

    tp4d = load_tp4d(mat_path)

    print("Preprocessing all pixels (baseline subtract, clip, max-normalize, "
          "quantize to ap_fixed<16,2> int16 codes)...")
    quantized_frame = np.zeros((TOTAL_PIXELS, SEQ_LEN), dtype=np.int16)
    idx = 0
    for r in range(N_ROWS):
        for c in range(N_COLS):
            raw_pixel = tp4d[r, c, :].astype(np.float64)
            norm = preprocess_pixel(raw_pixel)
            quantized_frame[idx, :] = quantize_to_input_t(norm)
            idx += 1

    print(f"Splitting {TOTAL_PIXELS} pixels into {NUM_LANES} lanes of "
          f"{PIXELS_PER_LANE} pixels each...")

    for lane_id in range(NUM_LANES):
        start = lane_id * PIXELS_PER_LANE
        end = start + PIXELS_PER_LANE
        lane_data = quantized_frame[start:end, :]

        out_path = os.path.join(output_dir, f"tpsf_in_lane{lane_id}.bin")
        lane_data.astype("<i2").tofile(out_path)
        print(f"[lane {lane_id}] wrote {lane_data.shape[0]} pixels "
              f"({lane_data.nbytes} bytes) to {out_path}")

    print("Done. Each .bin file is a flat little-endian int16 array of "
          f"{PIXELS_PER_LANE * SEQ_LEN} ap_fixed<16,2> codes (floor(v * 2^14)), "
          "matching the exact flattened [pixel][t] layout and the exact "
          "16-bit element encoding student_infer_frame_streaming expects.")


if __name__ == "__main__":
    main()