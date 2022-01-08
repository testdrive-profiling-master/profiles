//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Xilinx synthesis
// Rev.  : 1/8/2022 Sat (clonextop@gmail.com)
//================================================================================
#include "testdrive_document.inl"
#include "XilinxSynthesis.h"

REGISTER_LOCALED_DOCUMENT(XilinxSynthesis)

XilinxSynthesis::XilinxSynthesis(ITDDocument* pDoc)
{
	m_bInitialized		= FALSE;
	m_pSourceList		= NULL;
	m_pDoc				= pDoc;
	m_Config.sDocPath	= g_pSystem->RetrieveFullPath(_T("%CURRENT%"));
	m_pTable			= &m_Table;
	{
		ITDPropertyData*	pProperty;
		m_sNameFilter.GetBuffer(MAX_PATH * 2);
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_PATH_FILTER, _L(PATH_FILTER), (DWORD_PTR)((LPCTSTR)m_sNameFilter), _L(DESC_PATH_FILTER));
		pProperty->UpdateConfigFile();
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_DIRECTORY, PROPERTY_ID_INSTALL_PATH, _L(XILINX_INSTALL_PATH), (DWORD_PTR)(LPCTSTR)(m_Config.sXilinxPath), _L(DESC_XILINX_INSTALL_PATH));
		pProperty->UpdateConfigFile();
		m_pProperty[PROPERTY_ID_INSTALL_PATH]	= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_DIRECTORY, PROPERTY_ID_PROJECT_PATH, _L(PROJECT_PATH), (DWORD_PTR)(m_Config.sProjectPath), _L(DESC_PROJECT_PATH));
		pProperty->UpdateConfigFile();
		m_pProperty[PROPERTY_ID_PROJECT_PATH]	= pProperty;
		CheckProjectEmpty();
		// project configurations
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_FAMILY, _L(XILINX_FAMILY), (DWORD_PTR)(m_Config.spec.sFamily), _L(DESC_XILINX_FAMILY));
		m_pProperty[PROPERTY_ID_FAMILY]			= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_DEVICE, _L(XILINX_DEVICE), (DWORD_PTR)(m_Config.spec.sDevice), _L(DESC_XILINX_DEVICE));
		m_pProperty[PROPERTY_ID_DEVICE]			= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_PACKAGE, _L(XILINX_PACKAGE), (DWORD_PTR)(m_Config.spec.sPackage), _L(DESC_XILINX_PACKAGE));
		m_pProperty[PROPERTY_ID_PACKAGE]		= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SPEED_GRADE, _L(XILINX_SPEED_GRADE), (DWORD_PTR)(m_Config.spec.sSpeedGrade), _L(DESC_XILINX_SPEED_GRADE));
		m_pProperty[PROPERTY_ID_SPEED_GRADE]	= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_TARGET_SPEED, _L(TARGET_SPEED), (DWORD_PTR)(m_Config.spec.sTargetSpeed), _L(DESC_TARGET_SPEED));
		m_pProperty[PROPERTY_ID_TARGET_SPEED]	= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_CLOCKS, _L(LIST_CLOCKS), (DWORD_PTR)(m_Config.spec.sClocks), _L(DESC_LIST_CLOCKS));
		m_pProperty[PROPERTY_ID_CLOCKS]			= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_RESETS, _L(LIST_RESETS), (DWORD_PTR)(m_Config.spec.sResets), _L(DESC_LIST_RESETS));
		m_pProperty[PROPERTY_ID_RESETS]			= pProperty;
		pProperty	= pDoc->AddPropertyData(PROPERTY_TYPE_INT, PROPERTY_ID_MAX_PATHS, _L(MAX_PATHS), (DWORD_PTR)(&m_Config.iMaxPaths), _L(DESC_MAX_PATHS));
		m_pProperty[PROPERTY_ID_MAX_PATHS]		= pProperty;
	}
	m_Table.Create(pDoc, _T("../media/tables.html"), 0, 0, 10, 10, this);
	XilinxToolCheck();
	ResetProject();
}

