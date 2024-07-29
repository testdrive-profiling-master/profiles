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
// Title : utility framework
// Rev.  : 7/29/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <stdarg.h>
#include "TestDriver.inl"

string GetCommonToolPath(void)
{
	string sEnv;
#ifdef WIN32
	sEnv = getenv("TESTDRIVE_PROFILE");
	sEnv += "Common/";
#else // linux server
	sEnv = getenv("TESTDRIVE_PROFILE");
#endif

	if (sEnv == "")
		sEnv = "./";

	return sEnv;
}

Reference::Reference(void) : m_dwReferCount(1) {}

Reference::~Reference(void) {}

void Reference::AddRef(void)
{
	m_dwReferCount++;
}

void Reference::Release(void)
{
	m_dwReferCount--;
#ifdef DEBUG_MODE

	if (m_dwReferCount == (xDWORD)-1)
		xLOGE("Reference count is underflowed!");

#endif

	if (!m_dwReferCount)
		delete this;
}

bool isVarChar(char ch)
{
	return isalpha(ch) || isdigit(ch) || (ch == '_');
}

int			g_log_warning_count = 0;
int			g_log_error_count	= 0;
static bool __log_supress		= false;
void		LOG_Suppress(bool bSuppress)
{
	__log_supress = bSuppress;
}

void LOG(LOG_MODE id, const char *sFormat, ...)
{
	static const char *__sID[] = {
		"*I: ",
		"*W: ",
		"*E: ",
	};
	int	 color_id		= (int)id;
	bool bCustom		= false;
	bool bColor			= false;
	bool bPreserveColor = false;

	if (__log_supress)
		return;

	switch (id) {
	case LOG_MODE_WARNING:
		g_log_warning_count++;
		bColor = true;
		break;

	case LOG_MODE_ERROR:
		g_log_error_count++;
		bColor = true;
		break;
	}

	if (!sFormat)
		return;

	if (*sFormat == '*' || *sFormat == '@') {
		int new_color_id;
		bCustom = (*sFormat == '@');

		if (sFormat[1] == '^') { // preserve color after done.
			bPreserveColor = true;
			new_color_id   = (sFormat[2]) - '0';
		} else {
			new_color_id = (sFormat[1]) - '0';
		}

		if (new_color_id >= 0 && new_color_id <= 6) {
			color_id = new_color_id;
			sFormat += (bPreserveColor ? 3 : 2);
			bColor = true;
		}
	}

#ifdef WIN32
	static int __iColor[] = {
		15, // 0: white
		14, // 1: yellow
		12, // 2: red
		9,	// 3: blue
		10, // 4: green
		13, // 5: purple
		11, // 6: aqua
	};

	if (bColor)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), __iColor[color_id]);

#else
	static const char *__sColor[] = {
		"\x1b[38;2;255;255;255m", // 0: white
		"\x1b[1;33m",			  // 1: yellow
		"\x1b[1;31m",			  // 2: red
		"\x1b[1;34m",			  // 3: blue
		"\x1b[1;32m",			  // 4: green
		"\x1b[1;35m",			  // 5: purple
		"\x1b[1;36m",			  // 6: aqua
	};

	if (bColor)
		printf(__sColor[color_id]);

#endif
	{
		int		iLen = 0;
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
			char			 *pBuff = new char[iLen + 1];
			std::vector<char> zc(iLen + 1);
			std::vsnprintf(pBuff, iLen + 1, sFormat, vaArgs);
			va_end(vaArgs);

			if (bCustom)
				printf("%s", pBuff);
			else
				printf("%s%s\n", __sID[(int)id], pBuff);

			delete[] pBuff;
		}
	}
#ifdef WIN32

	if (bColor && !bPreserveColor)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

#else

	if (bColor && !bPreserveColor)
		printf("\x1b[0m");

#endif
	fflush(stdout);
}
