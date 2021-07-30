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
// Title : utility framework
// Rev.  : 7/30/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include <stdarg.h>
#include "TestDrive_LM.h"
#include "TestDriver.inl"

Reference::Reference(void) : m_dwReferCount(1)
{
}

Reference::~Reference(void)
{
}

void Reference::AddRef(void)
{
	m_dwReferCount++;
}

void Reference::Release(void)
{
	m_dwReferCount--;
#ifdef DEBUG_MODE

	if(m_dwReferCount == (xDWORD) - 1)
		xLOGE("Reference count is underflowed!");

#endif

	if(!m_dwReferCount) delete this;
}

bool isVarChar(char ch)
{
	return isalpha(ch) || isdigit(ch) || (ch == '_');
}

void LOG(LOG_MODE id, const char* sFormat, ...)
{
	static const char* __sID[] = {
		"*I: ",
		"*W: ",
		"*E: ",
	};
#ifdef WIN32
	static int __iColor[] = {
		15,
		14,		// yellow
		12,		// red
	};
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), __iColor[(int)id]);
#else
	static const char* __sColor[] = {
		"\x1b[38;2;255;255;255m",
		"\x1b[1;33m",		// yellow
		"\x1b[1;31m",		// red
	};

	if(id != LOG_MODE_INFO) printf(__sColor[(int)id]);

#endif
	{
		int iLen		= 0;
		va_list vaArgs;
		va_start(vaArgs, sFormat);
		{
			// get size only
			va_list vaCopy;
			va_copy(vaCopy, vaArgs);
			iLen = std::vsnprintf(NULL, 0, sFormat, vaCopy);
			va_end(vaCopy);
		}
		{
			char* pBuff = new char[iLen + 1];
			std::vector<char> zc(iLen + 1);
			std::vsnprintf(pBuff, iLen + 1, sFormat, vaArgs);
			va_end(vaArgs);
			printf("%s%s\n", __sID[(int)id], pBuff);
			delete [] pBuff;
		}
	}
#ifdef WIN32

	if(id != LOG_MODE_INFO) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), __iColor[0]);

#else
	printf("\x1b[0m");
#endif
	fflush(stdout);
}