XilinxSynthesis::~XilinxSynthesis(void)
{
	SAFE_DELETE(m_pSourceList);
}

void XilinxSynthesis::CheckProjectEmpty(void)
{
	if(!m_Config.sProjectPath[0]) {
		if(GetDefaultProjectPath()) {
			m_pProperty[PROPERTY_ID_PROJECT_PATH]->UpdateData(FALSE);
			g_pSystem->LogInfo(_L(SET_DEFAULT_PROJECT_PATH), (LPCTSTR)m_Config.sProjectPath);
		}
	}
}

BOOL XilinxSynthesis::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch(command) {
	case COMMAND_ID_SYNTHESIS:
		m_pDoc->KillTimer(command);

		if(OnSynthesis())
			m_pDoc->SetTimer(command, 0);

		break;

	default:
		break;
	}

	return TRUE;
}

void XilinxSynthesis::ResetProject(void)
{
	CheckProjectEmpty();
	UpdateConfigAll();
	UpdateProperty(PROPERTY_ID_FAMILY);
	UpdateProperty(PROPERTY_ID_DEVICE);
	UpdateProperty(PROPERTY_ID_PACKAGE);
	UpdateProperty(PROPERTY_ID_SPEED_GRADE);
	UpdateProperty(PROPERTY_ID_TARGET_SPEED);
	UpdateProperty(PROPERTY_ID_CLOCKS);
	UpdateProperty(PROPERTY_ID_RESETS);
	UpdateProperty(PROPERTY_ID_MAX_PATHS);
}

void XilinxSynthesis::UpdateProperty(PROPERTY_ID id, BOOL bClear)
{
	ITDPropertyData*	pProperty	= m_pProperty[id];

	switch(id) {
	case PROPERTY_ID_FAMILY:
		UpdateConfig(CONFIG_ID_FAMILY, FALSE);
		break;

	case PROPERTY_ID_DEVICE:
		if(bClear) m_Config.spec.sDevice[0]	= _T('\0');

		m_XilinxDevices.RefreshDeviceProperty(pProperty, m_Config.spec.sFamily);
		UpdateConfig(CONFIG_ID_DEVICE, FALSE);
		break;

	case PROPERTY_ID_PACKAGE:
		if(bClear) m_Config.spec.sPackage[0]	= _T('\0');

		m_XilinxDevices.RefreshPackageProperty(pProperty, m_Config.spec.sFamily, m_Config.spec.sDevice);
		UpdateConfig(CONFIG_ID_PACKAGE, FALSE);
		break;

	case PROPERTY_ID_SPEED_GRADE:
		if(bClear) m_Config.spec.sSpeedGrade[0]	= _T('\0');

		m_XilinxDevices.RefreshSpeedGradeProperty(pProperty, m_Config.spec.sFamily, m_Config.spec.sDevice, m_Config.spec.sPackage);
		UpdateConfig(CONFIG_ID_SPEED_GRADE, FALSE);
		break;

	case PROPERTY_ID_TARGET_SPEED: {
		int iOldSpeed, iNewSpeed;
		BOOL bSet;
		iOldSpeed	= _ttoi(m_Config.spec.sTargetSpeed);
		pProperty->UpdateData();
		iNewSpeed	= _ttoi(m_Config.spec.sTargetSpeed);
		bSet		= m_Config.spec.sTargetSpeed[0] != _T('\0');

		if(iNewSpeed <= 0) {
			m_Config.spec.sTargetSpeed[0]	= _T('\0');
			_stprintf(m_Config.spec.sTargetSpeed, _T("%d"), iOldSpeed);
			pProperty->UpdateData(FALSE);

			if(bSet) g_pSystem->LogError(_L(TARGET_SPEED_MUST_GREATER_THAN_ZERO));
		} else {
			UpdateConfig(CONFIG_ID_TARGET_SPEED, FALSE);
		}
	}
	break;

	case PROPERTY_ID_CLOCKS:
		if(bClear) _tcscpy(m_Config.spec.sClocks, _T("*CLK"));

		UpdateConfig(CONFIG_ID_CLOCKS, FALSE);
		break;

	case PROPERTY_ID_RESETS:
		if(bClear) _tcscpy(m_Config.spec.sResets, _T("*RST"));

		UpdateConfig(CONFIG_ID_RESETS, FALSE);
		break;

	case PROPERTY_ID_MAX_PATHS:
		if(bClear) m_Config.iMaxPaths		= 1;

		if(m_Config.iMaxPaths < 1)
			m_Config.iMaxPaths		= 1;
		else if(m_Config.iMaxPaths > 100)
			m_Config.iMaxPaths		= 100;

		UpdateConfig(CONFIG_ID_MAX_PATHS, FALSE);
		break;

	default:
		break;
	}

	pProperty->UpdateData(FALSE);
}

