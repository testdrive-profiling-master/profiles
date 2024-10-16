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
// Title : Serial console
// Rev.  : 10/16/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "SerialConsole.h"
#include "testdrive_document.inl"
#include "ProfileConfig.inl"

#define MAX_PORT_SEARCHING 32
static DWORD   __dwBaudRate[] = {1200, 2400, 4800, 9600, 1440, 19200, 38400, 57600, 115200, 0};
static DWORD   __dwDataBits[] = {5, 6, 7, 8, 0};

static LPCTSTR __sParity[] = {
	_T("None"), _T("Odd"), _T("Even"), _T("Mark"), _T("Space"), NULL,
};

static LPCTSTR __sStopBits[] = {
	_T("1"),
	_T("1.5"),
	_T("2"),
	NULL,
};

static LPCTSTR __sFlowControl[] = {
	_T("None"), _T("XON/XOFF"), _T("RTS/CTS"), _T("DSR/DTR"), NULL,
};

REGISTER_LOCALED_DOCUMENT(SerialConsole)

SerialConsole::SerialConsole(ITDDocument *pDoc)
{
	m_pDoc = pDoc;
	{
		ITDPropertyData *pProperty;
		m_sComPort.GetBuf(MAX_PATH);
		m_sComPort = _L(AUTODETECT);
		pProperty =
			pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_COM_PORT, _L(COM_PORT), (DWORD_PTR)(LPCTSTR)m_sComPort, _L(DESC_COM_PORT));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);
		UpdateComportList(pProperty);

		if (m_sComPort.IsEmpty()) {
			m_sComPort.Format(_T("%s"), _L(AUTODETECT));
			pProperty->UpdateData(FALSE);
		}

		m_dwBaudRate = 115200;
		pProperty	 = pDoc->AddPropertyData(PROPERTY_TYPE_INT, PROPERTY_ID_BAUDRATE, _L(BAUDRATE), (DWORD_PTR)&m_dwBaudRate, _L(DESC_BAUDRATE));
		pProperty->UpdateConfigFile();

		for (DWORD *pBaud = __dwBaudRate; *pBaud; pBaud++) {
			CString sCom;
			sCom.Format(_T("%d"), *pBaud);
			pProperty->AddOption(sCom);
		}

		m_dwDataBits = 8;
		pProperty = pDoc->AddPropertyData(PROPERTY_TYPE_INT, PROPERTY_ID_DATA_BITS, _L(DATA_BITS), (DWORD_PTR)&m_dwDataBits, _L(DESC_DATA_BITS));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for (DWORD *pDataBits = __dwDataBits; *pDataBits; pDataBits++) {
			CString sBits;
			sBits.Format(_T("%d"), *pDataBits);
			pProperty->AddOption(sBits);
		}

		m_sParity.GetBuf(MAX_PATH);
		m_sParity.Format(_T("%s"), __sParity[0]);
		pProperty = pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_PARITY, _L(PARITY), (DWORD_PTR)(LPCTSTR)m_sParity, _L(DESC_PARITY));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for (LPCTSTR *pParity = __sParity; *pParity; pParity++) {
			pProperty->AddOption(*pParity);
		}

		m_sStopBits.GetBuf(MAX_PATH);
		m_sStopBits.Format(_T("%s"), __sStopBits[0]);
		pProperty = pDoc->AddPropertyData(
			PROPERTY_TYPE_STRING, PROPERTY_ID_STOP_BITS, _L(STOP_BITS), (DWORD_PTR)(LPCTSTR)m_sStopBits, _L(DESC_STOP_BITS));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for (LPCTSTR *pStop = __sStopBits; *pStop; pStop++) {
			pProperty->AddOption(*pStop);
		}

		m_sFlowControl.GetBuf(MAX_PATH);
		m_sFlowControl.Format(_T("%s"), __sFlowControl[0]);
		pProperty = pDoc->AddPropertyData(
			PROPERTY_TYPE_STRING, PROPERTY_ID_FLOW_CONTROL, _L(FLOW_CONTROL), (DWORD_PTR)(LPCTSTR)m_sFlowControl, _L(DESC_FLOW_CONTROL));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for (LPCTSTR *pFlow = __sFlowControl; *pFlow; pFlow++) {
			pProperty->AddOption(*pFlow);
		}
	}
	m_pDoc->Show(FALSE);
}

SerialConsole::~SerialConsole(void) {}

BOOL SerialConsole::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch (command) {
	case 0:
		break;
	case 1:
		InvokeSerialConsole();
		break;
	default:
		break;
	}

	return TRUE;
}

