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
// Rev.  : 11/19/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "DocExcel.h"

void Excel_DateToYMD(int iDate, int& iYear, int& iMonth, int& iDay)
{
	// Modified Julian to YMD calculation with an addition of 2415019
	int l  = iDate + 68569 + 2415019;
	int n  = int((4 * l) / 146097);
	l      = l - int((146097 * n + 3) / 4);
	int i  = int((4000 * (l + 1)) / 1461001);
	l      = l - int((1461 * i) / 4) + 31;
	int j  = int((80 * l) / 2447);
	iDay   = l - int((2447 * j) / 80);
	l      = int(j / 11);
	iMonth = j + 2 - (12 * l);
	iYear  = 100 * (n - 49) + i + l;
}

int Excel_YMDToDate(int iYear, int iMonth, int iDay)
{
	// YMD to Modified Julian calculated with an extra subtraction of 2415019.
	return int((1461 * (iYear + 4800 + int((iMonth - 14) / 12))) / 4) +
		   int((367 * (iMonth - 2 - 12 * ((iMonth - 14) / 12))) / 12) -
		   int((3 * (int((iYear + 4900 + int((iMonth - 14) / 12)) / 100))) / 4) +
		   iDay - 2415019 - 32075;
}

void Excel_PosDecode(cstring sPos, int& x, int& y)
{
	const char* s	= sPos.c_str();

	// get y
	for(int i = 0; i < sPos.size(); i++) {
		if(isdigit(s[i])) {
			y	= atoi(s + i) - 1;
			sPos.erase(i, -1);
			break;
		}
	}

	// get x
	sPos.MakeUpper();
	s	= sPos.c_str();
	x	= 0;

	for(int i = 0; i < sPos.size(); i++) {
		x	*= 26;
		x	+= (int)(s[i] - 'A');
	}
}

string Excel_PosEncode(int x, int y)
{
	cstring sPos;

	if(x) {
		char	ch[2] = "A";

		while(x) {
			ch[0]	= (char)(x % 26) + 'A';
			x		/= 26;
			sPos.insert(0, ch);
		}
	} else {
		sPos	+= 'A';
	}

	sPos.AppendFormat("%d", y + 1);
	return sPos.c_str();
}

DocExcelSheet::DocExcelSheet(const char* sName, const char* sEntryPath, DocExcel* pExcel, int iID, pugi::xml_node node) : DocXML(node)
{
	m_sName			= sName;
	m_sEntryPath	= sEntryPath;
	m_pExcel		= pExcel;
	m_iID			= iID;
	m_bRecompute	= false;
	{
		// get dimension
		memset(&m_Dimension, 0, sizeof(m_Dimension));
		cstring	sDimension(child("dimension").attribute("ref").value());
		const char* sDelim	= ":";
		int		iPos	= 0;
		Excel_PosDecode(sDimension.Tokenize(iPos, sDelim), m_Dimension.sx, m_Dimension.sy);
		Excel_PosDecode(sDimension.Tokenize(iPos, sDelim), m_Dimension.ex, m_Dimension.ey);
		// initialize origin & current position
		m_CurPos.x	= m_Dimension.sx;
		m_CurPos.y	= m_Dimension.sy;
		m_Origin.x	= m_Dimension.sx;
		m_Origin.y	= m_Dimension.sy;
	}
	{
		// get data
		m_SheetData = child("sheetData");
	}
}

DocExcelSheet::~DocExcelSheet(void)
{
}

void DocExcelSheet::SetPosition(const char* sPos)
{
	if(sPos) {
		Excel_PosDecode(sPos, m_Origin.x, m_Origin.y);
		m_CurPos.x	= m_Origin.x;
		m_CurPos.y	= m_Origin.y;
		m_Row		= pugi::xml_node(NULL);
		m_Column	= pugi::xml_node(NULL);
	}
}

void DocExcelSheet::SetPos(int x, int y)
{
	if(x >= 0) m_CurPos.x	= m_Origin.x	= x;

	if(y >= 0) m_CurPos.y	= m_Origin.y	= y;

	m_Row		= pugi::xml_node(NULL);
	m_Column	= pugi::xml_node(NULL);
}

string DocExcelSheet::GetPosition(void)
{
	return Excel_PosEncode(m_CurPos.x, m_CurPos.y);
}

