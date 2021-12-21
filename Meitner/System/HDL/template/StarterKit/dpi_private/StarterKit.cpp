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
// Title : Template design
// Rev.  : 12/21/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "StarterKit.h"

static StarterKit		__starter_kit;

StarterKit::StarterKit(void)
{
	m_pReg				= NULL;
}

StarterKit::~StarterKit(void)
{
}

void StarterKit::Initialize(void)
{
	m_pReg				= (STARTERKIT_REGMAP*)GET_SYSTEM_REGMAP;
	m_pReg->magic_code	= SYSTEM_MAGIC_CODE;
	memset(&m_pReg->led, 0, sizeof(m_pReg->led));
	m_pReg->led.bUpdate	= true;
}

void StarterKit::LED(DWORD pins)
{
	for(int i = 0; i < 8; i++) {
		m_LEDs[i].Eval(pins);
		pins	>>= 1;
	}
}

void StarterKit::NumericDisplay(DWORD pins)
{
	union {		// KW4-56NCWB-P-Y
		DWORD		pins;
		struct {
			DWORD	E		: 1;	// pin #1
			DWORD	D		: 1;	// pin #2
			DWORD	DP		: 1;	// pin #3
			DWORD	C		: 1;	// pin #4
			DWORD	G		: 1;	// pin #5
			DWORD	COM1	: 1;	// pin #6
			DWORD	COM3	: 1;	// pin #7
			DWORD	MID		: 1;	// pin #8
			DWORD	B		: 1;	// pin #9
			DWORD	COM2	: 1;	// pin #10
			DWORD	COM4	: 1;	// pin #11
			DWORD	F		: 1;	// pin #12
			DWORD	A		: 1;	// pin #13
			DWORD	COM5	: 1;	// pin #14
			DWORD	/*reserved*/	: 0;
		};
	} nd;
	nd.pins	= pins;
	DWORD	num_en[4] = {
		~nd.COM1,
		~nd.COM2,
		~nd.COM4,
		~nd.COM5
	};

	for(int i = 0; i < 4; i++) {
		m_NumericDisplay.num[i].segment[0].Eval(nd.A & num_en[i]);
		m_NumericDisplay.num[i].segment[1].Eval(nd.B & num_en[i]);
		m_NumericDisplay.num[i].segment[2].Eval(nd.C & num_en[i]);
		m_NumericDisplay.num[i].segment[3].Eval(nd.D & num_en[i]);
		m_NumericDisplay.num[i].segment[4].Eval(nd.E & num_en[i]);
		m_NumericDisplay.num[i].segment[5].Eval(nd.F & num_en[i]);
		m_NumericDisplay.num[i].segment[6].Eval(nd.G & num_en[i]);
		m_NumericDisplay.num[i].DP.Eval(nd.DP & num_en[i]);
	}

	m_NumericDisplay.mid.Eval(nd.MID & ~nd.COM3);
}

void StarterKit::Eval(void)
{
	if(!m_pReg) return;

	for(int i = 0; i < 8; i++) {
		m_pReg->led.val[i]	= m_LEDs[i].Level();
	}
}

//-------------------------------------------------------------------------
// DPI functions
//-------------------------------------------------------------------------
void StarterKit_Initialize(void)
{
	__starter_kit.Initialize();
}

void StarterKit_LED(const svBitVecVal* pins)
{
	__starter_kit.LED(*pins);
}

void StarterKit_NumericDisplay(const svBitVecVal* pins)
{
	__starter_kit.NumericDisplay(*pins);
}

void StarterKit_Eval(void)
{
	__starter_kit.Eval();
}
