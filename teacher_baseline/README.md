# Float32 Seq2Seq teacher HLS baseline

This directory reproduces the float32 Seq2Seq reference design used for the Table 1 hardware comparison.

## System requirements

Vitis HLS 2022.2 build 3670227 was used for the reported synthesis.
No FPGA board is required for C synthesis.

Target device: xc7k410ti-ffv900-2L
Target clock period: 4.5 ns
Top function: seq2seqmodel

## Reproduction

Run from the repository root:

vitis_hls -f teacher_baseline/tcl/reproduce_teacher_baseline_4p5ns.tcl

Authors' Windows executable:
D:\Xlinx\Vitis_HLS\2022.2\bin\vitis_hls.bat

## Expected result

Latency: 1,619,577 cycles
Latency time: 7.288 ms
Interval: 1,619,578 cycles
BRAM: 1,843
DSP: 19,347
FF: 3,667,567
LUT: 4,032,176
Timing slack: -1.01 ns

This float32 baseline exceeds the XC7K410T resource budget and is not a deployable design.
It is included to reproduce the Table 1 comparison against the selected 8-bit Seq2SeqLite implementation.

The historical synthesis report and a fresh independent reproduction are included under reports.
The fresh Vitis HLS 2022.2 synthesis reproduced the historical latency and resource values exactly.
