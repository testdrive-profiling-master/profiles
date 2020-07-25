//================================================================================
// MIT License
// 
// Copyright (c) 2013 ~ 2017. HyungKi Jeong(clonextop@gmail.com)
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// Title : Common profiles
// Rev.  : 10/2/2017 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __PROFILE_CONFIG_INL__
#define __PROFILE_CONFIG_INL__
// Profile configuration

class ProfileConfig {
public:
	ProfileConfig(LPCTSTR sAppName) {
		m_sAppName		= sAppName;
	}
	~ProfileConfig() {
	}

	LPCTSTR GetString(LPCTSTR sKey, LPCTSTR sDefault = NULL) {
		RetrieveConfigPath();
		GetPrivateProfileString(m_sAppName, sKey, sDefault ? sDefault : _T(""), m_sStr, 1024, m_sConfigPath);
		return m_sStr;
	}

	LPCTSTR GetPath(LPCTSTR sKey, LPCTSTR sDefault = NULL) {
		GetString(sKey, sDefault);

		if(*m_sStr) {
			_tcscpy(m_sStr, g_pSystem->RetrieveFullPath(m_sStr));
		}

		return m_sStr;
	}

	int GetInt(LPCTSTR sApp, LPCTSTR sKey, int iDefault = 0) {
		RetrieveConfigPath();
		return GetPrivateProfileInt(sApp, sKey, iDefault, m_sConfigPath);
	}

private:
	void RetrieveConfigPath(void) {
		if(m_sConfigPath.IsEmpty())
			m_sConfigPath	= g_pSystem->RetrieveFullPath(_T("%PROJECT%Profiles\\Config.ini"));
	}
	TCHAR		m_sStr[1024];
	CString		m_sConfigPath;
	CString		m_sAppName;
};

#ifdef PROFILE_NAME
ProfileConfig	g_ProfileConfig(_T(PROFILE_NAME));
#endif

#endif//__PROFILE_CONFIG_INL__
