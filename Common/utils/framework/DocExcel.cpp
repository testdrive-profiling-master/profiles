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
// Rev.  : 5/29/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "DocExcel.h"
#include "ExcelNumFormat/ExcelNumFormat.h"

typedef struct {
	int			id;
	const char *sFormat;
} DEFAULT_NUMFORMAT;

/* clang-format off */
static DEFAULT_NUMFORMAT	__default_number_formats[] = {
	{EXCEL_NUMBER_FORMAT_GENERAL,					"General"},
	{EXCEL_NUMBER_FORMAT_NUMBER,					"0"},
	{EXCEL_NUMBER_FORMAT_NUMBER_D2,					"0.00"},
	{EXCEL_NUMBER_FORMAT_NUMBER_SEP,				"#,##0"},
	{EXCEL_NUMBER_FORMAT_NUMBER_SEP_D2,				"#,##0.00"},
	{EXCEL_NUMBER_FORMAT_CURRENCY_NEGBRA,			"\"$\"#,##0_);(\"$\"#,##0)"},
	{EXCEL_NUMBER_FORMAT_CURRENCY_NEGBRARED,		"\"$\"#,##0_);[Red](\"$\"#,##0)"},
	{EXCEL_NUMBER_FORMAT_CURRENCY_D2_NEGBRA,		"\"$\"#,##0.00_);(\"$\"#,##0.00)"},
	{EXCEL_NUMBER_FORMAT_CURRENCY_D2_NEGBRARED,		"\"$\"#,##0.00_);[Red](\"$\"#,##0.00)"},
	{EXCEL_NUMBER_FORMAT_PERCENT,					"0%"},
	{EXCEL_NUMBER_FORMAT_PERCENT_D2,				"0.00%"},
	{EXCEL_NUMBER_FORMAT_SCIENTIFIC_D2,				"0.00E+00"},
	{EXCEL_NUMBER_FORMAT_FRACTION_ONEDIG,			"# ?/?"},
	{EXCEL_NUMBER_FORMAT_FRACTION_TWODIG,			"# ??/??"},
	{EXCEL_NUMBER_FORMAT_DATE,						"M/D/YYYY"},
	{EXCEL_NUMBER_FORMAT_DATE_D_MON_YY,				"D-MMM-YY"},
	{EXCEL_NUMBER_FORMAT_DATE_D_MON,				"D-MMM"},
	{EXCEL_NUMBER_FORMAT_DATE_MON_YY,				"MMM-YY"},
	{EXCEL_NUMBER_FORMAT_TIME_HMM_AM,				"h:mm AM/PM"},
	{EXCEL_NUMBER_FORMAT_TIME_HMMSS_AM,				"h:mm:ss AM/PM"},
	{EXCEL_NUMBER_FORMAT_TIME_HMM,					"h:mm"},
	{EXCEL_NUMBER_FORMAT_TIME_HMMSS,				"h:mm:ss"},
	{EXCEL_NUMBER_FORMAT_DATE_TIME,					"M/D/YYYY h:mm"},
	{EXCEL_NUMBER_FORMAT_NUMBER_SEP_NEGBRA,			"_(#,##0_);(#,##0)"},
	{EXCEL_NUMBER_FORMAT_NUMBER_SEP_NEGBRARED,		"_(#,##0_);[Red](#,##0)"},
	{EXCEL_NUMBER_FORMAT_NUMBER_D2_SEP_NEGBRA,		"_(#,##0.00_);(#,##0.00)"},
	{EXCEL_NUMBER_FORMAT_NUMBER_D2_SEP_NEGBRARED,	"_(#,##0.00_);[Red](#,##0.00)"},
	{EXCEL_NUMBER_FORMAT_ACCOUNT,					"_(* #,##0_);_(* (#,##0);_(* \"-\"_);_(@_)"},
	{EXCEL_NUMBER_FORMAT_ACCOUNTCUR,				"_($* #,##0_);_($* (#,##0);_($* \"-\"_);_(@_)"},
	{EXCEL_NUMBER_FORMAT_ACCOUNT_D2,				"_(* #,##0.00_);_(* (#,##0.00);_(* \"-\"??_);_(@_)"},
	{EXCEL_NUMBER_FORMAT_ACCOUNT_D2_CUR,			"_($* #,##0.00_);_($* (#,##0.00);_($* \"-\"??_);_(@_)"},
	{EXCEL_NUMBER_FORMAT_TIME_MMSS,					"mm:ss"},
	{EXCEL_NUMBER_FORMAT_TIME_H0MMSS,				"[h]:mm:ss"},
	{EXCEL_NUMBER_FORMAT_TIME_MMSS0,				"mm:ss.0"},
	{EXCEL_NUMBER_FORMAT_SCIENTIFIC,				"##0.0E+0"},
	{EXCEL_NUMBER_FORMAT_TEXT,						"@"},
	{-1, NULL}
};

static const char*	__excel_numbered_color_codes[] = {
	"000000",
	"FFFFFF",
	"FF0000",
	"00FF00",
	"0000FF",
	"FFFF00",
	"FF00FF",
	"00FFFF",
	"800000",
	"008000",
	"000080",
	"808000",
	"800080",
	"008080",
	"C0C0C0",
	"808080",
	"9999FF",
	"993366",
	"FFFFCC",
	"CCFFFF",
	"660066",
	"FF8080",
	"0066CC",
	"CCCCFF",
	"000080",
	"FF00FF",
	"FFFF00",
	"00FFFF",
	"800080",
	"800000",
	"008080",
	"0000FF",
	"00CCFF",
	"CCFFFF",
	"CCFFCC",
	"FFFF99",
	"99CCFF",
	"FF99CC",
	"CC99FF",
	"FFCC99",
	"3366FF",
	"33CCCC",
	"99CC00",
	"FFCC00",
	"FF9900",
	"FF6600",
	"666699",
	"969696",
	"003366",
	"339966",
	"003300",
	"333300",
	"993300",
	"993366",
	"333399",
	"333333"
};
/* clang-format on */

void Excel_DateToYMD(int iDate, int &iYear, int &iMonth, int &iDay)
{
	// Modified Julian to YMD calculation with an addition of 2415019
	int l  = iDate + 68569 + 2415019;
	int n  = int((4 * l) / 146097);
	l	   = l - int((146097 * n + 3) / 4);
	int i  = int((4000 * (l + 1)) / 1461001);
	l	   = l - int((1461 * i) / 4) + 31;
	int j  = int((80 * l) / 2447);
	iDay   = l - int((2447 * j) / 80);
	l	   = int(j / 11);
	iMonth = j + 2 - (12 * l);
	iYear  = 100 * (n - 49) + i + l;
}

int Excel_YMDToDate(int iYear, int iMonth, int iDay)
{
	// YMD to Modified Julian calculated with an extra subtraction of 2415019.
	return int((1461 * (iYear + 4800 + int((iMonth - 14) / 12))) / 4) +
		   int((367 * (iMonth - 2 - 12 * ((iMonth - 14) / 12))) / 12) -
		   int((3 * (int((iYear + 4900 + int((iMonth - 14) / 12)) / 100))) / 4) + iDay - 2415019 - 32075;
}

void Excel_PosDecode(cstring sPos, int &x, int &y)
{
	const char *s = sPos.c_str();

	// get y
	for (int i = 0; i < sPos.size(); i++) {
		if (isdigit(s[i])) {
			y = atoi(s + i) - 1;
			sPos.erase(i, -1);
			break;
		}
	}

	// get x
	sPos.MakeUpper();
	s = sPos.c_str();
	x = 0;

	for (int i = 0; i < sPos.size(); i++) {
		x *= 26;
		x += (int)(s[i] - 'A');
	}
}

string Excel_PosEncode(int x, int y)
{
	cstring sPos;

	if (x) {
		char ch[2] = "A";

		while (x) {
			ch[0] = (char)(x % 26) + 'A';
			x /= 26;
			sPos.insert(0, ch);
		}
	} else {
		sPos += 'A';
	}

	sPos.AppendFormat("%d", y + 1);
	return sPos.c_str();
}

DocExcelPos::DocExcelPos(void)
{
	x = y = 0;
}
DocExcelPos::DocExcelPos(const char *sPos)
{
	x = y = 0;
	Set(sPos);
}
DocExcelPos::DocExcelPos(int iX, int iY)
{
	x = iX;
	y = iY;
}
DocExcelPos::DocExcelPos(DocExcelSheet *pSheet)
{
	x = y = 0;

	if (pSheet) {
		x = pSheet->GetPosX();
		y = pSheet->GetPosY();
	}
}
DocExcelPos::~DocExcelPos(void) {}
void DocExcelPos::Set(const char *sPos)
{
	if (sPos) {
		Excel_PosDecode(sPos, x, y);
	}
}

