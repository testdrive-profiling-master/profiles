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
// Rev.  : 3/19/2024 Tue (clonextop@gmail.com)
//================================================================================
#ifndef __SCENARIO_TEST_STATUS_H__
#define __SCENARIO_TEST_STATUS_H__
#include <stdio.h>

typedef enum {
	TEST_STATUS_NOT_TESTED,
	TEST_STATUS_RUN_PASSED,
	TEST_STATUS_RUN_FAILED,
	TEST_STATUS_RUN_CRACHED,
	TEST_STATUS_COMPILE_FAILED,
	TEST_STATUS_LINK_FAILED,
	TEST_STATUS_FILE_NOT_FOUND,
	TEST_STATUS_SYSTEM_IS_REQUIRED,
	TEST_STATUS_SCORE,
	TEST_STATUS_SIZE
} TEST_STATUS;

#ifndef DOCUMENT_EXPORTS
void TestResultOut(TEST_STATUS id);							  // except 'TEST_STATUS_SCORE'
void TestScoreOut(double fScore, const char *sFormat = NULL); // percentage score value
int	 GetScenarioTestSize(void);								  // get total scenario test size
#endif

#endif //__SCENARIO_TEST_STATUS_H__
