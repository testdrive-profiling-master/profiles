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
#include <turbojpeg.h>

DDKMemory *CreateFromJPEG(const char *sFileName, int byte_stride)
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
		if (byte_stride < iWidth * sizeof(DWORD))
			byte_stride = 0;
		pMem = CreateDDKMemory(byte_stride * iHeight, byte_stride ? byte_stride : 512 * 4);
		tjDecompress2(__hTurboJPEG, pFile, file_size, (unsigned char *)pMem->Virtual(), iWidth, byte_stride, iHeight, TJPF_BGRX, TJFLAG_FASTDCT);
		pMem->Flush();
	}
	tjDestroy(__hTurboJPEG);
	delete[] pFile;
	return pMem;
}
