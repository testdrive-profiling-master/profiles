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
// Title : Simulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "SimClock.h"
#include <assert.h>

DWORD			SimClock::m_dwElapsedTime		= 0;
BOOL			SimClock::m_bReset				= TRUE;

SimClock::SimClock(BYTE* pCLK, BYTE* pRST)
{
	m_dwID					= 0;
	m_pCLK					= pCLK;
	m_pRST					= pRST;
	m_NextClock				= 0;
	m_NextReset				= 0;
	m_clock_polarity		= 1;
	m_reset_polarity		= 0;
	m_reset_cycles			= 0;
	m_dwPeriod				= 5000;		// default 5000ps(5ns)
	m_dwLeftTime			= 0;
	// add to linked list
	m_pReset				= NULL;
	Refresh();
}

SimClock::~SimClock(void)
{
	SAFE_DELETE(m_pReset);
}

void SimClock::Tik(void)
{
	SimClock*	pNode	= Head();

	// present clocking
	while(pNode) {
		pNode->ProcessTik();
		pNode	= pNode->Next();
	}
}

DWORD SimClock::Tok(void)
{
	SimClock*	pNode			= Head();
	DWORD		dwMinTIme		= 0xFFFFFFFF;
	DWORD		dwCurElpaseTime	= m_dwElapsedTime;
	BOOL		bReset			= FALSE;

	if(pNode) {
		while(pNode) {
			bReset	|= pNode->ProcessTok(dwCurElpaseTime, dwMinTIme);
			pNode	= pNode->Next();
		}

		m_dwElapsedTime		= dwMinTIme;
	} else {
		m_dwElapsedTime		= 5000;		// default half clock is 5000ps(5ns)
	}

	m_bReset	= bReset;
	return dwCurElpaseTime;
}

void SimClock::ProcessTik(void)
{
	*m_pCLK		= m_NextClock;
}

BOOL SimClock::ProcessTok(DWORD dwElapsedTime, DWORD& dwMinTime)
{
	BOOL	bReset	= (m_pReset != NULL);
	m_dwLeftTime	-= dwElapsedTime;

	if(!m_dwLeftTime) {
		m_dwLeftTime	= m_dwPeriod;
		m_NextClock		^= 1;

		if(m_pReset && m_NextClock == m_clock_polarity) {
			if(!m_pReset->DoCycle()) {
				m_pReset	= NULL;
			}
		}
	}

	if(m_dwLeftTime < dwMinTime)
		dwMinTime	= m_dwLeftTime;

	return bReset;
}

void SimClock::SetParameters(DWORD dwID, DWORD dwPeriod, BYTE bInitValue, DWORD dwPhase, BYTE ClockPolarity, BYTE ResetPolarity)
{
	assert(dwPeriod != 0);
	m_dwID					= dwID;
	*m_pCLK					= bInitValue;
	m_NextClock				= bInitValue;
	m_clock_polarity		= ClockPolarity;
	m_reset_polarity		= ResetPolarity;
	m_dwPeriod				= dwPeriod;
	m_dwLeftTime			= dwPeriod + dwPhase;
	Refresh();
}

void SimClock::DoReset(DWORD dwCycles)
{
	if(!m_pReset) m_pReset	= new SimReset(m_pRST);

	m_pReset->Set(dwCycles, m_reset_polarity);
	m_bReset	= TRUE;
}

void SimClock::Release(void)
{
	delete this;
}

void SimClock::Refresh(void)
{
	m_dwElapsedTime			= 0;
	Tok();
}

CLOCK_INTERFACE* SimEngine::CreateClock(BYTE* pCLK, BYTE* pRST)
{
	return new SimClock(pCLK, pRST);
}
