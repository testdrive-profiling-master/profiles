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
// Title : System remote
// Rev.  : 12/24/2024 Tue (clonextop@gmail.com)
//================================================================================
// Rev.  : 8/15/2020 Sat (clonextop@gmail.com)
//================================================================================
#include "common.h"
#include "SystemRemote.h"
#include "testdrive_document.inl"
#include "ProfileConfig.inl"

REGISTER_LOCALED_DOCUMENT(SystemRemote)
LPCTSTR		   g_sConfigPath = _T("%PROJECT%Profiles/Config.ini");
static LPCTSTR __sAppName	 = _T("System");

SystemRemote::SystemRemote(ITDDocument *pDoc)
{
	CString sConfigPath = g_pSystem->RetrieveFullPath(g_sConfigPath);
	m_pDoc				= pDoc;
	m_pReport			= pDoc->CreateReport(NULL, 0, 0, 10, 10);
	m_pReport->ShowScrollBar(TRUE);
	{
		ITDPropertyData *pProperty;
		m_bServer = FALSE;
		pProperty =
			pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_SERVER_ENABLE, _L(SERVER), (DWORD_PTR)&m_bServer, _L(DESC_SERVER_ENABLE));
		pProperty->UpdateConfigFile();
		m_sServer.GetBuf(MAX_PATH);
		GetPrivateProfileString(__sAppName, _T("SERVER_IP"), _T(""), (LPTSTR)m_sServer.c_str(), MAX_PATH, sConfigPath);
		pProperty =
			pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SERVER_IP, _L(SERVER_IP), (DWORD_PTR)(LPCTSTR)m_sServer, _L(DESC_SERVER_IP));
		m_pPropertyServerIP = pProperty;
		pProperty->UpdateConfigFile();
		m_sPort.GetBuf(MAX_PATH);
		GetPrivateProfileString(__sAppName, _T("PORT"), _T("3232"), (LPTSTR)m_sPort.c_str(), MAX_PATH, sConfigPath);
		pProperty =
			pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_PORT, _L(SERVER_PORT), (DWORD_PTR)(LPCTSTR)m_sPort, _L(DESC_SERVER_PORT));
		pProperty->UpdateConfigFile();
	}
	m_pReport->SetText(_T("System Remote. (under construction...)\n"));
	m_pDoc->Show(FALSE);
	/*{
		// connect
		BOOL	bSuccess	= FALSE;

		if(m_bServer) {
			m_Sock.SetSmartAddressing();
			bSuccess	= m_Sock.CreateSocket(m_sPort, SOCK_TYPE_TCP);
		} else if(!m_sServer.IsEmpty()) {
			bSuccess	= m_Sock.ConnectTo(m_sServer, m_sPort, SOCK_TYPE_TCP);

			if(bSuccess) {
				g_pSystem->LogInfo(_L(CONNECTED_TO_SERVER_IS_SUCCEEDED), (LPCTSTR)m_sServer, (LPCTSTR)m_sPort);
			} else {
				g_pSystem->LogError(_L(CONNECTED_TO_SERVER_IS_FAILED), (LPCTSTR)m_sServer, (LPCTSTR)m_sPort);
			}
		} else {
			g_pSystem->LogInfo(_L(USAGE));
		}

		if(bSuccess) m_Sock.WatchComm();
	}*/
	Reset();
}

SystemRemote::~SystemRemote(void)
{
	// m_Sock.StopComm();
}

BOOL SystemRemote::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch (command) {
	case 0:
		break;

	default:
		break;
	}

	return TRUE;
}

BOOL SystemRemote::OnPropertyUpdate(ITDPropertyData *pProperty)
{
	pProperty->UpdateData();
	pProperty->UpdateConfigFile(FALSE);
	Reset();
	return TRUE;
}

void SystemRemote::OnSize(int width, int height)
{
	if (m_pReport) {
		ITDLayout *pLayout = m_pReport->GetObject()->GetLayout();
		pLayout->SetSize(width, height);
	}
}

void SystemRemote::DoStaticSystemRemote(void)
{
	BOOL bRet = TRUE;
	m_pReport->Clear();
	m_pReport->SetFont(_T("Arial"));
	m_pReport->SetColor(RGB(0, 0, 0));
}

void SystemRemote::Reset(void)
{
	m_pPropertyServerIP->Show(!m_bServer);
	// g_pSystem->LogInfo(_T("%s (%s)"), _L(INITIALIZE_SYSTEM_REMOTE), m_bServer ? _L(SERVER) : _L(CLIENT));
}
