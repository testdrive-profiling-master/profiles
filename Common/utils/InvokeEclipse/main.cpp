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
// Title : Invoke Eclipse
// Rev.  : 8/10/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <shellapi.h>

struct {
	char		sName[4096];
	char		sWorkspacePath[4096];
	char		sCurrentPath[4096];
} __env;

void GetRoot(const char* sRoot)
{
	cstring sFullPath;
	{
		// get current full path
		char	sPath[4096];
		GetFullPathName(sRoot, 4096, sPath, NULL);
		sFullPath	= sPath;
		sFullPath.TrimRight("\\");
		strcpy(__env.sCurrentPath, sFullPath);
	}
	{
		// find .workspace folder
		int iPos	= 0;
		BOOL bFound	= FALSE;
		cstring	sWorkPath(sFullPath);

		// search sub folder ".workspace"
		do {
			iPos	= sWorkPath.rfind('\\');

			if(iPos > 0) {
				sWorkPath.erase(iPos, sWorkPath.Length());
				cstring	sAccessPath;
				sAccessPath.Format("%s\\.workspace", sWorkPath.c_str());

				if(!_access(sAccessPath, 0)) {
					bFound	= TRUE;
					break;
				}
			}
		} while(iPos > 0);

		// if not found ".workspace"
		if(!bFound) {
			sWorkPath	= sFullPath;
			iPos	= sWorkPath.rfind('\\');
			sWorkPath.erase(iPos, sWorkPath.Length());
		}

		// add workspace
		{
			cstring sAddPath(sFullPath);
			iPos		= sWorkPath.Length();
			sWorkPath	+= "\\.workspace";
			sAddPath.erase(0, iPos);
			sWorkPath	+= sAddPath;
		}
		strcpy(__env.sWorkspacePath, sWorkPath);
	}
	{
		// get folder name
		cstring sName(sFullPath);
		int iPos	= sName.rfind('\\');

		if(iPos >= 0) sName.erase(0, iPos + 1);

		strcpy(__env.sName, sName);
	}
}

void ShowHelp(void)
{
	printf("Invoke Eclipse (" __DATE__ ", Q&A : clonextop@gmail.com)\n\n");
	printf("Usage : InvokeEclipse.exe [options]\n" \
		   "\t-help               Display this information\n");
}

BOOL CheckParams(int iSize, const char* sOpt[])
{
	memset(&__env, 0, sizeof(__env));

	while(iSize > 0) {
		const char*	sCurOpt	= *sOpt;

		if(sCurOpt[0] != '-') {
			if(!*__env.sWorkspacePath) {
				// get root path
				GetRoot(sCurOpt);
				iSize--;
				sOpt++;
				continue;
			} else {
				LOGE("Root directory must be specified once.");
				return FALSE;
			}
		}

		iSize--;
		sCurOpt++;
		sOpt++;
#define CHECK_PARAM(s)	if(!strcmp(sCurOpt, s))
#define PROCEED_NEXT_PARAM	\
	if(!iSize) return FALSE; \
	sCurOpt	= *sOpt; \
	iSize--; \
	sOpt++;
		CHECK_PARAM("help") {
			ShowHelp();
			exit(0);
		}
	}

	return TRUE;
}


int main(int argc, const char* argv[])
{
	if(argc == 1) {
		ShowHelp();
		return 0;
	}

	if(!CheckParams(argc - 1, &argv[1])) {
		LOGE("Option check is failed!");
		return 0;
	}

	if(__env.sWorkspacePath[0]) {
		char sTestDriveDir[4096];
		GetEnvironmentVariable("TESTDRIVE_PROFILE", sTestDriveDir, 4096);
		cstring sCommand, sArg;
		sCommand.Format("%scommon\\common\\eclipse.bat", sTestDriveDir);
		sArg.Format("\"%s\" \"%s\"", __env.sCurrentPath, __env.sWorkspacePath);
		ShellExecute(NULL, NULL, sCommand, sArg, NULL, SW_SHOW);
	} else {
		LOGE("workspace is not existed.");
	}

	return 0;
}
