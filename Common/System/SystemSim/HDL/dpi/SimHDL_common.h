//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
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
#ifndef SIMHDL_LIB
#	include "../verilator/SimTop__Dpi.h"
#endif

#define LOGI(...)		  printf("*I: [SimHDL] " __VA_ARGS__)
#define LOGE(...)		  printf("*E: [SimHDL] " __VA_ARGS__)
#define DPI_FUNCTION	  extern "C"
#define GET_SYSTEM_REGMAP ((SYSTEM_REGMAP *)GetMemoryPointer(0, 0, true))

// Non-DPI
extern uint64_t g_lSimulationTime;
SYSTEM_CONFIG  *GetSystemConfig(void);
DisplayConfig  *GetDisplayConfig(void);
BYTE		   *GetMemoryPointer(uint64_t lAddress, uint32_t dwSize = 0, bool bDisplay = false);
uint64_t		GetMemoryBaseAddress(void);
bool			GetMemory(const char *sName, void *&pConfig, void *&pMemory);
const char	   *GetConfigString(const char *sKeyName, const char *sDefault = NULL);
void			SetConfigString(const char *sKeyName, const char *sData = NULL);
int				GetConfigInt(const char *sKeyName, int iDefault = 0);
void			SetConfigInt(const char *sKeyName, int iData);
extern bool (*DPI_Initialize)(void);
extern void (*DPI_Finalize)(void);

// DPI functions
DPI_FUNCTION void	  SimulationLock(int iDelayTicks = 0);
DPI_FUNCTION void	  SimulationUnLock(void);
DPI_FUNCTION void	  SimulationQuit(bool bError = false);
DPI_FUNCTION void	  SimulationStop(void);
DPI_FUNCTION void	  SimulationFlush(void);
DPI_FUNCTION void	  SimulationDebugMode(bool bDebug = true);
DPI_FUNCTION uint64_t SimulationTime(void);
DPI_FUNCTION void	  SetSystemDescription(const char *sDesc);

#endif //__SIMHDL_COMMON_H__