string DocExcelPos::Get(void) const
{
	return Excel_PosEncode(x, y);
}

string DocExcelPos::Relative(int iIncreaseX, int iIncreaseY) const
{
	return Excel_PosEncode(x + iIncreaseX, y + iIncreaseY);
}

DocExcelSheet::DocExcelSheet(const char *sName, const char *sEntryPath, DocExcel *pExcel, int iID, pugi::xml_node node)
	: DocXML(node)
{
	m_sName			  = sName;
	m_sEntryPath	  = sEntryPath;
	m_pExcel		  = pExcel;
	m_iID			  = iID;
	m_bRecompute	  = false;
	m_LatestValueType = EXCEL_VALUE_TYPE_NIL;
	{
		// get dimension
		memset(&m_Dimension, 0, sizeof(m_Dimension));
		cstring		sDimension(child("dimension").attribute("ref").value());
		const char *sDelim = ":";
		int			iPos   = 0;
		Excel_PosDecode(sDimension.Tokenize(iPos, sDelim), m_Dimension.sx, m_Dimension.sy);
		Excel_PosDecode(sDimension.Tokenize(iPos, sDelim), m_Dimension.ex, m_Dimension.ey);
		// initialize origin & current position
		m_CurPos.x = m_Dimension.sx;
		m_CurPos.y = m_Dimension.sy;
		m_Origin.x = m_Dimension.sx;
		m_Origin.y = m_Dimension.sy;
	}
	{
		// get data
		m_SheetData = child("sheetData");
	}
}

DocExcelSheet::~DocExcelSheet(void) {}

void DocExcelSheet::SetPosition(const char *sPos)
{
	if (sPos) {
		Excel_PosDecode(sPos, m_Origin.x, m_Origin.y);
		m_CurPos.x = m_Origin.x;
		m_CurPos.y = m_Origin.y;
		m_Row	   = pugi::xml_node(NULL);
		m_Column   = pugi::xml_node(NULL);
	}
}

void DocExcelSheet::SetPos(int x, int y)
{
	if (x >= 0)
		m_CurPos.x = m_Origin.x = x;

	if (y >= 0)
		m_CurPos.y = m_Origin.y = y;

	m_Row	 = pugi::xml_node(NULL);
	m_Column = pugi::xml_node(NULL);
}

DocExcelPos DocExcelSheet::GetPos(void)
{
	DocExcelPos pos(m_CurPos.x - 1, m_CurPos.y);
	return pos;
}

string DocExcelSheet::GetPosition(void)
{
	return Excel_PosEncode(m_CurPos.x - 1, m_CurPos.y);
}

bool DocExcelSheet::GetRow(bool bAutoCreate)
{
	cstring sID;
	m_Column = pugi::xml_node(NULL);

	if (!m_Row.empty())
		m_CurPos.y++;

	m_CurPos.x = m_Origin.x;
	sID.Format("%d", m_CurPos.y + 1);
	m_Row = m_SheetData.find_child_by_attribute("r", sID.c_str());

	if (bAutoCreate && m_Row.empty()) {
		m_Row = m_SheetData.append_child("row");
		m_Row.append_attribute("r").set_value(sID.c_str());

		// fix dimension
		if (m_Dimension.sy > m_CurPos.y)
			m_Dimension.sy = m_CurPos.y;

		if (m_Dimension.ey < m_CurPos.y)
			m_Dimension.ey = m_CurPos.y;
	}

	return !m_Row.empty();
}

bool DocExcelSheet::GetColumn(bool bAutoCreate)
{
	if (m_Row.empty())
		return false;

	cstring sID = Excel_PosEncode(m_CurPos.x, m_CurPos.y);
	m_Column	= m_Row.find_child_by_attribute("r", sID.c_str());
	m_CurPos.x++;

	if (bAutoCreate && m_Column.empty()) {
		m_Column = m_Row.append_child("c");
		m_Column.append_attribute("r").set_value(sID.c_str());
		m_Column.append_child("v");

		// fix dimension
		if (m_Dimension.sx > (m_CurPos.x - 1))
			m_Dimension.sx = m_CurPos.x - 1;

		if (m_Dimension.ex < (m_CurPos.x - 1))
			m_Dimension.ex = m_CurPos.x - 1;
	}

	return !m_Column.empty();
}

bool DocExcelSheet::ValidateColumn(void)
{
	if (m_Row.empty()) {
		if (!GetRow())
			return false;
	}

	if (m_Column.empty()) {
		if (!GetColumn())
			return false;
	}

	return true;
}

double DocExcelSheet::GetColumnWidth(void)
{
	typedef struct {
		int	   iCol;
		double fWidth;
	} __private_data;
	__private_data p	= {m_CurPos.x, 8.5};
	DocXML		   cols = child("cols");
	cols.Enumerate("col", &p, [](DocXML node, void *pPrivate) -> bool {
		__private_data &p	 = *((__private_data *)pPrivate);
		int				iMin = node.attribute("min").as_int(0);
		int				iMax = node.attribute("max").as_int(0);

		if (p.iCol >= iMin && p.iCol <= iMax) {
			p.fWidth = node.attribute("width").as_double(8.5);
			return false;
		}

		return true;
	});
	return p.fWidth;
}

bool DocExcelSheet::IsEmpty(void)
{
	return (m_Row.empty() || m_Column.empty());
}

int DocExcelSheet::GetInt(int iDefault)
{
	int iData = iDefault;

	if (!IsEmpty()) {
		bool bString = !strcmp(m_Column.attribute("t").value(), "s");

		if (bString) {
			const char *sData = m_pExcel->GetString(m_Column.child("v").text().as_int(-1));

			if (sData)
				iData = atoi(sData);
		} else {
			iData = m_Column.child("v").text().as_int(iDefault);
		}
	}

	return iData;
}

double DocExcelSheet::GetDouble(int fDefault)
{
	int fData = fDefault;

	if (!IsEmpty()) {
		bool bString = !strcmp(m_Column.attribute("t").value(), "s");

		if (bString) {
			const char *sData = m_pExcel->GetString(m_Column.child("v").text().as_int(-1));

			if (sData)
				fData = atof(sData);
		} else {
			fData = m_Column.child("v").text().as_double(fDefault);
		}
	}

	return fData;
}

string DocExcelSheet::GetLatestValueTypeString(void)
{
	switch (m_LatestValueType) {
	case EXCEL_VALUE_TYPE_STRING:
		return "string";
	case EXCEL_VALUE_TYPE_BOOLEAN:
		return "boolean";
	case EXCEL_VALUE_TYPE_VALUE:
		return "value";
	}
	return "nil";
}

string DocExcelSheet::GetValue(bool bUseMergedData, bool bIgnoreFormat)
{
	cstring sValue;
	bool	bUseMergedCell = false;

	m_sLatestValueColor.clear();

	if (!IsEmpty()) {
		cstring sType	= m_Column.attribute("t").value();
		cstring sFormat = bIgnoreFormat ? "" : GetNumberFormat();
		sValue			= m_Column.child("v").text().get();

		if (sType == "s") { // string
			sValue = m_pExcel->GetString(atoi(sValue.c_str()));
			sValue.ChangeCharsetToANSI();

			if (!bIgnoreFormat && !sFormat.IsEmpty() && sFormat != "@") {
				excel_number_format::ExcelNumFormat num_fmt(sFormat);
				sValue				= num_fmt.Format(0, sValue.c_str());
				m_sLatestValueColor = num_fmt.Color();
			}
			m_LatestValueType = sValue.IsEmpty() ? EXCEL_VALUE_TYPE_NIL : EXCEL_VALUE_TYPE_STRING;
		} else if (sType == "b") { // boolean
			sValue			  = atoi(sValue.c_str()) ? "TRUE" : "FALSE";
			m_LatestValueType = EXCEL_VALUE_TYPE_BOOLEAN;
		} else if (!sValue.IsEmpty()) { // double
			if (!bIgnoreFormat) {
				if (sFormat.IsEmpty())
					sFormat = "General";
				excel_number_format::ExcelNumFormat num_fmt(sFormat);
				sValue				= num_fmt.Format(atof(sValue.c_str()));
				m_sLatestValueColor = num_fmt.Color();
			}
			m_LatestValueType = EXCEL_VALUE_TYPE_VALUE;
		} else if (bUseMergedData) {
			bUseMergedCell = true;
		}
	} else {
		bUseMergedCell	  = true;
		m_LatestValueType = EXCEL_VALUE_TYPE_NIL;
	}

	if (bUseMergedCell) {
		int delta_x, delta_y, width, height;
		// get merged top data
		if (GetMergeCellPos(delta_x, delta_y, width, height)) {
			if (delta_x || delta_y) {
				int iCurX = GetPosX();
				int iCurY = GetPosY();
				int iOrgX = m_Origin.x;
				int iOrgY = m_Origin.y;
				SetPos(iCurX - delta_x, iCurY - delta_y);
				GetRow();
				GetColumn();
				sValue = GetValue(false, bIgnoreFormat);
				SetPos(iCurX, iCurY);
				GetRow();
				GetColumn();
				// restore original base
				m_Origin.x = iOrgX;
				m_Origin.y = iOrgY;
			}
		}
	}

	if (m_pExcel->IsDocGenStyle()) {
		DocExcelStyle *pStyle = m_pExcel->GetStyleByIndex(m_Column.attribute("s").as_int());
		if (pStyle && pStyle->attribute("applyFont").as_int()) {
			DocXML fonts = pStyle->parent().parent().child("fonts");
			DocXML font	 = fonts.child_by_index("font", pStyle->attribute("fontId").as_int());
			// color
			cstring sTok = font.child("color").attribute("rgb").as_string();
			if (!sTok.IsEmpty()) {
				cstring sColor;
				sTok.erase(0, 2);
				sColor.Format("@<color:%s>", sTok.c_str());
				sValue.insert(0, sColor);
				sValue.Append("@</color>");
			}
			// bold
			if (!font.child("b").empty()) {
				sValue.insert(0, "@<b>");
				sValue.Append("@</b>");
			}
			// italic
			if (!font.child("i").empty()) {
				sValue.insert(0, "@<i>");
				sValue.Append("@</i>");
			}
			// underline
			if (!font.child("u").empty()) {
				sValue.insert(0, "@<u>");
				sValue.Append("@</u>");
			}
		}
	}

	return sValue.c_str();
}

