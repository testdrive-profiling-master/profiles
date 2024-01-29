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
// Title : Testbench
// Rev.  : 1/29/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "DUT.h"

typedef union {	// write data desc. for DUT's clock generator
	DWORD	all;
	struct {
		DWORD	wdata		: 16;
		DWORD	addr		: 7;
		DWORD	we			: 1;
		DWORD	en			: 1;
		DWORD	rst			: 1;
		DWORD	/*dummy*/	: 0;
	};
} CLOCKGEN_WDATA;

typedef union {	// read data desc. for DUT's clock generator
	DWORD	all;
	struct {
		DWORD	rdata		: 16;
		DWORD	locked		: 1;
		DWORD	ready		: 1;
		DWORD	/*dummy*/	: 12;
		DWORD	intr		: 1;
		DWORD	busy		: 1;
	};
} CLOCKGEN_RDATA;

DUT::DUT(DDK* pDDK) : m_MMCM(this, BUS_CLOCK_MHZ)	// 250Mhz reference clock
{
	printf("[DUT] Current bus clock : %.2fMHz\n", m_MMCM.GetReferenceClock());
	m_pDDK	= pDDK;

	if(strstr(m_pDDK->GetSystemDescription(), "Simplified"))	// skip on simulation/emulation
		m_MMCM.Enable(FALSE);
}

DUT::~DUT(void)
{
}

void DUT::SetClock(float mHz)
{
	printf("Set DUT Processor clock : %.2fMhz\n", m_MMCM.SetClock(0, mHz));
}

BOOL DUT::IsBusy(void)
{
	CLOCKGEN_RDATA	reg;
	reg.all	= m_pDDK->RegRead(DUT_CLOCKGEN_BASE);
	return (BOOL)reg.busy;
}

void DUT::MMCM_Set(DWORD dwID, BOOL bRST, BOOL bEN, BOOL bWE, DWORD dwAddr, DWORD dwData)
{
	CLOCKGEN_WDATA	reg;
	reg.wdata	= dwData;
	reg.addr	= dwAddr;
	reg.we		= bWE;
	reg.en		= bEN;
	reg.rst		= bRST;
	m_pDDK->RegWrite(DUT_CLOCKGEN_BASE, reg.all);
}

void DUT::MMCM_Get(DWORD dwID, MMCM_READ& info)
{
	CLOCKGEN_RDATA	reg;
	reg.all	= m_pDDK->RegRead(DUT_CLOCKGEN_BASE);
	info.bLocked	= reg.locked;
	info.bReady		= reg.ready;
	info.rdata		= reg.rdata;
}
