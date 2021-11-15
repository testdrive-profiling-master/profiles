//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : utility framework
// Rev.  : 11/9/2021 Tue (clonextop@gmail.com)
//================================================================================
#ifndef __DOC_EXCEL_H__
#define __DOC_EXCEL_H__
#include "DocDocument.h"

class DocExcel;

class DocExcelSheet : public DocXML {
	friend class DocExcel;
protected:
	DocExcelSheet(const char* sName, DocExcel* pExcel, int iID, pugi::xml_node node);
	virtual ~DocExcelSheet(void);
	void OnSave(void);

public:
	void SetPosition(const char* sPos);
	void SetPos(int x, int y);
	string GetPosition(void);
	inline int ID(void)			{
		return m_iID;
	}
	inline int GetPosX(void)	{
		return m_CurPos.x;
	}
	inline int GetPosY(void)	{
		return m_CurPos.y;
	}

	bool GetRow(bool bAutoCreate = false);
	bool GetColumn(bool bAutoCreate = false);

	bool IsEmpty(void);
	int GetInt(int iDefault = -1);
	double GetDouble(int fDefault = -1);
	string GetValue(void);
	struct tm* GetDate(int iDateOverride = -1);
	bool SetDate(int iYear, int iMonth, int iDay);
	bool SetInt(int iValue);
	bool SetDouble(double fValue);
	bool SetString(const char* sValue);
	bool SetFunction(const char* sFunction);

	const char* GetName(void) const	{
		return m_sName.c_str();
	}
	void SetName(const char* sName)	{
		m_sName	= sName;
	}

private:
	DocExcel*			m_pExcel;
	int					m_iID;
	DocXML				m_SheetData;
	DocXML				m_Row, m_Column;
	string				m_sName;
	bool				m_bRecompute;
	struct {
		int				sx, sy;
		int				ex, ey;
	} m_Dimension;
	struct {
		int				x, y;
	} m_CurPos, m_Origin;
};

class DocExcel : public DocFile {
public:
	DocExcel(void);
	virtual ~DocExcel(void);

	const char* GetString(int iIndex);
	int GetStringIndex(const char* sStr, bool bAutoAppend = true);
	int GetSheetCount(void);
	DocExcelSheet* GetSheet(const char* sSheetName);
	DocExcelSheet* GetSheetByIndex(int iIndex);

protected:
	virtual bool OnOpen(void);
	virtual void OnClose(void);
	virtual bool OnSave(void);

private:
	DocXML						m_Sheets;
	map<string, DocExcelSheet*>	m_SheetMap;

	DocXML						m_Relationships;
	DocXML						m_SharedStrings;
	vector<string>				m_StringTable;
};

void	Excel_PosDecode(cstring sPos, int& x, int& y);
string	Excel_PosEncode(int x, int y);
void	Excel_DateToYMD(int iDate, int& iYear, int& iMonth, int& iDay);
int		Excel_YMDToDate(int iYear, int iMonth, int iDay);

#endif//__DOC_EXCEL_H__
