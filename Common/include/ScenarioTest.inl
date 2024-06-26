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
// Title : Common profiles
// Rev.  : 5/3/2024 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __SCENARIO_TEST_INLINE__
#define __SCENARIO_TEST_INLINE__
#include "ScenarioTestStatus.h"
#include <string>
#include <tchar.h>
#include <windows.h>

using namespace std;

LPCTSTR __sTestStatusList[TEST_STATUS_SIZE] = {
	_T("Status : Not tested"),		   _T("Status : Process is passed"),	 _T("Status : Process is failed"),
	_T("Status : Program is crashed"), _T("Status : Compilation is failed"), _T("Status : Linking is failed"),
	_T("Status : File is not found"),  _T("Status : System is not ready"),	 _T("Status : Score = %.10lf"),
};

#ifndef DOCUMENT_EXPORTS
void TestResultOut(TEST_STATUS id)
{
	if (id < TEST_STATUS_SCORE) {
		fflush(stdout);
		printf("%s\n", __sTestStatusList[id]);
		fflush(stdout);
	}
}

void TestScoreOut(double fScore, const char *sFormat)
{
	fflush(stdout);
	if (sFormat) {
		string sFormat("Status : Score = ");
		sFormat += sFormat;
		printf(sFormat.c_str(), fScore);
	} else {
		printf(__sTestStatusList[TEST_STATUS_SCORE], fScore);
	}
	printf("\n");
	fflush(stdout);
}

int GetScenarioTestSize(void)
{
	int iSize = 0;
	{
		TCHAR sSize[128];

		if (GetEnvironmentVariable("GROUP_TEST_SIZE", sSize, MAX_PATH)) {
			iSize = atoi(sSize);
		}
	}
	return iSize;
}
#endif

#endif //__SCENARIO_TEST_INLINE__
