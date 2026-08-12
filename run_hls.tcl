# ============================================================================
# run_hls.tcl -- Vitis HLS project creation + csim + synthesis driver
# for the Seq2SeqLite student single-pixel int8 kernel.
# ============================================================================

open_project -reset seq2seqlite_student_proj

set_top student_infer_pixel

add_files src/student_top.cpp -cflags "-I src"
add_files src/student_hls_types.h -cflags "-I src"
add_files src/student_weight_convert.h -cflags "-I src"
add_files src/student_weights_int8.h -cflags "-I src"

# Testbench added separately once you have real reference I/O data:
# add_files -tb src/student_tb.cpp -cflags "-I src"

open_solution -reset "solution1"

# ---- FPGA part selection -----------------------------------------------
# Confirmed exact part for the paper's target platform:
#   Opal Kelly XEM7360-K410T  ->  AMD/Xilinx Kintex-7 XC7K410T
#   Package: FFG676   Speed grade: -1   Temp grade: Commercial (C)
set_part {xc7k410t-1ffg676c}

# ---- Clock -----------------------------------------------------------
# 100 MHz is the board's low-jitter oscillator frequency (confirmed from
# Opal Kelly XEM7360 spec: "100 MHz and 200 MHz low-jitter clock
# oscillators"). 10 ns period, 2 ns uncertainty margin is a conservative
# default for a first synthesis pass.
create_clock -period 10 -name default

csim_design -clean

csynth_design

# Uncomment once csim + csynth are both clean and you're ready to generate
# the IP / bitstream-ready RTL export:
# export_design -rtl verilog -format ip_catalog

exit