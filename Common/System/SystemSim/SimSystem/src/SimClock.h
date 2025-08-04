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
#ifndef __SIM_CLOCK_H__
#define __SIM_CLOCK_H__
#include "SimEngine.h"
#include "SimReset.h"
#include "AutoList.h"

class SimClock : public CLOCK_INTERFACE, public AutoList<SimClock>
{
public:
	SimClock(uint8_t *pCLK, uint8_t *pRST);
	virtual ~SimClock(void);

	static void		Tik(void); // Tiking clock (before the evaluation)
	static uint32_t Tok(void); // Toking clock (after the evaluation)

	virtual void	SetParameters(
		   uint32_t dwID, uint32_t dwPeriod, uint8_t bInitValue = 0, uint32_t dwPhase = 0, uint8_t ClockPolarity = 1, uint8_t ResetPolarity = 0);
	virtual void	DoReset(uint32_t dwCycles = 8);
	virtual void	Release(void);

	inline uint8_t *CLK(void)
	{
		return m_pCLK;
	}
	inline uint8_t *RST(void)
	{
		return m_pRST;
	}

protected:
	static void Refresh(void); // refresh clock list
	void		ProcessTik(void);
	void		ProcessTok(uint32_t dwElapsedTime, uint32_t &dwMinTime);

private:
	uint32_t		m_dwID;			  // clock ID#
	uint8_t		   *m_pCLK;			  // current clock
	uint8_t		   *m_pRST;			  // current reset
	uint8_t			m_NextClock;	  // next clock
	uint8_t			m_NextReset;	  // next reset
	uint8_t			m_reset_cycles;	  // reset cycles
	uint8_t			m_clock_polarity; // clock polarity
	uint8_t			m_reset_polarity; // reset polarity
	uint32_t		m_dwPeriod;		  // clock period (ps)
	uint32_t		m_dwLeftTime;	  // next toggle left time
	static uint32_t m_dwElapsedTime;
	SimReset	   *m_pReset;
};
#endif //__SIM_CLOCK_H__
