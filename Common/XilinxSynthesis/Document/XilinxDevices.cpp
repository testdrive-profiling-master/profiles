//================================================================================
// Copyright (c) 2013 ~ 2020. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Xilinx synthesis
// Rev.  : 12/22/2020 Tue (clonextop@gmail.com)
//================================================================================
#include "XilinxDevices.h"
#include "ProjectConfig.h"
#include <stdio.h>
#include <tchar.h>

//------------------------------------------------------------------------------------
XilinxFileToken::XilinxFileToken(void)
{
	m_fp		= NULL;
	Release();
}

XilinxFileToken::~XilinxFileToken(void)
{
	Release();
}

BOOL XilinxFileToken::Initialize(void)
{
	Release();
	CString sFilePath(ProjectConfig::m_Config.sXilinxPath);
	sFilePath	+= _T("data\\parts\\installed_devices.txt");

	if(!IsFileExist(sFilePath)) {
		// try to make device list
		CString sBatch, sArg;
		g_pSystem->LogInfo(_T("*I: Making Xilinx device list...\n"));
		sBatch.Format(_T("%s\\%s_make_device_list.bat"), ProjectConfig::m_Config.sDocPath, g_sGlobalName);
		sArg.Format(_T("\"%s\\settings64.bat\" %s"),
					(LPCTSTR)ProjectConfig::m_Config.sXilinxPath,
					(LPCTSTR)ProjectConfig::m_Config.sXilinxPath);
		sArg.Replace(_T("\\"), _T("/"));
		int iRet = g_pSystem->ExecuteFile(sBatch, sArg, TRUE, NULL, ProjectConfig::m_Config.sDocPath,
										  _T("failed:"), -1,
										  _T("ERROR:"), -1,
										  NULL);

		if(iRet >= 0) {
			g_pSystem->LogInfo(_T("Done!"));
		}

		// cleanup log files.
		sArg.Format(_T("%s\\vivado.jou"), ProjectConfig::m_Config.sDocPath);
		_tremove(sArg.c_str());
		sArg.Format(_T("%s\\vivado.log"), ProjectConfig::m_Config.sDocPath);
		_tremove(sArg.c_str());
	}

	m_fp = _tfopen(sFilePath, _T("rt"));

	if(!m_fp) {
		g_pSystem->LogWarning(_T("Can't found Xilinx's part list file (%s)."), sFilePath.c_str());
	}

	return m_fp != NULL;
}

void XilinxFileToken::Release(void)
{
	if(m_fp) {
		fclose(m_fp);
		m_fp		= NULL;
	}

	m_dwCurLine		= 0;
	m_iGroupLevel	= 0;
	m_iPos			= -1;
	m_bInlist		= FALSE;
	m_bAssign		= FALSE;
	m_sLine.Empty();
}

BOOL XilinxFileToken::GetLine(CString& sLine)
{
	if(!m_fp) return FALSE;

	TCHAR	sTemp[4096];

	if(!_fgetts(sTemp, 4096, m_fp)) {
		Release();
		return FALSE;
	}

	sLine	= sTemp;
	return TRUE;
}

XF_TOKEN XilinxFileToken::GetToken(CString& sTok)
{
RETRY_GET_LINE:

	if(!m_fp) return XF_TOKEN_EXIT;

	if(m_iPos < 0) {
		if(!GetLine(m_sLine)) {
			Release();
			return XF_TOKEN_EXIT;
		}

		m_dwCurLine++;
		m_iPos	= 0;
	}

	sTok	= m_sLine.Tokenize(_T(" ,\t\r\n"), m_iPos);

	if(m_iPos < 0) goto RETRY_GET_LINE;

	m_bAssign	= FALSE;

	switch(sTok.GetAt(0)) {
	case _T('\''): {
		sTok.TrimLeft(_T('\''));

		if(sTok.Find(_T('\'')) < 0) {
			sTok	+= _T(' ');
			sTok	+= m_sLine.Tokenize(_T("\'"), m_iPos);
		}

		sTok.TrimRight(_T('\''));
		return XF_TOKEN_STRING;
	}

	case _T('='):
		m_bAssign = TRUE;
		return XF_TOKEN_ASSIGN;

	case _T('{'):
		m_iGroupLevel++;
		return XF_TOKEN_GROUP_OPEN;

	case _T('}'):
		m_iGroupLevel--;
		return XF_TOKEN_GROUP_CLOSE;

	case _T('['):
		m_bInlist = TRUE;
		return XF_TOKEN_LIST_OPEN;

	case _T(']'):
		m_bInlist = FALSE;
		return XF_TOKEN_LIST_CLOSE;
	}

	return XF_TOKEN_EXIT;
}

