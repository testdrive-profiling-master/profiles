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
#ifndef __DPI_INTERFACES_H__
#define __DPI_INTERFACES_H__
#include "STDInterface.h"

//-----------------------------------------------------------------------
// Slave bus interface
//-----------------------------------------------------------------------
typedef struct {
	uint64_t lAddr;
	uint32_t dwData;
} BUS_SALVE_PACKET;

struct BUS_SLAVE_INTERFACE {
	virtual bool			  RequestWrite(uint64_t dwAddr, uint32_t dwData) = 0;
	virtual bool			  WaitWrite(void)								 = 0;
	virtual bool			  RequestRead(uint64_t dwAddr)					 = 0;
	virtual bool			  WaitRead(uint32_t &dwData)					 = 0;

	virtual BUS_SALVE_PACKET *GetWrite(void) = 0;
	virtual void			  WriteAck(void) = 0;

	virtual BUS_SALVE_PACKET *GetRead(void) = 0;
	virtual void			  ReadAck(void) = 0;
};

BUS_SLAVE_INTERFACE *CreateSlave(uint64_t lAddrBase, uint64_t lddrHigh);
BUS_SLAVE_INTERFACE *FindSlave(uint64_t lAddress);

//-----------------------------------------------------------------------
// clocking interface
//-----------------------------------------------------------------------
struct CLOCK_INTERFACE {
	virtual void SetParameters(
		uint32_t dwID, uint32_t dwPeriod, uint8_t bInitValue = 0, uint32_t dwPhase = 0, uint8_t ClockPolarity = 1,
		uint8_t ResetPolarity = 0)				= 0;
	virtual void DoReset(uint32_t dwCycles = 8) = 0;
	virtual void Release(void)					= 0;
};

CLOCK_INTERFACE *FindClock(uint8_t *pCLK);

//-----------------------------------------------------------------------
// Driver command interface
//-----------------------------------------------------------------------
typedef uint32_t (*DRIVER_COMMAND)(void *pCommand);
void RegisterDriverCommand(DRIVER_COMMAND func);

#endif //__DPI_INTERFACES_H__
