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
// Rev.  : 12/28/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "RegmapButton.h"


RegmapButton::RegmapButton(void) : Regmap(_T("BTN"))
{
}

RegmapButton::~RegmapButton(void)
{
}

BOOL RegmapButton::OnUpdate(void)
{
	return FALSE;
}

void RegmapButton::OnBroadcast(LPVOID pData)
{
	if(!pData) {
		UpdateSwitches();
	}
}

BOOL RegmapButton::OnCommand(LPCTSTR lpszURL)
{
	bool	bPressed	= (*lpszURL == _T('d'));
	int		iType, iID;
	{
		LPCTSTR		sDelim	= _T(".");
		int			iPos = 0;
		CString		sTok(lpszURL + 2);
		iType	= _ttoi(sTok.Tokenize(sDelim, iPos));
		iID		= _ttoi(sTok.Tokenize(sDelim, iPos));
	}

	switch(iType) {
	case 0:	// just link
		if(!bPressed) {
			switch(iID) {
			case 0:	// CPU
				g_pSystem->LogWarning(_T("Document is not ready yet."));
				break;

			case 1:	// Memory
				g_pSystem->ExecuteFile(_T("%PROJECT%/Profiles/common/view_system_memory.bat"), NULL, FALSE, NULL, NULL, NULL);
				break;

			case 2:	// Numeric Display
				g_pSystem->ExecuteFile(_T("explorer"), _T("https://cdn-shop.adafruit.com/datasheets/1001datasheet.pdf"), FALSE, NULL, NULL, NULL);
				break;
			}
		}

		break;

	case 1:	// push button
		if(bPressed)
			TestDrivePlaySound(_T("btn_start.wav"));
		else
			TestDrivePlaySound(_T("btn_end.wav"));

		if(bPressed)
			m_pReg->buttons	&= ~(1 << iID);
		else
			m_pReg->buttons	|= (1 << iID);

		break;

	case 2:	// toggle switch
		if(!bPressed) {
			TestDrivePlaySound(_T("switch.wav"));
			m_pReg->switches ^= (1 << iID);
			UpdateSwitches();
		}

		break;
	}

	return FALSE;
}

void RegmapButton::UpdateSwitches(void)
{
	g_pHtml->CallJScript(_T("SetSwitch(%s,%s,%s,%s,%s,%s,%s,%s)"),
						 (m_pReg->switches & (1 << 0)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 1)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 2)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 3)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 4)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 5)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 6)) ? _T("true") : _T("false"),
						 (m_pReg->switches & (1 << 7)) ? _T("true") : _T("false"));
}
