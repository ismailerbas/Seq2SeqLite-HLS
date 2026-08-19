#!/usr/bin/env python3
"""
decode_frame_lanes.py -- Read back the 4 sfd_out_lane*.bin buffers written
by the FPGA (student_infer_frame_streaming) from DDR dumps, decode the raw
output_t codes into float arrays, reassemble the full 500x500 frame, and
run the exact extract_lifetimes post-processing (matching
extract_lifetimes_pixel in src/student_top.cpp and extract_lifetimes() in
eval_experimental.py) to produce per-pixel tau1, tau2, and FRET-fraction
maps.

DATA FORMAT (must match the HLS kernel bit-exactly):
The kernel's m_axi output ports carry output_t = ap_fixed<16, 6, AP_RND,
AP_SAT> (see src/student_hls_types.h): 16-bit two's-complement fixed-point
codes with 6 integer bits and 10 fractional bits, LSB = 2^-10. Each lane
file is a flat little-endian int16 array laid out exactly as lane_worker
writes it: sfd_out_lane[pixel * SEQ_LEN * N_OUT + t * N_OUT + o], i.e.
row-major [pixel][t][channel]. Decoding is value = code / 1024.0, which is
exact in float64 -- the readback path introduces zero additional error.

Channel meaning (see src/student_top.h): channel 0 = full decay,
channel 1 = short-lifetime component, channel 2 = long-lifetime component.

Pixel-to-frame mapping is the same row-major convention used by
prepare_frame_lanes.py: global pixel index idx = r * N_COLS + c, split
into 4 contiguous lanes of PIXELS_PER_LANE pixels each.

Requires: numpy.
"""

import numpy as np
import os
import sys

N_ROWS = 500
N_COLS = 500
SEQ_LEN = 135
N_OUT = 3
NUM_LANES = 4
TOTAL_PIXELS = N_ROWS * N_COLS
PIXELS_PER_LANE = TOTAL_PIXELS // NUM_LANES

# output_t = ap_fixed<16, 6, AP_RND, AP_SAT>: 10 fractional bits.
OUTPUT_T_FRAC_BITS = 10
OUTPUT_T_SCALE = float(1 << OUTPUT_T_FRAC_BITS)

# Post-processing constants, matching src/student_hls_types.h
# (GATE_WIDTH_NS) and extract_lifetimes_pixel in src/student_top.cpp
# (1e-6 amplitude guards, 0.5 FRET fallback) exactly.
GATE_WIDTH_NS = 0.09
AMP_EPS = 1e-6
FRET_FALLBACK = 0.5

ELEMENTS_PER_LANE = PIXELS_PER_LANE * SEQ_LEN * N_OUT


def load_lane(path):
    print(f"[lane] Loading: {path}")
    if not os.path.isfile(path):
        print(f"ERROR: {path} does not exist")
        sys.exit(1)
    size_bytes = os.path.getsize(path)
    expected_bytes = ELEMENTS_PER_LANE * 2
    if size_bytes != expected_bytes:
        print(f"ERROR: {path} is {size_bytes} bytes, expected {expected_bytes} "
              f"({ELEMENTS_PER_LANE} little-endian int16 codes)")
        sys.exit(1)
    codes = np.fromfile(path, dtype="<i2")
    if codes.size != ELEMENTS_PER_LANE:
        print(f"ERROR: {path} yielded {codes.size} elements, expected "
              f"{ELEMENTS_PER_LANE}")
        sys.exit(1)
    return codes.reshape(PIXELS_PER_LANE, SEQ_LEN, N_OUT)


def decode_output_t(codes):
    """Convert raw output_t int16 codes to float64 values, bit-exactly:
    value = code / 2^10. Every int16 / 1024.0 is exactly representable in
    float64, so this decode introduces no error."""
    return codes.astype(np.float64) / OUTPUT_T_SCALE


