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
#include "RegmapSystem.h"

RegmapSystem::RegmapSystem(void) : Regmap(_T("System"))
{
	m_dwCoreID		= 0;
	m_dwThreadID	= 0;
	m_pCore			= &m_pReg->core[0];
}

RegmapSystem::~RegmapSystem(void)
{
}

BOOL RegmapSystem::OnUpdate(void)
{
	if(m_pReg->system.bUpdate) {
		m_pReg->system.bUpdate	= FALSE;
		UpdateSystem();
		Broadcast((PVOID)REGMAP_BROADCAST_SYSTEM_REBOOT);
	}

	if(m_pReg->core[m_dwCoreID].bUpdate) {
		m_pReg->core[m_dwCoreID].bUpdate	= FALSE;
		UpdateCore();
	}

	return FALSE;
}

void RegmapSystem::OnBroadcast(LPVOID pData)
{
	if(!pData) {	// on initialization
		InitTable();

		if(m_pReg->system.iCoreSize > MAX_CORE_SIZE) {
			m_pReg->system.iCoreSize	= MAX_CORE_SIZE;
			m_pReg->system.iCoreID		= 0;
			m_pReg->system.iThreadID	= 0;
		}

		m_dwCoreID					= m_pReg->system.iCoreID;
		m_dwThreadID				= m_pReg->system.iThreadID;
		m_pReg->system.bUpdate		= TRUE;
	}
}

BOOL RegmapSystem::OnCommand(LPCTSTR lpszURL)
{
	if(_tcsstr(lpszURL, _T("SelCore_")) == lpszURL) {
		m_pReg->system.iCoreID		= _ttoi(lpszURL + 8);
	} else if(_tcsstr(lpszURL, _T("SelThread_")) == lpszURL) {
		m_pReg->system.iThreadID	= _ttoi(lpszURL + 10);
	} else if(_tcsstr(lpszURL, _T("ToggleCore")) == lpszURL) {
		m_pReg->system.iCoreID++;

		if(m_pReg->system.iCoreID >= m_pReg->system.iCoreSize)
			m_pReg->system.iCoreID = 0;
	} else if(_tcsstr(lpszURL, _T("ToggleThread")) == lpszURL) {
		m_pReg->system.iThreadID++;

		if(m_pReg->system.iThreadID >= THREAD_SIZE) m_pReg->system.iThreadID = 0;
	} else if(_tcsstr(lpszURL, _T("slide"))) {
		return FALSE;
	}

	m_pReg->system.bUpdate		= TRUE;
	m_pReg->core[m_pReg->system.iCoreID].thread[m_pReg->system.iThreadID].GPRs.bUpdate	= TRUE;
	UpdateSystem();
	PostUpdate();
	return FALSE;
}

void RegmapSystem::InitTable(void)
{
	ClearTable();
	// header line
	NewRow();
	NewTH();
	SetTStyle(_T("font-weight"), _T("bold"));
	SetTBody(_T("Cores"));
	SetColSpan(3);
	NewTH();
	SetTStyle(_T("font-weight"), _T("bold"));
	SetTBody(_T("Commands"));
	SetColSpan(3);
	// data line
	NewRow();
	NewTD();
	SetTID(_T("CORES"));
	SetTextAlignment(TEXT_ALIGN_RIGHT);
	SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	NewTD();
	SetTID(_T("THREADS"));
	SetTStyle(_T("font-family"), _T("Courier New"));
	SetTextAlignment(TEXT_ALIGN_LEFT);
	NewTH();
	SetTStyle(_T("width"), _T("20px"));
	SetTBody(_T("<input type='range' id='core_slider' value='0' min='0' max='8' onchange='PostSlideValue(\\\"System\\\", \\\"Core\\\", this.value)'>"));
	NewTD();
	SetTID(_T("CMD_TIME"));
	SetTextAlignment(TEXT_ALIGN_RIGHT);
	SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	NewTD();
	SetTID(_T("CMD"));
	SetTextAlignment(TEXT_ALIGN_LEFT);
	NewTH();
	SetTStyle(_T("width"), _T("20px"));
	SetTBody(_T("<input type='range' id='command_slider' value='0' min='0' max='8' onchange='PostSlideValue(\\\"Command\\\", \\\"Command\\\", this.value)'>"));
}

