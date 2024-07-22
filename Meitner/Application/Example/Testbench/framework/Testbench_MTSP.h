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
#include "Testbench.h"
#include "SystemConfigMTSP.h"
#include "TD_Semaphore.h"
#include <atomic>

using namespace std;

#ifndef __TESTBENCH_MTSP_H__
#	define __TESTBENCH_MTSP_H__

class TestbenchFramework_MTSP : public TestbenchFramework
{
public:
	TestbenchFramework_MTSP(void);
	virtual ~TestbenchFramework_MTSP(void);

	virtual bool Initialize(void);

	bool		 Compile(DDKMemory *&pMem, const char *sFilename, const char *sWorkPath = NULL);
	void		 Run(uint64_t pc);
	void		 WaitForDone(void);
	short		 GetProceedCount(void);

protected:
	virtual void OnInterrupt(void);
	short		 ClearInterrupt(void); // clear interrupt and get run count

	uint32_t	 m_dwCoreSize;
	uint32_t	 m_dwCommandFiFoSize;

private:
	DWORD		  RegRead(DWORD dwOffset);
	void		  RegWrite(DWORD dwOffset, DWORD dwData);

	atomic_int	  m_iProcessingCount;
	int16_t		  m_iPrevRunCount;
	Semaphore	  m_SemaIntrrupt;
	MTSP_REG_INFO m_Info;
};

#endif //__TESTBENCH_MTSP_H__
