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
#ifndef __ALU_TEST_H__
#define __ALU_TEST_H__
#include "SimALU.h"

typedef enum{
	PROPERTY_TOP_DESIGN,
	PROPERTY_SAMPLE_SIZE,
	PROPERTY_SAMPLE_START,
	PROPERTY_SAMPLE_END,
	PROPERTY_SIZE
}PROPERTY_ID;

typedef enum{
	CHART_SERIES_ULP,
	CHART_SERIES_CORRECT,
	CHART_SERIES_APPROXIMATE,
	CHART_SERIES_SIZE
}CHART_SERIES;

typedef enum{
	BTN_ID_BUILD,
	BTN_ID_RUN,
	BTN_ID_SIZE
}BTN_ID;


class ALUTest :
	public TDImplDocumentBase,
	public ITDButtonManager
{
public:
	ALUTest(ITDDocument* pDoc);
	virtual ~ALUTest(void);

	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);
	STDMETHOD_(void, OnButtonClick)(DWORD dwID);
	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(void, OnSize)(int width, int height);

	BOOL Build(void);
	BOOL LoadALUModule(void);
	void EnableButtons(BOOL bEnable = TRUE);

protected:
	ITDDocument*		m_pDoc;
	CString				m_sProjectPath;
	CString				m_sTopDesign;

	float				m_fSampleStart;
	float				m_fSampleEnd;
	float				m_fSample;
	DWORD				m_dwProgress;
	DWORD				m_dwSamplingSize;
	DWORD				m_dwCurrentSampleID;

	ITDPropertyData*	m_pProperty[PROPERTY_SIZE];
	ITDChart*			m_pChart;
	ITDChartSerie*		m_pChartSeries[CHART_SERIES_SIZE];
	ITDReport*			m_pReport;
	ITDButton*			m_pBtn[BTN_ID_SIZE];

	int					m_iCurUlp;
	int					m_iMaxUlp;

	SimALU				m_SimALU;

	void Reset(void);
	BOOL Simulate(void);
	void UpdateChart(float real_value, float est_value);
};
#endif//__ALU_TEST_H__
