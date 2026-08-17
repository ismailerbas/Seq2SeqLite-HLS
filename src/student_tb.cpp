#include "student_top.h"
#include <cstdio>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

// ============================================================================
// Baseline + normalization matching training-time load_and_preprocess_mat
// (eval_experimental.py, function load_and_preprocess_mat).
//
// Real reference pipeline:
//   1. baseline[r,c] = mean(tp4d[r,c,0:BASELINE_BINS])   -- per-pixel mean of
//      the first BASELINE_BINS time bins, NOT the global minimum of the
//      trace and NOT a single-sample subtraction.
//   2. tp4d[r,c,:] -= baseline[r,c]
//   3. clip negatives to 0
//   4. per-pixel max normalization: tp4d[r,c,:] /= max(tp4d[r,c,:])
//      (if max == 0 the pixel is left at 0 everywhere -- "invalid" pixel)
//
// BASELINE_BINS must equal the --baseline-bins value used when
// eval_experimental.py generated the reference outputs you are testing
// against. The script default is 10 -- confirm this against the exact
// command line that produced experimental_preds.mat before trusting PASS.
// ============================================================================
static const int BASELINE_BINS = 10;

static void preprocess_pixel(const double raw[SEQ_LEN], input_t out[SEQ_LEN]) {
    int n_baseline = (BASELINE_BINS < SEQ_LEN) ? BASELINE_BINS : SEQ_LEN;
    double baseline = 0.0;
    for (int t = 0; t < n_baseline; t++) {
        baseline += raw[t];
    }
    baseline /= (double)n_baseline;

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

// ----------------------------------------------------------------------------
// Loads a single pixel's SEQ_LEN raw values from a one-row CSV file
// (exported by the MATLAB extraction script as raw_pixel_k.csv).
// Returns true on success, false on any format/IO error (prints the reason).
// ----------------------------------------------------------------------------
static bool load_raw_pixel_csv(const std::string& path, double raw[SEQ_LEN]) {
    std::ifstream f(path);
    if (!f.is_open()) {
        printf("ERROR: could not open %s\n", path.c_str());
        return false;
    }
    std::string line;
    if (!std::getline(f, line)) {
        printf("ERROR: %s is empty\n", path.c_str());
        return false;
    }
    std::stringstream ss(line);
    std::string cell;
    int t = 0;
    while (std::getline(ss, cell, ',') && t < SEQ_LEN) {
        raw[t] = std::stod(cell);
        t++;
    }
    if (t != SEQ_LEN) {
        printf("ERROR: %s has %d values, expected %d\n", path.c_str(), t, SEQ_LEN);
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------
// Loads a pixel's reference model output from a SEQ_LEN-row, N_OUT-column
// CSV file (exported by the MATLAB extraction script as expected_out_k.csv),
// one row per timestep, matching sfd_out[t][o].
// Returns true on success, false on any format/IO error (prints the reason).
// ----------------------------------------------------------------------------
static bool load_expected_out_csv(const std::string& path, double expected[SEQ_LEN][N_OUT]) {
    std::ifstream f(path);
    if (!f.is_open()) {
        printf("ERROR: could not open %s\n", path.c_str());
        return false;
    }
    std::string line;
    int t = 0;
    while (std::getline(f, line) && t < SEQ_LEN) {
        std::stringstream ss(line);
        std::string cell;
        int o = 0;
        while (std::getline(ss, cell, ',') && o < N_OUT) {
            expected[t][o] = std::stod(cell);
            o++;
        }
        if (o != N_OUT) {
            printf("ERROR: %s row %d has %d columns, expected %d\n", path.c_str(), t, o, N_OUT);
            return false;
        }
        t++;
    }
    if (t != SEQ_LEN) {
        printf("ERROR: %s has %d rows, expected %d\n", path.c_str(), t, SEQ_LEN);
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------
// Runs preprocess_pixel + student_infer_pixel on one pixel and compares
// against the expected reference output. Returns the max abs error found.
// ----------------------------------------------------------------------------
static double run_one_pixel_test(const std::string& tag,
                                  const double raw_pixel[SEQ_LEN],
                                  const double expected_out[SEQ_LEN][N_OUT]) {
    input_t  tpsf_in[SEQ_LEN];
    output_t sfd_out[SEQ_LEN][N_OUT];

    preprocess_pixel(raw_pixel, tpsf_in);
    student_infer_pixel(tpsf_in, sfd_out);

    double max_abs_err = 0.0;
    int max_t = -1, max_o = -1;
    for (int t = 0; t < SEQ_LEN; t++) {
        for (int o = 0; o < N_OUT; o++) {
            double got = (double)sfd_out[t][o];
            double err = std::fabs(got - expected_out[t][o]);
            if (err > max_abs_err) {
                max_abs_err = err;
                max_t = t;
                max_o = o;
            }
        }
    }

    printf("[%s] Max abs error vs expected model output: %f (t=%d, o=%d)\n",
           tag.c_str(), max_abs_err, max_t, max_o);
    printf("[%s] %s\n", tag.c_str(), (max_abs_err < 0.02) ? "PASS" : "FAIL");
    return max_abs_err;
}

int main() {
    // ------------------------------------------------------------------
    // 5 experimental pixels selected via ginput() in MATLAB from the same
    // af700 Accumulated.mat frame used to generate experimental_preds.mat.
    // Each pair (raw_pixel_k.csv, expected_out_k.csv) is exported by the
    // MATLAB extraction script using the confirmed row-major pixel index
    // mapping: preds_idx = (row-1)*ncols + col. Place these 10 CSV files
    // in the working directory this binary is run from before executing.
    // ------------------------------------------------------------------
    static const int NUM_PIXELS = 5;
    static const char* raw_paths[NUM_PIXELS] = {
        "raw_pixel_1.csv",
        "raw_pixel_2.csv",
        "raw_pixel_3.csv",
        "raw_pixel_4.csv",
        "raw_pixel_5.csv"
    };
    static const char* expected_paths[NUM_PIXELS] = {
        "expected_out_1.csv",
        "expected_out_2.csv",
        "expected_out_3.csv",
        "expected_out_4.csv",
        "expected_out_5.csv"
    };

    double overall_max_err = 0.0;
    int n_failed = 0;

    for (int k = 0; k < NUM_PIXELS; k++) {
        double raw_pixel[SEQ_LEN];
        double expected_out[SEQ_LEN][N_OUT];

        if (!load_raw_pixel_csv(raw_paths[k], raw_pixel)) {
            printf("FATAL: failed to load %s, aborting test\n", raw_paths[k]);
            return 1;
        }
        if (!load_expected_out_csv(expected_paths[k], expected_out)) {
            printf("FATAL: failed to load %s, aborting test\n", expected_paths[k]);
            return 1;
        }

        char tag[32];
        snprintf(tag, sizeof(tag), "pixel_%d", k + 1);
        double err = run_one_pixel_test(tag, raw_pixel, expected_out);
        if (err > overall_max_err) overall_max_err = err;
        if (err >= 0.02) n_failed++;
    }

    printf("========================================\n");
    printf("Overall max abs error across %d pixels: %f\n", NUM_PIXELS, overall_max_err);
    printf("Pixels failed: %d / %d\n", n_failed, NUM_PIXELS);
    printf(n_failed == 0 ? "ALL PASS\n" : "SOME FAILED\n");

    return (n_failed == 0) ? 0 : 1;
}