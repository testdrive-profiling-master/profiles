//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TestDrive System Driver wrapper
// Rev.  : 10/13/2026 Tue (clonextop@gmail.com)
//================================================================================
#include "InterruptService.h"

#define __LOCK__ const lock_guard<shared_mutex> _QLOCK_(m_QeueueLock);

static void __interrupt_service_routine_default(int iVector, void *pPrivate)
{
	UNUSED(pPrivate);
	LOGI("Interrupt(%d) is invoked.", iVector);
}

InterruptService::InterruptService(void)
{
	m_bRun	   = true;
	m_bEnable  = false;
	m_pPrivate = NULL;
	m_ISR	   = __interrupt_service_routine_default;
}

InterruptService::~InterruptService(void) {}

void InterruptService::MonitorThread(void)
{
	for (; m_bRun;) {
		g_pDriver->InterruptLock();

		if (m_bEnable) {
			bool bQueue;
			int	 iVector;
			{ // check interrupt queue.
				__LOCK__
				bQueue = !m_Qeueue.empty();
				if (bQueue) {
					iVector = m_Qeueue.front();
					m_Qeueue.pop();
				}
			}
			if (bQueue) {
				m_ISR(iVector, m_pPrivate);
				m_Pending[iVector] = false;
			}
		}
	}
}

void InterruptService::OnThreadKill(void)
{
	m_bRun	  = false;
	m_bEnable = false;
	m_Pending.clear();
	g_pDriver->InterruptFree();
}

void InterruptService::Enable(bool bEnable)
{
	if (bEnable && (m_ISR == __interrupt_service_routine_default)) {
		LOGI("You must register ISR, first!");
		return;
	}

	if (m_bEnable != bEnable) {
		if (bEnable) {
			m_Pending.clear();
		}

		m_bEnable = bEnable;
	}
}

bool InterruptService::Awake(int iVector)
{
	if (m_bEnable) {
		__LOCK__
		if (!m_Pending[iVector]) {
			m_Pending[iVector] = true;
			m_Qeueue.push(iVector);
			g_pDriver->InterruptFree();
			return true;
		}
	}
	return false;
}

void InterruptService::RegisterService(INTRRUPT_SERVICE service, void *pPrivate)
{
	bool bEnable = m_bEnable;
	Enable(false);
	m_ISR	   = service ? service : __interrupt_service_routine_default;
	m_pPrivate = pPrivate;
	{
		__LOCK__
		m_Pending.clear();
		m_Qeueue = {};
	}
	Enable(bEnable);
}
