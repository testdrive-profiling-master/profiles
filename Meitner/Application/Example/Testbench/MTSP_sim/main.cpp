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
#include "Testbench_MTSP.h"
#include <thread>

class Testbench : public TestbenchFramework_MTSP
{
	DDKMemory *m_pInst;

public:
	Testbench(void)
	{
		m_pInst = NULL;
	}

	virtual ~Testbench(void) {}

	virtual bool OnInitialize(void)
	{
		m_pInst		 = CreateDDKMemory(1024 * 4, 64);
		DWORD *pInst = (DWORD *)m_pInst->Virtual();
		memset(pInst, 0, m_pInst->ByteSize());
		//            end                 phase       mo:branch     end
#if FULL_THREAD_TEST
		pInst[0] = ((DWORD)1 << 31) | (1 << 30) | (0xE << 26) | (1 << 22); // all
		pInst[1] = ((DWORD)1 << 31) | (1 << 30) | (0xE << 26) | (3 << 22); // sync
		pInst[2] = ((DWORD)1 << 31) | (1 << 30) | (0xE << 26) | (7 << 22); // sync.global
		//                                            MOV           r3          r5          wmask        swX        swY        swZ        swW
		pInst[3] = ((DWORD)1 << 31) | (0 << 30) | (0x0 << 26) | (3 << 21) | (5 << 16) | (0xF << 8) | (0 << 6) | (1 << 4) | (2 << 2) |
				   (3 << 0);											   // mov r3, r5
		pInst[4] = ((DWORD)1 << 31) | (1 << 30) | (0xF << 26);			   // mem
		pInst[5] = ((DWORD)1 << 31) | (1 << 30) | (0xE << 26) | (2 << 22); // end
#else
		//            end                 phase       mo:branch     end
		// pInst[0]	= ((DWORD)1 << 31) | (1 << 30) | (0xE << 26) | (7 << 22);	// sync.global
		//                                            MOV           r3          r5          wmask        swX        swY        swZ        swW
		pInst[0] = ((DWORD)1 << 31) | (0 << 30) | (0x0 << 26) | (3 << 21) | (5 << 16) | (0xF << 8) | (0 << 6) | (1 << 4) | (2 << 2) |
				   (3 << 0); // mov r3, r5
		pInst[1] = ((DWORD)1 << 31) | (0 << 30) | (0x3 << 26) | (5 << 21) | (6 << 16) | (0xF << 8) | (3 << 6) | (2 << 4) | (1 << 2) |
				   (0 << 0);											   // mov r3, r5
		pInst[2] = ((DWORD)1 << 31) | (1 << 30) | (0xE << 26) | (2 << 22); // end
#endif
		m_pInst->Flush();
		return true;
	}

	virtual void OnRelease(void)
	{
		SAFE_RELEASE(m_pInst);
	}

	virtual bool OnTestBench(void)
	{
		for (int i = 0; i < 1; i++) {
			printf("Job post : %d\n", i);
			Run(m_pInst->Physical());
		}

		printf("Running...\n");

		for (int i = 0; i < 1; i++) {
			WaitForDone();
			printf("Job Done : %d\n", i);
		}

		printf("Done!\n");
		return true;
	}
};

int main(int argc, char **argv)
{
	int		  iRet = 1;
	Testbench tb;

	if (tb.Initialize()) {
		iRet = tb.DoTestbench();
	}

	tb.Release();
	return iRet;
}