string DocExcelSheet::GetNumberFormat(void)
{
	DocExcelStyle *pStyle = GetStyle();
	if (pStyle && pStyle->attribute("applyNumberFormat").as_bool(false)) {
		string sNumFmt = m_pExcel->StyleNumberFormatString(pStyle->attribute("numFmtId").as_int(-1));
		return sNumFmt.c_str();
	}
	return "";
}

bool DocExcelSheet::IsMergedCell(void)
{
	int delta_x, delta_y, width, height;
	if (GetMergeCellPos(delta_x, delta_y, width, height)) {
		return true;
	}
	return false;
}

struct tm *DocExcelSheet::GetDate(int iDateOverride)
{
	int				 iDate = (iDateOverride == -1) ? GetInt() : iDateOverride;
	static struct tm t;

	if (iDate != -1) {
		memset(&t, 0, sizeof(t));
		Excel_DateToYMD(iDate, t.tm_year, t.tm_mon, t.tm_mday);
		return &t;
	}

	return NULL;
}

bool DocExcelSheet::SetDate(int iYear, int iMonth, int iDay)
{
	int iDate = Excel_YMDToDate(iYear, iMonth, iDay);
	return SetInt(iDate);
}

DocExcelStyle *DocExcelSheet::GetStyle(void)
{
	if (m_pExcel) {
		return m_pExcel->GetStyleByIndex(m_Column.attribute("s").as_int());
	}

	return NULL;
}

bool DocExcelSheet::SetInt(int iValue)
{
	if (!ValidateColumn())
		return false;

	m_Column.remove_attribute("t");

	if (!m_Column.child("v"))
		m_Column.append_child("v");

	m_Column.child("v").text().set(iValue);
	m_Column.remove_child("f");
	m_bRecompute = true;
	return true;
}

bool DocExcelSheet::SetDouble(double fValue)
{
	if (!ValidateColumn())
		return false;

	m_Column.remove_attribute("t");

	if (!m_Column.child("v"))
		m_Column.append_child("v");

	m_Column.child("v").text().set(fValue);
	m_Column.remove_child("f");
	m_bRecompute = true;
	return true;
}

bool DocExcelSheet::SetString(const char *sValue)
{
	if (!sValue || !ValidateColumn())
		return false;

	{
		// set string type
		pugi::xml_attribute attr = m_Column.attribute("t");

		if (attr.empty())
			attr = m_Column.append_attribute("t");

		attr.set_value("s");
	}

	if (!m_Column.child("v"))
		m_Column.append_child("v");

	m_Column.child("v").text().set(m_pExcel->GetStringIndex(sValue));
	m_Column.remove_child("f");
	m_bRecompute = true;
	return true;
}

bool DocExcelSheet::SetFunction(const char *sFunction)
{
	if (!sFunction || !ValidateColumn())
		return false;

	{
		DocXML node = m_Column.child("f");

		if (!node) {
			node						= m_Column.append_child("f");
			node.append_attribute("ca") = 1;
		}

		node.text().set(sFunction);
		m_Column.remove_child("v");
		m_Column.remove_child("t");
		Parent()->AddCalChain(this);
	}
	m_bRecompute = true;
	return true;
}

void DocExcelSheet::SetPane(const char *sPos)
{
	DocXML node = child("sheetViews").child("sheetView");
	node.remove_child("pane");

	if (sPos) {
		int x, y;
		Excel_PosDecode(sPos, x, y);
		node = node.append_child("pane");

		if (x)
			node.append_attribute("xSplit") = x;

		if (y)
			node.append_attribute("ySplit") = y;

		node.append_attribute("topLeftCell") = sPos;
		node.append_attribute("activePane")	 = "bottomLeft";
		node.append_attribute("state")		 = "frozen";
	}
}

bool DocExcelSheet::SetStyle(int iCellStyle)
{
	if (!ValidateColumn())
		return false;

	if (m_Column.attribute("s").as_int(-1) == -1) {
		// no style existed
		m_Column.append_attribute("s") = iCellStyle;
	} else {
		m_Column.attribute("s") = iCellStyle;
	}

	return true;
}

bool DocExcelSheet::SetColumnWidth(double fWidth, bool bBestFit)
{
	if (!ValidateColumn())
		return false;

	cstring sPos;
	sPos.Format("%d", m_CurPos.x);
	// get 'cols'
	DocXML node = child("cols");

	if (!node) { // create if not existed
		node = insert_child_after("cols", child("sheetData").previous_sibling());
	}

	// remove previous 'col'
	DocXML col = node.find_child_by_attribute("min", sPos);

	if (col)
		node.remove_child(col);

	// create new 'col'
	col = node.append_child("col");
	// set attributes
	col.append_attribute("min")	  = sPos;
	col.append_attribute("max")	  = sPos;
	col.append_attribute("width") = fWidth;

	if (bBestFit)
		col.append_attribute("bestFit") = 1;

	col.append_attribute("customWidth") = 1;
	return true;
}

void DocExcelSheet::SetTabColor(unsigned int dwColorRGB)
{
	DocXML	node = child("sheetPr");
	cstring sColor;
	sColor.Format("%08X", dwColorRGB | 0xFF000000);

	if (!node) { // create if not existed
		node = prepend_child("sheetPr");
	}

	DocXML tab_color = node.child("tabColor");

	if (!tab_color)
		tab_color = node.append_child("tabColor");

	if (!tab_color.attribute("rgb"))
		tab_color.append_attribute("rgb");

	tab_color.attribute("rgb") = sColor;
}

bool DocExcelSheet::MergeCells(const char *sBegin, const char *sEnd)
{
	if (!Size("mergeCells")) {
		insert_child_after("mergeCells", child("sheetData")).append_attribute("count") = 0;
	}

	DocXML	node = child("mergeCells");
	cstring sRef;
	sRef.Format("%s:%s", sBegin, sEnd);
	node.append_child("mergeCell").append_attribute("ref") = sRef;
	node.attribute("count")								   = node.Size("mergeCell");
	{
		int x, y;
		Excel_PosDecode(sBegin, x, y);
		ExpandDimension(x, y);
		Excel_PosDecode(sEnd, x, y);
		ExpandDimension(x, y);
	}
	return true;
}