bool DocExcelSheet::GetRow(bool bAutoCreate)
{
	cstring	sID;
	m_Column	= pugi::xml_node(NULL);

	if(!m_Row.empty()) m_CurPos.y++;

	m_CurPos.x	= m_Origin.x;
	sID.Format("%d", m_CurPos.y + 1);
	m_Row	= m_SheetData.find_child_by_attribute("r", sID.c_str());

	if(bAutoCreate && m_Row.empty()) {
		m_Row	= m_SheetData.append_child("row");
		m_Row.append_attribute("r").set_value(sID.c_str());

		// fix dimension
		if(m_Dimension.sy > m_CurPos.y) m_Dimension.sy = m_CurPos.y;

		if(m_Dimension.ey < m_CurPos.y) m_Dimension.ey = m_CurPos.y;
	}

	return !m_Row.empty();
}

bool DocExcelSheet::GetColumn(bool bAutoCreate)
{
	if(m_Row.empty()) return false;

	cstring	sID	= Excel_PosEncode(m_CurPos.x, m_CurPos.y);
	m_Column	= m_Row.find_child_by_attribute("r", sID.c_str());
	m_CurPos.x++;

	if(bAutoCreate && m_Column.empty()) {
		m_Column	= m_Row.append_child("c");
		m_Column.append_attribute("r").set_value(sID.c_str());
		m_Column.append_child("v");

		// fix dimension
		if(m_Dimension.sx > (m_CurPos.x - 1)) m_Dimension.sx = m_CurPos.x - 1;

		if(m_Dimension.ex < (m_CurPos.x - 1)) m_Dimension.ex = m_CurPos.x - 1;
	}

	return !m_Column.empty();
}

bool DocExcelSheet::IsEmpty(void)
{
	return (m_Row.empty() || m_Column.empty());
}

int DocExcelSheet::GetInt(int iDefault)
{
	int	iData	= iDefault;

	if(!IsEmpty()) {
		bool	bString	= !strcmp(m_Column.attribute("t").value(), "s");

		if(bString) {
			const char* sData	= m_pExcel->GetString(m_Column.child("v").text().as_int(-1));

			if(sData) iData		= atoi(sData);
		} else {
			iData				= m_Column.child("v").text().as_int(iDefault);
		}
	}

	return iData;
}

double DocExcelSheet::GetDouble(int fDefault)
{
	int	fData	= fDefault;

	if(!IsEmpty()) {
		bool	bString	= !strcmp(m_Column.attribute("t").value(), "s");

		if(bString) {
			const char* sData	= m_pExcel->GetString(m_Column.child("v").text().as_int(-1));

			if(sData) fData		= atof(sData);
		} else {
			fData				= m_Column.child("v").text().as_double(fDefault);
		}
	}

	return fData;
}

string DocExcelSheet::GetValue(void)
{
	string	sValue;

	if(!IsEmpty()) {
		bool	bString	= !strcmp(m_Column.attribute("t").value(), "s");
		sValue			= m_Column.child("v").text().get();

		if(bString) {
			sValue	= m_pExcel->GetString(atoi(sValue.c_str()));
		}
	}

	return sValue;
}

struct tm* DocExcelSheet::GetDate(int iDateOverride)
{
	int					iDate	= (iDateOverride == -1) ? GetInt() : iDateOverride;
	static struct tm	t;

	if(iDate != -1) {
		memset(&t, 0, sizeof(t));
		Excel_DateToYMD(iDate, t.tm_year, t.tm_mon, t.tm_mday);
		return &t;
	}

	return NULL;
}

bool DocExcelSheet::SetDate(int iYear, int iMonth, int iDay)
{
	int iDate	= Excel_YMDToDate(iYear, iMonth, iDay);
	return SetInt(iDate);
}

bool DocExcelSheet::SetInt(int iValue)
{
	if(m_Column.empty()) return false;

	m_Column.remove_attribute("t");
	m_Column.child("v").text().set(iValue);
	m_Column.remove_child("f");
	m_bRecompute	= true;
	return true;
}

bool DocExcelSheet::SetDouble(double fValue)
{
	if(m_Column.empty()) return false;

	m_Column.remove_attribute("t");
	m_Column.child("v").text().set(fValue);
	m_Column.remove_child("f");
	m_bRecompute	= true;
	return true;
}

bool DocExcelSheet::SetString(const char* sValue)
{
	if(!sValue || m_Column.empty()) return false;

	{
		// set string type
		pugi::xml_attribute attr = m_Column.attribute("t");

		if(attr.empty()) attr = m_Column.append_attribute("t");

		attr.set_value("s");
	}
	m_Column.child("v").text().set(m_pExcel->GetStringIndex(sValue));
	m_Column.remove_child("f");
	m_bRecompute	= true;
	return true;
}

