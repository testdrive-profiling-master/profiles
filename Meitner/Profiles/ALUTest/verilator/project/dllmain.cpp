//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Private dynamic document
// Rev.  : 3/31/2021 Wed (clonextop@gmail.com)
//================================================================================
#include "common.h"
#include "TestDriver.inl"
#include "SimALUInterface.h"
#include "SimTop.h"
#include <time.h>

#ifdef WIN32
BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}
#endif

class SimALU : public ISimALU {
public:
	SimALU(void) {
		m_pTop	= NULL;
	}
	~SimALU(void) {
		if(m_pTop) {
			m_pTop->final();
			delete m_pTop;
			m_pTop	= NULL;
		}
	}

	BOOL Initialize(void) {
		srand(time(NULL));
		Verilated::randReset(2);	// randomize all bits
		Verilated::traceEverOn(false);
		m_pTop		= new SimTop;
		m_pTop->CLK		= 0;
		m_pTop->nRST	= 0;
		m_pTop->EN		= 0;
		m_pTop->DIN		= 0;

		for(int i = 0; i < 10; i++) TickClock();

		m_pTop->nRST	= 1;

		for(int i = 0; i < 10; i++) TickClock();

		return TRUE;
	}

	virtual void Release(void) {
		delete this;
	}

	virtual BOOL SimulateALU(float DIN, float& DOUT, float& DOUT_GOLDEN) {
		if(!m_pTop) return FALSE;

		m_pTop->EN		= 1;
		m_pTop->DIN		= *((DWORD*)&DIN);
		TickClock();
		m_pTop->EN		= 0;

		for(int i = 0;; i++) {
			if(m_pTop->OE) {
				DOUT		= *((float*)&m_pTop->DOUT);
				DOUT_GOLDEN	= *((float*)&m_pTop->DOUT_GOLDEN);
				break;
			}

			TickClock();

			if(i == 2000) return FALSE;
		}

		return TRUE;
	}

private:
	void TickClock(void) {
		if(m_pTop) {
			m_pTop->CLK	= 1;
			m_pTop->eval();
			m_pTop->CLK	= 0;
			m_pTop->eval();
		}
	}

	SimTop*		m_pTop;
};

SIMHDL_API ISimALU* CreateSimALU(void)
{
	SimALU* pSim	= new SimALU;

	if(!pSim->Initialize()) {
		delete pSim;
		pSim	= NULL;
	}

	return pSim;
}
