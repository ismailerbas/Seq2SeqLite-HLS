// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
// Version: 2022.2
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1 ns / 1 ps
module student_infer_pixel_student_infer_pixel_Pipeline_VITIS_LOOP_210_7_p_ZL17sdec_dense_kernel_3_1_ROMdsG (
    address0, ce0, q0, 
    reset, clk);

parameter DataWidth = 7;
parameter AddressWidth = 2;
parameter AddressRange = 3;
 
input[AddressWidth-1:0] address0;
input ce0;
output reg[DataWidth-1:0] q0;

input reset;
input clk;

 
reg [DataWidth-1:0] rom0[0:AddressRange-1];


initial begin
     
    $readmemh("./student_infer_pixel_student_infer_pixel_Pipeline_VITIS_LOOP_210_7_p_ZL17sdec_dense_kernel_3_1_ROMdsG.dat", rom0);
end

  
always @(posedge clk) 
begin 
    if (ce0) 
    begin
        q0 <= rom0[address0];
    end
end


endmodule

