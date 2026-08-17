open_project -reset seq2seqlite_student_proj
add_files src/student_top.cpp -cflags "-I src"
add_files src/student_top.h -cflags "-I src"
add_files src/student_top_parallel.cpp -cflags "-I src"
add_files src/student_top_parallel.h -cflags "-I src"
add_files src/student_hls_types.h -cflags "-I src"
add_files src/student_weight_convert.h -cflags "-I src"
add_files src/student_weights_int8.h -cflags "-I src"
set_top student_infer_batch_parallel
open_solution -reset "solution_256px" -flow_target vivado
set_part {xc7k410t-ffg676-1}
create_clock -period 4.5 -name default
config_interface -m_axi_latency 0
csynth_design
exit