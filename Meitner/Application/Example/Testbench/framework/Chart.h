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
#ifndef __DPI_CHART_H__
#define __DPI_CHART_H__
#include "common.h"

typedef enum {
	CHART_STYLE_POINT,
	CHART_STYLE_LINE,
	CHART_STYLE_SURFACE,
	CHART_STYLE_BAR,
	CHART_STYLE_CANDLESTICK,
	CHART_STYLE_GANTT
} CHART_STYLE;

class Chart  : protected TestDriveResource
{
public:
	Chart(void);
	~Chart(void);

	BOOL Initialize(void);

	void CreateSerie(DWORD dwID, const char* sName = NULL, CHART_STYLE style = CHART_STYLE_LINE, BOOL bSecondaryHorizAxis = FALSE, BOOL bSecondaryVertAxis = FALSE);
	void RemoveSerie(DWORD dwIndex = (DWORD) - 1);
	void SetName(DWORD dwID, const char* sName);
	void AddPoint(DWORD dwID, double Xvalue, double Yvalue);
	void AddPointArray(DWORD dwID, DWORD dwCount, double* pXvalues, double* pYvalues);
	void SetLabel(DWORD dwID, DWORD dwIndex, const char* sLabel);
	void SetWidth(DWORD dwID, int iWidth);
	void SetSmooth(DWORD dwID, BOOL bSmooth);
	void SetForeground(void);

protected:
	inline	BOOL CheckValidate(void) {
		if(m_hWnd) {
			if(!IsWindow(m_hWnd)) m_hWnd = NULL;
		}

		return m_hWnd != NULL;
	}

private:
	HWND				m_hWnd;
	void*				m_pCommand;
};

#endif//__DPI_CHART_H__
