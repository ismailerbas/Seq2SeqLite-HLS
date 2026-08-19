// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of tpsf_in_lane0
//        bit 31~0 - tpsf_in_lane0[31:0] (Read/Write)
// 0x14 : Data signal of tpsf_in_lane0
//        bit 31~0 - tpsf_in_lane0[63:32] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of tpsf_in_lane1
//        bit 31~0 - tpsf_in_lane1[31:0] (Read/Write)
// 0x20 : Data signal of tpsf_in_lane1
//        bit 31~0 - tpsf_in_lane1[63:32] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of tpsf_in_lane2
//        bit 31~0 - tpsf_in_lane2[31:0] (Read/Write)
// 0x2c : Data signal of tpsf_in_lane2
//        bit 31~0 - tpsf_in_lane2[63:32] (Read/Write)
// 0x30 : reserved
// 0x34 : Data signal of tpsf_in_lane3
//        bit 31~0 - tpsf_in_lane3[31:0] (Read/Write)
// 0x38 : Data signal of tpsf_in_lane3
//        bit 31~0 - tpsf_in_lane3[63:32] (Read/Write)
// 0x3c : reserved
// 0x40 : Data signal of tpsf_in_lane4
//        bit 31~0 - tpsf_in_lane4[31:0] (Read/Write)
// 0x44 : Data signal of tpsf_in_lane4
//        bit 31~0 - tpsf_in_lane4[63:32] (Read/Write)
// 0x48 : reserved
// 0x4c : Data signal of tpsf_in_lane5
//        bit 31~0 - tpsf_in_lane5[31:0] (Read/Write)
// 0x50 : Data signal of tpsf_in_lane5
//        bit 31~0 - tpsf_in_lane5[63:32] (Read/Write)
// 0x54 : reserved
// 0x58 : Data signal of tpsf_in_lane6
//        bit 31~0 - tpsf_in_lane6[31:0] (Read/Write)
// 0x5c : Data signal of tpsf_in_lane6
//        bit 31~0 - tpsf_in_lane6[63:32] (Read/Write)
// 0x60 : reserved
// 0x64 : Data signal of tpsf_in_lane7
//        bit 31~0 - tpsf_in_lane7[31:0] (Read/Write)
// 0x68 : Data signal of tpsf_in_lane7
//        bit 31~0 - tpsf_in_lane7[63:32] (Read/Write)
// 0x6c : reserved
// 0x70 : Data signal of sfd_out_lane0
//        bit 31~0 - sfd_out_lane0[31:0] (Read/Write)
// 0x74 : Data signal of sfd_out_lane0
//        bit 31~0 - sfd_out_lane0[63:32] (Read/Write)
// 0x78 : reserved
// 0x7c : Data signal of sfd_out_lane1
//        bit 31~0 - sfd_out_lane1[31:0] (Read/Write)
// 0x80 : Data signal of sfd_out_lane1
//        bit 31~0 - sfd_out_lane1[63:32] (Read/Write)
// 0x84 : reserved
// 0x88 : Data signal of sfd_out_lane2
//        bit 31~0 - sfd_out_lane2[31:0] (Read/Write)
// 0x8c : Data signal of sfd_out_lane2
//        bit 31~0 - sfd_out_lane2[63:32] (Read/Write)
// 0x90 : reserved
// 0x94 : Data signal of sfd_out_lane3
//        bit 31~0 - sfd_out_lane3[31:0] (Read/Write)
// 0x98 : Data signal of sfd_out_lane3
//        bit 31~0 - sfd_out_lane3[63:32] (Read/Write)
// 0x9c : reserved
// 0xa0 : Data signal of sfd_out_lane4
//        bit 31~0 - sfd_out_lane4[31:0] (Read/Write)
// 0xa4 : Data signal of sfd_out_lane4
//        bit 31~0 - sfd_out_lane4[63:32] (Read/Write)
// 0xa8 : reserved
// 0xac : Data signal of sfd_out_lane5
//        bit 31~0 - sfd_out_lane5[31:0] (Read/Write)
// 0xb0 : Data signal of sfd_out_lane5
//        bit 31~0 - sfd_out_lane5[63:32] (Read/Write)
// 0xb4 : reserved
// 0xb8 : Data signal of sfd_out_lane6
//        bit 31~0 - sfd_out_lane6[31:0] (Read/Write)
// 0xbc : Data signal of sfd_out_lane6
//        bit 31~0 - sfd_out_lane6[63:32] (Read/Write)
// 0xc0 : reserved
// 0xc4 : Data signal of sfd_out_lane7
//        bit 31~0 - sfd_out_lane7[31:0] (Read/Write)
// 0xc8 : Data signal of sfd_out_lane7
//        bit 31~0 - sfd_out_lane7[63:32] (Read/Write)
// 0xcc : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL            0x00
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_GIE                0x04
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_IER                0x08
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_ISR                0x0c
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE0_DATA 0x10
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE0_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE1_DATA 0x1c
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE1_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE2_DATA 0x28
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE2_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE3_DATA 0x34
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE3_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE4_DATA 0x40
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE4_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE5_DATA 0x4c
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE5_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE6_DATA 0x58
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE6_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE7_DATA 0x64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_TPSF_IN_LANE7_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE0_DATA 0x70
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE0_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE1_DATA 0x7c
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE1_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE2_DATA 0x88
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE2_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE3_DATA 0x94
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE3_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE4_DATA 0xa0
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE4_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE5_DATA 0xac
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE5_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE6_DATA 0xb8
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE6_DATA 64
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE7_DATA 0xc4
#define XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_BITS_SFD_OUT_LANE7_DATA 64

