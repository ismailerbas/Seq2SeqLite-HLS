############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project seq2seqlite_student_proj
set_top student_infer_frame_streaming8
add_files src/student_hls_types.h -cflags "-Isrc"
add_files src/student_top.cpp -cflags "-Isrc"
add_files src/student_top.h -cflags "-Isrc"
add_files src/student_top_streaming8.cpp -cflags "-Isrc"
add_files src/student_top_streaming8.h -cflags "-Isrc"
add_files src/student_weight_convert.h -cflags "-Isrc"
add_files src/student_weights_int8.h -cflags "-Isrc"
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_1.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_2.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_3.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_4.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_5.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_reshape_verified_1.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_reshape_verified_2.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_reshape_verified_3.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_reshape_verified_4.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/expected_out_reshape_verified_5.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/raw_pixel_1.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/raw_pixel_2.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/raw_pixel_3.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/raw_pixel_4.csv
add_files -tb E:/ismail/IBM/nminewmodel/vitis/raw_pixel_5.csv
add_files -tb src/student_tb.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution_streaming8" -flow_target vivado
set_part {xc7k410t-ffg676-1}
create_clock -period 4.5 -name default
config_interface -m_axi_latency 0
source "./seq2seqlite_student_proj/solution_streaming8/directives.tcl"
csim_design -clean
csynth_design
cosim_design
export_design -format ip_catalog
