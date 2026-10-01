\# Seq2SeqLite HLS Reproduction Package



This repository contains the Vitis HLS implementation used to evaluate the

hardware cost and latency of the quantized Seq2SeqLite model described in:



Resource-Aware Hardware–Software Co-Design for Biomedical Imaging



The reviewer-facing reproduction files are provided under:

paper\_reproduction/



\## 1. System requirements



\### Software



The synthesis results reported for the paper were reproduced with:



\- AMD/Xilinx Vitis HLS 2022.2

\- Vitis HLS build 3670227

\- Windows host environment

\- C/C++ synthesis flow supplied with Vitis HLS 2022.2



No Python environment is required to reproduce the synthesis reports in

paper\_reproduction/.



\### FPGA target



All paper reproduction scripts target:



\- Device family: Xilinx Kintex-7

\- Part: xc7k410t-ffg676-1

\- Target clock period: 4.5 ns

\- HLS flow target: Vivado IP



A physical FPGA board is not required for C synthesis (csynth\_design).



\## 2. Installation guide



Install AMD/Xilinx Vitis HLS 2022.2 and ensure that vitis\_hls is available

from the command line.



On the authors' Windows system, the executable was invoked directly as:



"D:\\Xlinx\\Vitis\_HLS\\2022.2\\bin\\vitis\_hls.bat"



No compilation or package installation is required before running the HLS

scripts.



\## 3. Demo



The smallest reproduction target is the single-pixel Seq2SeqLite inference

kernel.



From the repository root, run:



"D:\\Xlinx\\Vitis\_HLS\\2022.2\\bin\\vitis\_hls.bat" -f paper\_reproduction\\tcl\\reproduce\_single\_pixel\_4p5ns.tcl



The script synthesizes the top-level function:



student\_infer\_pixel



Expected synthesis result:



Latency: 110,297 cycles

Latency: approximately 4.963e+05 ns = 0.496 ms

DSP:     183

FF:      54,802

LUT:     37,140

BRAM:    0



On the authors' workstation, the csynth\_design step completed in

approximately 95 seconds.



A reference synthesis report is included at:



paper\_reproduction/reports/student\_single\_pixel\_csynth.rpt



\## 4. Instructions for use



The HLS source code is located under:



src/



The principal single-pixel inference kernel is implemented in:



src/student\_top.cpp



Supporting fixed-point datatypes and quantized model parameters are contained

in the corresponding headers under src/.



The implementation uses the quantized Seq2SeqLite student network used in the

paper. The recurrent and dense computations include the HLS pragmas used for

resource-controlled synthesis, including pipelining and cyclic array

partitioning.



\## 5. Reproduction of manuscript HLS results



The scripts under:



paper\_reproduction/tcl/



reproduce the 1-lane, 4-lane, and 8-lane Vitis HLS configurations reported in

the manuscript and Supplementary Table S2.



\### Single lane



Run:



vitis\_hls -f paper\_reproduction\\tcl\\reproduce\_single\_pixel\_4p5ns.tcl



Expected result:



DSP   183

FF    54,802

LUT   37,140

BRAM  0

single-pixel latency approximately 0.496 ms



Reference report:



paper\_reproduction/reports/student\_single\_pixel\_csynth.rpt



\### Four lanes



Run:



vitis\_hls -f paper\_reproduction\\tcl\\reproduce\_streaming4\_4p5ns.tcl



Expected result:



DSP   732

FF    228,916

LUT   163,186

BRAM  8

full-frame latency approximately 30.079 s



Reference report:



paper\_reproduction/reports/student\_streaming4\_csynth.rpt



On the authors' workstation, the csynth\_design step completed in

approximately 259 seconds.



\### Eight lanes



Run:



vitis\_hls -f paper\_reproduction\\tcl\\reproduce\_streaming8\_4p5ns.tcl



Expected result:



DSP   1,464

FF    457,772

LUT   326,282

BRAM  16

full-frame latency approximately 15.040 s



The eight-lane configuration exceeds the available LUT capacity of the target

device and is therefore reported as a resource-limit point rather than a

physically realizable implementation.



Reference report:



paper\_reproduction/reports/student\_streaming8\_csynth.rpt



On the authors' workstation, the csynth\_design step completed in

approximately 485 seconds.



\## Reproduction notes



The standalone single-pixel synthesis and the replicated streaming

architectures are intentionally reported separately.



The 0.496 ms per-pixel latency is obtained from synthesis with

student\_infer\_pixel as the HLS top-level function.



When student\_infer\_pixel is instantiated inside the four- and eight-lane

streaming designs, Vitis HLS reports a different submodule latency. That

submodule estimate is not the standalone latency reported in the main

manuscript.



The solution\_256px development experiment is not the 256-pixel

resource-aware scheduling result reported in the manuscript. The manuscript's

256-pixel operating point is produced by the separate hardware-software

scheduling workflow rather than by direct replication of 256 complete HLS

datapaths.



\## Repository structure



src/

&#x20;   HLS implementation and quantized model parameters



paper\_reproduction/

&#x20;   tcl/

&#x20;       reproduce\_single\_pixel\_4p5ns.tcl

&#x20;       reproduce\_streaming4\_4p5ns.tcl

&#x20;       reproduce\_streaming8\_4p5ns.tcl



&#x20;   reports/

&#x20;       student\_single\_pixel\_csynth.rpt

&#x20;       student\_streaming4\_csynth.rpt

&#x20;       student\_streaming8\_csynth.rpt



Generated Vitis HLS project directories are not required for reproduction and

are intentionally not part of the reviewer-facing package.



\## License



This repository is licensed under the GNU Affero General Public License v3.0

(AGPL-3.0). See the `LICENSE` file for the full license text.



\## Code availability



Repository:

https://github.com/ismailerbas/Seq2SeqLite-HLS


## Float32 teacher baseline

The Table 1 float32 Seq2Seq reference baseline is provided under `teacher_baseline/`.
See `teacher_baseline/README.md` for source, exact Vitis HLS configuration, historical and fresh synthesis reports, and reproduction instructions.

## Float32 teacher baseline

The Table 1 float32 Seq2Seq reference baseline is provided under teacher_baseline/.
See teacher_baseline/README.md for source, exact Vitis HLS configuration, historical and fresh synthesis reports, and reproduction instructions.
