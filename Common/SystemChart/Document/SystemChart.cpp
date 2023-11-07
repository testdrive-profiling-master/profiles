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
// Title : System chart
// Rev.  : 7/24/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "SystemChart.h"
#include "testdrive_document.inl"

REGISTER_LOCALED_DOCUMENT(SystemChart)

SystemChart::SystemChart(ITDDocument* pDoc)
{
	m_pDoc					= pDoc;
	{
		// get memory
		CString	sName;
		// get system memory & it's name
		m_pMemory	= g_pSystem->GetMemory();

		if(m_pMemory) sName	= m_pMemory->GetName();

		sName	+= _T("_Chart");
		m_pMemory	= g_pSystem->GetMemory(sName, TRUE);

		if(sName.Compare(m_pMemory->GetName())) {	// if not created
			m_pMemory->Create(1024 * 1024, sName);
		}
	}
	m_pConfig				= (SYSTEM_CHART_CONFIG*)m_pMemory->GetConfig();
	m_pConfig->hWnd			= pDoc->GetWindowHandle();
	m_pCommandBuffer		= (void*)m_pMemory->GetPointer();
	// get chart objects
	m_pChart				= pDoc->GetChart(_T("Chart"));
	ZeroMemory(m_pChartSeries, sizeof(m_pChartSeries));
}

SystemChart::~SystemChart(void)
{
	m_pConfig->hWnd			= NULL;
}

BOOL SystemChart::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	pProperty->UpdateData(TRUE);
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

void SystemChart::OnSize(int width, int height)
{
	if(m_pChart && height > 0) {
		ITDLayout* pLayout = m_pChart->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
	}
}

CString GetStringFromSystemMemory(void* pBuffer)
{
	char*	str_ansi	= (char*)pBuffer;
	int		str_size	= MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, NULL, NULL);
	TCHAR*	str_unicode	= new TCHAR[str_size];
	MultiByteToWideChar(CP_ACP, 0, str_ansi, strlen(str_ansi) + 1, str_unicode, str_size);
	CString	str(str_unicode);
	delete [] str_unicode;
	return str;
}

ITDChartSerie* SystemChart::GetSerie(DWORD dwIndex)
{
	if(dwIndex >= MAX_SERIES_COUNT) return NULL;

	return m_pChartSeries[dwIndex];
}

BOOL SystemChart::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	if(command != TD_EXTERNAL_COMMAND) return 0;

	WORD	op	= HIWORD(wParam);

	switch(LOWORD(wParam)) {
	case CHART_CMD_CREATE_SERIE: {
		DWORD*			pData		= (DWORD*)m_pCommandBuffer;
		ITDChartSerie* pSerie		= m_pChart->CreateSerie((CHART_STYLE)op, pData[1], pData[2]);
		m_pChartSeries[pData[0]]	= pSerie;
	}
	break;

	case CHART_CMD_REMOVE_SERIE: {
		int iIndex = (int)lParam;

		if(iIndex < 0) {
			m_pChart->RemoveAllSeries();
			ZeroMemory(m_pChartSeries, sizeof(m_pChartSeries));
		} else {
			m_pChart->RemoveSerie(iIndex);
			m_pChartSeries[iIndex]	= NULL;
		}
	}
	break;

	case CHART_CMD_CLEAR_SERIE: {
		int iIndex = (int)lParam;

		if(iIndex < 0) {
			for(int i = 0; i < MAX_SERIES_COUNT; i++) {
				m_pChartSeries[i]->Clear();
			}
		} else {
			if(m_pChartSeries[iIndex])
				m_pChartSeries[iIndex]->Clear();
		}
	}
	break;

	case CHART_CMD_SET_NAME: {
		ITDChartSerie*	pSerie	= GetSerie(op);

		if(pSerie)
			pSerie->SetName(GetStringFromSystemMemory(m_pCommandBuffer));
	}
	break;

	case CHART_CMD_ADD_POINT: {
		ITDChartSerie*	pSerie	= GetSerie(op);
		double*			pData	= (double*)m_pCommandBuffer;

		if(pSerie)
			pSerie->AddPoint(pData[0], pData[1]);
	}
	break;

	case CHART_CMD_ADD_POINT_ARRAY: {
		ITDChartSerie*	pSerie		= GetSerie(op);
		int 			iCount		= (int)lParam;
		DWORD			dwOffsetX	= ((DWORD*)m_pCommandBuffer)[0];
		DWORD			dwOffsetY	= ((DWORD*)m_pCommandBuffer)[1];
		double*			pX			= (double*)(m_pCommandBuffer + dwOffsetX);
		double*			pY			= (double*)(m_pCommandBuffer + dwOffsetY);

		if(pSerie)
			pSerie->AddPointArray(pX, pY, iCount);
	}
	break;

	case CHART_CMD_SET_LABEL: {
		ITDChartSerie*	pSerie	= GetSerie(op);
		DWORD*			pIndex	= (DWORD*)m_pCommandBuffer;

		if(pSerie) {
			CString	sLabel	= GetStringFromSystemMemory(&(pIndex[1]));
			pSerie->AddLabel(pIndex[0], (LPCTSTR)sLabel);
			g_pSystem->LogInfo(_T("%s : %s"), m_pDoc->DocumentTitle(), (LPCTSTR)sLabel);
		}
	}
	break;

	case CHART_CMD_SET_WIDTH: {
		ITDChartSerie*	pSerie	= GetSerie(op);

		if(pSerie)
			pSerie->SetWidth((int)lParam);
	}
	break;

	case CHART_CMD_SET_SMOOTH: {
		ITDChartSerie*	pSerie	= GetSerie(op);

		if(pSerie)
			pSerie->SetSmooth((BOOL)lParam);
	}
	break;

	case CHART_CMD_SET_FOREGROUND:
		m_pDoc->SetForegroundDocument();
		break;
	}

	return TRUE;
}