BOOL XilinxSynthesis::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	if(pProperty->GetID() != PROPERTY_ID_TARGET_SPEED)
		pProperty->UpdateData();

	switch(pProperty->GetID()) {
	case PROPERTY_ID_PATH_FILTER:
		pProperty->UpdateConfigFile(FALSE);
		RefreshButtonClick();
		break;

	case PROPERTY_ID_INSTALL_PATH:
		pProperty->UpdateConfigFile(FALSE);
		break;

	case PROPERTY_ID_PROJECT_PATH:
		pProperty->UpdateConfigFile(FALSE);
		ResetProject();
		RefreshTable();
		break;

	case PROPERTY_ID_FAMILY:
		UpdateProperty(PROPERTY_ID_FAMILY);
		UpdateProperty(PROPERTY_ID_DEVICE, TRUE);
		UpdateProperty(PROPERTY_ID_PACKAGE, TRUE);
		UpdateProperty(PROPERTY_ID_SPEED_GRADE, TRUE);
		break;

	case PROPERTY_ID_DEVICE:
		UpdateProperty(PROPERTY_ID_DEVICE);
		UpdateProperty(PROPERTY_ID_PACKAGE, TRUE);
		UpdateProperty(PROPERTY_ID_SPEED_GRADE, TRUE);
		break;

	case PROPERTY_ID_PACKAGE:
		UpdateProperty(PROPERTY_ID_PACKAGE);
		UpdateProperty(PROPERTY_ID_SPEED_GRADE, TRUE);
		break;

	case PROPERTY_ID_SPEED_GRADE:
		UpdateProperty(PROPERTY_ID_SPEED_GRADE);
		break;

	case PROPERTY_ID_TARGET_SPEED:
		UpdateProperty(PROPERTY_ID_TARGET_SPEED);
		break;

	case PROPERTY_ID_CLOCKS:
		UpdateProperty(PROPERTY_ID_CLOCKS);
		break;

	case PROPERTY_ID_RESETS:
		UpdateProperty(PROPERTY_ID_RESETS);
		break;

	case PROPERTY_ID_MAX_PATHS:
		UpdateProperty(PROPERTY_ID_MAX_PATHS);
		break;

	default:
		break;
	}

	return TRUE;
}

void XilinxSynthesis::OnSize(int width, int height)
{
	if((ITDHtml*)m_Table) {
		ITDLayout* pLayout;
		pLayout = ((ITDHtml*)m_Table)->GetObject()->GetLayout();
		pLayout->SetSize(width, height - 4);
	}
}

