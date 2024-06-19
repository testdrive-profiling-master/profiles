//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Meitner processor register map document
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "RegmapCommand.h"

#define COMMAND_TABLE_LINES 8
#define COMMAND_TABLE_RANGE (MTSP_COMMAND_RING_SIZE - COMMAND_TABLE_LINES)

RegmapCommand::RegmapCommand(void) : Regmap(_T("Command"))
{
	m_pCmd	  = &m_pReg->command;
	m_dwIndex = 0;
}

RegmapCommand::~RegmapCommand(void) {}

BOOL RegmapCommand::OnUpdate(void)
{
	BOOL bRet = FALSE;

	if (m_pCmd->bUpdate) {
		m_pCmd->bUpdate = FALSE;
		UpdateCommand();
		bRet = TRUE;
	}

	return bRet;
}

void RegmapCommand::OnBroadcast(LPVOID pData)
{
	switch ((DWORD_PTR)pData) {
	case 0:
		for (int i = 0; i < COMMAND_TABLE_LINES; i++) {
			NewRow();
			NewTD();
			SetTID(_T("CMDQ_I_%d"), i);
			SetTBody(_T("-"));
			NewTD();
			SetTID(_T("CMDQ_T_%d"), i);
			SetTBody(_T("-"));
			NewTD();
			SetTID(_T("CMDQ_D_%d"), i);
			SetTClassAdd(_T("text-start"));
			SetTBody(_T("-"));

			if (!i) {
				NewTD();
				SetRowSpan(16);
				SetTID(_T("CMDQ_Slide"));
			}
		}

		break;

	case REGMAP_BROADCAST_SYSTEM_REBOOT:
		m_dwIndex = 0;
		UpdateSlider();
		break;
	}
}

BOOL RegmapCommand::OnCommand(LPCTSTR lpszURL)
{
	if (_tcsstr(lpszURL, _T("offset")) == lpszURL) {
		m_dwIndex = _tstoi(lpszURL + 5);
		UpdateCommand();
	} else if (!_tcscmp(lpszURL, _T("latest"))) {
	}

	return FALSE;
}

void RegmapCommand::UpdateCommand(void)
{
	uint32_t dwIndex = m_dwIndex;

	if (m_pCmd->dwHead > COMMAND_TABLE_LINES) {
		dwIndex += (m_pCmd->dwHead - COMMAND_TABLE_LINES);
	}

	{
		for (int i = 0; i < COMMAND_TABLE_LINES; i++) {
			MTSP_REGMAP_COMMAND_DATA *pCmd = &(m_pCmd->cmd[dwIndex & (MTSP_COMMAND_RING_SIZE - 1)]);
			CString					  sID;
			// index update
			sID.Format(_T("CMDQ_I_%d"), i);

			if (dwIndex < m_pCmd->dwHead)
				SetText(sID, _T("%d"), dwIndex);
			else
				SetText(sID, _T("-"));

			// command data
			sID.Format(_T("CMDQ_D_%d"), i);

			if (dwIndex < m_pCmd->dwHead) {
				switch (pCmd->id) {
				case 1: {
					MTSP_REG_COMMAND op;
					CString			 sInfo;
					op.m = pCmd->op;

					if (op.branch_en) {
						sInfo.AppendFormat(_T("Run 0x%X_%08X"), pCmd->data[1], pCmd->data[0]);
					}

					if (op.run_count_enable) {
						if (sInfo.length())
							sInfo += _T(", ");

						sInfo.AppendFormat(_T("RUNCOUNT"));
					}

					if (op.intrrupt_enable) {
						if (sInfo.length())
							sInfo += _T(", ");

						sInfo.AppendFormat(_T("INTR"));
					}

					if (op.icache_clear) {
						if (sInfo.length())
							sInfo += _T(", ");

						sInfo.AppendFormat(_T("I$"));
					}

					if (op.gcache_clear) {
						if (sInfo.length())
							sInfo += _T(", ");

						sInfo.AppendFormat(_T("G$"));
					}

					if (op.lcache_clear) {
						if (sInfo.length())
							sInfo += _T(", ");

						sInfo.AppendFormat(_T("L$"));
					}

					SetText(sID, sInfo);
				} break;

				default:
					SetText(sID, _T("unknown : ID(%d) op(0x%X) data(0x%llX)"), pCmd->id, pCmd->op,
							*(uint64_t *)pCmd->data);
					break;
				}
			} else {
				SetText(sID, _T("-"));
			}

			// elapsed time
			sID.Format(_T("CMDQ_T_%d"), i);

			if (dwIndex < m_pCmd->dwCurrent) {
				CString sTime;
				RetrieveSimulationTime(sTime, pCmd->elapsed_time);
				SetText(sID, sTime);
			} else if (dwIndex == m_pCmd->dwCurrent && dwIndex < m_pCmd->dwHead) {
				SetText(sID, _T("on progress..."));
			} else {
				SetText(sID, _T("-"));
			}

			dwIndex++;
		}

		UpdateSlider();
	}
}

void RegmapCommand::UpdateSlider(void)
{
	CString sSlide;
	int		iMax = 0;

	if (m_pCmd->dwHead > COMMAND_TABLE_RANGE) {
		iMax = m_pCmd->dwHead - COMMAND_TABLE_LINES;

		if (iMax > COMMAND_TABLE_RANGE)
			iMax = COMMAND_TABLE_RANGE;
	}

	sSlide.Format(_T("<div><input type='range' class='form-range' min='0' max='%d' value='%d' id='cmd_slider' ")
				  _T("oninput='post_message(\\\"Command/offset/\\\" + this.value);'></div>"),
				  iMax, 0);
	SetText(_T("CMDQ_Slide"), (LPCTSTR)sSlide);
}

/*
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
*/
