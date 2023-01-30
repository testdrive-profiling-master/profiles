//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Testbench
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "Testbench.h"
#include "ScenarioTest.inl"
#include "Conformance.h"
#include "hw/MTSP.h"

char					g_sTitle[MAX_PATH], g_sItem[MAX_PATH];

bool GetArgName(const char* sArg, char* sTitle, char* sItem)
{
	char	sCom[1024], *sTok;

	if(!sArg) return false;

	{
		// get name only
		strcpy(sCom, sArg);

		while(char* sTarget = strstr(sCom, "/")) * sTarget = '\\';	// convert windows style '\'

		{
			// change to "title + '\' + item + .ext" type
			char* sTarget	= sCom;

			while(sTarget) {
				char* sNext	= strstr(sTarget, "\\");

				if(!sNext) return false;

				sNext++;

				if(!strstr(sNext, "\\")) {
					break;
				}

				sTarget	= sNext;
			}

			strcpy(sCom, sTarget);
		}
	}
	{
		const char*	sDelim	= ".\\";

		// get title name
		if(!(sTok = strtok(sCom, sDelim))) return false;

		strcpy(sTitle, sTok);

		// get item name
		if(!(sTok = strtok(NULL, sDelim))) return false;

		strcpy(sItem, sTok);
	}
	return true;
}

class Testbench : public TestbenchFramework, public TestDriveResource {
	MTSP*					m_pMTSP;		// Processor (Design Under Testing)
	ConformanceTestItem*	m_pItem;		// test item

	virtual bool OnInitialize(void) {
		m_pItem	= NULL;

		if(!(m_pMTSP = new MTSP(m_pDDK))) {
			printf("*E: System is not ready...\n");
			return false;
		}

		{
			// initialize MTSP
			m_pMTSP->SetClock(300);	// set 300MHz
			printf("*I: MTSP Command FiFo count = %d entries\n", m_pMTSP->GetCommandFiFoSize());
		}

		{
			const char* sSystemDesc	= m_pDDK->GetSystemDescription();

			// check simulation system...
			if(!strstr(sSystemDesc, "Simulation Simplified")) {
				printf("\t*E: Must be simulation only...\n");
				return false;
			}
		}

		{
			// Get test item
			ConformanceTest* 		pRoot	= ConformanceTest::Find(g_sTitle);

			if(!pRoot) {
				printf("*I: This test root title is not exist yet. : \"%s\"\n", g_sTitle);
				return false;
			}

			if(!pRoot->CheckSystemValidation(m_pDDK->GetSystemDescription())) {
				TestResultOut(TEST_STATUS_SYSTEM_IS_REQUIRED);
				return false;
			}

			if(!(m_pItem = pRoot->Item(g_sItem))) {
				printf("*I: This test item is not exist yet. : \"%s (%s)\"\n", g_sTitle, g_sItem);
				return false;
			}

			printf("Target Conformance Test : \"%s (%s)\"\n", g_sTitle, g_sItem);
		}

		return true;
	}

	virtual void OnRelease(void) {
		m_pMTSP->SetClock(50);	// set minimum operation speed
		SAFE_DELETE(m_pMTSP);
	}

	virtual bool OnTestBench(void) {
		// start test
		m_pItem->Function()(m_pMTSP, (MTSP_REGMAP*)m_TestDrive.Memory.pDisplay->GetPointer(0, 0));
		printf("Conformance test is done!\n");
		return true;
	}
};

int main(int argc, char** argv)
{
	Testbench	tb;
	{
		// arguments check
		if(argc != 2) {
			printf("*E: No argument...\n");
			return false;
		}

		if((_access(argv[1], 0)) == -1) {
			printf("*E: File is not existed...\n");
			return false;
		}
	}

	if(!GetArgName(argv[1], g_sTitle, g_sItem)) {
		printf("*E: Invalid argument...\n");
		return false;
	}

	if(tb.Initialize()) {
		if(!tb.DoTestbench())
			printf("Testbench is failed.\n");
	} else {
		printf("Initialization is failed.\n");
	}

	tb.Release();
}
