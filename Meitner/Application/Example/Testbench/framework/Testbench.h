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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "common.h"

#ifndef __TESTBENCH_H__
#define __TESTBENCH_H__

class TestbenchFramework
{
public:
	TestbenchFramework(void);
	virtual ~TestbenchFramework(void);
	BOOL Initialize(int argc, char** argv);
	void Release(void);

	inline BOOL IsInitialized(void)		{return (m_pDDK != NULL);}
	BOOL DoTestbench(void)				{return OnTestBench();}

	static inline TestbenchFramework* GetInstance(void)	{return m_pTestbench;}

protected:
	virtual BOOL OnInitialize(int argc, char** argv)	{return TRUE;}
	virtual BOOL OnTestBench(void) = 0;
	virtual void OnRelease(void)	{}
	virtual void OnInterrupt(void);
	BOOL CheckSimulation(const char* sRequired = NULL, BOOL bLogOut = TRUE);
	
	DDK*			m_pDDK;

private:
	static void __ISR__(DDK* pDDK);
	static TestbenchFramework*		m_pTestbench;
};

TestbenchFramework* CreateTestbench(void);

#define TESTBENCH_DESIGN		static class Testbench : public TestbenchFramework

#endif//__TESTBENCH_H__
