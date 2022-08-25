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
#include "Display.h"
#include "JuliaSet.h"

#define	SCREEN_WIDTH		320
#define	SCREEN_HEIGHT		240
#define	SCREEN_BUFFER_SIZE	(SCREEN_WIDTH*SCREEN_HEIGHT*4)

typedef enum {
	FRAMEBUFFER_FRONT,
	FRAMEBUFFER_BACK,
	FRAMEBUFFER_SIZE
};

class Testbench : public TestbenchFramework, public JuliaSet {
	Display			m_Display;
	DDKMemory*		m_pFrame[FRAMEBUFFER_SIZE];
	DWORD*			m_pScreen;

	virtual bool OnInitialize(int argc, char** argv) {
		// create memory
		m_pFrame[FRAMEBUFFER_FRONT]	= CreateDDKMemory(SCREEN_BUFFER_SIZE, 16);
		m_pFrame[FRAMEBUFFER_BACK]	= CreateDDKMemory(SCREEN_BUFFER_SIZE, 16);
		// setup display
		m_Display.Initialize(m_pDDK);
		m_Display.SetResolution(SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_FORMAT_ARGB_8888);
		ExchangeFramebuffer();
		m_Display.SetForeground();
		SetMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
		return true;
	}
	virtual void OnRelease(void) {
		// release memory
		for(int i = 0; i < FRAMEBUFFER_SIZE; i++)
			SAFE_RELEASE(m_pFrame[i]);

		m_Display.Release();
	}

	void ExchangeFramebuffer(void) {
		DDKMemory* pTemp			= m_pFrame[FRAMEBUFFER_BACK];
		m_pFrame[FRAMEBUFFER_BACK]	= m_pFrame[FRAMEBUFFER_FRONT];
		m_pFrame[FRAMEBUFFER_FRONT]	= pTemp;
		m_pFrame[FRAMEBUFFER_FRONT]->Flush();
		m_Display.SetBuffer(m_pFrame[FRAMEBUFFER_FRONT], true);
		m_Display.SetBuffer(m_pFrame[FRAMEBUFFER_BACK], false);
		m_Display.Invalidate(true);
		m_pScreen	= (DWORD*)m_pFrame[FRAMEBUFFER_BACK]->Virtual();
	}

	virtual void OnPresent(int x, int y, DWORD color) {
		m_pScreen[x + y * SCREEN_WIDTH]	= color;
		m_Display.Invalidate(false);
	}

	virtual bool OnTestBench(void) {
		printf("Press 'ESC' key to stop the process...\n");
		_flushall();
		DWORD mode	= 0;
		double	I	= 0.5;
		double	R	= -0.7;

		while(GetKeyState(VK_ESCAPE) >= 0) {
			memset(m_pScreen, 0, SCREEN_BUFFER_SIZE);

			switch(mode) {
			case 0:
				I	-= 0.002;

				if(I < -0.5) {
					mode = 1;
				}

				break;

			case 1:
				I	+= 0.002;

				if(I > 0.5) {
					mode = 0;
				}

				break;
			}

			SetConstant(I, R);
			Present();
			ExchangeFramebuffer();
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
