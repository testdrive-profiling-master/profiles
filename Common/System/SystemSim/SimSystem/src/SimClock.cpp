//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common profiles
// Rev.  : 8/4/2025 Mon (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "SimClock.h"
#include <assert.h>

uint32_t SimClock::m_dwElapsedTime = 0;

SimClock::SimClock(uint8_t *pCLK, uint8_t *pRST)
{
	m_dwID			 = 0;
	m_pCLK			 = pCLK;
	m_pRST			 = pRST;
	m_NextClock		 = 0;
	m_NextReset		 = 0;
	m_clock_polarity = 1;
	m_reset_polarity = 0;
	m_reset_cycles	 = 0;
	m_dwPeriod		 = 5000; // default 5000ps(5ns)
	m_dwLeftTime	 = 0;
	// add to linked list
	m_pReset = NULL;
	Refresh();
}

SimClock::~SimClock(void)
{
	SAFE_DELETE(m_pReset);
}

void SimClock::Tik(void)
{
	SimClock *pNode = Head();

	// present clocking
	while (pNode) {
		pNode->ProcessTik();
		pNode = pNode->Next();
	}
}

uint32_t SimClock::Tok(void)
{
	SimClock *pNode			  = Head();
	uint32_t  dwMinTIme		  = 0xFFFFFFFF;
	uint32_t  dwCurElpaseTime = m_dwElapsedTime;

	if (pNode) {
		while (pNode) {
			pNode->ProcessTok(dwCurElpaseTime, dwMinTIme);
			pNode = pNode->Next();
		}

		m_dwElapsedTime = dwMinTIme;
	} else {
		m_dwElapsedTime = 5000; // default half clock is 5000ps(5ns)
	}

	return dwCurElpaseTime;
}

void SimClock::ProcessTik(void)
{
	*m_pCLK = m_NextClock;
}

void SimClock::ProcessTok(uint32_t dwElapsedTime, uint32_t &dwMinTime)
{
	m_dwLeftTime -= dwElapsedTime;

	if (!m_dwLeftTime) {
		m_dwLeftTime = m_dwPeriod;
		m_NextClock ^= 1;

		if (m_pReset && m_NextClock == m_clock_polarity) {
			if (!m_pReset->DoCycle()) {
				m_pReset = NULL;
			}
		}
	}

	if (m_dwLeftTime < dwMinTime)
		dwMinTime = m_dwLeftTime;
}

void SimClock::SetParameters(
	uint32_t dwID, uint32_t dwPeriod, uint8_t bInitValue, uint32_t dwPhase, uint8_t ClockPolarity, uint8_t ResetPolarity)
{
	assert(dwPeriod != 0);
	m_dwID			 = dwID;
	*m_pCLK			 = bInitValue;
	m_NextClock		 = bInitValue;
	m_clock_polarity = ClockPolarity;
	m_reset_polarity = ResetPolarity;
	m_dwPeriod		 = dwPeriod;
	m_dwLeftTime	 = dwPeriod + dwPhase;
	Refresh();
}

void SimClock::DoReset(uint32_t dwCycles)
{
	if (!m_pReset)
		m_pReset = new SimReset(m_pRST);

	m_pReset->Set(dwCycles, m_reset_polarity);
}

void SimClock::Release(void)
{
	delete this;
}

void SimClock::Refresh(void)
{
	m_dwElapsedTime = 0;
	Tok();
}

CLOCK_INTERFACE *SimEngine::CreateClock(uint8_t *pCLK, uint8_t *pRST)
{
	return new SimClock(pCLK, pRST);
}

CLOCK_INTERFACE *SimEngine::FindClock(uint8_t *pCLK)
{
	SimClock *pNode = SimClock::Head();

	while (pNode) {
		if (pNode->CLK() == pCLK)
			return pNode;

		pNode = pNode->Next();
	}

	return NULL;
}
