//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "Testbench.h"
#include "Display.h"
#include "BruteForce.h"

#define	SCREEN_WIDTH		640
#define	SCREEN_HEIGHT		480
#define	SCREEN_BUFFER_SIZE	(SCREEN_WIDTH*SCREEN_HEIGHT*4)

typedef enum {
	FRAMEBUFFER_FRONT,
	FRAMEBUFFER_BACK,
	FRAMEBUFFER_SIZE
};

class Testbench : public TestbenchFramework, public BruteForce {
	Display			m_Display;
	DDKMemory*		m_pFrame[FRAMEBUFFER_SIZE];
	DWORD*			m_pScreen;

	virtual bool OnInitialize(void) {
		// create memory
		m_pFrame[FRAMEBUFFER_FRONT]	= CreateDDKMemory(SCREEN_BUFFER_SIZE, 16);
		m_pFrame[FRAMEBUFFER_BACK]	= CreateDDKMemory(SCREEN_BUFFER_SIZE, 16);
		// setup display
		m_Display.Initialize(m_pDDK);
		m_Display.SetResolution(SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_FORMAT_ARGB_8888);
		ExchangeFramebuffer();
		m_Display.SetForeground();
		InitializeBruteForce(1000);	// 1000 bodies
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
	virtual void OnPresent(Body* pBody) {
		int x = (int)(pBody->Position().x / (1e16)) + (SCREEN_WIDTH / 2);
		int y = (int)(pBody->Position().y / (1e16)) + (SCREEN_HEIGHT / 2);

		if(x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT) {
			m_pScreen[x + y * SCREEN_WIDTH]	= pBody->Color();
		} else {
			double v;
			v	= pBody->Velocity().x;

			if((x < 0 && v < 0) ||
			   (x >= SCREEN_WIDTH && v > 0)) {
				pBody->Velocity().x	= -v / 2;
			}

			v	= pBody->Velocity().y;

			if((y < 0 && v < 0) ||
			   (y >= SCREEN_HEIGHT && v > 0)) {
				pBody->Velocity().y	= -v / 2;
			}
		}

		m_Display.Invalidate(false);
	}
	virtual bool OnTestBench(void) {
		printf("Press 'ESC' key to stop the process...\n");
		_flushall();

		while(GetKeyState(VK_ESCAPE) >= 0) {
			memset(m_pScreen, 0, SCREEN_BUFFER_SIZE);
			Present();
			ExchangeFramebuffer();
		}

		return true;
	}
};

int main(int argc, char** argv)
{
	Testbench	tb;

	if(tb.Initialize()) {
		if(!tb.DoTestbench())
			printf("Testbench is failed.\n");
	} else {
		printf("Initialization is failed.\n");
	}

	tb.Release();
}
