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
// Title : Private dynamic document
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "ALUTest.h"
#include "testdrive_document.inl"

REGISTER_LOCALED_DOCUMENT(ALUTest)

#define RANGE_SAMPLE	7
static LPCTSTR		__sRangeSample[RANGE_SAMPLE] = {
	_T("0"),
	_T("1"),
	_T("-1"),
	_T("1.570797 (PI/2)"),
	_T("-1.570797 (-PI/2)"),
	_T("3.141593 (PI)"),
	_T("-3.141593 (-PI)")
};

BOOL GetTopDesignFilePath(int iID, LPTSTR sDesignPath)
{
	CString sKeyName;
	CString sTopListFile	= g_pSystem->RetrieveFullPath(_T("%PROJECT%/System/HDL/top_list.ini"));
	sKeyName.Format(_T("DESIGN_%d"), iID);
	GetPrivateProfileString(_T("ALU_TEST"), sKeyName, _T(""), sDesignPath, 1024, sTopListFile);

	if(*sDesignPath) return TRUE;

	return FALSE;
}

ALUTest::ALUTest(ITDDocument* pDoc)
{
	m_pDoc	= pDoc;
	// get chart objects
	{
		m_pChart			= pDoc->GetChart(_T("Chart"));

		for(int i = 0; i < CHART_SERIES_SIZE; i++) {
			m_pChartSeries[i] = m_pChart->GetSerie(i);
		}
	}
	m_pReport = pDoc->CreateReport(NULL, 0, 4, 200, 28);
	m_pReport->SetFont(_T("dotum"));

	for(int i = 0; i < BTN_ID_SIZE; i++) {
		ITDButton*	pBtn	= pDoc->CreateButton(NULL, 0, 0, 120, 28);
		pBtn->SetManager(this, i);
		m_pBtn[i]	= pBtn;
	}

	m_pBtn[BTN_ID_BUILD]->SetText(_L(BTN_BUILD));
	m_pBtn[BTN_ID_RUN]->SetText(_L(BTN_RUN));
	{
		// target
		m_sTopDesign.GetBuffer(MAX_PATH);
		m_sTopDesign = _T("top.v");
		m_pProperty[PROPERTY_TOP_DESIGN]	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_TOP_DESIGN, _L(TOP_DESIGN), (DWORD_PTR)m_sTopDesign.GetBuffer(), _L(DESC_TOP_DESIGN));
		{
			TCHAR sDesign[1024];

			for(int i = 0; GetTopDesignFilePath(i, sDesign); i++)
				m_pProperty[PROPERTY_TOP_DESIGN]->AddOption(sDesign);
		}
		// sampling size
		m_dwSamplingSize = 1000;
		m_pProperty[PROPERTY_SAMPLE_SIZE]	= pDoc->AddPropertyData(PROPERTY_TYPE_INT, PROPERTY_SAMPLE_SIZE, _L(SAMPLE_SIZE), (DWORD_PTR)&m_dwSamplingSize, _L(DESC_SAMPLE_SIZE));
		// sampling start
		m_fSampleStart	= 0.f;
		m_pProperty[PROPERTY_SAMPLE_START]	= pDoc->AddPropertyData(PROPERTY_TYPE_FLOAT, PROPERTY_SAMPLE_START, _L(SAMPLE_START), (DWORD_PTR)&m_fSampleStart, _L(DESC_SAMPLE_START));

		for(int i = 0; i < RANGE_SAMPLE; i++) m_pProperty[PROPERTY_SAMPLE_START]->AddOption(__sRangeSample[i]);

		// sampling end
		m_fSampleEnd		= 3.141592f / 2.f;
		m_pProperty[PROPERTY_SAMPLE_END]	= pDoc->AddPropertyData(PROPERTY_TYPE_FLOAT, PROPERTY_SAMPLE_END, _L(SAMPLE_END), (DWORD_PTR)&m_fSampleEnd, _L(DESC_SAMPLE_END));

		for(int i = 0; i < RANGE_SAMPLE; i++) m_pProperty[PROPERTY_SAMPLE_END]->AddOption(__sRangeSample[i]);

		for(int i = 0; i < PROPERTY_SIZE; i++) {
			if(m_pProperty[i]) m_pProperty[i]->UpdateConfigFile();
		}
	}
	Reset();
	LoadALUModule();
}

