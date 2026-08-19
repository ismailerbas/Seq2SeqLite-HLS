# ============================================================================
# run_hls_interleaved.tcl -- Vitis HLS driver for the pixel-interleaved
# Seq2SeqLite engine (student_infer_frame_interleaved).
# csim requires the same 10 reference CSVs as run_hls.tcl and is skipped
# automatically when they are absent; csynth always runs.
# ============================================================================

open_project seq2seqlite_student_ilv_proj

set_top student_infer_frame_interleaved

add_files src/student_top.cpp -cflags "-I src"
add_files src/student_top_interleaved.cpp -cflags "-I src"
add_files src/student_hls_types.h -cflags "-I src"
add_files src/student_weight_convert.h -cflags "-I src"
add_files src/student_weights_int8.h -cflags "-I src"

set tb_data_files [list \
    raw_pixel_1.csv raw_pixel_2.csv raw_pixel_3.csv raw_pixel_4.csv raw_pixel_5.csv \
    expected_out_1.csv expected_out_2.csv expected_out_3.csv expected_out_4.csv expected_out_5.csv \
]

set tb_data_present 1
foreach f $tb_data_files {
    if {![file exists $f]} {
        set tb_data_present 0
        puts "WARNING: testbench data file '$f' not found -- csim will be skipped."
    }
}

add_files -tb src/student_tb_interleaved.cpp -cflags "-I src"
if {$tb_data_present} {
    foreach f $tb_data_files {
        add_files -tb $f
    }
}

open_solution -reset "solution_interleaved"

set_part {xc7k410t-1ffg676c}

create_clock -period 4.5 -name default

if {$tb_data_present} {
    csim_design -clean
} else {
    puts "INFO: skipping csim_design (testbench reference CSVs not present)."
}

csynth_design

exit