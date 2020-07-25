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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "STDInterface.h"
#include "ScenarioTestStatus.h"
#include "hw/MTSP.h"
#include <io.h>
#include <list>
#include <math.h>
using namespace std;

typedef void (*CONFORMANCE_TEST_FUNC)(MTSP* pMTSP, MTSP_REGMAP* pReg);

class ConformanceTestItem;

class ConformanceTest {
public:
	ConformanceTest(const char* sSystem, const char* sTitle);
	virtual ~ConformanceTest(void);
	BOOL CheckSystemValidation(const char* sSystemDescription);
	const char* Title(void)				{return m_sTitle;}
	ConformanceTest* Next(void)			{return m_pNext;}
	ConformanceTestItem* Item(const char* sName);
	void AddItem(ConformanceTestItem* pItem);

	static ConformanceTest* Find(const char* sTitle);
	static void ReleaseAll(void);

private:
	static ConformanceTest*		m_pHead;
	ConformanceTest*			m_pNext;
	const char*					m_sSystem;
	const char*					m_sTitle;
	list<ConformanceTestItem*>	m_ItemList;
};

class ConformanceTestItem {
public:
	ConformanceTestItem(ConformanceTest* pRoot, const char* sName, CONFORMANCE_TEST_FUNC func);
	virtual ~ConformanceTestItem(void);
	const char* Name(void)	{return m_sName;}
	CONFORMANCE_TEST_FUNC Function(void)	{return m_TestFunc;}

private:
	const char*					m_sName;
	CONFORMANCE_TEST_FUNC		m_TestFunc;
};

BOOL SystemValidationCheck(MTSP* pMTSP);

#define CONFORMANCE_TEST(System, Title)	\
		static ConformanceTest	__conformance_test(System, #Title);

#define CONFORMANCE_ITEM(Name)		\
		static void __item##Name(MTSP* pMTSP, MTSP_REGMAP* pReg); \
		static ConformanceTestItem	__conformance_item##Name(&__conformance_test, #Name, __item##Name); \
		static void __item##Name(MTSP* pMTSP, MTSP_REGMAP* pReg)