ALUTest::~ALUTest(void)
{
	m_pDoc->KillTimer(0);
}

BOOL ALUTest::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData(TRUE);
	pProperty->UpdateConfigFile(FALSE);

	if(pProperty->GetID() == PROPERTY_TOP_DESIGN) {
		OnButtonClick(BTN_ID_BUILD);
	}

	return TRUE;
}

void ALUTest::OnButtonClick(DWORD dwID)
{
	switch(dwID) {
	case BTN_ID_BUILD:
		Reset();
		Build();
		break;

	case BTN_ID_RUN:
		if(!m_dwCurrentSampleID) {
			Reset();
			m_pBtn[BTN_ID_RUN]->SetText(_L(BTN_STOP));
			m_pDoc->SetTimer(0, 0);
		} else {
			Reset();
		}

		break;
	}
}

void ALUTest::EnableButtons(BOOL bEnable)
{
	for(int i = 0; i < BTN_ID_SIZE; i++) {
		m_pBtn[i]->GetObject()->SetEnable(bEnable);
	}
}

void ALUTest::Reset(void)
{
	m_pDoc->KillTimer(0);
	m_pReport->SetText(_L(READY));
	// initialize variables
	m_dwCurrentSampleID = 0;
	m_iCurUlp			= 0;
	m_iMaxUlp			= 0;
	m_fSample			= m_fSampleStart;
	m_dwProgress		= -1;

	for(int i = 0; i < CHART_SERIES_SIZE; i++)
		if(m_pChartSeries[i]) m_pChartSeries[i]->Clear();

	m_pBtn[BTN_ID_RUN]->SetText(_L(BTN_RUN));
}

BOOL ALUTest::LoadALUModule(void)
{
	BOOL bRet	= m_SimALU.Initialize(g_pSystem->RetrieveFullPath(_T("%PROJECT%Profiles\\ALUTest\\SimTop.dll")));

	if(!bRet)
		g_pSystem->LogWarning(_L(SIMULATION_MODULE_NEED_TO_BUILD));
	else
		g_pSystem->LogInfo(_L(SIMULATION_MODULE_LOADING_IS_SUCCESSFUL));

	return bRet;
}

BOOL ALUTest::Build(void)
{
	int iRet	= 0;
	EnableButtons(FALSE);
	g_pSystem->ClearLog();
	m_SimALU.Release();
	g_pSystem->LogInfo(_L(BUILDING_ALU_HW), (LPCTSTR)m_sTopDesign);
	iRet = g_pSystem->ExecuteFile(_T("%PROJECT%Profiles\\ALUTest\\verilator\\project\\hw_build.bat"), m_sTopDesign, TRUE, NULL, _T("%PROJECT%Profiles\\ALUTest\\verilator"),
								  _T("%Error: "), -1,
								  NULL);

	if(iRet < 0) {
		g_pSystem->LogError(_L(HW_BUILD_IS_FAILED));
		goto END_BUILD;
	}

	iRet = g_pSystem->ExecuteFile(_T("make"), _T(""), TRUE, NULL, _T("%PROJECT%Profiles\\ALUTest\\verilator\\project"),
								  _T("error: "), -1,
								  NULL);

	if(iRet < 0) {
		g_pSystem->LogError(_T("H/W build error is occurred!"));
		goto END_BUILD;
	} else
		g_pSystem->LogInfo(_T("H/W build is successful."));

	if(!LoadALUModule()) {
		iRet = -1;
		goto END_BUILD;
	}

END_BUILD:
	EnableButtons();
	return (iRet >= 0);
}

