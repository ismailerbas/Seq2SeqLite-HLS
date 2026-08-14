#include "student_top.h"
#include <cstdio>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

// ============================================================================
// Baseline + normalization matching training-time load_and_preprocess_mat.
// Training data (synthetic, PyFLI-generated) has zero baseline offset, so
// the original formula was clip(raw,0,None)/max(raw) with no subtraction.
// Real SPAD/TCSPC pixel data carries a nonzero dark-count/background
// baseline, so it is removed here using the minimum value of the trace
// (no fixed pre-pulse window assumption required).
// ============================================================================
static void preprocess_pixel(const double raw[SEQ_LEN], input_t out[SEQ_LEN]) {
    double baseline = raw[0];
    for (int t = 1; t < SEQ_LEN; t++) {
        if (raw[t] < baseline) baseline = raw[t];
    }

    double corrected[SEQ_LEN];
    double max_val = 0.0;
    for (int t = 0; t < SEQ_LEN; t++) {
        double v = raw[t] - baseline;
        if (v < 0.0) v = 0.0;
        corrected[t] = v;
        if (v > max_val) max_val = v;
    }

    for (int t = 0; t < SEQ_LEN; t++) {
        double n = (max_val > 0.0) ? (corrected[t] / max_val) : 0.0;
        out[t] = input_t(n);
    }
}

int main() {
    // ------------------------------------------------------------------
    // PLACEHOLDER: replace with the real 135-bin raw TPSF counts for one
    // experimental pixel. Do not run this testbench until this array
    // contains real acquired data.
    // ------------------------------------------------------------------
    double raw_pixel[SEQ_LEN] = { /* PASTE 135 REAL RAW TPSF VALUES HERE */ };

    // ------------------------------------------------------------------
    // PLACEHOLDER: replace with the real reference model output for the
    // same pixel, one row per timestep (SEQ_LEN=135 rows), N_OUT=3
    // columns per row, matching sfd_out[t][o] from student_infer_pixel.
    // ------------------------------------------------------------------
    double expected_out[SEQ_LEN][N_OUT] = { /* PASTE REAL EXPECTED OUTPUT ROWS HERE */ };

    input_t  tpsf_in[SEQ_LEN];
    output_t sfd_out[SEQ_LEN][N_OUT];

    preprocess_pixel(raw_pixel, tpsf_in);
    student_infer_pixel(tpsf_in, sfd_out);

    double max_abs_err = 0.0;
    for (int t = 0; t < SEQ_LEN; t++) {
        for (int o = 0; o < N_OUT; o++) {
            double got = (double)sfd_out[t][o];
            double err = std::fabs(got - expected_out[t][o]);
            if (err > max_abs_err) max_abs_err = err;
        }
    }

    printf("Max abs error vs expected model output: %f\n", max_abs_err);
    printf(max_abs_err < 0.02 ? "PASS\n" : "FAIL\n");

    // ------------------------------------------------------------------
    // FUTURE MULTI-PIXEL TEST: load a full frame exported from the .mat
    // file to a plain CSV (one row per pixel, 135 columns), and run
    // preprocess_pixel + student_infer_pixel per row. Disabled until a
    // CSV export is available.
    // ------------------------------------------------------------------
    /*
    std::ifstream f("frame_pixels.csv");
    std::string line;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        double row[SEQ_LEN];
        for (int t = 0; t < SEQ_LEN; t++) {
            std::string cell;
            std::getline(ss, cell, ',');
            row[t] = std::stod(cell);
        }
        input_t tpsf_row[SEQ_LEN];
        output_t sfd_row[SEQ_LEN][N_OUT];
        preprocess_pixel(row, tpsf_row);
        student_infer_pixel(tpsf_row, sfd_row);
    }
    */

    return (max_abs_err < 0.02) ? 0 : 1;
}