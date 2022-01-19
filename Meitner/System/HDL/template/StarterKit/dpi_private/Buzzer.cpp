//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 1/19/2022 Wed (clonextop@gmail.com)
//================================================================================
#include "Buzzer.h"

Buzzer::Buzzer(void)
{
	m_PrevTime		= 0;
	m_High			= 0;
	m_Low			= 0;
	m_ToggleCount	= 0;
	m_PrevPin		= 0;
}

Buzzer::~Buzzer(void)
{
}

void Buzzer::Eval(BYTE pin)
{
	UINT64	CurTime	= SimulationTime();
	DWORD	dwDuty	= (CurTime - m_PrevTime);

	if(pin) {
		m_Low	+= dwDuty;
	} else {
		m_High	+= dwDuty;
	}

	m_ToggleCount++;
	m_PrevTime	= CurTime;
	m_PrevPin	= pin;
}

void Buzzer::Get(DWORD& dwFrequency, float& fVolume)
{
	{
		// last duty
		UINT64	CurTime	= SimulationTime();
		DWORD	dwDuty	= (CurTime - m_PrevTime);

		if(m_PrevPin) {
			m_High	+= dwDuty;
		} else {
			m_Low	+= dwDuty;
		}

		m_PrevTime	= CurTime;
	}

	if(m_ToggleCount > 2) {
		dwFrequency	= m_ToggleCount >> 1;
		fVolume		= ((float)(((m_Low > m_High) ? m_High : m_Low) * 2)) / (m_High + m_Low);
	} else {
		dwFrequency	= 0;
		fVolume		= 0;
	}

	m_High			= 0;
	m_Low			= 0;
	m_ToggleCount	= 0;
}

