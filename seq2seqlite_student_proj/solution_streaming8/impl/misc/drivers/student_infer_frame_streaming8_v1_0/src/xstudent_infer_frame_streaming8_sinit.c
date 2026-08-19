// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
// Tool Version Limit: 2019.12
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xstudent_infer_frame_streaming8.h"

extern XStudent_infer_frame_streaming8_Config XStudent_infer_frame_streaming8_ConfigTable[];

XStudent_infer_frame_streaming8_Config *XStudent_infer_frame_streaming8_LookupConfig(u16 DeviceId) {
	XStudent_infer_frame_streaming8_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XSTUDENT_INFER_FRAME_STREAMING8_NUM_INSTANCES; Index++) {
		if (XStudent_infer_frame_streaming8_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XStudent_infer_frame_streaming8_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XStudent_infer_frame_streaming8_Initialize(XStudent_infer_frame_streaming8 *InstancePtr, u16 DeviceId) {
	XStudent_infer_frame_streaming8_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XStudent_infer_frame_streaming8_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XStudent_infer_frame_streaming8_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