bool DocExcelSheet::SetFunction(const char* sFunction)
{
	if(!sFunction || m_Column.empty()) return false;

	{
		DocXML	node	= m_Column.child("f");

		if(node.empty())
			node		= m_Column.append_child("f");

		node.text().set(sFunction);
		m_Column.remove_child("v");
		m_Column.remove_child("t");
	}
	m_bRecompute	= true;
	return true;
}

void DocExcelSheet::SetPane(const char* sPos)
{
	DocXML node = child("sheetViews").child("sheetView");
	node.remove_child("pane");

	if(sPos) {
		int x, y;
		Excel_PosDecode(sPos, x, y);
		node = node.append_child("pane");

		if(x) node.append_attribute("xSplit")	= x;

		if(y) node.append_attribute("ySplit")	= y;

		node.append_attribute("topLeftCell")	= sPos;
		node.append_attribute("activePane")		= "bottomLeft";
		node.append_attribute("state")			= "frozen";
	}
}

bool DocExcelSheet::SetStyle(int iCellStyle)
{
	if(m_Column.empty()) return false;

	if(m_Column.attribute("s").as_int(-1) == -1) {
		// no style existed
		m_Column.append_attribute("s") = iCellStyle;
	} else {
		m_Column.attribute("s") = iCellStyle;
	}

	return true;
}

bool DocExcelSheet::SetColumnWidth(double fWidth, bool bBestFit)
{
	if(m_Column.empty()) return false;

	cstring	sPos;
	sPos.Format("%d", m_CurPos.x);
	// get 'cols'
	DocXML	node	= child("cols");

	if(!node) {	// create if not existed
		node	= insert_child_after("cols", child("sheetData").previous_sibling());
	}

	// remove previous 'col'
	DocXML	col		= node.find_child_by_attribute("min", sPos);

	if(col) node.remove_child(col);

	// create new 'col'
	col	= node.append_child("col");
	// set attributes
	col.append_attribute("min")				= sPos;
	col.append_attribute("max")				= sPos;
	col.append_attribute("width")			= fWidth;

	if(bBestFit) col.append_attribute("bestFit")	= 1;

	col.append_attribute("customWidth")		= 1;
	return true;
}

void DocExcelSheet::SetTabColor(unsigned int dwColorRGB)
{
	DocXML	node	= child("sheetPr");
	cstring	sColor;
	sColor.Format("%08X", dwColorRGB | 0xFF000000);

	if(!node) {	// create if not existed
		node	= prepend_child("sheetPr");
	}

	DocXML	tab_color	= node.child("tabColor");

	if(!tab_color)	tab_color = node.append_child("tabColor");

	if(!tab_color.attribute("rgb")) tab_color.append_attribute("rgb");

	tab_color.attribute("rgb")		= sColor;
}

bool DocExcelSheet::MergeCells(const char* sBegin, const char* sEnd)
{
	if(!Size("mergeCells")) {
		insert_child_after("mergeCells", child("sheetData")).append_attribute("count")	= 0;
	}

	DocXML node = child("mergeCells");
	cstring	sRef;
	sRef.Format("%s:%s", sBegin, sEnd);
	node.append_child("mergeCell").append_attribute("ref")	= sRef;
	node.attribute("count")		= node.Size("mergeCell");
	{
		int x, y;
		Excel_PosDecode(sBegin, x, y);
		ExpandDimension(x, y);
		Excel_PosDecode(sEnd, x, y);
		ExpandDimension(x, y);
	}
	return true;
}

void DocExcelSheet::SetName(const char* sName)
{
	if(m_pExcel) m_pExcel->ReplaceSheetName(this, sName);
}

void DocExcelSheet::ExpandDimension(int x, int y)
{
	// fix dimension
	if(m_Dimension.sx > x) m_Dimension.sx = x;
	else if(m_Dimension.ex < x) m_Dimension.ex = x;

	if(m_Dimension.sy > y) m_Dimension.sy = y;
	else if(m_Dimension.ey < y) m_Dimension.ey = y;
}


bool DocExcelSheet::OnSave(void)
{
	// apply dimension
	cstring sDimension;
	sDimension.Format("%s:%s",
					  Excel_PosEncode(m_Dimension.sx, m_Dimension.sy).c_str(),
					  Excel_PosEncode(m_Dimension.ex, m_Dimension.ey).c_str());
	child("dimension").attribute("ref")		= sDimension;

	// delete value on function cell to recompute all value
	if(m_bRecompute) {
		m_SheetData.EnumerateInDepth("c", NULL, [](DocXML node, void* pPrivate) -> bool {
			if(!node.child("f").empty())
			{
				node.remove_child("v");
			}
			return true;
		});
	}

	return false;
}

DocExcel::DocExcel(void)
{
}

DocExcel::~DocExcel(void)
{
	OnClose();
}