BOOL SerialConsole::OnPropertyUpdate(ITDPropertyData *pProperty)
{
	pProperty->UpdateData();

	switch (pProperty->GetID()) {
	case PROPERTY_ID_COM_PORT:
		m_sComPort.Replace(_T(' '), _T('\0'));
		m_sComPort.Replace(_T('('), _T('\0'));

		if (!m_sComPort.IsEmpty()) {
			pProperty->UpdateData(FALSE);
		}
		break;

	default:
		break;
	}

	UpdateComportList();
	pProperty->UpdateConfigFile(FALSE);
	return TRUE;
}

CONNECTABLE SerialConsole::CheckPortExist(LPCTSTR lpszDevice)
{
	CONNECTABLE bConnectable = CONNECTABLE_FALSE;
	// Try to open the device
	CString sComport;
	sComport.Format(_T("\\\\.\\%s"), lpszDevice);
	HANDLE hFile = CreateFile(sComport, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);

	// Check if we could open the device
	if (hFile != INVALID_HANDLE_VALUE) {
		bConnectable = CONNECTABLE_TRUE;
		// Close handle
		CloseHandle(hFile);
	} else if (GetLastError() == ERROR_ACCESS_DENIED) {
		bConnectable = CONNECTABLE_OCCUPIED;
	}

	return bConnectable;
}

void SerialConsole::UpdateComportList(ITDPropertyData *pProperty)
{
	static ITDPropertyData *pComportProperty = NULL;

	if (pProperty)
		pComportProperty = pProperty;

	if (pComportProperty) {
		pComportProperty->AddOption(NULL);
		pComportProperty->AddOption(_L(AUTODETECT));

		LPCTSTR connect_status[] = {_L(UNCONNECTABLE), _L(CONNECTABLE), _L(CONNECTION_OCCUPIED)};

		for (int i = 1; i <= MAX_PORT_SEARCHING; i++) {
			CString sCom;

			sCom.Format(_T("COM%d"), i);
			CONNECTABLE status = CheckPortExist(sCom);
			if (status != CONNECTABLE_FALSE)
				sCom.AppendFormat(_T(" %s"), connect_status[(int)status]);
			pComportProperty->AddOption(sCom);
		}
	}
}

BOOL SerialConsole::AutoDetectPort(BOOL bWarning)
{
	for (int i = 1; i <= MAX_PORT_SEARCHING; i++) {
		CString sCom;
		sCom.Format(_T("COM%d"), i);

		if (CheckPortExist(sCom)) {
			m_sComPort.Format(sCom);
			return TRUE;
		}
	}

	if (bWarning)
		g_pSystem->LogWarning(_L(NO_AVAILABLE_PORT));

	return FALSE;
}

bool SerialConsole::InvokeSerialConsole(void)
{
	// example : putty -serial COM6 -sercfg 115200,8,n,1,N
	CString sArg(_T("start \"\" /I /B putty"));
	{ // set COM port
		CString sCom   = m_sComPort;
		bool	bFound = false;
		sCom.Replace(_T(' '), _T('\0'));
		sCom.Replace(_T('('), _T('\0'));

		if (sCom.IsEmpty()) {
			for (int i = 1; i <= MAX_PORT_SEARCHING; i++) {
				sCom.Format(_T("COM%d"), i);
				if (CheckPortExist(sCom) == CONNECTABLE_TRUE) {
					bFound = true;
					break;
				}
			}
		} else {
			switch (CheckPortExist(sCom)) {
			case CONNECTABLE_FALSE:
				g_pSystem->LogError(_L(PORT_IS_NOT_AVAILABLE), (LPCTSTR)sCom);
				return false;
			case CONNECTABLE_OCCUPIED:
				g_pSystem->LogError(_L(PORT_IS_ALREADY_IN_USE), (LPCTSTR)sCom);
				return false;
			default:
				bFound = true;
				break;
			}
		}
		if (!bFound) {
			g_pSystem->LogError(_L(NO_AVAILABLE_PORT));
			return false;
		}
		sArg.AppendFormat(_T(" -serial %s"), (LPCTSTR)sCom);
	}
	{ // set configuration
		CString sParity		 = m_sParity;
		CString sStopBits	 = m_sStopBits;
		CString sFlowControl = m_sFlowControl;
		sParity.Delete(1, -1);
		sStopBits.Delete(1, -1);
		sFlowControl.Delete(1, -1);
		sParity.MakeLower();
		sArg.AppendFormat(
			_T(" -sercfg %d,%d,%s,%s,%s"), m_dwBaudRate, m_dwDataBits, (LPCTSTR)sParity, (LPCTSTR)sStopBits, (LPCTSTR)sFlowControl);
	}

	//  Run putty
	ShellExecute(NULL, _T("open"), _T(".run.bat"), sArg, NULL, SW_SHOWDEFAULT);
	UpdateComportList();
	return true;
}
