//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Testbench
// Rev.  : 8/7/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "Testbench.h"
#include "TestDriver.inl"

static TestbenchFramework *__pTestbench = NULL;

// interrupt service routine
void TestbenchFramework::__ISR__(DDK *pDDK)
{
	if (__pTestbench)
		__pTestbench->OnInterrupt();
	else
		pDDK->ClearInterruptPending();
}

TestbenchFramework::TestbenchFramework(void)
{
	m_pDDK		 = NULL;
	__pTestbench = this;
}

TestbenchFramework::~TestbenchFramework(void)
{
	__pTestbench = NULL;
	Release();
}

bool TestbenchFramework::Initialize(void)
{
	// create DDK
	if (!m_pDDK) {
		m_pDDK = CreateDDK();

		if (m_pDDK) {
			printf("*I: Current system is \"%s\".\n", m_pDDK->GetSystemDescription());
			fflush(stdout);
			// activate interrupt
			m_pDDK->RegisterInterruptService((DDK_INTRRUPT_SERVICE)__ISR__); // register interrupt service routine
			m_pDDK->ClearInterruptPending();								 // clear interrupt pending
			m_pDDK->EnableInterrupt();										 // enable interrupt
			g_mmapview.Initialize(m_pDDK->GetMemoryBase());
			return OnInitialize();
		}
	}

	return IsInitialized();
}

void TestbenchFramework::OnInterrupt(void)
{
	printf("ISR done.\n");
	m_pDDK->ClearInterruptPending();
}

bool TestbenchFramework::CheckSimulation(const char *sRequired, bool bLogOut)
{
	bool bSuccess = true;

	if (sRequired) {
		const char *sSystemDesc = m_pDDK->GetSystemDescription();

		if (!strstr(sSystemDesc, "Simulation Simplified"))
			return true; // check simulation environment only.

		bSuccess = sSystemDesc && (strstr(sSystemDesc, sRequired) != NULL);

		if (bLogOut) {
			if (bSuccess)
				printf("'%s' System is ready!\n", sRequired);
			else
				printf(
					"*E: Invalid your H/W system. Switch your H/W system to '%s'!\n    Current system is '%s'\n", sRequired,
					sSystemDesc ? sSystemDesc : "Unknown");
		}
	}

	return bSuccess;
}

void TestbenchFramework::Release(void)
{
	if (IsInitialized()) {
		OnRelease();
		m_pDDK->EnableInterrupt(FALSE); // disable interrupt
		m_pDDK->RegisterInterruptService(NULL);
		m_pDDK->Release(); // release DDK
		m_pDDK = NULL;
		_flushall();
	}
}