bool DocExcel::Open(const char* sFileName)
{
	if(!sFileName) {
		string	sTemplateFileName	= GetCommonToolPath() + "/bin/lua/DocExcel_template.xlsx";
		return DocFile::Open(sTemplateFileName.c_str());
	}

	return DocFile::Open(sFileName);
}

bool DocExcel::OnOpen(void)
{
	m_ContentTypes		= GetXML("[Content_Types].xml")->child("Types");
	m_Workbook			= GetXML("xl/workbook.xml")->child("workbook").child("sheets");
	m_Styles			= GetXML("xl/styles.xml")->child("styleSheet");
	m_Relationships		= GetXML("xl/_rels/workbook.xml.rels")->child("Relationships");

	if(GetXML("xl/sharedStrings.xml")) {
		m_SharedStrings		= GetXML("xl/sharedStrings.xml")->child("sst");
	} else {
		m_SharedStrings		= GetXML("xl/sharedStrings.xml", true)->append_child("sst");
		m_SharedStrings.append_attribute("xmlns")		= "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
		m_SharedStrings.append_attribute("count")		= "0";
		m_SharedStrings.append_attribute("uniqueCount")	= "0";
		// add t contents
		DocXML	xml			= m_ContentTypes.append_child("Override");
		xml.append_attribute("PartName")		= "/xl/sharedStrings.xml";
		xml.append_attribute("ContentType")		= "application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml";
		CreateRelationship(EXCEL_RELATIONSHIP_sharedStrings, "/xl/sharedStrings.xml");
	}

	if(GetXML("xl/calcChain.xml")) {
		m_CalcChain			= GetXML("xl/calcChain.xml")->child("calcChain");
	} else {
		m_CalcChain			= GetXML("xl/calcChain.xml", true)->child("calcChain");
		m_CalcChain.append_attribute("xmlns")	= "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
		// add t contents
		DocXML	xml			= m_ContentTypes.append_child("Override");
		xml.append_attribute("PartName")		= "/xl/calcChain.xml";
		xml.append_attribute("ContentType")		= "application/vnd.openxmlformats-package.core-properties+xml";
		CreateRelationship(EXCEL_RELATIONSHIP_calcChain, "/xl/calcChain.xml");
	}

	// make string map
	m_SharedStrings.EnumerateInDepth("si", &m_StringTable, [](DocXML node, void* pPrivate) -> bool {
		vector<string>* pStringTable			= (vector<string>*)pPrivate;
		cstring			sText;

		node.EnumerateInDepth("t", &sText, [](DocXML node, void* pPrivate) -> bool {
			cstring* pText	= (cstring*)pPrivate;
			pText->Append(node.text().get());
			return true;
		});
		sText.Replace("_x000D_", "", true);
		pStringTable->push_back(sText.c_str());
		return true;
	});
	// make sheet table map
	{
		typedef	struct {
			DocExcel*						pExel;
			map<string, DocExcelSheet*>*	pSheetMap;
			DocXML*							pRelationships;
		} SHEET_REF;
		SHEET_REF	packed = {this, &m_SheetMap, &m_Relationships};
		m_Workbook.Enumerate("sheet", &packed, [](DocXML node, void* pPrivate) -> bool {
			SHEET_REF*	p	= (SHEET_REF*)pPrivate;
			cstring		sheet_name		= node.attribute("name").value();
			int			sheet_id		= node.attribute("sheetId").as_int(0);
			cstring		sheet_entry_name;
			sheet_entry_name.Format("xl/%s", p->pRelationships->find_child_by_attribute("Id", node.attribute("r:id").value()).attribute("Target").value());
			DocExcelSheet*	pNewSheet				= new DocExcelSheet(sheet_name, sheet_entry_name, p->pExel, sheet_id, p->pExel->GetXML(sheet_entry_name.c_str())->child("worksheet"));
			(*(p->pSheetMap))[sheet_name.c_str()]	= pNewSheet;
			return true;
		});
	}
	return true;
}

void DocExcel::OnClose(void)
{
	for(auto& i : m_SheetMap) {
		delete i.second;
	}

	m_SheetMap.clear();
	m_StringTable.clear();
	m_CalcChain			= pugi::xml_node();
	m_SharedStrings		= pugi::xml_node();
	m_Relationships		= pugi::xml_node();
	m_Workbook			= pugi::xml_node();
	m_ContentTypes		= pugi::xml_node();
}

