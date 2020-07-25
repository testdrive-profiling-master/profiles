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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "MMCM.h"
#include <math.h>
#include <assert.h>

typedef enum {
	MMCM_REG_CLKOUT5_1		= 0x06,
	MMCM_REG_CLKOUT5_2,
	MMCM_REG_CLKOUT0_1,
	MMCM_REG_CLKOUT0_2,
	MMCM_REG_CLKOUT1_1,
	MMCM_REG_CLKOUT1_2,
	MMCM_REG_CLKOUT2_1,
	MMCM_REG_CLKOUT2_2,
	MMCM_REG_CLKOUT3_1,
	MMCM_REG_CLKOUT3_2,
	MMCM_REG_CLKOUT4_1,
	MMCM_REG_CLKOUT4_2,
	MMCM_REG_CLKOUT6_1,
	MMCM_REG_CLKOUT6_2,
	MMCM_REG_FBOUT_1,
	MMCM_REG_FBOUT_2,
	MMCM_REG_DIVLCK,
	MMCM_REG_LOCK_1			= 0x18,
	MMCM_REG_LOCK_2,
	MMCM_REG_LOCK_3,
	MMCM_REG_POWER			= 0x28,
	MMCM_REG_FILTER_1		= 0x4E,
	MMCM_REG_FILTER_2
} MMCM_REG;

MMCM::MMCM(IMMCM* pMMCM, float CLKIN_MHz)
{
	assert(pMMCM != NULL);
	m_fCLKIN	= CLKIN_MHz;
	m_pMMCM		= pMMCM;
	m_bEnable	= TRUE;
	m_dwID		= 0;
	memset(&m_Signal, 0, sizeof(m_Signal));
}

MMCM::~MMCM(void)
{
}

void MMCM::Enable(BOOL bEnable)
{
	m_bEnable	= bEnable;
}

void MMCM::Reset(BOOL bRST)
{
	m_Signal.bRST	= bRST;
	UpdateSignal();
	Write(MMCM_REG_POWER, bRST ? 0 : 0xFFFF);
}

float MMCM::FindClock(float ReqMHz, DWORD& D, DWORD& M, DWORD& O)
{
	float	ideal_clk	= 0;
	{
		float	diff	= 100000000000.f;

		for(int i_D = 1; i_D <= 106; i_D++) {
			if((m_fCLKIN / i_D) < 30.f) continue;	// divider limit

			for(int i_O = 1; i_O <= 128; i_O++) {
				for(int i_M = i_D * 4; i_M <= 64 && i_M <= i_D * 16; i_M++) {
					float	new_freq	= (m_fCLKIN * i_M) / (i_D * i_O);
					float	new_diff	= abs(ReqMHz - new_freq);

					if((m_fCLKIN * i_M / i_D) > 2800.f) break;	// multiplier limit

					if(new_diff < diff) {
						diff		= new_diff;
						ideal_clk	= new_freq;
						D			= i_D;
						M			= i_M;
						O			= i_O;
					}

					if(new_freq > ideal_clk) break;
				}
			}
		}
	}
	return ideal_clk;
}

float MMCM::SetClock(DWORD dwID, float mHz)
{
	float	actual_clk;
	m_dwID	= dwID;
	Reset();		// reset
	DWORD	D, M, O;
	actual_clk	= FindClock(mHz, D, M, O);
	SetDivide(D);
	SetOutputDivide(0, O);
	SetMultiplier(M);
	Reset(FALSE);	// un-reset
	WaitLock();
	return actual_clk;
}

typedef union {
	WORD		all;
	struct {
		WORD	lowTime		: 6;
		WORD	hiTime		: 6;
		WORD	no_count	: 1;
		WORD	edge		: 1;
		WORD	/*dummy*/	: 0;
	};
} MMCM_DIVREG;

typedef union {
	DWORD	all;
	struct {
		union {
			WORD		all;
			struct {
				WORD	lowTime		: 6;
				WORD	hiTime		: 6;
				WORD	RESERVED	: 1;
				WORD	phase_mux	: 3;
				WORD	/*dummy*/	: 0;
			};
		} m1;
		union {
			WORD		all;
			struct {
				WORD	delayTime	: 6;
				WORD	no_count	: 1;
				WORD	edge		: 1;
				WORD	mx			: 2;	// must be set to 2'b00
				WORD	frac_wf_r	: 1;	// clk0 only
				WORD	frac_en		: 1;	// clk0 only
				WORD	frac		: 3;	// clk0 only
				WORD	/*dummy*/	: 0;
			};
		} m2;
	};
} MMCM_CLKREG;