def extract_lifetimes_frame(sfd):
    """Vectorized replica of extract_lifetimes_pixel (src/student_top.cpp)
    over all pixels. sfd has shape (TOTAL_PIXELS, SEQ_LEN, N_OUT).

    int1 = trapz(sfd[:, :, 1], dx=GATE_WIDTH_NS)  short component
    int2 = trapz(sfd[:, :, 2], dx=GATE_WIDTH_NS)  long component
    amp1 = sfd[:, 0, 1], amp2 = sfd[:, 0, 2]
    tau1 = int1/amp1 if amp1 > 1e-6 else 0.0
    tau2 = int2/amp2 if amp2 > 1e-6 else 0.0
    fret = amp1/(amp1+amp2) if (amp1+amp2) > 1e-6 else 0.5

    numpy.trapz with dx=h computes h*(0.5*y[0] + y[1] + ... + y[N-2]
    + 0.5*y[N-1]) for uniform spacing -- the identical formula written out
    explicitly in extract_lifetimes_pixel."""
    int1 = np.trapz(sfd[:, :, 1], dx=GATE_WIDTH_NS, axis=1)
    int2 = np.trapz(sfd[:, :, 2], dx=GATE_WIDTH_NS, axis=1)

    amp1 = sfd[:, 0, 1]
    amp2 = sfd[:, 0, 2]

    tau1 = np.zeros(TOTAL_PIXELS, dtype=np.float64)
    tau2 = np.zeros(TOTAL_PIXELS, dtype=np.float64)
    np.divide(int1, amp1, out=tau1, where=(amp1 > AMP_EPS))
    np.divide(int2, amp2, out=tau2, where=(amp2 > AMP_EPS))

    denom = amp1 + amp2
    fret = np.full(TOTAL_PIXELS, FRET_FALLBACK, dtype=np.float64)
    np.divide(amp1, denom, out=fret, where=(denom > AMP_EPS))

    return tau1, tau2, fret


def main():
    if len(sys.argv) != 3:
        print("Usage: python decode_frame_lanes.py <input_dir_with_lane_bins> <output_dir>")
        sys.exit(1)

    input_dir = sys.argv[1]
    output_dir = sys.argv[2]
    os.makedirs(output_dir, exist_ok=True)

    sfd = np.zeros((TOTAL_PIXELS, SEQ_LEN, N_OUT), dtype=np.float64)
    for lane_id in range(NUM_LANES):
        lane_path = os.path.join(input_dir, f"sfd_out_lane{lane_id}.bin")
        codes = load_lane(lane_path)
        start = lane_id * PIXELS_PER_LANE
        end = start + PIXELS_PER_LANE
        sfd[start:end, :, :] = decode_output_t(codes)

    print("Extracting per-pixel lifetimes (tau1, tau2, fret) with the exact "
          "extract_lifetimes_pixel formulas...")
    tau1, tau2, fret = extract_lifetimes_frame(sfd)

    sfd_frame = sfd.reshape(N_ROWS, N_COLS, SEQ_LEN, N_OUT)
    tau1_map = tau1.reshape(N_ROWS, N_COLS)
    tau2_map = tau2.reshape(N_ROWS, N_COLS)
    fret_map = fret.reshape(N_ROWS, N_COLS)

    sfd_path = os.path.join(output_dir, "sfd_frame.npy")
    tau1_path = os.path.join(output_dir, "tau1_map.npy")
    tau2_path = os.path.join(output_dir, "tau2_map.npy")
    fret_path = os.path.join(output_dir, "fret_map.npy")

    np.save(sfd_path, sfd_frame.astype(np.float32))
    np.save(tau1_path, tau1_map)
    np.save(tau2_path, tau2_map)
    np.save(fret_path, fret_map)

    print(f"[out] wrote {sfd_path} shape {sfd_frame.shape} (float32)")
    print(f"[out] wrote {tau1_path} shape {tau1_map.shape} (float64, ns)")
    print(f"[out] wrote {tau2_path} shape {tau2_map.shape} (float64, ns)")
    print(f"[out] wrote {fret_path} shape {fret_map.shape} (float64)")

    valid = (tau1 > 0.0) & (tau2 > 0.0)
    n_valid = int(valid.sum())
    print(f"Pixels with valid lifetimes: {n_valid} / {TOTAL_PIXELS}")
    if n_valid > 0:
        print(f"tau1 range over valid pixels: [{tau1[valid].min():.6f}, "
              f"{tau1[valid].max():.6f}] ns")
        print(f"tau2 range over valid pixels: [{tau2[valid].min():.6f}, "
              f"{tau2[valid].max():.6f}] ns")
        print(f"fret range over valid pixels: [{fret[valid].min():.6f}, "
              f"{fret[valid].max():.6f}]")
    print("Done. Round trip is complete: prepare_frame_lanes.py encodes "
          "input_t codes into DDR, the kernel computes, and this script "
          "decodes output_t codes back out bit-exactly.")


if __name__ == "__main__":
    main()