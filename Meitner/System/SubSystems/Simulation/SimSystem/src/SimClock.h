//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 3/31/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SIM_CLOCK_H__
#define __SIM_CLOCK_H__
#include "SimEngine.h"
#include "SimReset.h"
#include "AutoList.h"

class SimClock :
		public CLOCK_INTERFACE,
		public AutoList<SimClock>
{
public:
	SimClock(BYTE* pCLK, BYTE* pRST);
	virtual ~SimClock(void);

	static BOOL IsReset(void){return m_bReset;}

	static void Tik(void);		// Tiking clock (before the evaluation)
	static DWORD Tok(void);		// Toking clock (after the evaluation)

	virtual void SetParameters(DWORD dwID, DWORD dwPeriod, BYTE bInitValue = 0, DWORD dwPhase = 0, BYTE ClockPolarity = 1, BYTE ResetPolarity = 0);
	virtual void DoReset(DWORD dwCycles = 8);
	virtual void Release(void);

protected:
	static void Refresh(void);	// refresh clock list
	void ProcessTik(void);
	BOOL ProcessTok(DWORD dwElapsedTime, DWORD& dwMinTime);

private:
	DWORD				m_dwID;				// clock ID#
	BYTE*				m_pCLK;				// current clock
	BYTE*				m_pRST;				// current reset
	BYTE				m_NextClock;		// next clock
	BYTE				m_NextReset;		// next reset
	BYTE				m_reset_cycles;		// reset cycles
	BYTE				m_clock_polarity;	// clock polarity
	BYTE				m_reset_polarity;	// reset polarity
	DWORD				m_dwPeriod;			// clock period (ps)
	DWORD				m_dwLeftTime;		// next toggle left time
	static DWORD		m_dwElapsedTime;
	static BOOL			m_bReset;
	SimReset*			m_pReset;
};
#endif//__SIM_CLOCK_H__
