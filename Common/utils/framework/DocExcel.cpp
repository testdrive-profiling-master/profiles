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
// Rev.  : 7/30/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "DocExcel.h"

static void __ExcelPos(cstring sPos, int& x, int& y)
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

static string __ExcelPosString(int x, int y)
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

DocExcelSheet::DocExcelSheet(const char* sName, DocExcel* pExcel, pugi::xml_node node) : DocXML(node)
{
	m_sName			= sName;
	m_pExcel		= pExcel;
	m_bRecompute	= false;
	{
		// get dimension
		memset(&m_Dimension, 0, sizeof(m_Dimension));
		cstring	sDimension(child("dimension").attribute("ref").value());
		const char* sDelim	= ":";
		int		iPos	= 0;
		__ExcelPos(sDimension.Tokenize(iPos, sDelim), m_Dimension.sx, m_Dimension.sy);
		__ExcelPos(sDimension.Tokenize(iPos, sDelim), m_Dimension.ex, m_Dimension.ey);
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
		__ExcelPos(sPos, m_Origin.x, m_Origin.y);
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
	return __ExcelPosString(m_CurPos.x, m_CurPos.y);
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

	cstring	sID	= __ExcelPosString(m_CurPos.x, m_CurPos.y);
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
	}
	m_bRecompute	= true;
	return true;
}

void DocExcelSheet::OnSave(void)
{
	// apply dimension
	cstring sDimension;
	sDimension.Format("%s:%s",
					  __ExcelPosString(m_Dimension.sx, m_Dimension.sy).c_str(),
					  __ExcelPosString(m_Dimension.ex, m_Dimension.ey).c_str());
	child("dimension").attribute("ref").set_value(sDimension.c_str());

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
}

DocExcel::DocExcel(void)
{
}

DocExcel::~DocExcel(void)
{
	OnClose();
}

bool DocExcel::OnOpen(void)
{
	m_Sheets			= GetXML("xl/workbook.xml")->child("workbook").child("sheets");
	m_Relationships		= GetXML("xl/_rels/workbook.xml.rels")->child("Relationships");
	m_SharedStrings		= GetXML("xl/sharedStrings.xml")->child("sst");
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
		m_Sheets.Enumerate("sheet", &packed, [](DocXML node, void* pPrivate) -> bool {
			SHEET_REF*	p	= (SHEET_REF*)pPrivate;
			string		sheet_name		= node.attribute("name").value();
			cstring		sheet_entry_name;
			sheet_entry_name.Format("xl/%s", p->pRelationships->find_child_by_attribute("Id", node.attribute("r:id").value()).attribute("Target").value());
			DocExcelSheet*	pNewSheet				= new DocExcelSheet(sheet_name.c_str(), p->pExel, p->pExel->GetXML(sheet_entry_name.c_str())->child("worksheet"));

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
}

bool DocExcel::OnSave(void)
{
	for(auto& i : m_SheetMap) {
		if(i.first != i.second->GetName()) {	// name is changed
			m_Sheets.find_child_by_attribute("name", i.first.c_str()).attribute("name").set_value(i.second->GetName());
		}

		i.second->OnSave();
	}

	return true;
}

int DocExcel::GetSheetCount(void)
{
	return m_SheetMap.size();
}

DocExcelSheet* DocExcel::GetSheet(const char* sSheetName)
{
	if(m_SheetMap.count(sSheetName))
		return m_SheetMap[sSheetName];

	return NULL;
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

	// search from shared string table
	for(auto i = m_StringTable.begin(); i != m_StringTable.end(); i++) {
		if((*i) == sStr)
			return iIndex;

		iIndex++;
	}

	// add new string
	if(bAutoAppend) {
		iIndex	= m_StringTable.size();
		m_StringTable.push_back(sStr);
		DocXML	node	= m_SharedStrings.append_child("si");
		node.append_child("t").text().set(sStr);
		node	= node.append_child("phoneticPr");
		node.append_attribute("fontId").set_value("1");
		node.append_attribute("type").set_value("noConversion");
		// set spread size
		cstring sNum;
		sNum.Format("%lld", m_StringTable.size());
		m_SharedStrings.attribute("uniqueCount").set_value(sNum.c_str());
		//sNum.Format("%lld", m_StringTable.size() + 1);
		m_SharedStrings.attribute("count").set_value(sNum.c_str());
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
