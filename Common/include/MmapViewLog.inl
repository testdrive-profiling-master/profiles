//================================================================================
// MIT License
// 
// Copyright (c) 2013 ~ 2017. HyungKi Jeong(clonextop@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// Title : Common profiles
// Rev.  : 10/2/2017 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __MMAP_VIEW_LOG_INLINE__
#define __MMAP_VIEW_LOG_INLINE__
#include "MmapViewLog.h"
#include <stdarg.h>

MMAP_VIEW	g_mmapview;

const char* GetBytesString(DWORD dwSize)
{
	const char*	sUnit	= " KMG";
	static char	sResult[1024];
	char			sTemp[1024];
	DWORD			dwRest;
	sprintf(sResult, "Bytes");

	for(int i = 0; i < 3; i++) {
		dwRest	= dwSize % 1024;
		dwSize	/= 1024;

		if(!dwSize || dwRest) {
			strcpy(sTemp, sResult);
			sprintf(sResult, "%5d%c%s", dwRest, sUnit[i], sTemp);
		}

		if(!dwSize) break;
	}

	return sResult;
}

MMAP_VIEW::MMAP_VIEW(void)
{
	Initialize(0);
}
MMAP_VIEW::~MMAP_VIEW(void) {}

void MMAP_VIEW::Initialize(DWORD dwBaseAddress)
{
	GetEnvironmentVariable("PROJECT_NAME", m_sMapName, MAX_PATH);
	m_dwBaseAddress	= dwBaseAddress;
}

void MMAP_VIEW::Log(DWORD dwPhyAddress, DWORD dwSize, const char* sName, const char* sFormat, ...)
{
	fflush(stdout);
	printf("$$LINK;document://System?65536?MMAP://%s/%x/%x/%x/%s;", m_sMapName, m_dwBaseAddress, dwPhyAddress - m_dwBaseAddress, dwSize, sName);
	{
		//char sBuff[1024];
		va_list ap;
		va_start(ap, sFormat);
		vprintf(sFormat, ap);
		va_end(ap);
		printf("");
	}
	printf(";00CF00;;\n");
	fflush(stdout);
}

#endif//__MMAP_VIEW_LOG_INLINE__
