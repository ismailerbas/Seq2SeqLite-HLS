// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xstudent_infer_frame_streaming8.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XStudent_infer_frame_streaming8_CfgInitialize(XStudent_infer_frame_streaming8 *InstancePtr, XStudent_infer_frame_streaming8_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XStudent_infer_frame_streaming8_Start(XStudent_infer_frame_streaming8 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL) & 0x80;
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XStudent_infer_frame_streaming8_IsDone(XStudent_infer_frame_streaming8 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XStudent_infer_frame_streaming8_IsIdle(XStudent_infer_frame_streaming8 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XStudent_infer_frame_streaming8_IsReady(XStudent_infer_frame_streaming8 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XStudent_infer_frame_streaming8_EnableAutoRestart(XStudent_infer_frame_streaming8 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XStudent_infer_frame_streaming8_DisableAutoRestart(XStudent_infer_frame_streaming8 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_AP_CTRL, 0);
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane0(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE0_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE0_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane0(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE0_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE0_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane1(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE1_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE1_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane1(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE1_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE1_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane2(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE2_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE2_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane2(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE2_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE2_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane3(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE3_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE3_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane3(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE3_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE3_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane4(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE4_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE4_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane4(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE4_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE4_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane5(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE5_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE5_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane5(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE5_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE5_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane6(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE6_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE6_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane6(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE6_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE6_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane7(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE7_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE7_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane7(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE7_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_TPSF_IN_LANE7_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane0(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE0_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE0_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane0(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE0_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE0_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane1(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE1_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE1_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane1(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE1_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE1_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane2(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE2_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE2_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane2(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE2_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE2_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane3(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE3_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE3_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane3(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE3_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE3_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane4(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE4_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE4_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane4(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE4_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE4_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane5(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE5_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE5_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane5(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE5_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE5_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane6(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE6_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE6_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane6(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE6_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE6_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_Set_sfd_out_lane7(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE7_DATA, (u32)(Data));
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE7_DATA + 4, (u32)(Data >> 32));
}

u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane7(XStudent_infer_frame_streaming8 *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE7_DATA);
    Data += (u64)XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_SFD_OUT_LANE7_DATA + 4) << 32;
    return Data;
}

void XStudent_infer_frame_streaming8_InterruptGlobalEnable(XStudent_infer_frame_streaming8 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_GIE, 1);
}

void XStudent_infer_frame_streaming8_InterruptGlobalDisable(XStudent_infer_frame_streaming8 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_GIE, 0);
}

void XStudent_infer_frame_streaming8_InterruptEnable(XStudent_infer_frame_streaming8 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_IER);
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_IER, Register | Mask);
}

void XStudent_infer_frame_streaming8_InterruptDisable(XStudent_infer_frame_streaming8 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_IER);
    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_IER, Register & (~Mask));
}

void XStudent_infer_frame_streaming8_InterruptClear(XStudent_infer_frame_streaming8 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XStudent_infer_frame_streaming8_WriteReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_ISR, Mask);
}

u32 XStudent_infer_frame_streaming8_InterruptGetEnabled(XStudent_infer_frame_streaming8 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_IER);
}

u32 XStudent_infer_frame_streaming8_InterruptGetStatus(XStudent_infer_frame_streaming8 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XStudent_infer_frame_streaming8_ReadReg(InstancePtr->Control_BaseAddress, XSTUDENT_INFER_FRAME_STREAMING8_CONTROL_ADDR_ISR);
}

