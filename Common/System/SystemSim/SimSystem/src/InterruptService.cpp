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
// Rev.  : 1/31/2025 Fri (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "InterruptService.h"
#include "SimEngine.h"
#include <assert.h>

static void __interrupt_service_routine_default(void *pPrivate)
{
	LOGI("Interrupt signal invoked.");
}

InterruptService::InterruptService(void)
	: m_SemaInterrupt(0), m_bRun(true), m_bEnable(false), m_bPending(false), m_ISR(__interrupt_service_routine_default), m_pPrivate(NULL)
{
}

InterruptService::~InterruptService(void) {}

void InterruptService::MonitorThread(void)
{
	while (m_bRun) {
		m_SemaInterrupt.Down();

		if (m_bEnable)
			m_ISR(m_pPrivate);
	}
}

void InterruptService::OnThreadKill(bool bForced)
{
	m_bRun	   = false;
	m_bEnable  = false;
	m_bPending = false;
	m_SemaInterrupt.Up();
}

bool InterruptService::Awake(void)
{
	if (m_bEnable && !m_bPending) {
		m_bPending = true;
		m_SemaInterrupt.Up();
		return true;
	}

	return false;
}

void InterruptService::ClearPending(void)
{
	if (m_bPending) {
		m_bPending = false;
		// wake up simulation a little bit for h/w interrupt deassertion
		SimResource::Sim()->Unlock();
		SimResource::Sim()->Lock(200);
	}
}

void InterruptService::Enable(bool bEnable)
{
	if (m_bEnable != bEnable) {
		if (bEnable) {
			m_bPending = false;
		}

		m_bEnable = bEnable;
	}
}

void InterruptService::RegisterService(INTRRUPT_SERVICE service, void *pPrivate)
{
	bool bEnable = m_bEnable;
	Enable(false);
	m_ISR	   = service ? service : __interrupt_service_routine_default;
	m_pPrivate = pPrivate;
	ClearPending();
	Enable(bEnable);
}
