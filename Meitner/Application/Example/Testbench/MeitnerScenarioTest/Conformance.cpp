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
#include "Conformance.h"

ConformanceTest*		ConformanceTest::m_pHead	= NULL;

ConformanceTest::ConformanceTest(const char* sSystem, const char* sTitle)
{
	m_pNext			= m_pHead;
	m_pHead			= this;
	m_sSystem		= sSystem;
	m_sTitle		= sTitle;
}

ConformanceTest::~ConformanceTest(void)
{
	ConformanceTest*	pNode	= m_pHead;

	if(pNode == this) {
		m_pHead	= Next();
	} else {
		while(pNode) {
			if(pNode->Next() == this) {
				pNode->m_pNext = Next();
				break;
			}

			pNode	= pNode->Next();
		}
	}
}

BOOL ConformanceTest::CheckSystemValidation(const char* sSystemDescription)
{
	if(!m_sSystem) return TRUE;

	if(sSystemDescription) {
		BOOL			bRet			= TRUE;
		const char*		sDelim			= ",;";
		char*			sRequirement	= new char[strlen(m_sSystem) + 1];
		char*			sTok;
		strcpy(sRequirement, m_sSystem);
		sTok = strtok(sRequirement, sDelim);

		while(sTok) {
			if(!strstr(sSystemDescription, sTok)) {
				bRet	= FALSE;
				break;
			}

			sTok = strtok(NULL, sDelim);
		}

		delete [] sRequirement;

		if(bRet)
			LOG("*I: System requirement('%s') is satisfied.\n", m_sSystem);
		else {
			LOG("*E: System requirement('%s') is not satisfied. (Check your current system : '%s')\n", m_sSystem, sSystemDescription);
		}

		return bRet;
	}

	return FALSE;
}

ConformanceTest* ConformanceTest::Find(const char* sTitle)
{
	ConformanceTest*	pNode	= m_pHead;

	while(pNode) {
		if(!strcmp(pNode->Title(), sTitle))
			return pNode;

		pNode	= pNode->Next();
	}

	return NULL;
}

void ConformanceTest::ReleaseAll(void)
{
	while(m_pHead) delete m_pHead;
}

ConformanceTestItem* ConformanceTest::Item(const char* sName)
{
	for(list<ConformanceTestItem*>::iterator i = m_ItemList.begin(); i != m_ItemList.end(); i++) {
		if(!strcmp((*i)->Name(), sName)) return (*i);
	}

	return NULL;
}

void ConformanceTest::AddItem(ConformanceTestItem* pItem)
{
	m_ItemList.push_back(pItem);
}

ConformanceTestItem::ConformanceTestItem(ConformanceTest* pRoot, const char* sName, CONFORMANCE_TEST_FUNC func)
{
	pRoot->AddItem(this);
	m_sName	= sName;
	m_TestFunc	= func;
}

ConformanceTestItem::~ConformanceTestItem(void)
{
}