bool DocExcel::OnSave(void)
{
	if(!m_SharedStrings.Size("si")) {	// no string table contents
		DeleteFile("xl/sharedStrings.xml");
	}

	if(!m_SharedStrings.Size("c")) {	// no functions
		DeleteFile("xl/calcChain.xml");
	}

	for(auto& i : m_SheetMap) {
		i.second->OnSave();
	}

	return true;
}

bool DocExcel::OnDelete(const char* sEntryName)
{
	cstring	sPath;
	sPath.Format("/%s", sEntryName);
	m_ContentTypes.remove_child(m_ContentTypes.find_child_by_attribute("PartName", sPath));
	sPath.DeleteFront("/xl/");
	m_Relationships.remove_child(m_Relationships.find_child_by_attribute("Target", sPath));
	return true;
}

string DocExcel::CreateRelationship(EXCEL_RELATIONSHIP type, const char* sEntryPath)
{
	static const char* __relationship_list[EXCEL_RELATIONSHIP_SIZE] = {
		"worksheet",
		"calcChain",
		"sharedStrings",
		"styles",
		"theme",
	};
	cstring sID;
	cstring sTarget(sEntryPath);
	cstring sType;
	sTarget.DeleteFront("/xl/");
	sTarget.DeleteFront("xl/");
	sType.Format("http://schemas.openxmlformats.org/officeDocument/2006/relationships/%s", __relationship_list[(int)type]);

	for(int i = 1;; i++) {
		sID.Format("rId%d", i);

		if(m_Relationships.find_child_by_attribute("Id", sID).empty()) {
			DocXML node	= m_Relationships.append_child("Relationship");
			node.append_attribute("Id")			= sID;
			node.append_attribute("Type")		= sType;
			node.append_attribute("Target")		= sTarget;
			break;
		}
	}

	return sID.c_str();
}

int DocExcel::GetSheetCount(void)
{
	return m_SheetMap.size();
}

