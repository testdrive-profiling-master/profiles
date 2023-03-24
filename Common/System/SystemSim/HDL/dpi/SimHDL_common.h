//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : Common DPI
// Rev.  : 3/24/2023 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __SIMHDL_COMMON_H__
#define __SIMHDL_COMMON_H__
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "TestDriver.h"
#include "SystemConfig.h"
#include "vltstd/svdpi.h"
#include "VirtualDisplayConfig.h"
#include "dpi_interfaces.h"
#include "../verilator/SimTop__Dpi.h"

#define LOGI(...)			printf("*I: [SimHDL] " __VA_ARGS__)
#define LOGE(...)			printf("*E: [SimHDL] " __VA_ARGS__)
#define DPI_FUNCTION		extern "C"
#define GET_SYSTEM_REGMAP	((SYSTEM_REGMAP*)GetMemoryPointer(0, 0, true))

// Non-DPI
extern UINT64	g_lSimulationTime;
SYSTEM_CONFIG* GetSystemConfig(void);
DisplayConfig* GetDisplayConfig(void);
BYTE* GetMemoryPointer(UINT64 lAddress, DWORD dwSize = 0, bool bDisplay = false);
UINT64 GetMemoryBaseAddress(void);
bool GetMemory(const char* sName, void*& pConfig, void*& pMemory);
extern bool (*DPI_Initialize)(void);
extern void (*DPI_Finalize)(void);

// DPI functions
DPI_FUNCTION void SimulationLock(int iDelayTicks = 0);
DPI_FUNCTION void SimulationUnLock(void);
DPI_FUNCTION void SimulationQuit(bool bError = false);
DPI_FUNCTION void SimulationStop(void);
DPI_FUNCTION void SimulationFlush(void);
DPI_FUNCTION void SimulationDebugMode(bool bDebug = true);
DPI_FUNCTION UINT64 SimulationTime(void);
DPI_FUNCTION void SetSystemDescription(const char* sDesc);

#endif//__SIMHDL_COMMON_H__
