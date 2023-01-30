//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Simulation sub-system
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "STDInterface.h"
#include "DelayLock.h"
#include "../SimHDL/SimHDL.h"

DelayLock::DelayLock(void) : m_Lock(0), m_DelaySemaphore(1)
{
	m_bRun			= true;
	m_iLock			= 0;
	m_iDelayTime	= 0;
	m_iDelayLock	= 0;
}

DelayLock::~DelayLock(void)
{
}

void DelayLock::Lock(int iDelayTime)
{
	if(iDelayTime) {
		bool bLock;
		m_DelaySemaphore.Down();
		SetDelay(iDelayTime);
		bLock	= (m_iLock > 1);

		if(bLock) m_iLock--;
		else m_iDelayLock++;

		m_DelaySemaphore.Up();

		if(bLock) {
			SimulationFlush();
			m_Lock.Down();
		}
	} else {
		SimulationFlush();
		m_Lock.Down();
		m_DelaySemaphore.Down();
		m_iLock--;
		m_DelaySemaphore.Up();
	}
}

void DelayLock::UnLock(void)
{
	m_DelaySemaphore.Down();
	m_iLock++;
	m_DelaySemaphore.Up();
	m_Lock.Up();
}

bool DelayLock::Check(void)
{
	if(m_iDelayTime) {
		m_DelaySemaphore.Down();
		m_iDelayTime--;
		m_DelaySemaphore.Up();

		if(!m_iDelayTime) {
			int iLocked;
			m_DelaySemaphore.Down();
			iLocked			= m_iDelayLock;
			m_iLock			-= m_iDelayLock;
			m_iDelayLock	= 0;
			m_DelaySemaphore.Up();
			SimulationFlush();

			for(; iLocked != 0; iLocked--) m_Lock.Down();
		}
	}

	return (m_iLock != 0) | m_bRun;
}

void DelayLock::SetDelay(int iDelayTime)
{
	if(m_iDelayTime < iDelayTime) m_iDelayTime = iDelayTime;
}
