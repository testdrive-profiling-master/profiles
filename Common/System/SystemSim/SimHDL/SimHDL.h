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
// Title : Simulation HDL module
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SIM_HDL_H__
#define __SIM_HDL_H__
#include "STDInterface.h"
#include "SystemConfig.h"
#include "VirtualDisplayConfig.h"
#include "dpi_interfaces.h"

#ifndef svBitVecVal
typedef unsigned int svBitVecVal;
#endif

struct SimControl {
	virtual BUS_SLAVE_INTERFACE *CreateSlave(uint64_t ulAddrBase, uint64_t ulAddrHigh)		 = 0;
	virtual BUS_SLAVE_INTERFACE *FindSlave(uint64_t ulAddress)								 = 0;
	virtual CLOCK_INTERFACE		*CreateClock(uint8_t *pCLK, uint8_t *pRST)					 = 0;
	virtual CLOCK_INTERFACE		*FindClock(uint8_t *pCLK)									 = 0;
	virtual bool				 AwakeInterrupt(void)										 = 0; // true(Awaked), false(Pending)
	virtual void				 SetMemoryBaseAddress(uint64_t ulAddress)					 = 0;
	virtual void				 MemoryRead32(int iID, uint64_t ulAddress, uint32_t &dwData) = 0;
	virtual void				 MemoryWrite32(int iID, uint64_t ulAddress, uint32_t dwData) = 0;
	virtual void				 MemoryRead16(int iID, uint64_t ulAddress, uint16_t &dwData) = 0;
	virtual void				 MemoryWrite16(int iID, uint64_t ulAddress, uint16_t dwData) = 0;
	virtual void				 MemoryRead8(int iID, uint64_t ulAddress, uint8_t &dwData)	 = 0;
	virtual void				 MemoryWrite8(int iID, uint64_t ulAddress, uint8_t dwData)	 = 0;
	virtual SYSTEM_CONFIG		*GetSystemConfig(void)										 = 0;
	virtual DisplayConfig		*GetDisplayConfig(void)										 = 0;
	virtual uint8_t				*GetMemoryPointer(uint64_t ulAddress, uint64_t ulSize = 0, bool bDisplay = false) = 0;
	virtual uint64_t			 GetMemoryBaseAddress(void)														  = 0;
	virtual bool				 GetMemory(const char *sName, void *&pConfig, void *&pMemory)					  = 0;
	virtual void				 SimulationLock(int iDelayTicks = 0)											  = 0;
	virtual void				 SimulationUnLock(void)															  = 0;
	virtual void				 SimulationAddBusy(uint8_t *pBusy)												  = 0;
	virtual void				 SimulationDebugMode(bool bDebug = true)										  = 0;
	virtual void				 SetSystemDescription(const char *sDesc)										  = 0;
	virtual void				 SetError(bool bError = true)													  = 0;
	virtual void				 RegisterDriverCommand(DRIVER_COMMAND func)										  = 0;
};

struct SimHDL {
	virtual bool Initialize(void) = 0;
	virtual bool IsTrace(void)	  = 0;
	virtual bool Eval(void)		  = 0;
	virtual void Release(void)	  = 0;
};

extern "C" SimHDL  *CreateSimHDL(SimControl *pControl);
extern "C" void		SimulationFlush(void);
extern "C" void		VerilatorFlush(void);
extern "C" uint64_t SimulationTime(void);
extern "C" void		AdvenceSimulationTime(uint32_t dwTime);

#endif //_SIM_HDL_H_
