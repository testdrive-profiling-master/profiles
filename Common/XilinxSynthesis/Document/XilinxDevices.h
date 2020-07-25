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
// Title : Xilinx synthesis
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __XILINX_DEVICES_H__
#define __XILINX_DEVICES_H__
#include "common.h"
#include <vector>
using namespace std;

typedef enum{
	XF_TOKEN_EXIT,
	XF_TOKEN_STRING,
	XF_TOKEN_ASSIGN,
	XF_TOKEN_GROUP_OPEN,
	XF_TOKEN_GROUP_CLOSE,
	XF_TOKEN_LIST_OPEN,
	XF_TOKEN_LIST_CLOSE,
}XF_TOKEN;

class XilinxFileToken{
public:
	XilinxFileToken(void);
	~XilinxFileToken(void);

	BOOL Initialize(void);
	void Release(void);
	XF_TOKEN GetToken(CString& sTok);
	BOOL GetLine(CString& sLine);
	BOOL GetString(CString& sStr);		// Is 'XF_TOKEN_STRING'
	BOOL Mustbe(XF_TOKEN type, CString* sStr = NULL);
	int GroupLevel(void)				{return m_iGroupLevel;}
	DWORD GetLineNumber(void)			{return m_dwCurLine;}
	BOOL GetTopName(CString& sName);
	void DownToGroupLevel(int iLevel);
	BOOL IsInlist(void)					{return m_bInlist;}

private:
	FILE*		m_fp;			// file pointer
	DWORD		m_dwCurLine;	// current line number
	CString		m_sLine;		// current line
	int			m_iPos;			// token position
	int			m_iGroupLevel;	// current group level
	BOOL		m_bInlist;		// in list cases
	BOOL		m_bAssign;		// is assignment
};

class XilinxPackage{
public:
	XilinxPackage(LPCTSTR sDesc);
	~XilinxPackage(void);
	inline CString&	Name(void)					{return m_sName;}

	inline DWORD Size(void)						{return m_sGradeList.size();}
	inline LPCTSTR  Grade(int iIndex)			{return m_sGradeList[iIndex]->c_str();}
	void Add(LPCTSTR sGrade);
private:
	CString						m_sName;
	vector<CString*>			m_sGradeList;
};

class XilinxDevice{
public:
	XilinxDevice(LPCTSTR sDesc);
	~XilinxDevice(void);

	inline CString&	Name(void)					{return m_sName;}
	inline DWORD Size(void)						{return m_PackageList.size();}
	inline XilinxPackage*  Package(int iIndex)	{return m_PackageList[iIndex];}
	XilinxPackage* Add(LPCTSTR sPackage);

	XilinxPackage* FindPackage(LPCTSTR sPackage);

private:
	CString						m_sName;
	vector<XilinxPackage*>		m_PackageList;
};

class XilinxFamily{
public:
	XilinxFamily(LPCTSTR sName);
	~XilinxFamily(void);

	inline CString&	Name(void)				{return m_sName;}
	inline DWORD Size(void)					{return m_DeviceList.size();}
	inline XilinxDevice* Device(int iIndex)	{return m_DeviceList[iIndex];}
	XilinxDevice* Add(LPCTSTR sDesc);

	XilinxDevice* FindDevice(LPCTSTR sDevice);

private:
	CString						m_sName;
	vector<XilinxDevice*>		m_DeviceList;
};

class XilinxDevices
{
public:
	XilinxDevices(void);
	~XilinxDevices(void);

	void Initialize(void);
	inline DWORD Size(void)					{return m_FamilyList.size();}
	inline XilinxFamily* Family(int iIndex)	{return m_FamilyList[iIndex];}
	XilinxFamily* Add(LPCTSTR sName);

	XilinxFamily* FindFamily(LPCTSTR sFamily);
	XilinxDevice* FindDevice(LPCTSTR sFamily, LPCTSTR sDevice);
	void RefreshFamilyProperty(ITDPropertyData* pProperty);
	void RefreshDeviceProperty(ITDPropertyData* pProperty, LPCTSTR sFamily);
	void RefreshPackageProperty(ITDPropertyData* pProperty, LPCTSTR sFamily, LPCTSTR sDevice);
	void RefreshSpeedGradeProperty(ITDPropertyData* pProperty, LPCTSTR sFamily, LPCTSTR sDevice, LPCTSTR sPackage);

private:
	void ListupDevices(void);
	void ListupFamilies(void);
	void ListupBoards(void);
	void RemapPartName(void);

	XilinxFileToken				m_Token;
	vector<XilinxFamily*>		m_FamilyList;
};
#endif//__XILINX_DEVICES_H__
