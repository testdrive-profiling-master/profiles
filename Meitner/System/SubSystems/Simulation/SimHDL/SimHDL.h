//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : Simulation HDL module
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef _SIM_HDL_H_
#define _SIM_HDL_H_
#include "STDInterface.h"
#include "SystemConfig.h"
#include "VirtualDisplayConfig.h"
#include "dpi_interfaces.h"

#ifdef SIM_HDL_EXPORTS
	#define SIM_HDL_API __declspec(dllexport)
#else
	#define SIM_HDL_API __declspec(dllimport)
	#pragma comment(lib, "SimHDL.lib")
#endif

#ifndef svBitVecVal
typedef unsigned int	svBitVecVal;
#endif

interface SimControl{
	STDMETHOD_(BUS_SLAVE_INTERFACE*, CreateSlave)(DWORD dwAddrBase, DWORD dwAddrHigh) PURE;
	STDMETHOD_(BUS_SLAVE_INTERFACE*, FindSlave)(DWORD dwAddress) PURE;
	STDMETHOD_(CLOCK_INTERFACE*, CreateClock)(BYTE* pCLK, BYTE* pRST) PURE;
	STDMETHOD_(BOOL, AwakeInterrupt)(void) PURE;	// TRUE(Awaked), FALSE(Pending)
	STDMETHOD_(void, SetMemoryBaseAddress)(DWORD dwAddress) PURE;
	STDMETHOD_(void, MemoryRead32)(int iID, DWORD dwAddress, DWORD& dwData) PURE;
	STDMETHOD_(void, MemoryWrite32)(int iID, DWORD dwAddress, DWORD dwData) PURE;
	STDMETHOD_(void, MemoryRead16)(int iID, DWORD dwAddress, WORD& dwData) PURE;
	STDMETHOD_(void, MemoryWrite16)(int iID, DWORD dwAddress, WORD dwData) PURE;
	STDMETHOD_(void, MemoryRead8)(int iID, DWORD dwAddress, BYTE& dwData) PURE;
	STDMETHOD_(void, MemoryWrite8)(int iID, DWORD dwAddress, BYTE dwData) PURE;
	STDMETHOD_(SYSTEM_CONFIG*, GetSystemConfig)(void) PURE;
	STDMETHOD_(DisplayConfig*, GetDisplayConfig)(void) PURE;
	STDMETHOD_(BYTE*, GetMemoryPointer)(DWORD dwAddress, DWORD dwSize = 0, BOOL bDisplay = FALSE) PURE;
	STDMETHOD_(DWORD, GetMemoryBaseAddress)(void) PURE;
	STDMETHOD_(BOOL, GetMemory)(const char* sName, void*& pConfig, void*& pMemory) PURE;
	STDMETHOD_(void, SimulationLock)(int iDelayTicks = 0) PURE;
	STDMETHOD_(void, SimulationUnLock)(void) PURE;
	STDMETHOD_(void, SimulationAddBusy)(BYTE* pBusy) PURE;
	STDMETHOD_(void, SimulationDebugMode)(BOOL bDebug = TRUE) PURE;
	STDMETHOD_(void, SetSystemDescription)(const char* sDesc) PURE;
};

interface SimHDL{
	STDMETHOD_(BOOL, Initialize)(void) PURE;
	STDMETHOD_(BOOL, IsTrace)(void) PURE;
	STDMETHOD_(BOOL, Eval)(void) PURE;
	STDMETHOD_(void, Release)(void) PURE;
};

extern "C" SIM_HDL_API SimHDL* CreateSimHDL(SimControl* pControl);
extern "C" SIM_HDL_API void SimulationFlush(void);
extern "C" SIM_HDL_API UINT64 SimulationTime(void);
extern "C" SIM_HDL_API void AdvenceSimulationTime(DWORD dwTime);

#endif//_SIM_HDL_H_
