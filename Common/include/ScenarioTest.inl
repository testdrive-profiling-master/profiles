//================================================================================
// Copyright (c) 2013 ~ 2018. HyungKi Jeong(clonextop@gmail.com)
// 
// MIT License
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
// Rev.  : 6/21/2018 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SCENARIO_TEST_INLINE__
#define __SCENARIO_TEST_INLINE__
#include <tchar.h>
#include <stdio.h>
#include "ScenarioTestStatus.h"

LPCTSTR	__sTestStatusList[TEST_STATUS_SIZE] = {
	_T("Status : Not tested"),
	_T("Status : Process is passed"),
	_T("Status : Process is failed"),
	_T("Status : Program is crashed"),
	_T("Status : Compilation is failed"),
	_T("Status : Linking is failed"),
	_T("Status : File is not found"),
	_T("Status : System is not ready"),
	_T("Status : Score = %lf"),
};

#ifndef DOCUMENT_EXPORTS
void TestResultOut(TEST_STATUS id)
{
	if(id < TEST_STATUS_SCORE) {
		fflush(stdout);
		printf("%s\n", __sTestStatusList[id]);
		fflush(stdout);
	}
}

void TestScoreOut(double fScore)
{
	fflush(stdout);
	printf(__sTestStatusList[TEST_STATUS_SCORE], fScore);
	printf("\n");
	fflush(stdout);
}

#include <windows.h>
int GetScenarioTestSize(void)
{
	int iSize	= 0;
	{
		TCHAR	sSize[128];

		if(GetEnvironmentVariable("GROUP_TEST_SIZE", sSize, MAX_PATH)) {
			iSize	= atoi(sSize);
		}
	}
	return iSize;
}
#endif

#endif//__SCENARIO_TEST_INLINE__