bool DocExcelSheet::GetMergeCellPos(int &delta_x, int &delta_y, int &width, int &height)
{
	typedef struct {
		bool bMerge;
		int	 x, y;
		int	 delta_x, delta_y;
		int	 width, height;
	} __private_data;
	__private_data p	= {false, m_CurPos.x - 1, m_CurPos.y, 0, 0, 1, 1};
	DocXML		   node = child("mergeCells");
	node.Enumerate("mergeCell", &p, [](DocXML node, void *pPrivate) -> bool {
		__private_data &p = *((__private_data *)pPrivate);
		int				sx, sy, ex, ey;
		{
			// get range position
			cstring sRef(node.attribute("ref").as_string());
			int		iPos   = 0;
			cstring sStart = sRef.Tokenize(iPos, ":");
			cstring sEnd   = sRef.Tokenize(iPos, ":");
			Excel_PosDecode(sStart, sx, sy);
			Excel_PosDecode(sEnd, ex, ey);
		}

		if (p.x >= sx && p.x <= ex && p.y >= sy && p.y <= ey) {
			p.bMerge  = true;
			p.width	  = ex - sx + 1;
			p.height  = ey - sy + 1;
			p.delta_x = p.x - sx;
			p.delta_y = p.y - sy;
			return false;
		}

		return true;
	});
	delta_x = p.delta_x;
	delta_y = p.delta_y;
	width	= p.width;
	height	= p.height;
	return p.bMerge;
}

bool DocExcelSheet::HideColumn(bool bHide)
{
	if (!ValidateColumn())
		return false;

	cstring sPos;
	sPos.Format("%d", m_CurPos.x);
	// get 'cols'
	DocXML node = child("cols");

	if (!node) { // create if not existed
		node = insert_child_after("cols", child("sheetData").previous_sibling());
	}

	// find 'col'
	DocXML col = node.find_child_by_attribute("min", sPos);

	// if not existed, create new 'col'
	if (!col) {
		col							= node.append_child("col");
		col.append_attribute("min") = sPos;
		col.append_attribute("max") = sPos;
	}

	// create new 'col'
	col.remove_attribute("hidden");

	if (bHide) {
		col.append_attribute("hidden") = 1;
	}

	return true;
}

void DocExcelSheet::SetProtection(const char *sHash, const char *sSalt, const char *sExceptionRangeList)
{
	DocXML node = child("sheetProtection");

	if (node)
		remove_child(node);

	if (sHash && sSalt) {
		node								   = insert_child_after("sheetProtection", child("sheetData"));
		node.append_attribute("algorithmName") = "SHA-512";
		node.append_attribute("hashValue")	   = sHash;
		node.append_attribute("saltValue")	   = sSalt;
		node.append_attribute("spinCount")	   = 100000;
		node.append_attribute("sheet")		   = 1;
		node.append_attribute("objects")	   = 1;
		node.append_attribute("scenarios")	   = 1;

		if (sExceptionRangeList) {
			int		iPos = 0;
			cstring sExceptions(sExceptionRangeList);
			remove_child("protectedRanges");
			node = insert_child_after("protectedRanges", node);

			for (int i = 1; iPos >= 0; i++) {
				cstring sRange = sExceptions.Tokenize(iPos, " ,;");

				if (iPos > 0) {
					DocXML child					= node.append_child("protectedRange");
					child.append_attribute("sqref") = sRange;
					cstring sName;
					sName.Format("Range%d", i);
					child.append_attribute("name") = sName;
				}
			}
		}
	}
}

bool DocExcelSheet::SetConditionalFormatting(const char *sFomula, int iStyleFormat)
{
	if (!ValidateColumn())
		return false;

	cstring sPos = GetPosition();
	DocXML	node = this->find_child_by_attribute("conditionalFormatting", "sqref", sPos);

	// get condition formatting
	if (!node) {
		node = insert_child_after("conditionalFormatting", child("pageMargins").previous_sibling());
		node.append_attribute("sqref") = sPos;
	}

	// get rule
	if (node.Size("cfRule")) {
		node = child("cfRule");
	} else {
		node							  = node.append_child("cfRule");
		node.append_attribute("type")	  = "expression";
		node.append_attribute("dxfId")	  = 0;
		node.append_attribute("priority") = 1;
	}

	node.attribute("dxfId") = iStyleFormat;

	// get formula
	if (node.Size("formula")) {
		node = node.child("formula");
	} else {
		node = node.append_child("formula");
	}

	node.text() = sFomula;
	return false;
}

void DocExcelSheet::Hide(bool bHide)
{
	DocXML node = m_pExcel->Workbook().find_child_by_attribute("name", GetName());
	node.remove_attribute("state");

	if (bHide) {
		node.append_attribute("state").set_value("hidden");
	}
}

void DocExcelSheet::SetName(const char *sName)
{
	if (m_pExcel)
		m_pExcel->ReplaceSheetName(this, sName);
}

void DocExcelSheet::ExpandDimension(int x, int y)
{
	// fix dimension
	if (m_Dimension.sx > x)
		m_Dimension.sx = x;
	else if (m_Dimension.ex < x)
		m_Dimension.ex = x;

	if (m_Dimension.sy > y)
		m_Dimension.sy = y;
	else if (m_Dimension.ey < y)
		m_Dimension.ey = y;
}

bool DocExcelSheet::OnSave(void)
{
	// apply dimension
	cstring sDimension;
	sDimension.Format("%s:%s", Excel_PosEncode(m_Dimension.sx, m_Dimension.sy).c_str(),
					  Excel_PosEncode(m_Dimension.ex, m_Dimension.ey).c_str());
	child("dimension").attribute("ref") = sDimension;

	// delete value on function cell to recompute all value
	if (m_bRecompute) {
		m_SheetData.EnumerateInDepth("c", NULL, [](DocXML node, void *pPrivate) -> bool {
			if (!node.child("f").empty()) {
				node.remove_child("v");
			}
			return true;
		});
	}

	return false;
}

DocExcelStyle::DocExcelStyle(DocExcel *pExcel, int iID, pugi::xml_node node) : DocXML(node)
{
	m_pExcel = pExcel;
	m_iID	 = iID;
}

DocExcelStyle::~DocExcelStyle() {}

string DocExcelStyle::AlignmentHorizontal(void)
{
	return child("alignment").attribute("horizontal").as_string("left");
}

string DocExcelStyle::BackgroundColor(void)
{
	if (attribute("applyFill").as_int()) {
		int	   id	 = attribute("fillId").as_int();
		DocXML fills = parent().parent().child("fills");
		DocXML fill	 = fills.child_by_index("fill", id);
		if (!fill.empty()) {
			DocXML	pattern	 = fill.child("patternFill");
			cstring sPattern = pattern.attribute("patternType").as_string();

			if (sPattern != "none") {
				if (sPattern.CompareFront("gray")) {
					sPattern.DeleteFront("gray");
					int iFactor = atoi(sPattern.c_str());
					sPattern.Format("%02X%02X%02X", iFactor, iFactor, iFactor);
					return sPattern.c_string();
				}
			}
			if (sPattern == "solid") {
				DocXML	fgColor = pattern.child("fgColor");
				cstring sColor	= fgColor.attribute("rgb").as_string();
				if (!sColor.IsEmpty()) {
					sColor.erase(0, 2);
					return sColor.c_string();
				}
				int iTheme = fgColor.attribute("theme").as_int();
				if (iTheme) {
					double fTint	   = fgColor.attribute("tint").as_double();
					DocXML theme_color = m_pExcel->GetThemeColorByIndex(iTheme);
					sColor			   = theme_color.child("a:srgbClr").attribute("val").as_string();

					if (!sColor.IsEmpty()) {
						uint8_t r, g, b;
						sscanf(sColor.c_str(), "%02X%02X%02X", &r, &g, &b);
						auto make_color = [](uint8_t c, double tint) -> uint8_t {
							double Lum = c;
							if (tint < 0) { // tint operation (refer MS's ColorType.Tint Property)
								Lum = Lum * (1.0 + tint);
							} else {
								Lum = Lum * (1 - tint) + (255 - 255 * (1 - tint));
							}
							return (uint8_t)Lum;
						};
						r = make_color(r, fTint);
						g = make_color(g, fTint);
						b = make_color(b, fTint);
						sColor.Format("%02X%02X%02X", r, g, b);
						return sColor.c_string();
					}
				}
			}
		}
	}
	return "";
}

DocExcel::DocExcel(void)
{
	m_bUseDocGenStyle = false;
}

DocExcel::~DocExcel(void)
{
	OnClose();
}

bool DocExcel::Open(const char *sFileName, bool bUseDocGenStyle)
{
	m_bUseDocGenStyle = bUseDocGenStyle;
	if (!sFileName || !*sFileName) {
		string sTemplateFileName = GetCommonToolPath() + "/bin/codegen/DocExcel_template.xlsx";
		return DocFile::Open(sTemplateFileName.c_str());
	}

	return DocFile::Open(sFileName);
}

