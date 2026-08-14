############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project seq2seqlite_student_proj
set_top student_infer_pixel
add_files src/student_weights_int8.h -cflags "-Isrc"
add_files src/student_weight_convert.h -cflags "-Isrc"
add_files src/student_hls_types.h -cflags "-Isrc"
add_files src/student_top.cpp -cflags "-Isrc"
open_solution "solution1" -flow_target vivado

create_clock -period 2 -name default
config_interface -m_axi_latency 0
#source "./seq2seqlite_student_proj/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
