//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Starter Kit document
// Rev.  : 12/31/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "RegmapTFT.h"

RegmapTFT::RegmapTFT(void) : Regmap(_T("TFT"))
{
	m_pTFT		= &m_pReg->tft_lcd_display;
	m_pBuffer	= g_pDoc->CreateBuffer(_T("TFT"), 0, 0, 200, 200);
	m_pBuffer->Create(TFT_LCD_DISPLAY_WIDTH, TFT_LCD_DISPLAY_HEIGHT, COLORTYPE_ARGB_8888);
	m_pBuffer->SetEnable(BUFFER_INTERFACE_ZOOM, FALSE);
	m_pBuffer->SetEnable(BUFFER_INTERFACE_ANTIALIASING, TRUE);
	m_pBuffer->SetEnable(BUFFER_INTERFACE_LINK, FALSE);
	m_pBuffer->SetEnable(BUFFER_INTERFACE_LOAD_FROM_FILE, FALSE);
	m_pBuffer->SetEnable(BUFFER_INTERFACE_AUTOFIT, FALSE);
}

RegmapTFT::~RegmapTFT(void)
{
}

BOOL RegmapTFT::OnUpdate(void)
{
	if(m_pTFT->bUpdate) {
		m_pTFT->bUpdate	= false;
		UpdateData();
		return true;
	}

	return FALSE;
}

void RegmapTFT::OnBroadcast(LPVOID pData)
{
	if(!pData) {
		UpdateData();
	}
}

BOOL RegmapTFT::OnCommand(LPCTSTR lpszURL)
{
	int		sx, sy, width, height;
	ITDLayout*	pBufferLayout	= m_pBuffer->GetObject()->GetLayout();
	{
		// parsing
		LPCTSTR	sDelim	= _T(".");
		int		iPos	= 0;
		CString	sTok(lpszURL);
		sx		= _ttoi(sTok.Tokenize(sDelim, iPos));
		sy		= _ttoi(sTok.Tokenize(sDelim, iPos));
		width	= _ttoi(sTok.Tokenize(sDelim, iPos));
		height	= _ttoi(sTok.Tokenize(sDelim, iPos));
	}
	//m_pBuffer->SetDrawRect(&rc);
	pBufferLayout->SetPosition(sx, sy);
	pBufferLayout->SetSize(width, height);
	m_pBuffer->GetObject()->UpdateLayout();
	g_pSystem->LogInfo(_T("Got : %s (%d,%d)"), lpszURL, sx, sy);
	return FALSE;
}

void RegmapTFT::UpdateData(void)
{
	m_pBuffer->CopyFromMemory((const BYTE*)m_pTFT->buffer.front, TFT_LCD_DISPLAY_WIDTH * sizeof(DWORD), FALSE);
	m_pBuffer->Present();
}
