// ============================================================================
// Testbench for the pixel-interleaved engine.
//
// Two checks, in order of strength:
//   1. BIT-EXACT cross-check: the interleaved engine's output for each of
//      the 5 experimental pixels must be IDENTICAL to student_infer_pixel's
//      output for the same pixel. The two implementations share the same
//      cell math; only the schedule differs, so any nonzero difference is
//      an implementation bug, not a tolerance question.
//   2. Model agreement: the interleaved outputs must match the real QKeras
//      model reference (expected_out_k.csv) within 0.02, the same gate the
//      verified single-pixel design passes.
// Pixels occupy contexts 0..4; the remaining contexts run on zero input,
// exercising the padded-batch path of the frame wrapper.
// ============================================================================

#include <cstdio>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

#include "student_top.h"
#include "student_top_interleaved.h"

#define TB_NUM_PIXELS 5
#define TB_BASELINE_BINS 10

static bool load_raw_pixel(const std::string &path, double raw[SEQ_LEN]) {
    std::ifstream f(path.c_str());
    if (!f.is_open()) {
        printf("ERROR: cannot open %s\n", path.c_str());
        return false;
    }
    int n = 0;
    std::string line;
    while (std::getline(f, line) && n < SEQ_LEN) {
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',') && n < SEQ_LEN) {
            if (!cell.empty()) {
                raw[n] = std::atof(cell.c_str());
                n++;
            }
        }
    }
    if (n != SEQ_LEN) {
        printf("ERROR: %s yielded %d values, expected %d\n", path.c_str(), n, SEQ_LEN);
        return false;
    }
    return true;
}

static bool load_expected_out(const std::string &path, double expected[SEQ_LEN][N_OUT]) {
    std::ifstream f(path.c_str());
    if (!f.is_open()) {
        printf("ERROR: cannot open %s\n", path.c_str());
        return false;
    }
    int row = 0;
    std::string line;
    while (std::getline(f, line) && row < SEQ_LEN) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (std::getline(ss, cell, ',') && col < N_OUT) {
            expected[row][col] = std::atof(cell.c_str());
            col++;
        }
        if (col != N_OUT) {
            printf("ERROR: %s row %d has %d columns, expected %d\n",
                   path.c_str(), row, col, N_OUT);
            return false;
        }
        row++;
    }
    if (row != SEQ_LEN) {
        printf("ERROR: %s has %d rows, expected %d\n", path.c_str(), row, SEQ_LEN);
        return false;
    }
    return true;
}

// Identical preprocessing to student_tb.cpp / eval_experimental.py:
// baseline = mean of first TB_BASELINE_BINS bins, subtract, clip >= 0,
// per-pixel max normalize, convert with input_t's AP_TRN semantics.
static void preprocess_pixel_ilv(const double raw[SEQ_LEN], input_t out[SEQ_LEN]) {
    double baseline = 0.0;
    for (int i = 0; i < TB_BASELINE_BINS; i++) {
        baseline += raw[i];
    }
    baseline /= (double)TB_BASELINE_BINS;

    double corrected[SEQ_LEN];
    double max_val = 0.0;
    for (int t = 0; t < SEQ_LEN; t++) {
        double v = raw[t] - baseline;
        if (v < 0.0) v = 0.0;
        corrected[t] = v;
        if (v > max_val) max_val = v;
    }
    for (int t = 0; t < SEQ_LEN; t++) {
        double n = (max_val > 0.0) ? (corrected[t] / max_val) : corrected[t];
        out[t] = input_t(n);
    }
}

int main() {
    static double raw[TB_NUM_PIXELS][SEQ_LEN];
    static double expected[TB_NUM_PIXELS][SEQ_LEN][N_OUT];

    for (int k = 0; k < TB_NUM_PIXELS; k++) {
        char raw_path[64];
        char exp_path[64];
        std::snprintf(raw_path, sizeof(raw_path), "raw_pixel_%d.csv", k + 1);
        std::snprintf(exp_path, sizeof(exp_path), "expected_out_%d.csv", k + 1);
        if (!load_raw_pixel(raw_path, raw[k])) return 1;
        if (!load_expected_out(exp_path, expected[k])) return 1;
    }

    // ---- reference: the verified single-pixel engine ----
    static input_t  tpsf_single[TB_NUM_PIXELS][SEQ_LEN];
    static output_t sfd_single[TB_NUM_PIXELS][SEQ_LEN][N_OUT];
    for (int k = 0; k < TB_NUM_PIXELS; k++) {
        preprocess_pixel_ilv(raw[k], tpsf_single[k]);
        student_infer_pixel(tpsf_single[k], sfd_single[k]);
    }

    // ---- interleaved batch: pixels in contexts 0..4, zeros elsewhere ----
    static input_t  tpsf_batch[NPIX_ILV][SEQ_LEN];
    static output_t sfd_batch[NPIX_ILV][SEQ_LEN][N_OUT];
    for (int p = 0; p < NPIX_ILV; p++) {
        for (int t = 0; t < SEQ_LEN; t++) {
            tpsf_batch[p][t] = (p < TB_NUM_PIXELS) ? tpsf_single[p][t] : input_t(0.0);
        }
    }
    student_infer_batch_interleaved(tpsf_batch, sfd_batch);

    // ---- check 1: bit-exact vs student_infer_pixel ----
    int mismatches = 0;
    for (int k = 0; k < TB_NUM_PIXELS; k++) {
        for (int t = 0; t < SEQ_LEN; t++) {
            for (int o = 0; o < N_OUT; o++) {
                if (sfd_batch[k][t][o] != sfd_single[k][t][o]) {
                    if (mismatches < 10) {
                        printf("[crosscheck] pixel_%d t=%d o=%d: interleaved=%f "
                               "single=%f\n", k + 1, t, o,
                               (double)sfd_batch[k][t][o],
                               (double)sfd_single[k][t][o]);
                    }
                    mismatches++;
                }
            }
        }
    }
    if (mismatches == 0) {
        printf("[crosscheck] PASS: interleaved output is BIT-EXACT with "
               "student_infer_pixel on all %d pixels x %d timesteps x %d channels\n",
               TB_NUM_PIXELS, SEQ_LEN, N_OUT);
    } else {
        printf("[crosscheck] FAIL: %d mismatching values\n", mismatches);
    }

    // ---- check 2: agreement with the real QKeras model reference ----
    double overall = 0.0;
    int failed = 0;
    for (int k = 0; k < TB_NUM_PIXELS; k++) {
        double max_err = 0.0;
        int max_t = -1, max_o = -1;
        for (int t = 0; t < SEQ_LEN; t++) {
            for (int o = 0; o < N_OUT; o++) {
                double err = std::fabs((double)sfd_batch[k][t][o] - expected[k][t][o]);
                if (err > max_err) {
                    max_err = err;
                    max_t = t;
                    max_o = o;
                }
            }
        }
        if (max_err > overall) overall = max_err;
        bool pass = (max_err < 0.02);
        if (!pass) failed++;
        printf("[ilv_pixel_%d] Max abs error vs real QKeras model output: %f "
               "(t=%d, o=%d)  %s\n", k + 1, max_err, max_t, max_o,
               pass ? "PASS" : "FAIL");
    }

    printf("========================================\n");
    printf("Interleaved overall max abs error: %f\n", overall);
    printf("Interleaved pixels failed: %d / %d\n", failed, TB_NUM_PIXELS);

    if (mismatches == 0 && failed == 0) {
        printf("ALL PASS\n");
        return 0;
    }
    printf("SOME FAILED\n");
    return 1;
}