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
// Title : utility framework
// Rev.  : 3/5/2024 Tue (clonextop@gmail.com)
//================================================================================
#ifndef __DOC_EXCEL_H__
#define __DOC_EXCEL_H__
#include "DocDocument.h"

typedef enum {
	EXCEL_RELATIONSHIP_worksheet,
	EXCEL_RELATIONSHIP_calcChain,
	EXCEL_RELATIONSHIP_sharedStrings,
	EXCEL_RELATIONSHIP_styles,
	EXCEL_RELATIONSHIP_theme,
	EXCEL_RELATIONSHIP_SIZE
} EXCEL_RELATIONSHIP;

typedef enum { // default number formats
	EXCEL_NUMBER_FORMAT_GENERAL					= 0,
	EXCEL_NUMBER_FORMAT_NUMBER					= 1,
	EXCEL_NUMBER_FORMAT_NUMBER_D2				= 2,
	EXCEL_NUMBER_FORMAT_NUMBER_SEP				= 3,
	EXCEL_NUMBER_FORMAT_NUMBER_SEP_D2			= 4,
	EXCEL_NUMBER_FORMAT_CURRENCY_NEGBRA			= 5,
	EXCEL_NUMBER_FORMAT_CURRENCY_NEGBRARED		= 6,
	EXCEL_NUMBER_FORMAT_CURRENCY_D2_NEGBRA		= 7,
	EXCEL_NUMBER_FORMAT_CURRENCY_D2_NEGBRARED	= 8,
	EXCEL_NUMBER_FORMAT_PERCENT					= 9,
	EXCEL_NUMBER_FORMAT_PERCENT_D2				= 10,
	EXCEL_NUMBER_FORMAT_SCIENTIFIC_D2			= 11,
	EXCEL_NUMBER_FORMAT_FRACTION_ONEDIG			= 12,
	EXCEL_NUMBER_FORMAT_FRACTION_TWODIG			= 13,
	EXCEL_NUMBER_FORMAT_DATE					= 14,
	EXCEL_NUMBER_FORMAT_DATE_D_MON_YY			= 15,
	EXCEL_NUMBER_FORMAT_DATE_D_MON				= 16,
	EXCEL_NUMBER_FORMAT_DATE_MON_YY				= 17,
	EXCEL_NUMBER_FORMAT_TIME_HMM_AM				= 18,
	EXCEL_NUMBER_FORMAT_TIME_HMMSS_AM			= 19,
	EXCEL_NUMBER_FORMAT_TIME_HMM				= 20,
	EXCEL_NUMBER_FORMAT_TIME_HMMSS				= 21,
	EXCEL_NUMBER_FORMAT_DATE_TIME				= 22,
	EXCEL_NUMBER_FORMAT_NUMBER_SEP_NEGBRA		= 37,
	EXCEL_NUMBER_FORMAT_NUMBER_SEP_NEGBRARED	= 38,
	EXCEL_NUMBER_FORMAT_NUMBER_D2_SEP_NEGBRA	= 39,
	EXCEL_NUMBER_FORMAT_NUMBER_D2_SEP_NEGBRARED = 40,
	EXCEL_NUMBER_FORMAT_ACCOUNT					= 41,
	EXCEL_NUMBER_FORMAT_ACCOUNTCUR				= 42,
	EXCEL_NUMBER_FORMAT_ACCOUNT_D2				= 43,
	EXCEL_NUMBER_FORMAT_ACCOUNT_D2_CUR			= 44,
	EXCEL_NUMBER_FORMAT_TIME_MMSS				= 45,
	EXCEL_NUMBER_FORMAT_TIME_H0MMSS				= 46,
	EXCEL_NUMBER_FORMAT_TIME_MMSS0				= 47,
	EXCEL_NUMBER_FORMAT_SCIENTIFIC				= 48,
	EXCEL_NUMBER_FORMAT_TEXT					= 49,
} EXCEL_NUMBER_FORMAT;

class DocExcel;
class DocExcelSheet;
class DocExcelStyle;

class DocExcelPos
{
public:
	DocExcelPos(void);
	DocExcelPos(const char *sPos);
	DocExcelPos(int iX, int iY);
	DocExcelPos(DocExcelSheet *pSheet);
	~DocExcelPos(void);
	void   Set(const char *sPos);
	string Get(void) const;
	string Relative(int iIncreaseX, int iIncreaseY) const;

	int	   x, y;
};

class DocExcelSheet : public DocXML
{
	friend class DocExcel;

protected:
	DocExcelSheet(const char *sName, const char *sEntryPath, DocExcel *pExcel, int iID, pugi::xml_node node);
	virtual ~DocExcelSheet(void);
	bool OnSave(void);

public:
	void		SetPosition(const char *sPos);
	void		SetPos(int x, int y);
	string		GetPosition(void);
	DocExcelPos GetPos(void);
	inline int	ID(void)
	{
		return m_iID;
	}
	inline int GetPosX(void)
	{
		return m_CurPos.x - 1;
	}
	inline int GetPosY(void)
	{
		return m_CurPos.y;
	}
	inline DocExcel *Parent(void)
	{
		return m_pExcel;
	}
	bool		   GetRow(bool bAutoCreate = false);
	bool		   GetColumn(bool bAutoCreate = false);
	bool		   ValidateColumn(void);
	double		   GetColumnWidth(void);

