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
// Rev.  : 6/26/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "Testbench.h"
#include "Display.h"
#include "hw/hdmi_controller.h"
#include <turbojpeg.h>

DDKMemory *CreateFromJPEG(const char *sFileName)
{
	static tjhandle __hTurboJPEG = NULL;
	BYTE		   *pFile		 = NULL;
	size_t			file_size	 = 0;
	DDKMemory	   *pMem		 = NULL;
	{
		// read jpeg file
		FILE *fp;
		fp = fopen(sFileName, "rb");

		if (!fp)
			return NULL;

		fseek(fp, 0, SEEK_END);
		file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		pFile = new BYTE[file_size];
		fread(pFile, file_size, 1, fp);
		fclose(fp);
	}
	// decode image
	__hTurboJPEG = tjInitDecompress();
	{
		int iWidth, iHeight;
		int jpegSubsamp;
		tjDecompressHeader2(__hTurboJPEG, pFile, file_size, &iWidth, &iHeight, &jpegSubsamp);
		pMem = CreateDDKMemory(iWidth * iHeight * sizeof(DWORD), 64);
		tjDecompress2(__hTurboJPEG, pFile, file_size, (unsigned char *)pMem->Virtual(), iWidth, 0 /*pitch*/, iHeight, TJPF_BGRX, TJFLAG_FASTDCT);
		pMem->Flush();
	}
	tjDestroy(__hTurboJPEG);
	delete[] pFile;
	return pMem;
}

class Testbench : public TestbenchFramework
{
	HDMI_Controller *m_pHDMI;
	DDKMemory		*m_pImg;

	virtual bool	 OnInitialize(void)
	{
		m_pHDMI = NULL;
		m_pImg	= NULL;

		// H/W system equality check
		if (!CheckSimulation("HDMI controller"))
			return false;

		m_pHDMI = new HDMI_Controller(m_pDDK, 0x10000, 200.f); // 0x10000 base address, @200Mhz

		if (!m_pHDMI->SetScreen(1920, 1080, 60, TRUE))
			return false;

		m_pImg = CreateFromJPEG("media\\fullhd_test.jpg");
		m_pImg->Flush();
		m_pHDMI->SetBaseAddress(m_pImg->Physical());
		return true;
	}

	virtual void OnRelease(void)
	{
		SAFE_RELEASE(m_pImg);
		SAFE_DELETE(m_pHDMI);
	}

	virtual bool OnTestBench(void)
	{
		printf("process is done!\n");
		return true;
	}
};

int main(int argc, char **argv)
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