void MMCM::SetDivide(DWORD dwNum)
{
	MMCM_DIVREG	reg;
	assert(dwNum != 0);
	assert(dwNum <= 80);
	reg.all		= 0;
	reg.hiTime	= dwNum / 2;
	reg.lowTime	= dwNum - reg.hiTime;

	if(!reg.hiTime) {
		reg.no_count	= 1;
		reg.lowTime		= 0;
	} else {
		if(reg.lowTime != reg.hiTime)
			reg.edge	= 1;
	}

	Write(MMCM_REG_DIVLCK, reg.all);
}

void MMCM::SetOutputDivide(DWORD dwID, DWORD dwNum)
{
	MMCM_CLKREG	reg;
	assert(dwNum != 0);
	assert(dwNum <= 128);
	reg.all			= 0;
	reg.m1.RESERVED	= 1;
	reg.m1.hiTime	= dwNum / 2;
	reg.m1.lowTime	= dwNum - reg.m1.hiTime;

	if(!reg.m1.hiTime) {
		reg.m2.no_count		= 1;
		reg.m1.lowTime		= 0;
	} else {
		if(reg.m1.lowTime != reg.m1.hiTime)
			reg.m2.edge		= 1;
	}

	{
		DWORD reg_index;

		switch(dwID) {
		case 0:
			reg_index	= MMCM_REG_CLKOUT0_1;
			break;

		case 1:
			reg_index	= MMCM_REG_CLKOUT1_1;
			break;

		case 2:
			reg_index	= MMCM_REG_CLKOUT2_1;
			break;

		case 3:
			reg_index	= MMCM_REG_CLKOUT3_1;
			break;

		case 4:
			reg_index	= MMCM_REG_CLKOUT4_1;
			break;

		case 5:
			reg_index	= MMCM_REG_CLKOUT5_1;
			break;

		case 6:
			reg_index	= MMCM_REG_CLKOUT6_1;
			break;

		default:
			reg_index	= MMCM_REG_FBOUT_1;
			break;
		}

		Write(reg_index, reg.m1.all);
		Write(reg_index + 1, reg.m2.all);
	}
}

DWORD MMCM::Read(DWORD dwAddr)
{
	MMCM_READ info;

	if(!IsEnable()) return 0;

	m_Signal.bEN		= TRUE;
	m_Signal.bWE		= FALSE;
	m_Signal.dwAddr		= dwAddr;
	UpdateSignal();
	WaitReady();
	m_pMMCM->MMCM_Get(m_dwID, info);
	m_Signal.bEN		= FALSE;
	UpdateSignal();
	return info.rdata;
}

void MMCM::Write(DWORD dwAddr, DWORD dwData)
{
	if(!IsEnable()) return;

	m_Signal.bEN		= TRUE;
	m_Signal.bWE		= FALSE;
	m_Signal.dwAddr		= dwAddr;
	m_Signal.dwData		= dwData;
	UpdateSignal();
	WaitReady();
	m_Signal.bWE		= TRUE;
	UpdateSignal();
	WaitReady();
	m_Signal.bWE		= FALSE;
	m_Signal.bEN		= FALSE;
	UpdateSignal();
}

void MMCM::WaitLock(void)
{
	MMCM_READ info;

	while(IsEnable()) {
		m_pMMCM->MMCM_Get(m_dwID, info);

		if(info.bLocked) break;
	}
}

void MMCM::WaitReady(void)
{
	MMCM_READ info;

	while(IsEnable()) {
		m_pMMCM->MMCM_Get(m_dwID, info);

		if(info.bReady) break;
	}
}

void MMCM::UpdateSignal(void)
{
	if(IsEnable())
		m_pMMCM->MMCM_Set(m_dwID, m_Signal.bRST, m_Signal.bEN, m_Signal.bWE, m_Signal.dwAddr, m_Signal.dwData);
}
