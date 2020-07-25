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
// Title : Common DPI
// Rev.  : 11/5/2019 Tue (clonextop@gmail.com)
//================================================================================
#include "dpi_common.h"
#include <shellapi.h>
#include <list>
using namespace std;

typedef struct {
	const char* sFileName;
	int			iLine;
} BREAK_POINT_DESC;

class BreakPointManager {
public:
	BreakPointManager(void) {
	}
	~BreakPointManager(void) {
	}

	void CheckPoint(const char* sFileName, int iLine) {
		{
			// bypass check
			list<BREAK_POINT_DESC>::iterator	iter;

			for(list<BREAK_POINT_DESC>::iterator i = m_Bypass.begin(); i != m_Bypass.end(); i++) {
				if(i->sFileName == sFileName && i->iLine == iLine) return;
			}
		}
		{
			SimulationDebugMode(TRUE);

			for(int i = 0; i < 7; i++) {
				if(i & 1)
					printf("\r");	// delete current line
				else
					printf("*I: HDL Break point - %s(%d) / Press 'F8' key to continue, 'F6' key to continue & eliminate this break point...", sFileName, iLine);

				fflush(stdout);
				Sleep(50);
			}

			{
				// open code & pointing to breakpoint line...
				char sNppPath[MAX_PATH];
				char sArg[MAX_PATH];
				{
					// get notepad++ path
					char sDir[MAX_PATH];
					GetEnvironmentVariable("TESTDRIVE_DIR", sDir, MAX_PATH);
					sprintf(sNppPath, "%sbin\\notepad\\notepad++.exe", sDir);
					// get notepad++ path
					GetEnvironmentVariable("PROJECT", sDir, MAX_PATH);
					sprintf(sArg, "-n%d %sSystem/HDL/%s", iLine, sDir, sFileName);
				}
				ShellExecute(NULL, NULL, sNppPath, sArg, NULL, SW_SHOWDEFAULT);
			}

			// wait key down
			for(;;) {
				if(GetKeyState(VK_F6) < 0) {	// bypass
					BREAK_POINT_DESC	desc;
					desc.sFileName	= sFileName;
					desc.iLine		= iLine;
					m_Bypass.push_back(desc);
					break;
				}

				// continue
				if(GetKeyState(VK_F8) < 0 || GetKeyState(VK_ESCAPE) < 0) break;

				Sleep(1);
			}

			printf("\r ");	// delete current line
			fflush(stdout);

			// wait key up
			while((GetKeyState(VK_F6) < 0 ||
				   GetKeyState(VK_F8) < 0))
				Sleep(1);

			printf("\r");	// select current line
			fflush(stdout);
			SimulationDebugMode(FALSE);
		}
	}

protected:
	list<BREAK_POINT_DESC>	m_Bypass;
};

static BreakPointManager		__bp;

DPI_FUNCTION void BreakPoint(const char* sFileName, int iLine)
{
	__bp.CheckPoint(sFileName, iLine);
}
