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
// Title : Hierarchical Make
// Rev.  : 8/25/2025 Mon (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"

struct {
	char	sRootPath[4096];
	cstring sArg;
	int		iErrorCode;
	bool	bFoundMakefile;
	bool	bUseDebug;
} __env;

void ShowHelp(void)
{
	printf("Hierarchical Make 1.0c (" __DATE__ ", Q&A : clonextop@gmail.com)\n\n");
	printf(
		"Usage : HierarchicalMake.exe [options] root_directory\n"
		"\t-arg <arguments>    set build arguments\n"
		"\t                    ex) -arg \"-j clean\"\n");
}

bool CheckParams(int iSize, const char *sOpt[])
{
	memset(&__env, 0, sizeof(__env));

	while (iSize > 0) {
		const char *sCurOpt = *sOpt;

		if (sCurOpt[0] != '-') {
			if (!*__env.sRootPath) {
				// get root path
				GetFullPathName(sCurOpt, 4096, __env.sRootPath, NULL);
				iSize--;
				sOpt++;
				continue;
			} else {
				LOGE("Root directory must be specified once.");
				return false;
			}
		}

		iSize--;
		sCurOpt++;
		sOpt++;
#define CHECK_PARAM(s) if (!strcmp(sCurOpt, s))
#define PROCEED_NEXT_PARAM                                                                                                                      \
	if (!iSize)                                                                                                                                 \
		return false;                                                                                                                           \
	sCurOpt = *sOpt;                                                                                                                            \
	iSize--;                                                                                                                                    \
	sOpt++;
		CHECK_PARAM("arg")
		{
			PROCEED_NEXT_PARAM
			__env.sArg = sCurOpt;
		}
		else CHECK_PARAM("help")
		{
			ShowHelp();
			exit(0);
		}
	}

	return true;
}

bool LoopSearchPath(const char *sPath)
{
	bool			bRet = false;
	HANDLE			hFind;
	WIN32_FIND_DATA FindFileData;
	{
		// no search file definition
		cstring sNoSearchPath, sNoBuildPath;
		sNoSearchPath.Format("%s\\.TestDrive.nosearch", sPath);
		sNoBuildPath.Format("%s\\.TestDrive.nobuild", sPath);
		if (_access(sNoSearchPath, 0) != -1 || _access(sNoBuildPath, 0) != -1)
			return false;
	}
	{
		cstring sFindPath;
		sFindPath.Format("%s\\*.*", sPath);
		hFind = FindFirstFile(sFindPath, &FindFileData);
	}

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
				cstring sNextSearchPath;
				sNextSearchPath.Format("%s\\%s", sPath, FindFileData.cFileName);
				bRet |= LoopSearchPath(sNextSearchPath);
			}
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	{
		// check makefile exist...
		cstring sMakefilePath;
		sMakefilePath.Format("%s\\makefile", sPath);

		if (_access(sMakefilePath, 0) == -1)
			return false;
	}

	__env.bFoundMakefile = true;

	if (!__env.iErrorCode) {
		char	sCurPath[4096];
		cstring sCurArg(__env.sArg);

		if (__env.bUseDebug && (sCurArg.find(" clean") < 0) && (sCurArg.find(" dep") < 0)) {
			sCurArg.insert(0, "USE_DEBUG=1 ");
		}

		LOGI("Hierarchical Make : \"%s\\makefile\" %s", sPath, sCurArg.c_str());
		GetCurrentDirectory(4096, sCurPath);
		SetCurrentDirectory(sPath);
		bRet = true;
		{
			cstring sCommand;
			sCommand.Format(_T("mingw32-make %s"), sCurArg.c_str());

			if (sCommand.find(" clean") >= 0) { // "make clean"
				sCommand.insert(0, "start /B ");
			} else {
				system("mingw32-make dep"); // "make dep" first
			}

			fflush(stdout);
			__env.iErrorCode = system(sCommand);
			fflush(stdout);

			if (__env.iErrorCode) {
				LOGE("build is failed.");
			}
		}
		SetCurrentDirectory(sCurPath);
		printf("\n");
	}

	return bRet;
}

int main(int argc, const char *argv[])
{
	if (argc == 1) {
		ShowHelp();
		return 0;
	}

	if (!CheckParams(argc - 1, &argv[1])) {
		LOGE("Option check is failed!");
		return 1;
	}

	{
		// check debug mode
		cstring sUseDebug;
		__env.bUseDebug = sUseDebug.GetEnvironment("USE_DEBUG");
	}

	if (!*__env.sRootPath) {
		LOGE("Root path must be specified.");
	} else {
		LoopSearchPath(__env.sRootPath);

		if (!__env.bFoundMakefile)
			LOGE("Root directory is not exist.");
	}

	return __env.iErrorCode;
}
