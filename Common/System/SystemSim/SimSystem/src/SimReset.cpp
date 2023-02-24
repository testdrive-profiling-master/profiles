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
// Rev.  : 2/23/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "SimReset.h"
#include "SimClock.h"

SimReset::SimReset(BYTE* pRST)
{
	m_pRST	= pRST;
	Set(8, 0);
	TRACE_UNLOCK
	m_pSim->Unlock();
}

SimReset::~SimReset(void)
{
	TRACE_LOCK
	m_pSim->Lock(2);
}

bool SimReset::OnRun(void)
{
	return true;
}

void SimReset::Set(DWORD dwCycles, BYTE Polarity)
{
	*m_pRST			= Polarity;
	m_dwResetCycles	= dwCycles ? dwCycles : 1;
	m_dwLifeCycle	= 30;
}

bool SimReset::DoCycle(void)
{
	if(m_dwResetCycles) {
		m_dwResetCycles--;

		if(!m_dwResetCycles)
			*m_pRST	^= 1;
	} else {
		if(!m_dwLifeCycle) {
			delete this;
			return false;
		} else
			m_dwLifeCycle--;
	}

	return true;
}