void RegmapSystem::UpdateSystem(void)
{
	if(m_pReg->system.iCoreID != m_dwCoreID) {
		m_dwCoreID		= m_pReg->system.iCoreID;
		m_pReg->core[m_dwCoreID].bUpdate	= TRUE;
		goto THREAD_CHANGED;
	}

	if(m_dwThreadID != m_pReg->system.iThreadID) {
	THREAD_CHANGED:
		m_dwThreadID	= m_pReg->system.iThreadID;
		m_pReg->core[m_dwCoreID].thread[m_dwThreadID].bUpdate	= TRUE;
	}

	{
		// current core
		HString sDesc;
		sDesc.Format(_T("Core #%d"), m_dwCoreID);
		sDesc.SetLink(_T("cmd://System/ToggleCore"));
		SetText(_T("CORE"), sDesc);
	}

	{
		CString sContents;

		for(int i = 0; i < m_pReg->system.iCoreSize; i++) {
			HString sDesc;
			sDesc.Format(_T("#%d :"), i);

			if(i == m_dwCoreID) {
				sDesc.Insert(0, _T("*"));
				sDesc.SetColor(0xFF0000);
			} else {
				sDesc.SetLink(_T("cmd://System/SelCore_%d"), i);
			}

			if(i) sContents.Append(_T("<br>"));

			sContents.Append(sDesc);
		}

		SetText(_T("CORES"), sContents);
	}

	{
		CString sContents;

		for(int i = 0; i < m_pReg->system.iCoreSize; i++) {
			CORE_T*	pCore	= &m_pReg->core[i];

			if(i) sContents.Append(_T("<br>"));

			for(int t = THREAD_SIZE - 1; t >= 0; t--) {
				THREAD_T*	pThread	= &pCore->thread[t];
				HString sDesc;

				if(pThread->status.bBusy) {
					sDesc.Append(_T("A"));
				} else {
					sDesc.Append(_T("-"));
					sDesc.SetColor(0x7F7F7F);
				}

				if(t) sDesc.Append(_T(" "));

				sContents.Append(sDesc);
			}
		}

		SetText(_T("THREADS"), sContents);
	}

	{
		// current thread
		HString sDesc;
		sDesc.Format(_T("Thread #%d"), m_pReg->system.iThreadID);
		sDesc.SetLink(_T("cmd://System/ToggleThread"));
		SetText(_T("THREAD"), sDesc);
	}

	{
		THREAD_T*	pThread	= m_pCore->thread;

		for(int i = 0; i < THREAD_SIZE; i++) {
			CString	sTarget;
			HString sDesc;
			sTarget.Format(_T("TRD_%d"), i);
			sDesc.Format(_T("#%d - %s"), i, pThread[i].status.bBusy ? _T("BUSY") : _T("IDLE"));

			if(i == m_pReg->system.iThreadID) {
				sDesc.Insert(0, _T("["));
				sDesc.Append(_T("]"));
				sDesc.SetColor(0xFF0000);
				sDesc.bold();
			} else {
				sDesc.SetLink(_T("cmd://System/SelThread_%d"), i);
			}

			SetText(sTarget, sDesc);
		}
	}
}

void RegmapSystem::UpdateCore(void)
{
	m_pCore					= &m_pReg->core[m_dwCoreID];
	m_pCore->LMB.bUpdate	= TRUE;
	UpdateThread();
}

void RegmapSystem::UpdateThread(void)
{
	if(m_dwThreadID != m_pReg->system.iThreadID) {
		m_dwThreadID	= m_pReg->system.iThreadID;
		m_pCore->thread[m_dwThreadID].bUpdate	= TRUE;
	}

	for(int i = 0; i < THREAD_SIZE; i++) {
		CString sTarget;
		HString	sStatus;
		THREAD_T*	pThread	= &m_pCore->thread[i];
		sTarget.Format(_T("STATUS_%d"), i);

		if(pThread->status.bBusy) {
			sStatus.Append(_T("Running..."));
			sStatus.SetColor(0xff0000);
		} else {
			sStatus.Append(_T("Idle"));
			sStatus.SetColor(0xafafaf);
		}

		SetText(sTarget, sStatus);
	}
}
