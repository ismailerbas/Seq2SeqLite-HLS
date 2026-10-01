open_project -reset teacher_baseline_paper_repro_proj
set_top seq2seqmodel
add_files teacher_baseline/src/function/adj_offset.c
add_files teacher_baseline/src/function/adj_offset.h
add_files teacher_baseline/src/function/findmax.c
add_files teacher_baseline/src/function/findmax.h
add_files teacher_baseline/src/layer/grucell.c
add_files teacher_baseline/src/layer/grucell.h
add_files teacher_baseline/src/function/innerproduct.c
add_files teacher_baseline/src/function/innerproduct.h
add_files teacher_baseline/src/function/matmul.c
add_files teacher_baseline/src/function/matmul.h
add_files teacher_baseline/src/function/min.c
add_files teacher_baseline/src/function/min.h
add_files teacher_baseline/src/function/normalize.c
add_files teacher_baseline/src/function/normalize.h
add_files teacher_baseline/src/layer/seq2seq.c
add_files teacher_baseline/src/layer/seq2seq.h
add_files teacher_baseline/src/function/sigmoid.c
add_files teacher_baseline/src/function/sigmoid.h
add_files teacher_baseline/src/function/tanhf.c
add_files teacher_baseline/src/function/tanhf.h
open_solution -reset "solution_paper_teacher" -flow_target vivado
set_part {xc7k410ti-ffv900-2L}
create_clock -period 4.5 -name default
config_export -flow impl -vivado_clock 4000 -vivado_phys_opt all
source "teacher_baseline/directives/teacher_original_directives.tcl"
csynth_design
exit
