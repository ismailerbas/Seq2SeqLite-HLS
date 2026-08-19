// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XSTUDENT_INFER_FRAME_STREAMING8_H
#define XSTUDENT_INFER_FRAME_STREAMING8_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xstudent_infer_frame_streaming8_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u64 Control_BaseAddress;
} XStudent_infer_frame_streaming8_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XStudent_infer_frame_streaming8;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XStudent_infer_frame_streaming8_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XStudent_infer_frame_streaming8_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XStudent_infer_frame_streaming8_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XStudent_infer_frame_streaming8_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XStudent_infer_frame_streaming8_Initialize(XStudent_infer_frame_streaming8 *InstancePtr, u16 DeviceId);
XStudent_infer_frame_streaming8_Config* XStudent_infer_frame_streaming8_LookupConfig(u16 DeviceId);
int XStudent_infer_frame_streaming8_CfgInitialize(XStudent_infer_frame_streaming8 *InstancePtr, XStudent_infer_frame_streaming8_Config *ConfigPtr);
#else
int XStudent_infer_frame_streaming8_Initialize(XStudent_infer_frame_streaming8 *InstancePtr, const char* InstanceName);
int XStudent_infer_frame_streaming8_Release(XStudent_infer_frame_streaming8 *InstancePtr);
#endif

void XStudent_infer_frame_streaming8_Start(XStudent_infer_frame_streaming8 *InstancePtr);
u32 XStudent_infer_frame_streaming8_IsDone(XStudent_infer_frame_streaming8 *InstancePtr);
u32 XStudent_infer_frame_streaming8_IsIdle(XStudent_infer_frame_streaming8 *InstancePtr);
u32 XStudent_infer_frame_streaming8_IsReady(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_EnableAutoRestart(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_DisableAutoRestart(XStudent_infer_frame_streaming8 *InstancePtr);

void XStudent_infer_frame_streaming8_Set_tpsf_in_lane0(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane0(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane1(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane1(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane2(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane2(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane3(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane3(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane4(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane4(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane5(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane5(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane6(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane6(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_tpsf_in_lane7(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_tpsf_in_lane7(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane0(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane0(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane1(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane1(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane2(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane2(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane3(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane3(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane4(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane4(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane5(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane5(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane6(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane6(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_Set_sfd_out_lane7(XStudent_infer_frame_streaming8 *InstancePtr, u64 Data);
u64 XStudent_infer_frame_streaming8_Get_sfd_out_lane7(XStudent_infer_frame_streaming8 *InstancePtr);

void XStudent_infer_frame_streaming8_InterruptGlobalEnable(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_InterruptGlobalDisable(XStudent_infer_frame_streaming8 *InstancePtr);
void XStudent_infer_frame_streaming8_InterruptEnable(XStudent_infer_frame_streaming8 *InstancePtr, u32 Mask);
void XStudent_infer_frame_streaming8_InterruptDisable(XStudent_infer_frame_streaming8 *InstancePtr, u32 Mask);
void XStudent_infer_frame_streaming8_InterruptClear(XStudent_infer_frame_streaming8 *InstancePtr, u32 Mask);
u32 XStudent_infer_frame_streaming8_InterruptGetEnabled(XStudent_infer_frame_streaming8 *InstancePtr);
u32 XStudent_infer_frame_streaming8_InterruptGetStatus(XStudent_infer_frame_streaming8 *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
