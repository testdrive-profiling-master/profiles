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
// Rev.  : 7/24/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "DefaultSystemConfig.h"
#include "Chart.h"

Chart::Chart(void)
{
	m_hWnd		= NULL;
	m_pCommand	= NULL;
}

Chart::~Chart(void)
{
}

BOOL Chart::Initialize(void)
{
	if(!m_hWnd) {
		char sName[MAX_PATH];
		sprintf(sName, "%s_Chart", TESTDRIVE_MEMORY_SYSTEM_NAME);
		{
			ITestDriverMemory* pMemory = TestDriver_GetMemory(sName);

			if(pMemory) {
				m_pCommand	= pMemory->GetPointer();
				m_hWnd		= ((SYSTEM_CHART_CONFIG*)pMemory->GetConfig())->hWnd;
			}
		}
	}

	return CheckValidate();
}

void Chart::CreateSerie(DWORD dwID, const char* sName, CHART_STYLE style, BOOL bSecondaryHorizAxis, BOOL bSecondaryVertAxis)
{
	if(!CheckValidate()) return;

	DWORD*	pData	= (DWORD*)m_pCommand;
	pData[0]		= dwID;
	pData[1]		= bSecondaryHorizAxis;
	pData[2]		= bSecondaryVertAxis;
	SendMessage(m_hWnd, WM_USER, CHART_CMD_CREATE_SERIE | (((DWORD)style) << 16), 0);

	if(sName) SetName(dwID, sName);
}

void Chart::RemoveSerie(DWORD dwIndex)
{
	if(!CheckValidate()) return;

	// remove all series if dwIndex == -1
	SendMessage(m_hWnd, WM_USER, CHART_CMD_REMOVE_SERIE, dwIndex);
}

void Chart::ClearSerie(DWORD dwIndex)
{
	if(!CheckValidate()) return;

	// clear all series if dwIndex == -1
	SendMessage(m_hWnd, WM_USER, CHART_CMD_CLEAR_SERIE, dwIndex);
}

void Chart::SetName(DWORD dwID, const char* sName)
{
	if(!CheckValidate()) return;

	strcpy((char*)m_pCommand, sName);
	SendMessage(m_hWnd, WM_USER, CHART_CMD_SET_NAME | (dwID << 16), 0);
}

void Chart::AddPoint(DWORD dwID, double Xvalue, double Yvalue)
{
	if(!CheckValidate()) return;

	double* pData	= (double*)m_pCommand;
	pData[0]		= Xvalue;
	pData[1]		= Yvalue;
	SendMessage(m_hWnd, WM_USER, CHART_CMD_ADD_POINT | (dwID << 16), 0);
}

void Chart::AddPointArray(DWORD dwID, DWORD dwCount, double* pXvalues, double* pYvalues)
{
	if(!CheckValidate()) return;

	double* pX		= (double*)((BYTE*)m_pCommand + sizeof(DWORD) * 2);
	double* pY		= &(((double*)((BYTE*)m_pCommand + sizeof(DWORD) * 2))[dwCount]);
	((DWORD*)m_pCommand)[0]	= sizeof(DWORD) * 2;
	((DWORD*)m_pCommand)[1]	= sizeof(DWORD) * 2 + sizeof(double) * dwCount;
	memcpy(pX, pXvalues, sizeof(double)*dwCount);
	memcpy(pY, pYvalues, sizeof(double)*dwCount);
	SendMessage(m_hWnd, WM_USER, CHART_CMD_ADD_POINT | (dwID << 16), dwCount);
}

void Chart::SetLabel(DWORD dwID, DWORD dwIndex, const char* sLabel)
{
	if(!CheckValidate()) return;

	*((DWORD*)m_pCommand)	= dwIndex;
	strcpy(((char*)m_pCommand) + 4, sLabel);
	SendMessage(m_hWnd, WM_USER, CHART_CMD_SET_LABEL | (dwID << 16), 0);
}

void Chart::SetWidth(DWORD dwID, int iWidth)
{
	if(!CheckValidate()) return;

	SendMessage(m_hWnd, WM_USER, CHART_CMD_SET_WIDTH | (dwID << 16), (LPARAM)iWidth);
}

void Chart::SetSmooth(DWORD dwID, BOOL bSmooth)
{
	if(!CheckValidate()) return;

	SendMessage(m_hWnd, WM_USER, CHART_CMD_SET_SMOOTH | (dwID << 16), (LPARAM)bSmooth);
}

void Chart::SetForeground(void)
{
	if(!CheckValidate()) return;

	SendMessage(m_hWnd, WM_USER, CHART_CMD_SET_FOREGROUND, 0);
}