	bool		   IsEmpty(void);
	int			   GetInt(int iDefault = -1);
	double		   GetDouble(int fDefault = -1);
	string		   GetValue(bool bUseMergedData = false);
	string		   GetNumberFormat(void);
	struct tm	  *GetDate(int iDateOverride = -1);
	bool		   IsMergedCell(void);
	DocExcelStyle *GetStyle(void);
	bool		   SetDate(int iYear, int iMonth, int iDay);
	bool		   SetInt(int iValue);
	bool		   SetDouble(double fValue);
	bool		   SetString(const char *sValue);
	bool		   SetFunction(const char *sFunction);
	void		   SetPane(const char *sPos);
	bool		   SetStyle(int iCellStyle);
	bool		   SetColumnWidth(double fWidth, bool bBestFit = true);
	void		   SetTabColor(unsigned int dwColorRGB);
	bool		   SetConditionalFormatting(const char *sFomula, int iStyleFormat);
	void		   Hide(bool bHide = true);
	bool		   MergeCells(const char *sBegin, const char *sEnd);
	bool		   GetMergeCellPos(int &delta_x, int &delta_y, int &width, int &height);
	bool		   HideColumn(bool bHide = true);
	void		   SetProtection(const char *sHash, const char *sSalt, const char *sExceptionRangeList);

	const char	  *GetName(void) const
	{
		return m_sName.c_str();
	}
	void		SetName(const char *sName);
	const char *EntryPath(void) const
	{
		return m_sEntryPath.c_str();
	}

	bool m_bDebugMode; // TODO : delete me! It's temporary.
protected:
	void ExpandDimension(int x, int y);

private:
	DocExcel *m_pExcel;
	int		  m_iID;
	DocXML	  m_SheetData;
	DocXML	  m_Row, m_Column;
	string	  m_sName;
	string	  m_sEntryPath;
	bool	  m_bRecompute;
	struct {
		int sx, sy;
		int ex, ey;
	} m_Dimension;
	struct {
		int x, y;
	} m_CurPos, m_Origin;
};

class DocExcelStyle : public DocXML
{
	friend class DocExcel;

public:
	DocExcelStyle(DocXML *pParent, int iID, pugi::xml_node node);
	virtual ~DocExcelStyle(void);

	const char *AlignmentHorizontal(void);

	inline int	ID(void)
	{
		return m_iID;
	}

private:
	DocXML *m_pParent;
	int		m_iID;
};

class DocExcel : public DocFile
{
public:
	DocExcel(void);
	virtual ~DocExcel(void);

	virtual bool   Open(const char *sFileName = NULL);

	const char	  *GetString(int iIndex);
	int			   GetStringIndex(const char *sStr, bool bAutoAppend = true);
	int			   GetSheetCount(void);
	DocExcelSheet *GetSheet(const char *sName);
	DocExcelSheet *GetSheetByIndex(int iIndex);
	DocExcelSheet *CreateSheet(const char *sName);
	DocExcelStyle *GetStyleByIndex(int iIndex);
	void		   DeleteSheet(DocExcelSheet *pSheet);
	int			   StyleFont(const char *sFontName, int iFontSize, bool bBold = false, bool bItalic = false,
							 unsigned int dwARGB = 0);
	int			   StyleFill(unsigned int dwColorARGB);
	int			   StyleBorder(const char *sBorderStyle);
	int			   StyleNumberFormat(const char *sFormat = NULL);
	string		   StyleNumberFormatString(int iID);
	int	 StyleCell(int iStyleFont, int iStyleFill, int iStyleBorder, int iStyleNumberFormat, const char *sAlignment);
	int	 StyleFormat(const char *sFormat);
	bool ReplaceSheetName(DocExcelSheet *pSheet, const char *sName);
	inline DocXML &Workbook(void)
	{
		return m_Workbook;
	}

protected:
	virtual bool OnOpen(void);
	virtual void OnClose(void);
	virtual bool OnSave(void);
	virtual bool OnDelete(const char *sEntryName);

	friend class DocExcelSheet;
	string CreateRelationship(EXCEL_RELATIONSHIP type, const char *sEntryPath);
	void   AddCalChain(DocExcelSheet *pSheet);

private:
	DocXML							  m_ContentTypes;
	DocXML							  m_Workbook;
	DocXML							  m_Styles;
	map<string, DocExcelSheet *>	  m_SheetMap;

	DocXML							  m_Relationships;
	DocXML							  m_SharedStrings;
	DocXML							  m_CalcChain;
	vector<string>					  m_StringTable;
	vector<unique_ptr<DocExcelStyle>> m_StyleList;
};

void   Excel_PosDecode(cstring sPos, int &x, int &y);
string Excel_PosEncode(int x, int y);
void   Excel_DateToYMD(int iDate, int &iYear, int &iMonth, int &iDay);
int	   Excel_YMDToDate(int iYear, int iMonth, int iDay);

#endif //__DOC_EXCEL_H__
