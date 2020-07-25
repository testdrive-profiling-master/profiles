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
#include "Testbench.h"
#include "hw/DUT.h"
#include "hw/hdmi_controller.h"

TESTBENCH_DESIGN {
	DDKMemory*			m_pFrame;
	DUT*				m_pDUT;
	HDMI_Controller*	m_pHDMI;

	virtual BOOL OnInitialize(int argc, char** argv)
	{
		m_pHDMI		= NULL;
		m_pDUT		= NULL;

		// H/W system equality check
		if(!CheckSimulation("Meitner"))
			return FALSE;

		m_pHDMI		= new HDMI_Controller(m_pDDK, 0x10000, 200.f);	// 0x10000 hdmi base address, @200Mhz
		m_pDUT		= new DUT(m_pDDK);
		m_pDUT->SetClock(300.f);			// set processor clock to 300MHz (High speed.) for performance
		{
			// initialize HDMI buffer
			m_pHDMI->SetScreen(1920, 1080);		// set resolution
			m_pFrame	= CreateDDKMemory(1920 *  1080 * 4, 32);
			DWORD*	pBuffer	= (DWORD*)m_pFrame->Virtual();

			// initialize screen check box
			for(int y = 0; y < 1080; y++)
				for(int x = 0; x < 1920; x++)
					pBuffer[x + y * 1920] = (x ^ y) & 0x20 ? 0xFFFFFF : 0xCFCFCF;

			m_pFrame->Flush();
			// set *front framebuffer base address
			m_pHDMI->SetBaseAddress(m_pFrame->Physical());
		}
		return TRUE;
	}

	virtual void OnRelease(void)
	{
		if(m_pDUT) m_pDUT->SetClock(50.f);		// set processor clock to 50MHz (Low speed.) for IDLE status

		SAFE_DELETE(m_pDUT);
		SAFE_RELEASE(m_pFrame);
	}

	virtual BOOL OnTestBench(void)
	{
		{
			// slave read test
			printf("Slave Write test.\n");
			printf("\n\t*** Write 4 times...\n");

			for(int i = 1; i <= 4; i++) {
				m_pDUT->RegWrite(i << 2, i);
				printf("\tWrite[%d] = %d\n", i, i);
			}

			printf("\n\t*** Read 4 times...\n");

			for(int i = 1; i <= 4; i++) {
				printf("\tRead = %d\n", m_pDUT->RegRead(i));
			}
		}
		return TRUE;
	}
} __END__;
