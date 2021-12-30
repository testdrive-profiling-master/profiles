//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Simulation HDL module
// Rev.  : 12/30/2021 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SIM_HDL_H__
#define __SIM_HDL_H__
#include "STDInterface.h"
#include "SystemConfig.h"
#include "VirtualDisplayConfig.h"
#include "dpi_interfaces.h"

#ifndef svBitVecVal
typedef unsigned int	svBitVecVal;
#endif

struct SimControl {
	virtual BUS_SLAVE_INTERFACE* CreateSlave(DWORD dwAddrBase, DWORD dwAddrHigh) = 0;
	virtual BUS_SLAVE_INTERFACE* FindSlave(DWORD dwAddress) = 0;
	virtual CLOCK_INTERFACE* CreateClock(BYTE* pCLK, BYTE* pRST) = 0;
	virtual CLOCK_INTERFACE* FindClock(BYTE* pCLK) = 0;
	virtual BOOL AwakeInterrupt(void) = 0;	// TRUE(Awaked), FALSE(Pending)
	virtual void SetMemoryBaseAddress(DWORD dwAddress) = 0;
	virtual void MemoryRead32(int iID, DWORD dwAddress, DWORD& dwData) = 0;
	virtual void MemoryWrite32(int iID, DWORD dwAddress, DWORD dwData) = 0;
	virtual void MemoryRead16(int iID, DWORD dwAddress, WORD& dwData) = 0;
	virtual void MemoryWrite16(int iID, DWORD dwAddress, WORD dwData) = 0;
	virtual void MemoryRead8(int iID, DWORD dwAddress, BYTE& dwData) = 0;
	virtual void MemoryWrite8(int iID, DWORD dwAddress, BYTE dwData) = 0;
	virtual SYSTEM_CONFIG* GetSystemConfig(void) = 0;
	virtual DisplayConfig* GetDisplayConfig(void) = 0;
	virtual BYTE* GetMemoryPointer(DWORD dwAddress, DWORD dwSize = 0, BOOL bDisplay = FALSE) = 0;
	virtual DWORD GetMemoryBaseAddress(void) = 0;
	virtual BOOL GetMemory(const char* sName, void*& pConfig, void*& pMemory) = 0;
	virtual void SimulationLock(int iDelayTicks = 0) = 0;
	virtual void SimulationUnLock(void) = 0;
	virtual void SimulationAddBusy(BYTE* pBusy) = 0;
	virtual void SimulationDebugMode(BOOL bDebug = TRUE) = 0;
	virtual void SetSystemDescription(const char* sDesc) = 0;
	virtual void SetError(bool bError = true) = 0;
};

struct SimHDL {
	virtual BOOL Initialize(void) = 0;
	virtual BOOL IsTrace(void) = 0;
	virtual BOOL Eval(void) = 0;
	virtual void Release(void) = 0;
};

extern "C" SimHDL* CreateSimHDL(SimControl* pControl);
extern "C" void SimulationFlush(void);
extern "C" UINT64 SimulationTime(void);
extern "C" void AdvenceSimulationTime(DWORD dwTime);

#endif//_SIM_HDL_H_
