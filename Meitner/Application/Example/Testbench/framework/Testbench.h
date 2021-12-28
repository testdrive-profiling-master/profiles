//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 12/28/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "common.h"

#ifndef __TESTBENCH_H__
#define __TESTBENCH_H__

class TestbenchFramework {
public:
	TestbenchFramework(void);
	virtual ~TestbenchFramework(void);
	bool Initialize(int argc, char** argv);
	void Release(void);

	inline bool IsInitialized(void)		{
		return (m_pDDK != NULL);
	}
	inline DDK* GetDDK(void)			{
		return m_pDDK;
	}
	bool DoTestbench(void)				{
		return OnTestBench();
	}

protected:
	virtual bool OnInitialize(int argc, char** argv)	{
		return true;
	}
	virtual bool OnTestBench(void) = 0;
	virtual void OnRelease(void)	{}
	virtual void OnInterrupt(void);
	bool CheckSimulation(const char* sRequired = NULL, bool bLogOut = true);

	DDK*			m_pDDK;

private:
	static void __ISR__(DDK* pDDK);
};

#endif//__TESTBENCH_H__