LPCTSTR XilinxSynthesis::OnHtmlBeforeNavigate(DWORD dwID, LPCTSTR lpszURL)
{
	if(m_bInitialized) {
		if(_tcsstr(lpszURL, _T("folder:")) == lpszURL) {
			m_pSourceList->OpenFolder(&lpszURL[7]);
		} else if(_tcsstr(lpszURL, _T("open:")) == lpszURL) {
			m_pSourceList->OpenSource(&lpszURL[5]);
		} else if(_tcsstr(lpszURL, _T("group:")) == lpszURL) {
			StartSynthesis(&lpszURL[6], TRUE);
		} else if(_tcsstr(lpszURL, _T("source:")) == lpszURL) {
			StartSynthesis(&lpszURL[7], FALSE);
		} else if(_tcsstr(lpszURL, _T("report:")) == lpszURL) {
			XilinxReport(&lpszURL[7]);
		} else if(_tcsstr(lpszURL, _T("mark:")) == lpszURL) {
			SourceGroup*	pGroup	= m_pSourceList->FindGroup(&lpszURL[5]);

			if(pGroup) pGroup->SetMark();
		} else if(_tcsstr(lpszURL, _T("updatetable:")) == lpszURL) {
			RefreshButtonClick();
		}

		return NULL;
	}

	return lpszURL;
}

void XilinxSynthesis::RefreshButtonClick(void)
{
	if(m_pDoc->IsLocked()) {
		g_pSystem->LogInfo(_L(SYNTHESIS_IS_BUSY));
	} else {
		m_pSourceList->RemoveScoreFile();
		m_pSourceList->FlushScoreFile();
		RefreshTable();
	}
}

void XilinxSynthesis::OnHtmlDocumentComplete(DWORD dwID, LPCTSTR lpszURL)
{
	if(!m_bInitialized) {
		m_Table.JScript(_T("SetTitle(\"<table  class='null0'><tbody><td><a href='updatetable:'><button class='ShadowButton'>%s</button></a></td><td style='text-align:right'></td></tbody></table>\");"), _L(REFRESH_TABLE));
		RefreshTable();
		m_bInitialized	= TRUE;
	}
}

void XilinxSynthesis::StartSynthesis(LPCTSTR sPath, BOOL bGroup)
{
	if(!*m_Config.sXilinxPath) {	// xilinx path is not existed.
		g_pSystem->LogError(_L(CAN_NOT_FIND_XILINX_PATH));
		return;
	}

	if(bGroup) {
		SourceGroup* pGroup	= m_pSourceList->FindGroup(sPath);

		if(pGroup) {
			SourceVector* pSource	= pGroup->GetNextSource(NULL);

			while(pSource) {
				m_pSynthesisList.push_back(pSource->FullName());
				pSource	= pGroup->GetNextSource(pSource);
			}
		}
	} else {
		m_pSynthesisList.push_back(sPath);
	}

	if(m_pDoc->IsLocked()) {
		g_pSystem->LogInfo(_T("%s : '%s'"), _L(ADD_SYNTHESIS_LIST), sPath);
	} else {
		m_pDoc->Lock();
		m_pDoc->SetTimer(COMMAND_ID_SYNTHESIS, 0);
	}
}

BOOL XilinxSynthesis::OnSynthesis(void)
{
	if(m_pSynthesisList.size()) {
		CString sPath	= m_pSynthesisList.front();
		m_pSynthesisList.pop_front();
		SourceVector*	pSource	= m_pSourceList->FindVector(sPath);
		pSource->Synthesis();
	} else {
		m_pDoc->UnLock();
		return FALSE;
	}

	return TRUE;
}

void XilinxSynthesis::XilinxReport(LPCTSTR sPath)
{
	SourceVector* pSource	= m_pSourceList->FindVector(sPath);

	if(pSource) pSource->Report();
}

void FilePrint(FILE* fp, LPCTSTR sFormat, ...)
{
	CString	sLine;
	{
		CString sFieldFormat;
		va_list args;
		va_start(args, sFormat);
		sLine.FormatV(sFormat, args);
		va_end(args);
	}
	{
		// store to utf-8 format string
		int iLen	= WideCharToMultiByte(CP_UTF8, 0, sLine.Buffer(), lstrlenW(sLine.Buffer()), NULL, 0, NULL, NULL);

		if(iLen) {
			char* sMsg	= new char[iLen];
			WideCharToMultiByte(CP_UTF8, 0, sLine.Buffer(), lstrlenW(sLine.Buffer()), sMsg, iLen, NULL, NULL);
			fwrite(sMsg, iLen, 1, fp);
			delete [] sMsg;
		}
	}
}
