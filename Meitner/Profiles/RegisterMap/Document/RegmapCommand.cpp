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
// Title : Meitner processor register map document
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "RegmapCommand.h"

RegmapCommand::RegmapCommand(void) : Regmap(_T("Command"))
{
	m_pCommand	= &m_pReg->command;
}

RegmapCommand::~RegmapCommand(void)
{
}

BOOL RegmapCommand::OnUpdate(void)
{
	if(m_pCommand->bUpdate) {
		m_pCommand->bUpdate	= FALSE;
		UpdateCommand();
	}

	return FALSE;
}

void RegmapCommand::OnBroadcast(LPVOID pData)
{
	if(!pData) {	// on initialization
		m_pCommand->bUpdate		= TRUE;
	}
}

BOOL RegmapCommand::OnCommand(LPCTSTR lpszURL)
{
	//m_pCommand->bUpdate		= TRUE;
	//UpdateCommand();
	return FALSE;
}

void RegmapCommand::UpdateCommand(void)
{
	int				iPos		= m_pCommand->iPos;
	DWORD			dwCount		= m_pCommand->dwCount;
	COMMAND_TRACE*	pTrace		= m_pCommand->history;

	if(dwCount > 16) dwCount = 16;

	iPos	= (iPos - dwCount) & (MAX_COMMAND_HISTORY_SIZE - 1);
	{
		HString	sHistory;
		CString sTime;

		for(int i = 0; i < dwCount; i++) {
			if(i) {
				sHistory.Append(_T("<br>"));
				sTime.Append(_T("<br"));
			}

			{
				CString			sCommand, sTimestamp;
				COMMAND_TRACE*	pCommand	= &pTrace[iPos];
				DecodeCommand(sCommand, pCommand);
				PresentTime(sTimestamp, pCommand->uiTime);
				sHistory.AppendFormat(_T(": %s"), (LPCTSTR)sCommand);
				sTime.Append(sTimestamp);
			}

			iPos = (iPos + 1) & (MAX_COMMAND_HISTORY_SIZE - 1);
		}

		SetText(_T("CMD"), sHistory);
		SetText(_T("CMD_TIME"), sTime);
	}
}

void RegmapCommand::DecodeCommand(CString& sCommand, COMMAND_TRACE* pCommand)
{
	switch(pCommand->id) {
	case 0:
	case 1:
	case 2:
	case 3:
		sCommand.Format(_T("Set memory descriptor[%d] = 0x%08X"), pCommand->id, pCommand->dwData);
		break;

	case 4:
		sCommand.Format(_T("Start PC(0x%08X)"), pCommand->dwData);
		break;

	case 5:
		sCommand.Format(_T("Start Video Base(0x%08X)"), pCommand->dwData);
		break;

	case 8:
		sCommand.Append(_T("Do memory control"));
		break;

	case 11:
		sCommand.Append(_T("Wait for ("));

		if(pCommand->dwData & 1)
			sCommand.Append(_T("Process done."));

		if(pCommand->dwData & 2)
			sCommand.Append(_T("Memory done."));

		sCommand.Append(_T(")"));
		break;

	default:
		sCommand.Format(_T("[Unknown] id(%d), data(0x%08X)"), pCommand->id, pCommand->dwData);
		break;
	}
}