bool DocExcel::OnOpen(void)
{
	m_ContentTypes	= GetXML("[Content_Types].xml")->child("Types");
	m_Workbook		= GetXML("xl/workbook.xml")->child("workbook").child("sheets");
	m_Styles		= GetXML("xl/styles.xml")->child("styleSheet");
	m_Relationships = GetXML("xl/_rels/workbook.xml.rels")->child("Relationships");
	m_Theme			= GetXML("xl/theme/theme1.xml")->child("a:theme").child("a:themeElements");

	if (GetXML("xl/sharedStrings.xml")) {
		m_SharedStrings = GetXML("xl/sharedStrings.xml")->child("sst");
	} else {
		m_SharedStrings									= GetXML("xl/sharedStrings.xml", true)->append_child("sst");
		m_SharedStrings.append_attribute("xmlns")		= "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
		m_SharedStrings.append_attribute("count")		= "0";
		m_SharedStrings.append_attribute("uniqueCount") = "0";
		// add t contents
		DocXML xml						 = m_ContentTypes.append_child("Override");
		xml.append_attribute("PartName") = "/xl/sharedStrings.xml";
		xml.append_attribute("ContentType") =
			"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml";
		CreateRelationship(EXCEL_RELATIONSHIP_sharedStrings, "/xl/sharedStrings.xml");
	}

	if (GetXML("xl/calcChain.xml")) {
		m_CalcChain = GetXML("xl/calcChain.xml")->child("calcChain");
	} else {
		m_CalcChain							  = GetXML("xl/calcChain.xml", true)->append_child("calcChain");
		m_CalcChain.append_attribute("xmlns") = "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
		// add t contents
		DocXML xml							= m_ContentTypes.append_child("Override");
		xml.append_attribute("PartName")	= "/xl/calcChain.xml";
		xml.append_attribute("ContentType") = "application/vnd.openxmlformats-package.core-properties+xml";
		CreateRelationship(EXCEL_RELATIONSHIP_calcChain, "/xl/calcChain.xml");
	}

	// make string map
	{
		typedef struct {
			vector<string> *table;
			cstring			sText;
			bool			bUseDocGenStyle;
		} string_table;
		string_table tbl;
		tbl.table			= &m_StringTable;
		tbl.bUseDocGenStyle = m_bUseDocGenStyle;

		m_SharedStrings.EnumerateInDepth("si", &tbl, [](DocXML node, void *pPrivate) -> bool {
			string_table *pTable = (string_table *)pPrivate;
			pTable->sText.clear();

			node.EnumerateInDepth("t", pPrivate, [](DocXML node, void *pPrivate) -> bool {
				string_table *pTable = (string_table *)pPrivate;

				if (pTable->bUseDocGenStyle) {
					DocXML	rPr = node.parent().child("rPr");
					cstring prefix, postfix;
					cstring sTok = rPr.child("color").attribute("rgb").as_string();

					// text color
					if (!sTok.IsEmpty()) {
						sTok.erase(0, 2); // delete alpha
						prefix.AppendFormat("@<color:%s>", sTok.c_str());
						postfix.insert(0, "@</color>");
					}

					// bold
					if (!rPr.child("b").empty()) {
						prefix.Append("@<b>");
						postfix.insert(0, "@</b>");
					}

					// bold
					if (!rPr.child("u").empty()) {
						prefix.Append("@<u>");
						postfix.insert(0, "@</u>");
					}
					// italic
					if (!rPr.child("i").empty()) {
						prefix.Append("@<i>");
						postfix.insert(0, "@</i>");
					}

					pTable->sText.AppendFormat("%s%s%s", prefix.c_str(), node.text().get(), postfix.c_str());
				} else {
					pTable->sText.Append(node.text().get());
				}

				return true;
			});
			pTable->sText.Replace("_x000D_", "", true);
			pTable->table->push_back(pTable->sText.c_str());
			return true;
		});
	}
	// make style list
	{
		DocXML node = m_Styles.child("cellXfs");
		typedef struct {
			DocExcel						  *pExcel;
			vector<unique_ptr<DocExcelStyle>> *pStyleList;
		} STYLE_REF;
		STYLE_REF style_ref = {this, &m_StyleList};
		node.Enumerate("xf", (void *)&style_ref, [](DocXML node, void *pPrivate) -> bool {
			STYLE_REF				 &p = *((STYLE_REF *)pPrivate);
			unique_ptr<DocExcelStyle> pStyle(new DocExcelStyle(p.pExcel, p.pStyleList->size(), node));
			p.pStyleList->push_back(move(pStyle));
			return true;
		});
	}
	// make sheet table map
	{
		typedef struct {
			DocExcel					 *pExel;
			map<string, DocExcelSheet *> *pSheetMap;
			DocXML						 *pRelationships;
		} SHEET_REF;
		SHEET_REF packed = {this, &m_SheetMap, &m_Relationships};
		m_Workbook.Enumerate("sheet", &packed, [](DocXML node, void *pPrivate) -> bool {
			SHEET_REF *p		  = (SHEET_REF *)pPrivate;
			cstring	   sheet_name = node.attribute("name").value();
			int		   sheet_id	  = node.attribute("sheetId").as_int(0);
			cstring	   sheet_entry_name;
			sheet_entry_name.Format("xl/%s",
									p->pRelationships->find_child_by_attribute("Id", node.attribute("r:id").value())
										.attribute("Target")
										.value());
			DocExcelSheet *pNewSheet =
				new DocExcelSheet(sheet_name, sheet_entry_name, p->pExel, sheet_id,
								  p->pExel->GetXML(sheet_entry_name.c_str())->child("worksheet"));
			(*(p->pSheetMap))[sheet_name.c_str()] = pNewSheet;
			return true;
		});
	}
	return true;
}

void DocExcel::OnClose(void)
{
	for (auto &i : m_SheetMap) {
		delete i.second;
	}

	m_SheetMap.clear();
	m_StringTable.clear();
	m_StyleList.clear();
	m_CalcChain		= pugi::xml_node();
	m_SharedStrings = pugi::xml_node();
	m_Relationships = pugi::xml_node();
	m_Workbook		= pugi::xml_node();
	m_ContentTypes	= pugi::xml_node();
}

bool DocExcel::OnSave(void)
{
	if (!m_SharedStrings.Size("si")) { // no string table contents
		DeleteFile("xl/sharedStrings.xml");
	}

	if (!m_CalcChain.Size("c")) { // no functions
		DeleteFile("xl/calcChain.xml");
	}

	for (auto &i : m_SheetMap) {
		i.second->OnSave();
	}

	return true;
}

bool DocExcel::OnDelete(const char *sEntryName)
{
	cstring sPath;
	sPath.Format("/%s", sEntryName);
	m_ContentTypes.remove_child(m_ContentTypes.find_child_by_attribute("PartName", sPath));
	sPath.DeleteFront("/xl/");
	m_Relationships.remove_child(m_Relationships.find_child_by_attribute("Target", sPath));
	return true;
}

string DocExcel::CreateRelationship(EXCEL_RELATIONSHIP type, const char *sEntryPath)
{
	static const char *__relationship_list[EXCEL_RELATIONSHIP_SIZE] = {
		"worksheet", "calcChain", "sharedStrings", "styles", "theme",
	};
	cstring sID;
	cstring sTarget(sEntryPath);
	cstring sType;
	sTarget.DeleteFront("/xl/");
	sTarget.DeleteFront("xl/");
	sType.Format("http://schemas.openxmlformats.org/officeDocument/2006/relationships/%s",
				 __relationship_list[(int)type]);

	for (int i = 1;; i++) {
		sID.Format("rId%d", i);

		if (m_Relationships.find_child_by_attribute("Id", sID).empty()) {
			DocXML node						= m_Relationships.append_child("Relationship");
			node.append_attribute("Id")		= sID;
			node.append_attribute("Type")	= sType;
			node.append_attribute("Target") = sTarget;
			break;
		}
	}

	return sID.c_str();
}

void DocExcel::AddCalChain(DocExcelSheet *pSheet)
{
	if (!pSheet)
		return;

	// find function
	typedef struct {
		cstring sPos;
		cstring sID;
		int		bFound;
	} private_data;
	private_data p;
	p.sID.Format("%d", pSheet->ID());
	p.sPos	 = pSheet->GetPosition();
	p.bFound = false;
	m_CalcChain.Enumerate("c", &p, [](DocXML node, void *pPrivate) -> bool {
		private_data *p = (private_data *)pPrivate;

		if (p->sPos == node.attribute("r").as_string() && p->sID == node.attribute("i").as_string()) {
			p->bFound = true;
			return false;
		}
		return true;
	});

	if (!p.bFound) {
		size_t node_count		   = m_CalcChain.Size("c");
		DocXML node				   = m_CalcChain.append_child("c");
		node.append_attribute("r") = p.sPos;
		node.append_attribute("i") = p.sID;

		if (!node_count)
			node.append_attribute("l") = 1;
	}
}

int DocExcel::GetSheetCount(void)
{
	return m_SheetMap.size();
}

