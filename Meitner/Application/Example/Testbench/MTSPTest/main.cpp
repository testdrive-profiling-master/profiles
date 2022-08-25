//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 8/25/2022 Thu (clonextop@gmail.com)
//================================================================================
#include "Testbench.h"
#include "hw/MTSP.h"

class Testbench : public TestbenchFramework {
	DDKMemory*			m_pFrame;
	MTSP*				m_pMTSP;
	DDKMemory*			m_pProgram;

	virtual bool OnInitialize(int argc, char** argv) {
		m_pMTSP		= NULL;
		m_pProgram	= NULL;

		// H/W system equality check
		if(!CheckSimulation("MTSP"))
			return false;

		m_pMTSP		= new MTSP(m_pDDK);
		{
			// report status
			MTSP_STATUS	status;
			status.all	= m_pMTSP->RegRead(0);
			printf("MTSP core size    : %d\n", status.core_size);
			printf("Command FiFo size : %d\n", 1 << status.cmd_fifo_depth);
		}
		m_pMTSP->SetClock(300.f);			// set processor clock to 300MHz (High speed.) for performance
		{
			// initialize HDMI buffer
			m_pFrame	= CreateDDKMemory(1920 *  1080 * 4, 32);
			DWORD*	pBuffer	= (DWORD*)m_pFrame->Virtual();

			// initialize screen check box
			for(int y = 0; y < 1080; y++)
				for(int x = 0; x < 1920; x++)
					pBuffer[x + y * 1920] = (x ^ y) & 0x20 ? 0xFFFFFF : 0xCFCFCF;

			m_pFrame->Flush();
			// set *front framebuffer base address
			m_pMTSP->HDMI()->SetBaseAddress(m_pFrame->Physical());
			m_pMTSP->HDMI()->SetScreen(1920, 1080);		// set resolution
		}
		return true;
	}

	virtual void OnRelease(void) {
		if(m_pMTSP) m_pMTSP->SetClock(50.f);		// set processor clock to 50MHz (Low speed.) for IDLE status

		SAFE_DELETE(m_pMTSP);
		SAFE_RELEASE(m_pFrame);
		SAFE_RELEASE(m_pProgram);
	}

	virtual bool OnTestBench(void) {
		m_pProgram	= m_pMTSP->Compile("GPASM/sample/simple_program.gpp");

		if(m_pProgram) {
			m_pMTSP->RunProgram(m_pProgram->Physical());
			/*{
				// memory test
				DDKMemory*	pMem = CreateDDKMemory(32, 32);
				memset(pMem->Virtual(), 0xCD, pMem->ByteSize());
				pMem->Flush();
				m_pMTSP->WriteGMB(pMem, 0, 0);
				Sleep(1000);
				pMem->Flush(FALSE);

				for(int i = 0; i < 32 / 4; i++) {
					printf("mem[%d] = 0x%08X\n", i, ((DWORD*)pMem->Virtual())[i]);
				}

				pMem->Release();
			}*/
		}

		return true;
	}
};

int main(int argc, char** argv)
{
	Testbench	tb;

	if(tb.Initialize(argc, argv)) {
		if(!tb.DoTestbench())
			printf("Testbench is failed.\n");
	} else {
		printf("Initialization is failed.\n");
	}

	tb.Release();
}
