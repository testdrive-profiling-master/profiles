//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Title : System manager
// Rev.  : 1/16/2025 Thu (clonextop@gmail.com)
//================================================================================
#include "System.h"
#include "VirtualDisplayConfig.h"
#include <tchar.h>
#include <list>
using namespace std;

typedef struct {
	CString sName;
	CString sTag;
} DEFINITION;

typedef list<DEFINITION> DEFINITION_LIST;

BOOL					 CheckHeaderDefinition(const char *sFilePath, LPCTSTR sName, LPCTSTR sContent)
{
	BOOL	 bMatched = FALSE;
	CStringA sNameAnsi(sName);
	{
		FILE *fp = fopen(sFilePath, "rt");
		char  sLine[4096];

		if (fp) {
			while (fgets(sLine, 4096, fp)) {
				char *pTok = strstr(sLine, sNameAnsi);

				if (pTok) {
					CString sName(pTok + sNameAnsi.GetLength());
					sName.TrimLeft(_T(" \t"));
					sName.TrimRight(_T(" \t\r\n/"));

					if (!sName.Compare(sContent))
						bMatched = TRUE;

					break;
				}
			}

			fclose(fp);
		}
	}
	return bMatched;
}

void System::WriteConfiguration(LPCTSTR sKeyName, LPCTSTR sValue)
{
	static LPCTSTR __AppName   = _T("Configuration");
	CString		   sConfigFile = g_pSystem->RetrieveFullPath(_T("%PROJECT%Program/.TestDrive"));
	WritePrivateProfileString(__AppName, sKeyName, sValue, sConfigFile);
	SetEnvironmentVariable(sKeyName, sValue);
}

void System::UpdateDefaultSystemConfigHeader(void)
{
	DEFINITION_LIST DefList;
	CString			sDefaultSystemName = g_pSystem->GetMemory(NULL, TRUE)->GetName();
	CString			sDefaultDisplayName;
	CStringA		sHeaderFileName(g_pSystem->RetrieveFullPath(_T("%PROJECT%Application/include/DefaultSystemConfig.h")));
	sDefaultDisplayName.Format(_T("%s_Display"), (LPCTSTR)sDefaultSystemName);
	// Update TestDrive configuration
	{
		WriteConfiguration(_T("SYSTEM_MEMORY_NAME"), sDefaultSystemName);
		WriteConfiguration(_T("DISPLAY_MEMORY_NAME"), sDefaultDisplayName);
	}
	// make definition list
	{
		DEFINITION def;
		// system memory name
		def.sName = _T("TESTDRIVE_MEMORY_SYSTEM_NAME");
		def.sTag.Format(_T("\"%s\""), sDefaultSystemName);
		DefList.push_back(def);
		// system memory size
		def.sName = _T("TESTDRIVE_MEMORY_SYSTEM_SIZE");
		def.sTag.Format(_T("0x%08X"), m_pSystemConfig->dwMemorySize);
		DefList.push_back(def);
		// display memory name
		def.sName = _T("TESTDRIVE_MEMORY_DISPLAY_NAME");
		def.sTag.Format(_T("\"%s\""), sDefaultDisplayName);
		DefList.push_back(def);
		// display memory size
		def.sName = _T("TESTDRIVE_MEMORY_DISPLAY_SIZE");
		{
			DisplayConfig *pDisplayConfig = (DisplayConfig *)g_pSystem->GetMemory(sDefaultDisplayName, TRUE)->GetConfig();

			if (pDisplayConfig) {
				def.sTag.Format(_T("0x%08X"), pDisplayConfig->dwMemorySize);
				DefList.push_back(def);
			}
		}
	}
	// header source match checking
	{
		BOOL bMatched = TRUE;

		for (DEFINITION_LIST::iterator i = DefList.begin(); i != DefList.end() && bMatched; i++) {
			bMatched &= CheckHeaderDefinition(sHeaderFileName, i->sName.c_str(), i->sTag.c_str());
		}

		if (bMatched)
			return;
	}
	// update header, if not same default system name
	{
		FILE *fp = fopen(sHeaderFileName, "wt");

		if (fp) {
			for (DEFINITION_LIST::iterator i = DefList.begin(); i != DefList.end(); i++) {
				CString sLine;
				sLine.Format(_T("#define %-40s %s\n"), (LPCTSTR)i->sName, (LPCTSTR)i->sTag);
				fputs(CStringA(sLine).c_str(), fp);
			}

			fclose(fp);
		}
	}
}
