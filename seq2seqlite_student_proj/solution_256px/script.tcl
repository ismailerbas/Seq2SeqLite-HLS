############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project seq2seqlite_student_proj
set_top student_infer_batch_parallel
add_files src/student_weights_int8.h -cflags "-Isrc"
add_files src/student_weight_convert.h -cflags "-Isrc"
add_files src/student_hls_types.h -cflags "-Isrc"
add_files src/student_top_parallel.h -cflags "-Isrc"
add_files src/student_top_parallel.cpp -cflags "-Isrc"
add_files src/student_top.h -cflags "-Isrc"
add_files src/student_top.cpp -cflags "-Isrc"
open_solution "solution_256px" -flow_target vivado
set_part {xc7k410t-ffg676-1}
create_clock -period 4.5 -name default
config_interface -m_axi_latency 0
source "./seq2seqlite_student_proj/solution_256px/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