BOOL ALUTest::Simulate(void)
{
	float output_real, output_est;

	// run simulation
	if(!m_SimALU.SimulateALU(m_fSample, output_est, output_real)) {
		g_pSystem->LogError(_T("Error is occurred in ALU running.\n"));
		return FALSE;
	}

	// calculate ULP
	{
		DWORD error = (DWORD)abs((*(int*)&output_real) - (*(int*)&output_est));
		int ulps	= (error & 0xFFFF0000) ? 32 : 16;

		for(DWORD mask = 1 << (ulps - 1); ulps > 0; ulps--, mask >>= 1) {
			if(error & mask) break;
		}

		if(m_iCurUlp < ulps) m_iCurUlp = ulps;

		if(m_iMaxUlp < ulps) m_iMaxUlp = ulps;
	}

	if(m_dwSamplingSize) {
		// report
		m_pReport->SetText(_T("Current(%d/%d) Max error ulp = %d(0x%X)"), m_dwCurrentSampleID, m_dwSamplingSize, m_iMaxUlp, m_iMaxUlp);
		// update chart
		UpdateChart(output_real, output_est);

		if(m_dwCurrentSampleID == m_dwSamplingSize) {
			m_dwCurrentSampleID	= 0;
			return FALSE;
		}

		// next sample
		m_dwCurrentSampleID++;
		m_fSample = (float)(m_fSampleStart + ((double)(m_fSampleEnd - m_fSampleStart) * m_dwCurrentSampleID) / m_dwSamplingSize);
	} else {
		// report
		float progress	= m_fSampleEnd - m_fSampleStart;
		progress	= progress ? ((m_fSample - m_fSampleStart) * 100.f / progress) : 100.f;
		m_pReport->SetText(_T("Current(%.06f%%) Max error ulp = %d(0x%X), Value = %f (0x%08X)"), progress, m_iMaxUlp, m_iMaxUlp, m_fSample, (*(DWORD*)&m_fSample));
		// update chart
		{
			DWORD dwProgress = (DWORD)(progress * 50.f);

			if(dwProgress != m_dwProgress) {
				UpdateChart(output_real, output_est);
				m_dwProgress	= dwProgress;
			}
		}

		if(m_fSample == m_fSampleEnd) {
			m_dwCurrentSampleID	= 0;
			return FALSE;
		}

		m_dwCurrentSampleID++;
		// next sample
		{
			union {
				float		f;
				struct {
					DWORD		m			: 23;	// mantissa
					DWORD		e			: 8;	// exponent
					DWORD		s			: 1;	// sign
				};
				struct {
					DWORD		abs			: 31;	// absolute value
					DWORD		/*dummy*/	: 0;
				};
			} current, target;
			current.f	= m_fSample;
			target.f	= m_fSampleEnd;

			if(!current.abs) current.s	= target.s;

			if((current.s == target.s) ^ (current.abs < target.abs)) {
				current.abs--;
			} else {
				current.abs++;
			}

			m_fSample	= current.f;
		}
	}

	return TRUE;
}

void ALUTest::OnSize(int width, int height)
{
	height -= 32;

	if(m_pChart && height > 0) {
		ITDLayout* pLayout = m_pChart->GetObject()->GetLayout();
		pLayout->SetSize(width, height - 4);

		for(int i = 0; i < BTN_ID_SIZE; i++) {
			pLayout = m_pBtn[i]->GetObject()->GetLayout();
			pLayout->SetPosition(i * 125, height);
		}

		ITDLayout* pLayoutTarget = m_pReport->GetObject()->GetLayout();
		pLayoutTarget->SetPosition(250 + 5, height + 4);
		pLayoutTarget->SetWidth(width - (250 + 5));
	}
}

BOOL ALUTest::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch(command) {
	case 0:	// run simulator
		m_pDoc->KillTimer(0);

		if(!Simulate()) {
			m_pBtn[BTN_ID_RUN]->SetText(_L(BTN_RUN));
			break;
		}

		m_pDoc->SetTimer(0, 0);
		break;
	}

	return TRUE;
}

void ALUTest::UpdateChart(float real_value, float est_value)
{
	if(((*((DWORD*)&real_value)) & 0x7f800000) != 0x7f800000)	// infinite max
		m_pChartSeries[CHART_SERIES_CORRECT]->AddPoint(m_fSample, (double)real_value);

	if(((*((DWORD*)&est_value)) & 0x7f800000) != 0x7f800000)	// infinite max
		m_pChartSeries[CHART_SERIES_APPROXIMATE]->AddPoint(m_fSample, (double)est_value);

	m_pChartSeries[CHART_SERIES_ULP]->AddPoint(m_fSample, (double)m_iCurUlp);
	m_iCurUlp	= 0;
}