int DocExcel::StyleFont(const char* sFontName, int iFontSize, bool bBold, bool bItalic)
{
	string	sFont(sFontName);
	DocXML	node				= m_Styles.child("fonts");
	{
		// setup default
		DocXML	default_font		= node.child("font");

		if(!sFont.size()) sFont		= default_font.child("name").attribute("val").value();

		if(!iFontSize) iFontSize	= atoi(default_font.child("sz").attribute("val").value());
	}
	typedef struct {
		int			iIndex;
		cstring		sFontName;
		int			iFontSize;
		bool		bBold;
		bool		bItalic;
		int			iRet;
	} private_data;
	private_data	p;
	p.iIndex	= -1;
	p.sFontName	= sFont;
	p.iFontSize	= iFontSize;
	p.bBold		= bBold;
	p.bItalic	= bItalic;
	p.iRet		= -1;
	node.Enumerate("font", &p, [](DocXML node, void* pPrivate) -> bool {
		private_data* p = (private_data*)pPrivate;
		p->iIndex++;

		if(p->sFontName == node.child("name").attribute("val").as_string() &&
		   p->iFontSize == node.child("sz").attribute("val").as_int() &&
		   p->bBold == node.Size("b") &&
		   p->bItalic == node.Size("i"))
		{
			p->iRet	= p->iIndex;
			return false;
		}
		return true;
	});

	if(p.iRet < 0) {
		// not matched font found, must create it
		p.iRet			= node.Size("font");
		DocXML	font	= node.append_child("font");

		if(bBold) font.append_child("b");

		if(bItalic) font.append_child("i");

		font.append_child("sz").append_attribute("val")				= iFontSize;
		font.append_child("color").append_attribute("theme")		= 1;
		font.append_child("name").append_attribute("val")			= sFont.c_str();
		font.append_child("family").append_attribute("val")			= 2;
		font.append_child("scheme").append_attribute("val")			= "minor";
		node.attribute("count")										= p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleFill(unsigned int dwColorARGB)
{
	DocXML	node	= m_Styles.child("fills");
	typedef struct {
		int			iIndex;
		cstring		sColorValue;
		int			iRet;
	} private_data;
	private_data	p;
	p.iIndex	= -1;
	p.iRet		= -1;
	p.sColorValue.Format("%08X", dwColorARGB);
	node.Enumerate("fill", &p, [](DocXML node, void* pPrivate) -> bool {
		private_data* p = (private_data*)pPrivate;
		node	= node.child("patternFill");
		p->iIndex++;

		if(!node.empty() && !strcmp(node.attribute("patternType").as_string(), "solid") && !strcmp(node.child("fgColor").attribute("rgb").as_string(), p->sColorValue))
		{
			p->iRet	= p->iIndex;
			return false;
		}
		return true;
	});

	if(p.iRet < 0) {
		// not matched fill found, must create it
		p.iRet			= node.Size("fill");
		DocXML	fill	= node.append_child("fill").append_child("patternFill");
		fill.append_attribute("patternType")						= "solid";
		fill.append_child("fgColor").append_attribute("rgb")		= p.sColorValue;
		fill.append_child("bgColor").append_attribute("indexed")	= 64;
		node.attribute("count")										= p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleBorder(bool bLeft, bool bRight, bool bTop, bool bBottom, bool bThick)
{
	DocXML	node	= m_Styles.child("borders");
	cstring	sStyle	= bThick ? "thick" : "thin";
	typedef struct {
		int			iIndex;
		cstring		sLeft, sRight, sTop, sBottom;
		int			iRet;
	} private_data;
	private_data	p;
	p.iIndex		= -1;
	p.iRet			= -1;
	{
		if(bLeft)	p.sLeft		= sStyle;

		if(bRight)	p.sRight	= sStyle;

		if(bTop)	p.sTop		= sStyle;

		if(bBottom)	p.sBottom	= sStyle;
	}
	node.Enumerate("border", &p, [](DocXML node, void* pPrivate) -> bool {
		private_data* p = (private_data*)pPrivate;
		p->iIndex++;

		if(p->sLeft == node.child("left").attribute("style").value() &&
		   p->sRight == node.child("right").attribute("style").value() &&
		   p->sTop == node.child("top").attribute("style").value() &&
		   p->sBottom == node.child("bottom").attribute("style").value() &&
		   !*node.child("diagonal").attribute("style").value())
		{
			p->iRet	= p->iIndex;
			return false;
		}
		return true;
	});

	if(p.iRet < 0) {
		// not matched border found, must create it
		p.iRet			= node.Size("border");
		DocXML	border	= node.append_child("border");
		DocXML	att;
		att		= border.append_child("left");

		if(bLeft) {
			att.append_attribute("style")						= sStyle;
			att.append_child("color").append_attribute("auto")	= 1;
		}

		att		= border.append_child("right");

		if(bRight) {
			att.append_attribute("style")						= sStyle;
			att.append_child("color").append_attribute("auto")	= 1;
		}

		att		= border.append_child("top");

		if(bTop) {
			att.append_attribute("style")						= sStyle;
			att.append_child("color").append_attribute("auto")	= 1;
		}

		att		= border.append_child("bottom");

		if(bBottom) {
			att.append_attribute("style")						= sStyle;
			att.append_child("color").append_attribute("auto")	= 1;
		}

		att		= border.append_child("diagonal");
		node.attribute("count")						= p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleCell(int iStyleFont, int iStyleFill, int iStyleBorder, const char* sAlignment)
{
	DocXML	node	= m_Styles.child("cellXfs");
	typedef struct {
		int			iIndex;
		int			iFont, iFill, iBorder;
		struct {
			bool		bEnable;
			string		sHorizontal, sVertical;
		} alignment;
		int			iRet;
	} private_data;
	private_data	p;
	p.iIndex		= -1;
	p.iRet			= -1;
	p.alignment.bEnable	= sAlignment || *sAlignment;

	if(p.alignment.bEnable) {	// setup alignment
		const char*	sDelim	= ",;";
		int			iPos	= 0;
		cstring sAlign(sAlignment);

		for(;;) {
			cstring sTok = sAlign.Tokenize(iPos, sDelim);

			if(iPos >= 0) {
				const char*	sTokDelim	= " \"=";
				int iTokPos	= 0;
				cstring sTarget		= sTok.Tokenize(iTokPos, sTokDelim);
				cstring sVal		= sTok.Tokenize(iTokPos, sTokDelim);

				if(iTokPos < 0) {
					LOGE("Invalid DocExcel cell alignment : %s", sAlignment);
					exit(1);
				}

				if(sTarget == "horizontal" && !p.alignment.sHorizontal.size()) {
					p.alignment.sHorizontal		= sVal;
					const char*	sList[] = {"left", "center", "right", NULL};

					if(sVal.RetrieveTag(sList) < 0) {
						LOGE("Invalid DocExcel cell alignment value : %s(%s)", sAlignment, sVal.c_str());
						exit(1);
					}
				} else if(sTarget == "vertical" && !p.alignment.sVertical.size()) {
					p.alignment.sVertical		= sVal;
					const char*	sList[] = {"top", "center", "bottom", NULL};

					if(sVal.RetrieveTag(sList) < 0) {
						LOGE("Invalid DocExcel cell alignment value : %s(%s)", sAlignment, sVal.c_str());
						exit(1);
					}
				} else {
					LOGE("Invalid DocExcel cell alignment target : '%s'(%s, must be 'horizontal' or 'vertical')", sAlignment, sTarget.c_str());
					exit(1);
				}
			} else break;
		}
	}

	node.Enumerate("xf", &p, [](DocXML node, void* pPrivate) -> bool {
		private_data* p = (private_data*)pPrivate;
		p->iIndex++;

		if(p->iFont == node.attribute("fontId").as_int() &&
		   p->iFill == node.attribute("fillId").as_int() &&
		   p->iBorder == node.attribute("borderId").as_int() &&
		   (p->iFont != 0) == node.attribute("applyFont").as_int() &&
		   (p->iFill != 0) == node.attribute("applyFill").as_int() &&
		   (p->iBorder != 0) == node.attribute("applyBorder").as_int() &&
		   (p->alignment.bEnable) == node.attribute("applyAlignment").as_int())
		{
			bool	bMatched	= true;

			if(p->alignment.bEnable) {
				DocXML	align	= node.child("alignment");

				if(p->alignment.sHorizontal != align.attribute("horizontal").as_string() ||
				   p->alignment.sVertical != align.attribute("vertical").as_string()) {
					bMatched	= false;
				}
			}

			if(bMatched) {
				p->iRet	= p->iIndex;
				return false;
			}
		}
		return true;
	});

	if(p.iRet < 0) {
		// not matched cell style found, must create it
		p.iRet			= node.Size("xf");
		DocXML	xf		= node.append_child("xf");
		xf.append_attribute("numFmtId")			= 0;
		xf.append_attribute("fontId")			= iStyleFont;
		xf.append_attribute("fillId")			= iStyleFill;
		xf.append_attribute("borderId")			= iStyleBorder;
		xf.append_attribute("xfId")				= 0;

		if(iStyleFont) xf.append_attribute("applyFont")		= 1;

		if(iStyleFill) xf.append_attribute("applyFill")		= 1;

		if(iStyleBorder) xf.append_attribute("applyBorder")	= 1;

		if(p.alignment.bEnable) {
			xf.append_attribute("applyAlignment")	= 1;
			DocXML	align	= xf.append_child("alignment");

			if(p.alignment.sHorizontal.size()) align.append_attribute("horizontal")	= p.alignment.sHorizontal.c_str();

			if(p.alignment.sVertical.size()) align.append_attribute("vertical")	= p.alignment.sVertical.c_str();

			align.append_attribute("wrapText")	= 1;	// preserve 'enter' code
		}

		node.attribute("count")					= p.iRet + 1;
	}

	return p.iRet;
}

bool DocExcel::ReplaceSheetName(DocExcelSheet* pSheet, const char* sName)
{
	if(sName && *sName) {
		cstring	sSheetName(sName);
		sSheetName.ChangeCharsetToUTF8();
		sName	= sSheetName.c_str();
		DocXML	workbook_sheet	= m_Workbook.find_child_by_attribute("name", pSheet->GetName());

		// check name duplication
		for(auto& i : m_SheetMap) if(i.second != pSheet && i.first == sName) {
				LOGE("Sheet name('%s') is duplicated.", sName);
				return false;
			}

		// rename sheet map
		for(auto& i : m_SheetMap) if(i.second == pSheet) {
				m_SheetMap.erase(i.first);
				m_SheetMap[sName]	= pSheet;
				break;
			}

		// rename workbook
		workbook_sheet.attribute("name")		= sName;
		// rename sheet name
		pSheet->m_sName							= sName;
	}

	return true;
}

DocExcelSheet* DocExcel::GetSheet(const char* sName)
{
	cstring			sSheetName(sName);
	sSheetName.ChangeCharsetToUTF8();

	if(m_SheetMap.count(sSheetName.c_str()))
		return m_SheetMap[sSheetName.c_str()];

	return NULL;
}

DocExcelSheet* DocExcel::CreateSheet(const char* sName)
{
	DocExcelSheet*	pSheet		= NULL;
	cstring			sEntryPath;
	int				iSheetID;
	cstring			sSheetID;
	cstring			sSheetName(sName);
	sSheetName.ChangeCharsetToUTF8();

	// automatic sheet name
	if(!sSheetName.size()) {
		for(int i = 1;; i++) {
			sSheetName.Format("Sheet%d", i);

			if(!m_SheetMap.count(sSheetName.c_str())) break;
		}
	}

	for(int i = 1;; i++) {	// set new entry path
		bool	bFound = false;
		sEntryPath.Format("xl/worksheets/sheet%d.xml", i);

		for(auto& i : m_SheetMap) if(sEntryPath == i.second->EntryPath()) {
				bFound	= true;
				break;
			}

		if(!bFound) break;
	}

	if(m_FileReplacements.count(sEntryPath.c_str())) {
		m_FileReplacements.erase(sEntryPath.c_str());
	}

	for(iSheetID = 1;; iSheetID++) {
		sSheetID.Format("%d", iSheetID);

		if(m_Workbook.find_child_by_attribute("sheetId", sSheetID).empty()) break;
	}

	{
		// create sheet file
		DocXML xml	= GetXML(sEntryPath.c_str(), true)->append_child("worksheet");
		xml.append_attribute("xmlns")			= "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
		xml.append_attribute("xmlns:r")			= "http://schemas.openxmlformats.org/officeDocument/2006/relationships";
		xml.append_attribute("xmlns:mc")		= "http://schemas.openxmlformats.org/markup-compatibility/2006";
		xml.append_attribute("xmlns:x14ac")		= "http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac";
		xml.append_attribute("mc:Ignorable")	= "x14ac";
		xml.append_child("dimension").append_attribute("ref")	= "A1:A1";
		{
			DocXML node;
			node	= xml.append_child("sheetViews").append_child("sheetView");
			node.append_attribute("tabSelected")		= "1";
			node.append_attribute("workbookViewId")		= "0";
			node	= xml.append_child("sheetFormatPr");
			node.append_attribute("defaultRowHeight")	= "15";
			node.append_attribute("x14ac:dyDescent")	= "0.25";
			xml.append_child("sheetData");
			node	= xml.append_child("pageMargins");
			node.append_attribute("left")				= "0.7";
			node.append_attribute("right")				= "0.7";
			node.append_attribute("top")				= "0.75";
			node.append_attribute("bottom")				= "0.75";
			node.append_attribute("header")				= "0.3";
			node.append_attribute("footer")				= "0.3";
		}
		// create sheet object
		pSheet	= new DocExcelSheet(sSheetName, sEntryPath, this, iSheetID, xml);
		m_SheetMap[sSheetName.c_str()]	= pSheet;
		// add to contents
		xml				= m_ContentTypes.append_child("Override");
		xml.append_attribute("PartName")		= cstring("/") + sEntryPath.c_str();
		xml.append_attribute("ContentType")		= "application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml";
		// add to workbook
		DocXML	workbook	= m_Workbook.append_child("sheet");
		workbook.append_attribute("name")		= sSheetName;
		workbook.append_attribute("sheetId")	= sSheetID;
		workbook.append_attribute("r:id")		= CreateRelationship(EXCEL_RELATIONSHIP_worksheet, sEntryPath).c_str();
	}

	return pSheet;
}

void DocExcel::DeleteSheet(DocExcelSheet* pSheet)	//@FIXME : not working
{
	if(pSheet) {
		{
			DocXML	child	= m_Workbook.find_child_by_attribute("name", pSheet->GetName());
			m_Workbook.remove_child(child);
		}
		DeleteFile(pSheet->EntryPath());
		m_SheetMap.erase(pSheet->GetName());
		delete pSheet;
	}
}

DocExcelSheet* DocExcel::GetSheetByIndex(int iIndex)
{
	if(iIndex >= 0 && iIndex < GetSheetCount())
		for(auto i = m_SheetMap.begin(); i != m_SheetMap.end(); i++) {
			if(!iIndex) return i->second;

			iIndex--;
		}

	return NULL;
}

int DocExcel::GetStringIndex(const char* sStr, bool bAutoAppend)
{
	int iIndex	= 0;

	if(!sStr) return -1;

	cstring		sData(sStr);
	sData.ChangeCharsetToUTF8();

	// search from shared string table
	for(auto i = m_StringTable.begin(); i != m_StringTable.end(); i++) {
		if((*i) == sData.c_str())
			return iIndex;

		iIndex++;
	}

	// add new string
	if(bAutoAppend) {
		iIndex	= m_StringTable.size();
		m_StringTable.push_back(sData.c_str());
		DocXML	node	= m_SharedStrings.append_child("si");
		node.append_child("t").text().set(sData);
		node	= node.append_child("phoneticPr");
		node.append_attribute("fontId")		= 1;
		node.append_attribute("type")		= "noConversion";
		// set spread size
		size_t	str_count							= m_SharedStrings.Size("si");
		m_SharedStrings.attribute("count")			= str_count;
		m_SharedStrings.attribute("uniqueCount")	= str_count;
	} else {
		iIndex	= -1;
	}

	return iIndex;
}

const char* DocExcel::GetString(int iIndex)
{
	if(iIndex < 0 || iIndex >= m_StringTable.size())
		return NULL;

	return m_StringTable[iIndex].c_str();
}
