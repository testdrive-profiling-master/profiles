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
#include "RegmapSystem.h"

RegmapSystem::RegmapSystem(void) : Regmap(_T("System")) {}

RegmapSystem::~RegmapSystem(void) {}

BOOL RegmapSystem::OnUpdate(void)
{
	BOOL bRet = FALSE;

	if (m_pReg->system.bUpdate) {
		m_pReg->system.bUpdate = FALSE;
		UpdateSystem();
		Broadcast((PVOID)REGMAP_BROADCAST_SYSTEM_REBOOT);
		bRet = TRUE;
	}

	if (m_pCore->bUpdate) {
		m_pCore->bUpdate = FALSE;
		UpdateCore();
		bRet = TRUE;
	}

	return bRet;
}

void RegmapSystem::OnBroadcast(LPVOID pData)
{
	switch (*(REGMAP_BROADCAST *)(&pData)) {
	case REGMAP_BROADCAST_INITIALIZATION:
		break;

	case REGMAP_BROADCAST_SYSTEM_REBOOT:
		InitTable();
		break;
	}
}

BOOL RegmapSystem::OnCommand(LPCTSTR lpszURL)
{
	if (_tcsstr(lpszURL, _T("core/")) == lpszURL) {
		m_pReg->system.iCoreCurrent = _ttoi(lpszURL + 5);
		UpdateSystem();
	} else if (_tcsstr(lpszURL, _T("thread/")) == lpszURL) {
		m_pCore->iThreadCurrent = _ttoi(lpszURL + 7);
		UpdateSystem();
	}

	return FALSE;
}

void RegmapSystem::InitTable(void)
{
	ClearTable(_T("THREAD"));

	for (int i = 0; i < m_pReg->system.iCoreCount; i++) {
		NewRow(_T("THREAD"));
		NewTH();
		SetTBody(_T("Core #%d"), i);

		for (int t = 0; t < MTSP_THREAD_SIZE; t++) {
			NewTD();
			SetTID(_T("TRD%d_%d"), i, t);
		}
	}
}

void RegmapSystem::UpdateSystem(void)
{
	// update current core & thread
	{
		if (m_pReg->system.iCoreCount >= MTSP_MAX_CORE_SIZE) {
			m_pReg->system.iCoreCount = MTSP_MAX_CORE_SIZE;
		}

		if (m_pReg->system.iCoreCurrent >= m_pReg->system.iCoreCount) {
			m_pReg->system.iCoreCurrent = 0;
		}

		m_pCore = &m_pReg->core[m_pReg->system.iCoreCurrent];

		if (m_pCore->iThreadCurrent >= MTSP_THREAD_SIZE) {
			m_pCore->iThreadCurrent = 0;
		}

		m_pThread				= &m_pCore->thread[m_pCore->iThreadCurrent];
		m_pThread->GPRs.bUpdate = true;
		m_pThread->bUpdate		= true;
		m_pCore->bUpdate		= true;
		m_pReg->gmb.bUpdate		= true;
	}
	// setup core selection menu
	{
		g_pHtml->CallJScript(_T("Add_CoreMenu(-1);")); // clear menu

		if (m_pReg->system.iCoreCount) {
			for (int i = 0; i < m_pReg->system.iCoreCount; i++) {
				g_pHtml->CallJScript(_T("Add_CoreMenu(%d);"), i);
			}

			g_pHtml->CallJScript(_T("$('#core_name').val('Core #%d');"), m_pReg->system.iCoreCurrent);
		}

		// setup thread selection menu
		g_pHtml->CallJScript(_T("Add_ThreadMenu(-1);")); // clear menu

		for (int i = 0; i < MTSP_THREAD_SIZE; i++) {
			g_pHtml->CallJScript(_T("Add_ThreadMenu(%d);"), i);
		}

		g_pHtml->CallJScript(_T("$('#thread_name').val('Thread #%d');"), m_pCore->iThreadCurrent);
	}
	// setup tags
	SetText(_T("GPRs_tag"), _T("<b>GPRs</b> (Core #%d - Thread #%d) - %d entries."), m_pReg->system.iCoreCurrent,
			m_pCore->iThreadCurrent, MTSP_GPR_SIZE);
	SetText(_T("LMB_tag"), _T("<b>LMB</b> (Core #%d) - %d entries."), m_pReg->system.iCoreCurrent, MTSP_LMB_SIZE);
	SetText(_T("GMB_tag"), _T("<b>GMB</b> - %d entries."), m_pReg->system.iGMB_Count);
}

void RegmapSystem::UpdateCore(void)
{
	UpdateThread();
}

void RegmapSystem::UpdateThread(void)
{
	MTSP_REGMAP_CORE *pCore = m_pReg->core;

	for (int iCoreID = 0; iCoreID < m_pReg->system.iCoreCount; iCoreID++, pCore++) {
		MTSP_REGMAP_THREAD *pThread = pCore->thread;

		for (int iThreadID = 0; iThreadID < MTSP_THREAD_SIZE; iThreadID++, pThread++) {
			if (pThread->bUpdate) {
				CString sID;
				HString sData;
				sID.Format(_T("TRD%d_%d"), iCoreID, iThreadID);

				if (pThread->status.bAlive) {
					if (pThread->status.bBusy) {
						sData = _T("Busy");
						sData.SetColor(0xFF0000);
						sData.bold();
					} else {
						sData = _T("Alive");
						sData.SetColor(0x0000FF);
						sData.bold();
					}
				} else {
					sData = _T("-");
					sData.SetColor(0xAFAFAF);
				}

				SetText(sID, sData);
				pThread->bUpdate = false;
			}
		}
	}
}
