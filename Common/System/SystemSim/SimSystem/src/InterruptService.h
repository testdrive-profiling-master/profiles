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
#ifndef __INTERRUPT_SERVICE_H__
#define __INTERRUPT_SERVICE_H__
#include "ThreadManager.h"
#include "ddk/SystemHAL.h"

class InterruptService :
	public ThreadManager {
public:
	InterruptService(void);
	~InterruptService(void);

	void RegisterService(INTRRUPT_SERVICE service);
	bool Awake(void);
	void ClearPending(bool bForced = false);

	inline void Enable(bool bEnable = true)	{
		m_bEnable	= bEnable;
	}

private:
	virtual void MonitorThread(void);					// monitor thread
	virtual void OnThreadKill(bool bForced);			// it will call before kill thread

	INTRRUPT_SERVICE	m_ISR;
	Semaphore			m_SemaInterrupt;
	volatile bool		m_bRun;
	volatile bool		m_bEnable;
	volatile bool		m_bPending;
};
#endif//__INTERRUPT_SERVICE_H__