int DocExcel::StyleFont(const char *sFontName, int iFontSize, bool bBold, bool bItalic, unsigned int dwColorARGB)
{
	cstring sFont(sFontName);
	DocXML	node = m_Styles.child("fonts");
	{
		// setup default
		DocXML default_font = node.child("font");

		if (!sFont.size())
			sFont = default_font.child("name").attribute("val").value();

		if (!iFontSize)
			iFontSize = atoi(default_font.child("sz").attribute("val").value());
	}
	typedef struct {
		int			 iIndex;
		cstring		 sFontName;
		int			 iFontSize;
		bool		 bBold;
		bool		 bItalic;
		unsigned int dwColorARGB;
		int			 iRet;
	} private_data;
	private_data p;
	p.iIndex	  = -1;
	p.sFontName	  = sFont;
	p.iFontSize	  = iFontSize;
	p.bBold		  = bBold;
	p.bItalic	  = bItalic;
	p.dwColorARGB = dwColorARGB;
	p.iRet		  = -1;
	node.Enumerate("font", &p, [](DocXML node, void *pPrivate) -> bool {
		private_data *p = (private_data *)pPrivate;
		p->iIndex++;

		if (p->sFontName == node.child("name").attribute("val").as_string() &&
			p->iFontSize == node.child("sz").attribute("val").as_int() && p->bBold == node.Size("b") &&
			p->bItalic == node.Size("i")) {
			if (p->dwColorARGB) {
				cstring sARGB;
				sARGB.Format("%08X", p->dwColorARGB);

				if (sARGB != node.child("color").attribute("rgb").as_string(""))
					return true;
			}

			p->iRet = p->iIndex;
			return false;
		}
		return true;
	});

	if (p.iRet < 0) {
		// not matched font found, must create it
		p.iRet		= node.Size("font");
		DocXML font = node.append_child("font");

		if (bBold)
			font.append_child("b");

		if (bItalic)
			font.append_child("i");

		font.append_child("sz").append_attribute("val") = iFontSize;

		if (dwColorARGB) {
			cstring sARGB;
			sARGB.Format("%08X", dwColorARGB);
			font.append_child("color").append_attribute("rgb") = sARGB;
		} else
			font.append_child("color").append_attribute("theme") = 1;

		font.append_child("name").append_attribute("val")	= sFont.c_str();
		font.append_child("family").append_attribute("val") = 2;
		font.append_child("scheme").append_attribute("val") = "minor";
		node.attribute("count")								= p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleFill(unsigned int dwColorARGB)
{
	DocXML node = m_Styles.child("fills");
	typedef struct {
		int		iIndex;
		cstring sColorValue;
		int		iRet;
	} private_data;
	private_data p;
	p.iIndex = -1;
	p.iRet	 = -1;
	p.sColorValue.Format("%08X", dwColorARGB);
	node.Enumerate("fill", &p, [](DocXML node, void *pPrivate) -> bool {
		private_data *p = (private_data *)pPrivate;
		node			= node.child("patternFill");
		p->iIndex++;

		if (!node.empty() && !strcmp(node.attribute("patternType").as_string(), "solid") &&
			!strcmp(node.child("fgColor").attribute("rgb").as_string(), p->sColorValue)) {
			p->iRet = p->iIndex;
			return false;
		}
		return true;
	});

	if (p.iRet < 0) {
		// not matched fill found, must create it
		p.iRet												 = node.Size("fill");
		DocXML fill											 = node.append_child("fill").append_child("patternFill");
		fill.append_attribute("patternType")				 = "solid";
		fill.append_child("fgColor").append_attribute("rgb") = p.sColorValue;
		fill.append_child("bgColor").append_attribute("indexed") = 64;
		node.attribute("count")									 = p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleBorder(const char *sBorderStyle)
{
	if (!sBorderStyle || !*sBorderStyle)
		return 0;

	typedef struct {
		int		iIndex;
		cstring sLeft, sRight, sTop, sBottom, sDiagonal;
		bool	bDiagonalUp, bDiagonalDown;
		int		iRet;
	} private_data;
	private_data p;
	p.iIndex		= -1;
	p.iRet			= -1;
	p.bDiagonalUp	= false;
	p.bDiagonalDown = false;
	// parsing style
	{
		const char *__sTokensDelim = ",;";
		int			iPos		   = 0;
		cstring		sTokens(sBorderStyle);

		for (; iPos >= 0;) {
			const char *__sDelim = " =\t";
			cstring		sTok	 = sTokens.Tokenize(iPos, __sTokensDelim);

			if (iPos > 0) {
				int		iTokPos = 0;
				cstring sID		= sTok.Tokenize(iTokPos, __sDelim);
				cstring sVal	= sTok.Tokenize(iTokPos, __sDelim);

				if (iPos < 0 || sID == "")
					break;

				if (sID == "left") {
					p.sLeft = sVal;
				} else if (sID == "right") {
					p.sRight = sVal;
				} else if (sID == "top") {
					p.sTop = sVal;
				} else if (sID == "bottom") {
					p.sBottom = sVal;
				} else if (sID == "all") {
					p.sLeft	  = sVal;
					p.sRight  = sVal;
					p.sTop	  = sVal;
					p.sBottom = sVal;
				} else if (sID == "diagonal_up") {
					p.sDiagonal	  = sVal;
					p.bDiagonalUp = true;
				} else if (sID == "diagonal_down") {
					p.sDiagonal		= sVal;
					p.bDiagonalDown = true;
				} else if (sID == "diagonal_all") {
					p.sDiagonal		= sVal;
					p.bDiagonalUp	= true;
					p.bDiagonalDown = true;
				} else {
					LOGE("Invalid border style ID : %s", sID.c_str());
					return -1;
				}
			}
		}
	}
	// searching existed style
	DocXML node = m_Styles.child("borders");
	node.Enumerate("border", &p, [](DocXML node, void *pPrivate) -> bool {
		private_data *p = (private_data *)pPrivate;
		p->iIndex++;

		if ((p->sLeft == node.child("left").attribute("style").value() && !node.child("left").first_child()) &&
			(p->sRight == node.child("right").attribute("style").value() && !node.child("right").first_child()) &&
			(p->sTop == node.child("top").attribute("style").value() && !node.child("top").first_child()) &&
			(p->sBottom == node.child("bottom").attribute("style").value() && !node.child("bottom").first_child()) &&
			(p->bDiagonalUp == node.attribute("diagonalUp").as_bool() && !node.child("diagonalUp").first_child()) &&
			(p->bDiagonalDown == node.attribute("diagonalDown").as_bool() &&
			 !node.child("diagonalDown").first_child()) &&
			(p->sDiagonal == node.child("diagonal").attribute("style").as_string() &&
			 !node.child("diagonal").first_child())) {
			p->iRet = p->iIndex;
			return false;
		}
		return true;
	});

	if (p.iRet < 0) {
		// not matched border found, must create it
		p.iRet		  = node.Size("border");
		DocXML border = node.append_child("border");
		DocXML att;
		att = border.append_child("left");

		if (p.sLeft.size()) {
			att.append_attribute("style")					   = p.sLeft;
			att.append_child("color").append_attribute("auto") = 1;
		}

		att = border.append_child("right");

		if (p.sRight.size()) {
			att.append_attribute("style")					   = p.sRight;
			att.append_child("color").append_attribute("auto") = 1;
		}

		att = border.append_child("top");

		if (p.sTop.size()) {
			att.append_attribute("style")					   = p.sTop;
			att.append_child("color").append_attribute("auto") = 1;
		}

		att = border.append_child("bottom");

		if (p.sBottom.size()) {
			att.append_attribute("style")					   = p.sBottom;
			att.append_child("color").append_attribute("auto") = 1;
		}

		att = border.append_child("diagonal");

		if (p.bDiagonalUp || p.bDiagonalDown) {
			if (p.bDiagonalUp)
				border.append_attribute("diagonalUp") = 1;

			if (p.bDiagonalDown)
				border.append_attribute("diagonalDown") = 1;

			att.append_attribute("style")					   = p.sDiagonal;
			att.append_child("color").append_attribute("auto") = 1;
		}

		node.attribute("count") = p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleNumberFormat(const char *sFormat)
{
	if (sFormat && *sFormat) {
		DocXML node = m_Styles.child("numFmts");

		if (!node) { // add top child node
			node = m_Styles.insert_child_before("numFmts", m_Styles.first_child());
			node.append_attribute("count").set_value(0);
		}

		DocXML child = node.find_child_by_attribute("formatCode", sFormat);

		if (child != NULL) {
			// found
			return child.attribute("numFmtId").as_int();
		} else {
			// search on built-in format first!
			for (int i = 0; __default_number_formats[i].sFormat; i++) {
				if (!strcmp(__default_number_formats[i].sFormat, sFormat)) {
					return __default_number_formats[i].id;
				}
			}

			// add new format
			{
				// add new one
				int		iID = 180; // The first some values should be left blank for default number format.
				cstring sID;

				// find unused ID
				while (1) {
					sID.Format("%d", iID);

					if (!(node.find_child_by_attribute("numFmtId", sID.c_str())))
						break;

					iID++;
				}

				// add
				child								 = node.append_child("numFmt");
				child.append_attribute("numFmtId")	 = iID;
				child.append_attribute("formatCode") = sFormat;
				// fix count
				node.attribute("count") = node.Size("numFmt");
				return iID;
			}
		}
	}

	return 0;
}

string DocExcel::StyleNumberFormatString(int iID)
{
	if (iID >= 0) {
		cstring sNumID;
		DocXML	node = m_Styles.child("numFmts");
		sNumID.Format("%d", iID);
		node = node.find_child_by_attribute("numFmt", "numFmtId", sNumID.c_str());
		if (node) {
			return node.attribute("formatCode").as_string();
		} else { // search on default number format
			for (int i = 0; __default_number_formats[i].sFormat; i++) {
				if (__default_number_formats[i].id == iID)
					return __default_number_formats[i].sFormat;
			}
		}
	}
	return "";
}

int DocExcel::StyleCell(int iStyleFont, int iStyleFill, int iStyleBorder, int iStyleNumberFormat,
						const char *sAlignment)
{
	DocXML node = m_Styles.child("cellXfs");
	typedef struct {
		int iIndex;
		int iFont, iFill, iBorder, iNumberFormat;
		struct {
			bool   bEnable;
			string sHorizontal, sVertical;
		} alignment;
		int iRet;
	} private_data;
	private_data p;
	p.iIndex			= -1;
	p.iRet				= -1;
	p.iFont				= iStyleFont;
	p.iFill				= iStyleFill;
	p.iBorder			= iStyleBorder;
	p.iNumberFormat		= iStyleNumberFormat;
	p.alignment.bEnable = (sAlignment && *sAlignment);

	if (p.alignment.bEnable) { // setup alignment
		const char *sDelim = ",;";
		int			iPos   = 0;
		cstring		sAlign(sAlignment);

		for (;;) {
			cstring sTok = sAlign.Tokenize(iPos, sDelim);

			if (iPos >= 0) {
				const char *sTokDelim = " \"=";
				int			iTokPos	  = 0;
				cstring		sTarget	  = sTok.Tokenize(iTokPos, sTokDelim);
				cstring		sVal	  = sTok.Tokenize(iTokPos, sTokDelim);

				if (iTokPos < 0) {
					LOGE("Invalid DocExcel cell alignment : %s", sAlignment);
					exit(1);
				}

				if (sTarget == "horizontal" && !p.alignment.sHorizontal.size()) {
					p.alignment.sHorizontal = sVal;
					const char *sList[]		= {"left", "center", "right", NULL};

					if (sVal.RetrieveTag(sList) < 0) {
						LOGE("Invalid DocExcel cell alignment value : %s(%s)", sAlignment, sVal.c_str());
						exit(1);
					}
				} else if (sTarget == "vertical" && !p.alignment.sVertical.size()) {
					p.alignment.sVertical = sVal;
					const char *sList[]	  = {"top", "center", "bottom", NULL};

					if (sVal.RetrieveTag(sList) < 0) {
						LOGE("Invalid DocExcel cell alignment value : %s(%s)", sAlignment, sVal.c_str());
						exit(1);
					}
				} else {
					LOGE("Invalid DocExcel cell alignment target : '%s'(%s, must be 'horizontal' or 'vertical')",
						 sAlignment, sTarget.c_str());
					exit(1);
				}
			} else
				break;
		}
	}

	node.Enumerate("xf", &p, [](DocXML node, void *pPrivate) -> bool {
		private_data *p = (private_data *)pPrivate;
		p->iIndex++;

		if (p->iFont == node.attribute("fontId").as_int() && p->iFill == node.attribute("fillId").as_int() &&
			p->iBorder == node.attribute("borderId").as_int() &&
			p->iNumberFormat == node.attribute("numFmtId").as_int() &&
			(p->iFont != 0) == node.attribute("applyFont").as_int() &&
			(p->iFill != 0) == node.attribute("applyFill").as_int() &&
			(p->iBorder != 0) == node.attribute("applyBorder").as_int() &&
			((p->iNumberFormat != 0) ? 1 : 0) == node.attribute("applyNumberFormat").as_int() &&
			(p->alignment.bEnable) == node.attribute("applyAlignment").as_int()) {
			bool bMatched = true;

			if (p->alignment.bEnable) {
				DocXML align = node.child("alignment");

				if (p->alignment.sHorizontal != align.attribute("horizontal").as_string() ||
					p->alignment.sVertical != align.attribute("vertical").as_string()) {
					bMatched = false;
				}
			}

			if (bMatched) {
				p->iRet = p->iIndex;
				return false;
			}
		}
		return true;
	});

	if (p.iRet < 0) {
		// not matched cell style found, must create it
		p.iRet							= node.Size("xf");
		DocXML xf						= node.append_child("xf");
		xf.append_attribute("numFmtId") = iStyleNumberFormat;
		xf.append_attribute("fontId")	= iStyleFont;
		xf.append_attribute("fillId")	= iStyleFill;
		xf.append_attribute("borderId") = iStyleBorder;
		xf.append_attribute("xfId")		= 0;

		if (iStyleFont)
			xf.append_attribute("applyFont") = 1;

		if (iStyleFill)
			xf.append_attribute("applyFill") = 1;

		if (iStyleBorder)
			xf.append_attribute("applyBorder") = 1;

		if (iStyleNumberFormat)
			xf.append_attribute("applyNumberFormat") = 1;

		if (p.alignment.bEnable) {
			xf.append_attribute("applyAlignment") = 1;
			DocXML align						  = xf.append_child("alignment");

			if (p.alignment.sHorizontal.size())
				align.append_attribute("horizontal") = p.alignment.sHorizontal.c_str();

			if (p.alignment.sVertical.size())
				align.append_attribute("vertical") = p.alignment.sVertical.c_str();

			align.append_attribute("wrapText") = 1; // preserve 'enter' code
		}

		node.attribute("count") = p.iRet + 1;
	}

	return p.iRet;
}

int DocExcel::StyleFormat(const char *sFormat)
{
	int iPos = 0;
	typedef struct {
		int		iIndex;
		cstring font_color, bg_color;
		int		iRet;
	} private_data;
	private_data p;
	p.iIndex = -1;
	p.iRet	 = -1;
	// parsing format
	{
		const char *__sTokensDelim = ",;";
		cstring		sTokens(sFormat);

		for (; iPos >= 0;) {
			const char *__sDelim = " =\t";
			cstring		sTok	 = sTokens.Tokenize(iPos, __sTokensDelim);

			if (iPos > 0) {
				int		iTokPos = 0;
				cstring sID		= sTok.Tokenize(iTokPos, __sDelim);
				cstring sVal	= sTok.Tokenize(iTokPos, __sDelim);

				if (iPos < 0 || sID == "")
					break;

				if (sVal.CompareFront("0x"))
					sVal.DeleteFront("0x");

				sVal.MakeUpper();

				if (sID == "fontColor") {
					p.font_color = sVal;
				} else if (sID == "bgColor") {
					p.bg_color = sVal;
				} else {
					LOGE("Invalid Font style ID : %s", sID.c_str());
					return -1;
				}
			}
		}

		if (!p.font_color.size() && !p.bg_color.size()) {
			LOGE("No format style is specified.");
			return -1;
		}
	}
	// get 'dxfs'
	DocXML node = m_Styles.child("dxfs");

	if (!node) {
		const char *sFollowList[] = {"cellStyles", "cellXfs", "cellStyleXfs", "borders", NULL};

		for (int i = 0; sFollowList[i]; i++) {
			node = m_Styles.child("cellStyles");

			if (node) {
				node = m_Styles.insert_child_after("dxfs", node);
				break;
			}
		}

		node.append_attribute("count") = 0;
	}

	// searching existence
	node.Enumerate("dxf", &p, [](DocXML node, void *pPrivate) -> bool {
		private_data *p = (private_data *)pPrivate;
		p->iIndex++;

		if (p->bg_color.size()) {
			if (p->bg_color != node.child("fill").child("patternFill").child("bgColor").attribute("rgb").as_string())
				return true;
		} else if (node.Size("fill"))
			return true;

		if (p->font_color.size()) {
			if (p->font_color != node.child("font").child("color").attribute("rgb").as_string())
				return true;
		} else if (node.Size("font"))
			return true;

		p->iRet = p->iIndex;
		return false;
	});

	// not exist. must create it
	if (p.iRet < 0) {
		p.iRet = node.Size("dxf");
		node   = node.append_child("dxf");

		if (p.font_color.size())
			node.append_child("font").append_child("color").append_attribute("rgb") = p.font_color.c_str();

		if (p.bg_color.size())
			node.append_child("fill").append_child("patternFill").append_child("bgColor").append_attribute("rgb") =
				p.bg_color.c_str();

		node.attribute("count") = node.Size("dxf");
	}

	return p.iRet;
}

bool DocExcel::ReplaceSheetName(DocExcelSheet *pSheet, const char *sName)
{
	if (sName && *sName) {
		cstring sSheetName(sName);
		sSheetName.ChangeCharsetToUTF8();
		sName				  = sSheetName.c_str();
		DocXML workbook_sheet = m_Workbook.find_child_by_attribute("name", pSheet->GetName());

		// check name duplication
		for (auto &i : m_SheetMap)
			if (i.second != pSheet && i.first == sName) {
				LOGE("Sheet name('%s') is duplicated.", sName);
				return false;
			}

		// rename sheet map
		for (auto &i : m_SheetMap)
			if (i.second == pSheet) {
				m_SheetMap.erase(i.first);
				m_SheetMap[sName] = pSheet;
				break;
			}

		// rename workbook
		workbook_sheet.attribute("name") = sName;
		// rename sheet name
		pSheet->m_sName = sName;
	}

	return true;
}

DocExcelSheet *DocExcel::GetSheet(const char *sName)
{
	cstring sSheetName(sName);
	sSheetName.ChangeCharsetToUTF8();

	if (m_SheetMap.count(sSheetName.c_str()))
		return m_SheetMap[sSheetName.c_str()];

	return NULL;
}

DocExcelSheet *DocExcel::CreateSheet(const char *sName)
{
	DocExcelSheet *pSheet = NULL;
	cstring		   sEntryPath;
	int			   iSheetID;
	cstring		   sSheetID;
	cstring		   sSheetName(sName);
	sSheetName.ChangeCharsetToUTF8();

	// automatic sheet name
	if (!sSheetName.size()) {
		for (int i = 1;; i++) {
			sSheetName.Format("Sheet%d", i);

			if (!m_SheetMap.count(sSheetName.c_str()))
				break;
		}
	}

	for (int i = 1;; i++) { // set new entry path
		bool bFound = false;
		sEntryPath.Format("xl/worksheets/sheet%d.xml", i);

		for (auto &i : m_SheetMap)
			if (sEntryPath == i.second->EntryPath()) {
				bFound = true;
				break;
			}

		if (!bFound)
			break;
	}

	if (m_FileReplacements.count(sEntryPath.c_str())) {
		m_FileReplacements.erase(sEntryPath.c_str());
	}

	for (iSheetID = 1;; iSheetID++) {
		sSheetID.Format("%d", iSheetID);

		if (m_Workbook.find_child_by_attribute("sheetId", sSheetID).empty())
			break;
	}

	{
		// create sheet file
		DocXML xml							= GetXML(sEntryPath.c_str(), true)->append_child("worksheet");
		xml.append_attribute("xmlns")		= "http://schemas.openxmlformats.org/spreadsheetml/2006/main";
		xml.append_attribute("xmlns:r")		= "http://schemas.openxmlformats.org/officeDocument/2006/relationships";
		xml.append_attribute("xmlns:mc")	= "http://schemas.openxmlformats.org/markup-compatibility/2006";
		xml.append_attribute("xmlns:x14ac") = "http://schemas.microsoft.com/office/spreadsheetml/2009/9/ac";
		// xml.append_attribute("mc:Ignorable")	= "x14ac";
		xml.append_child("dimension").append_attribute("ref") = "A1:A1";
		{
			DocXML node;
			node = xml.append_child("sheetViews").append_child("sheetView");
			// node.append_attribute("tabSelected")		= "0";
			node.append_attribute("workbookViewId")	  = "0";
			node									  = xml.append_child("sheetFormatPr");
			node.append_attribute("defaultRowHeight") = "15";
			node.append_attribute("x14ac:dyDescent")  = "0.25";
			xml.append_child("sheetData");
			node							= xml.append_child("pageMargins");
			node.append_attribute("left")	= "0.7";
			node.append_attribute("right")	= "0.7";
			node.append_attribute("top")	= "0.75";
			node.append_attribute("bottom") = "0.75";
			node.append_attribute("header") = "0.3";
			node.append_attribute("footer") = "0.3";
		}
		// create sheet object
		pSheet						   = new DocExcelSheet(sSheetName, sEntryPath, this, iSheetID, xml);
		m_SheetMap[sSheetName.c_str()] = pSheet;
		// add to contents
		xml								 = m_ContentTypes.append_child("Override");
		xml.append_attribute("PartName") = cstring("/") + sEntryPath.c_str();
		xml.append_attribute("ContentType") =
			"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml";
		// add to workbook
		DocXML workbook						 = m_Workbook.append_child("sheet");
		workbook.append_attribute("name")	 = sSheetName;
		workbook.append_attribute("sheetId") = sSheetID;
		workbook.append_attribute("r:id")	 = CreateRelationship(EXCEL_RELATIONSHIP_worksheet, sEntryPath).c_str();
	}

	return pSheet;
}

DocExcelStyle *DocExcel::GetStyleByIndex(int iIndex)
{
	if (iIndex >= 0 && iIndex < m_StyleList.size()) {
		return m_StyleList[iIndex].get();
	}

	return NULL;
}

DocXML DocExcel::GetThemeColorByIndex(int iIndex)
{
	DocXML clrScheme = m_Theme.child("a:clrScheme");
	return clrScheme.child_by_index(NULL, iIndex);
}

void DocExcel::DeleteSheet(DocExcelSheet *pSheet) //@FIXME : not working
{
	if (pSheet) {
		{
			LOGI("pSheet->GetName : '%s'", pSheet->GetName());
			DocXML	child	 = m_Workbook.find_child_by_attribute("name", pSheet->GetName());
			int		iSheetID = child.attribute("sheetId").as_int();
			cstring sID		 = child.attribute("r:id").as_string();
			m_Workbook.remove_child(child);
			m_Workbook.Enumerate("sheet", &iSheetID, [](DocXML node, void *pPrivate) -> bool {
				int iSheetID = *(int *)pPrivate;

				if (node.attribute("sheetId").as_int() > iSheetID) {
					node.attribute("sheetId").set_value(node.attribute("sheetId").as_int() - 1);
				}
				return true;
			});
			child = m_Relationships.find_child_by_attribute("Id", sID.c_str());
			m_Relationships.remove_child(child);
		}
		DeleteFile(pSheet->EntryPath());
		m_SheetMap.erase(pSheet->GetName());
		delete pSheet;
	}
}

DocExcelSheet *DocExcel::GetSheetByIndex(int iIndex)
{
	if (iIndex >= 0 && iIndex < GetSheetCount())
		for (auto i = m_SheetMap.begin(); i != m_SheetMap.end(); i++) {
			if (!iIndex)
				return i->second;

			iIndex--;
		}

	return NULL;
}

int DocExcel::GetStringIndex(const char *sStr, bool bAutoAppend)
{
	int iIndex = 0;

	if (!sStr)
		return -1;

	cstring sData(sStr);
	sData.ChangeCharsetToUTF8();

	// search from shared string table
	for (auto i = m_StringTable.begin(); i != m_StringTable.end(); i++) {
		if ((*i) == sData.c_str())
			return iIndex;

		iIndex++;
	}

	// add new string
	if (bAutoAppend) {
		iIndex = m_StringTable.size();
		m_StringTable.push_back(sData.c_str());
		DocXML node = m_SharedStrings.append_child("si");

		if (sData.CompareFront("<r>")) {
			node.AddChildFromBuffer(sData);
		} else
			node.append_child("t").text().set(sData);

		// add tail
		node							= node.append_child("phoneticPr");
		node.append_attribute("fontId") = 1;
		node.append_attribute("type")	= "noConversion";
		// set spread size
		size_t str_count						 = m_SharedStrings.Size("si");
		m_SharedStrings.attribute("count")		 = str_count;
		m_SharedStrings.attribute("uniqueCount") = str_count;
	} else {
		iIndex = -1;
	}

	return iIndex;
}

const char *DocExcel::GetString(int iIndex)
{
	if (iIndex < 0 || iIndex >= m_StringTable.size())
		return NULL;

	return m_StringTable[iIndex].c_str();
}
