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
// Rev.  : 7/19/2024 Fri (clonextop@gmail.com)
//================================================================================
#include "Testbench_MTSP.h"

#define MTSP_BASE_ADDR 0x10000000

TestbenchFramework_MTSP::TestbenchFramework_MTSP(void) : m_SemaIntrrupt(0)
{
	memset(&m_Info, 0, sizeof(m_Info));
	m_dwCoreSize		= 0;
	m_dwCommandFiFoSize = 0;
	m_iPrevRunCount		= 0;
	m_iProcessingCount	= 0;
}

TestbenchFramework_MTSP::~TestbenchFramework_MTSP(void) {}

bool TestbenchFramework_MTSP::Initialize(void)
{
	if (TestbenchFramework::Initialize()) {
		if (!CheckSimulation("MTSP"))
			return false;

		m_Info.magic_code.m = RegRead(0);
		printf(
			"- Device magic code             : '%c%c%c%c' (%s)\n", m_Info.magic_code.c[0], m_Info.magic_code.c[1], m_Info.magic_code.c[2],
			m_Info.magic_code.c[3], (m_Info.magic_code.m == MTSP_MAGIC_CODE) ? "Ok." : "Not matched");

		if (m_Info.magic_code.m == MTSP_MAGIC_CODE) {
			m_Info.config.m		= RegRead(1 << 2);
			m_dwCoreSize		= m_Info.config.core_count;
			m_dwCommandFiFoSize = 1 << m_Info.config.fifo_depth;
			printf("- MTSP core size                : %5d ea\n", m_dwCoreSize);
			printf("- MTSP command FiFo entry size  : %5d entries\n", m_dwCommandFiFoSize);
			printf("- MTSP LMB entry size           : %5d entries\n", 1 << (m_Info.config.lmb_depth + 6));
			printf("- MTSP GMB entry size           : %5d entries\n", 1 << (m_Info.config.gmb_depth + 6));
			m_Info.status.m = RegRead(2 << 2);
			m_iPrevRunCount = m_Info.status.run_count;
			return true;
		}
	}

	return false;
}

bool TestbenchFramework_MTSP::Compile(DDKMemory *&pMem, const char *sFilename, const char *sWorkPath)
{
	// TODO : do something!!!
	return false;
}

DWORD TestbenchFramework_MTSP::RegRead(DWORD dwOffset)
{
	return m_pDDK->RegRead(MTSP_BASE_ADDR | dwOffset);
}

void TestbenchFramework_MTSP::RegWrite(DWORD dwOffset, DWORD dwData)
{
	m_pDDK->RegWrite(MTSP_BASE_ADDR | dwOffset, dwData);
}

void TestbenchFramework_MTSP::Run(uint64_t pc)
{
	uint32_t		*pPC = (uint32_t *)&pc;
	MTSP_REG_COMMAND reg;
	reg.m				 = 0;
	reg.branch_op		 = 0;
	reg.branch_en		 = 1;
	reg.intrrupt_enable	 = 1;
	reg.run_count_enable = 1;
	reg.icache_clear	 = 1;
	reg.gcache_clear	 = 1;
	reg.lcache_clear	 = 1;
	// set address
	RegWrite(0x0, pPC[1]); // high address
	RegWrite(0x0, pPC[0]); // low address
	// start
	RegWrite((1 << 2), reg.m);
	m_iProcessingCount++;
}

void TestbenchFramework_MTSP::WaitForDone(void)
{
	if (m_iProcessingCount) {
		m_SemaIntrrupt.Down();
		m_iProcessingCount--;
	} else {
		printf("No processing jobs.\n");
	}
}

short TestbenchFramework_MTSP::ClearInterrupt(void)
{
	return RegRead(2 << 2);
}

short TestbenchFramework_MTSP::GetProceedCount(void)
{
	short iProceedCount = m_Info.status.run_count - m_iPrevRunCount;
	m_iPrevRunCount		= m_Info.status.run_count;
	return iProceedCount;
}

void TestbenchFramework_MTSP::OnInterrupt(void)
{
	m_Info.status.m		= RegRead(2 << 2); // clear interrupt and get run count
	short iProceedCount = m_Info.status.run_count - m_iPrevRunCount;
	m_iPrevRunCount		= m_Info.status.run_count;

	for (short i = 0; i < iProceedCount; i++) m_SemaIntrrupt.Up();

	m_pDDK->ClearInterruptPending();
}
