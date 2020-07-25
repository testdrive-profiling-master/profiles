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
// Title : Emulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "InterruptService.h"
#include <assert.h>

static void __interrupt_service_routine_default(void)
{
}

InterruptService::InterruptService(void) :
	m_SemaInterrupt(0),
	m_bRun(TRUE),
	m_bEnable(FALSE),
	m_bPending(FALSE),
	m_ISR(__interrupt_service_routine_default)
{
}

InterruptService::~InterruptService(void)
{
}

void InterruptService::MonitorThread(void)
{
	for(; m_bRun;) {
		m_SemaInterrupt.Down();

		if(m_bEnable) m_ISR();
	}
}

void InterruptService::OnThreadKill(void)
{
	m_bRun		= FALSE;
	m_bEnable	= FALSE;
	m_SemaInterrupt.Up();
}

void InterruptService::Enable(BOOL bEnable)
{
	if(bEnable && (m_ISR == __interrupt_service_routine_default)) {
		LOGI("You must register ISR, fist!\n");
		return;
	}

	m_bEnable	= bEnable;
}

void InterruptService::ClearPending(void)
{
	m_bPending	= FALSE;
}

void InterruptService::Awake(void)
{
	if(m_bEnable && !m_bPending) {
		m_bPending	= TRUE;
		m_SemaInterrupt.Up();
	}
}

void InterruptService::RegisterService(INTRRUPT_SERVICE service)
{
	m_ISR	= service ? service : __interrupt_service_routine_default;
}
