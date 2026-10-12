//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 10/12/2026 Mon (clonextop@gmail.com)
//================================================================================
#include "tft_lcd.h"

class Testbench : public TestbenchFramework
{
	bool		 bTFT_LCD_solved;
	DDKMemory	*m_pTFT;
	virtual bool OnInitialize(void)
	{
		printf("Current system : %s\n", m_pDDK->GetSystemDescription());
		bTFT_LCD_solved = CheckSimulation("TFT_LCD_solved", false);

		if (bTFT_LCD_solved) {
			m_pTFT = CreateFromJPEG("media\\tft_test.jpg", 512 * 4);
			MemoryLog(m_pTFT, "frame-buffer memory");
		} else {
			m_pTFT = NULL;
		}

		return CheckSimulation("FPGA Starter Kit");
	}

	virtual void OnRelease(void)
	{
		SAFE_RELEASE(m_pTFT);
	}

	virtual bool OnTestBench(void)
	{
		printf("Press 'ESC' key to exit.\n");
		fflush(stdout);

		if (bTFT_LCD_solved) {
			uint32_t *pBuffer = (uint32_t *)m_pTFT->Virtual();
			m_pTFT->Flush();
			m_pDDK->RegWrite(0x20000, m_pTFT->Physical());
			m_pDDK->RegWrite(0x20004, 1);
		}

		while (GetKeyState(VK_ESCAPE) >= 0) Sleep(100);

		return true;
	}
};

int main(int argc, const char *argv[])
{
	Testbench tb;

	if (tb.Initialize()) {
		if (!tb.DoTestbench())
			printf("Testbench is failed.\n");
	} else {
		printf("Initialization is failed.\n");
	}

	tb.Release();
}