BOOL XilinxFileToken::Mustbe(XF_TOKEN type, CString* sStr)
{
	CString sTok;

	if(GetToken(sTok) != type) {
		Release();
		return FALSE;
	}

	if(sStr)(*sStr) = sTok;

	return TRUE;
}

BOOL XilinxFileToken::GetString(CString& sStr)
{
	return (GetToken(sStr) == XF_TOKEN_STRING);
}

void XilinxFileToken::DownToGroupLevel(int iLevel)
{
	CString sTok;

	while(GroupLevel() > iLevel || m_bAssign || m_bInlist) {
		GetToken(sTok);
	}
}

BOOL XilinxFileToken::GetTopName(CString& sName)
{
	CString sTok;

	while(GroupLevel() >= 0) {
		switch(GetToken(sTok)) {
		case XF_TOKEN_EXIT:
			return FALSE;

		case XF_TOKEN_STRING:
			if(m_iGroupLevel == 1) {
				sName = sTok;
				return TRUE;
			}

			break;

		case XF_TOKEN_ASSIGN:
			GetToken(sTok);
			break;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------------
XilinxPackage::XilinxPackage(LPCTSTR sDesc)
{
	m_sName	= sDesc;
}

XilinxPackage::~XilinxPackage(void)
{
	for(DWORD i = 0; i < Size(); i++)
		delete m_sGradeList[i];

	m_sGradeList.clear();
}

void XilinxPackage::Add(LPCTSTR sGrade)
{
	m_sGradeList.push_back(new CString(sGrade));
}

//------------------------------------------------------------------------------------
XilinxDevice::XilinxDevice(LPCTSTR sDesc)
{
	m_sName	= sDesc;
}

XilinxDevice::~XilinxDevice(void)
{
	for(DWORD i = 0; i < Size(); i++)
		delete m_PackageList[i];

	m_PackageList.clear();
}

XilinxPackage* XilinxDevice::Add(LPCTSTR sPackage)
{
	XilinxPackage* pPackage	= new XilinxPackage(sPackage);
	m_PackageList.push_back(pPackage);
	return pPackage;
}

XilinxPackage* XilinxDevice::FindPackage(LPCTSTR sPackage)
{
	XilinxPackage*	pPackage;

	for(DWORD i = 0; i < Size(); i++) {
		pPackage	= Package(i);

		if(!pPackage->Name().Compare(sPackage)) return pPackage;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
XilinxFamily::XilinxFamily(LPCTSTR sName)
{
	m_sName	= sName;
}

XilinxFamily::~XilinxFamily(void)
{
	for(DWORD i = 0; i < Size(); i++)
		delete m_DeviceList[i];

	m_DeviceList.clear();
}

XilinxDevice* XilinxFamily::Add(LPCTSTR sDesc)
{
	XilinxDevice* pDevice	= new XilinxDevice(sDesc);
	m_DeviceList.push_back(pDevice);
	return pDevice;
}

XilinxDevice* XilinxFamily::FindDevice(LPCTSTR sDevice)
{
	XilinxDevice*	pDevice;

	for(DWORD i = 0; i < Size(); i++) {
		pDevice	= Device(i);

		if(!pDevice->Name().Compare(sDevice)) return pDevice;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
XilinxDevices::XilinxDevices(void)
{
}

XilinxDevices::~XilinxDevices(void)
{
	for(DWORD i = 0; i < Size(); i++)
		delete m_FamilyList[i];

	m_FamilyList.clear();
}

void XilinxDevices::Initialize(void)
{
	if(m_Token.Initialize()) {
		CString sTop, sTok;

		while(m_Token.GetTopName(sTop)) {
			if(!sTop.Compare(_T("Version"))) {
				m_Token.Mustbe(XF_TOKEN_ASSIGN);

				if(m_Token.Mustbe(XF_TOKEN_STRING, &sTok)) {
					g_pSystem->LogInfo(_L(INSTALLED_VIVADO_VERSION), sTok.c_str());
				}
			} else if(!sTop.Compare(_T("DeviceListMapping"))) {
				m_Token.Mustbe(XF_TOKEN_ASSIGN);
				ListupDevices();
			} else if(!sTop.Compare(_T("Families"))) {
				m_Token.Mustbe(XF_TOKEN_ASSIGN);
				ListupFamilies();
			} else if(!sTop.Compare(_T("PartnameMapping"))) {
				m_Token.Mustbe(XF_TOKEN_ASSIGN);
				RemapPartName();
			} else if(!sTop.Compare(_T("BoardParts"))) {
				m_Token.Mustbe(XF_TOKEN_ASSIGN);
				ListupBoards();
			}
		}
	}
}

void XilinxDevices::ListupDevices(void)
{
	CString	sTok;

	if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
		while(m_Token.GetString(sTok)) {	// family name
			XilinxFamily* pFamily	= Add(sTok);

			if(m_Token.Mustbe(XF_TOKEN_ASSIGN)) {
				if(m_Token.Mustbe(XF_TOKEN_LIST_OPEN)) {
					while(m_Token.GetString(sTok)) {	// device name
						pFamily->Add(sTok);
					}
				}
			}
		}
	}
}

void XilinxDevices::ListupFamilies(void)
{
	CString	sTok;

	if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
		while(m_Token.GetString(sTok)) {
			XilinxFamily* pFamily	= FindFamily(sTok);	// family (2)

			if(pFamily && m_Token.Mustbe(XF_TOKEN_ASSIGN)) {
				if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
					while(m_Token.GetString(sTok)) {
						XilinxDevice* pDevice	= pFamily->FindDevice(sTok);	// device (3)

						if(pDevice && m_Token.Mustbe(XF_TOKEN_ASSIGN)) {
							if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
								while(m_Token.GetString(sTok)) {		// 'PackageSpeedMap' (4)
									m_Token.Mustbe(XF_TOKEN_ASSIGN);

									if(!sTok.Compare(_T("PackageSpeedMap"))) {
										m_Token.Mustbe(XF_TOKEN_GROUP_OPEN);

										while(m_Token.GetString(sTok)) {	// package
											XilinxPackage* pPackage	= pDevice->Add(sTok);
											m_Token.Mustbe(XF_TOKEN_ASSIGN);
											m_Token.Mustbe(XF_TOKEN_LIST_OPEN);

											while(m_Token.GetString(sTok)) {	// grade
												pPackage->Add(sTok);
											}
										}
									}

									m_Token.DownToGroupLevel(4);
								}
							}
						}

						m_Token.DownToGroupLevel(3);
					}
				}
			}

			m_Token.DownToGroupLevel(2);
		}
	}
}

void XilinxDevices::RemapPartName(void)
{
	CString	sTok;

	if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
		while(m_Token.GetString(sTok)) {	// device name
			XilinxFamily* pFamily	= FindFamily(sTok);

			if(m_Token.Mustbe(XF_TOKEN_ASSIGN)) {
				if(m_Token.GetString(sTok) && pFamily) {	// device descriptions
					pFamily->Name()	= sTok;
				}
			}
		}
	}
}

void XilinxDevices::ListupBoards(void)
{
	CString	sTok;

	if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
		while(m_Token.GetString(sTok)) {	// board description name
			//g_pSystem->LogInfo(_T("%s"), sTok.c_str());
			m_Token.Mustbe(XF_TOKEN_ASSIGN);

			if(m_Token.Mustbe(XF_TOKEN_GROUP_OPEN)) {
				while(m_Token.GetString(sTok)) {	// sub names
					m_Token.Mustbe(XF_TOKEN_ASSIGN);
					CString sName;
					m_Token.GetString(sName);

					if(!sTok.Compare(_T("Vendor"))) {
						//g_pSystem->LogInfo(_T("\tVendor = %s"), sName.c_str());
					} else if(!sTok.Compare(_T("Version"))) {
						//g_pSystem->LogInfo(_T("\tVersion = %s"), sName.c_str());
					} else if(!sTok.Compare(_T("BoardName"))) {
						//g_pSystem->LogInfo(_T("\tBoardName = %s"), sName.c_str());
					} else if(!sTok.Compare(_T("BoardPartName"))) {
						//g_pSystem->LogInfo(_T("\tBoardPartName = %s"), sName.c_str());
					} else if(!sTok.Compare(_T("DisplayName"))) {
						//g_pSystem->LogInfo(_T("\tDisplayName = %s"), sName.c_str());
					}

					m_Token.DownToGroupLevel(3);
				}
			}
		}
	}
}

XilinxFamily* XilinxDevices::FindFamily(LPCTSTR sFamily)
{
	XilinxFamily*	pFamily	= NULL;

	for(DWORD i = 0; i < Size(); i++) {
		pFamily	= Family(i);

		if(!pFamily->Name().Compare(sFamily)) return pFamily;
	}

	return NULL;
}

XilinxDevice* XilinxDevices::FindDevice(LPCTSTR sFamily, LPCTSTR sDevice)
{
	XilinxFamily*	pFamily	= FindFamily(sFamily);

	if(pFamily)
		return pFamily->FindDevice(sDevice);

	return NULL;
}

XilinxFamily* XilinxDevices::Add(LPCTSTR sName)
{
	XilinxFamily* pFamily	= new XilinxFamily(sName);
	m_FamilyList.push_back(pFamily);
	return pFamily;
}

void XilinxDevices::RefreshFamilyProperty(ITDPropertyData* pProperty)
{
	pProperty->AddOption(NULL);
	XilinxFamily*	pFamily	= NULL;

	for(DWORD i = 0; i < Size(); i++) {
		pFamily	= Family(i);
		pProperty->AddOption(pFamily->Name());
	}
}

void XilinxDevices::RefreshDeviceProperty(ITDPropertyData* pProperty, LPCTSTR sFamily)
{
	XilinxFamily*	pFamily	= FindFamily(sFamily);
	pProperty->AddOption(NULL);

	if(pFamily) {
		for(DWORD i = 0; i < pFamily->Size(); i++) {
			XilinxDevice* pDevice	= pFamily->Device(i);
			pProperty->AddOption(pDevice->Name());
		}
	}
}

void XilinxDevices::RefreshPackageProperty(ITDPropertyData* pProperty, LPCTSTR sFamily, LPCTSTR sDevice)
{
	XilinxDevice*	pDevice	= FindDevice(sFamily, sDevice);
	pProperty->AddOption(NULL);

	if(pDevice) {
		for(DWORD i = 0; i < pDevice->Size(); i++) {
			XilinxPackage* pPackage	= pDevice->Package(i);
			pProperty->AddOption(pPackage->Name());
		}
	}
}

void XilinxDevices::RefreshSpeedGradeProperty(ITDPropertyData* pProperty, LPCTSTR sFamily, LPCTSTR sDevice, LPCTSTR sPackage)
{
	XilinxDevice*	pDevice	= FindDevice(sFamily, sDevice);
	pProperty->AddOption(NULL);

	if(pDevice) {
		XilinxPackage*	pPackage	= pDevice->FindPackage(sPackage);

		if(pPackage) {
			for(DWORD i = 0; i < pPackage->Size(); i++) {
				pProperty->AddOption(pPackage->Grade(i));
			}
		}
	}
}
