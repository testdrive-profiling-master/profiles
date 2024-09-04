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
// Title : TestDrive codegen project
// Rev.  : 9/2/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "Script.h"
#include "ArgTable.h"
#include "DocExcel.h"
#include "DocWord.h"
#include "minGit.h"
#include <tuple>

static bool __bPause_on_error = false;
static bool __bUseTraceBack	  = false;
Script	   *Script::m_pScript = NULL;

int			luaopen_lfs(lua_State *L);

class lua_cstring : public cstring
{
	int iTokenizePos;

public:
	lua_cstring(void)
	{
		iTokenizePos = 0;
	}
	lua_cstring(LuaRef s)
	{
		if (!s.isNil()) {
			m_sStr = s.tostring();
		}

		iTokenizePos = 0;
	}
	lua_cstring(const cstring &s)
	{
		m_sStr		 = s.c_str();
		iTokenizePos = 0;
	}
	lua_cstring(const lua_cstring &s)
	{
		m_sStr		 = s.c_str();
		iTokenizePos = 0;
	}
	~lua_cstring(void) {}

	int Compare(const char *s)
	{
		return cstring::Compare(s);
	}
	bool CompareFront(const char *s) const
	{
		return cstring::CompareFront(s);
	}
	bool CompareBack(const char *s) const
	{
		return cstring::CompareBack(s);
	}
	int RetrieveTag(LuaRef t) const
	{
		if(t.isTable()) {
			for(int i=1;;i++) {
				LuaRef v = t[i];
				if(!v.isString()) break;
				if(m_sStr == (string)v)
					return i;
			}
		}
		return 0;
	}
	bool IsEmpty(void) const
	{
		return cstring::IsEmpty();
	}
	bool CutFront(const char *s, bool bRecursive = false)
	{
		iTokenizePos = 0;
		return cstring::CutFront(s, bRecursive);
	}
	bool CutBack(const char *s, bool bRecursive = false)
	{
		iTokenizePos = 0;
		return cstring::CutBack(s, bRecursive);
	}
	bool DeleteFront(const char *s)
	{
		iTokenizePos = 0;
		return cstring::DeleteFront(s);
	}
	bool DeleteBack(const char *s)
	{
		iTokenizePos = 0;
		return cstring::DeleteBack(s);
	}
	bool DeleteBlock(const char *sExpression, int iPos = 0)
	{
		iTokenizePos = 0;
		return cstring::DeleteBlock(sExpression, iPos);
	}
	void MakeUpper(void)
	{
		cstring::MakeUpper();
	}
	void MakeLower(void)
	{
		cstring::MakeLower();
	}
	bool Replace(const char *sSearch, const char *sReplace, bool bRecursive = false)
	{
		iTokenizePos = 0;
		return cstring::Replace(sSearch, sReplace, bRecursive);
	}
	bool ReplaceVariable(const char *sSearch, const char *sReplace)
	{
		iTokenizePos = 0;
		return cstring::ReplaceVariable(sSearch, sReplace);
	}
	void TrimLeft(const char *sDelim)
	{
		iTokenizePos = 0;
		cstring::TrimLeft(sDelim);
	}
	void TrimRight(const char *sDelim)
	{
		iTokenizePos = 0;
		cstring::TrimRight(sDelim);
	}
	void Trim(const char *sDelim)
	{
		iTokenizePos = 0;
		cstring::Trim(sDelim);
	}
	bool ReadFile(const char *sFile, bool bUseComment = false)
	{
		TextFile f;
		cstring	 sFileName(sFile);
		clear();
		sFileName.ChangeCharsetToANSI();

		if (f.Open(sFileName.c_str())) {
			f.GetAll(*(cstring *)this, bUseComment);
			return true;
		}

		return false;
	}
	int GetTokenizeLuaPos(void) const
	{
		return iTokenizePos;
	}
	void SetTokenizeLuaPos(int iPos)
	{
		iTokenizePos = iPos;
	}
	lua_cstring TokenizeLua(const char *sDelim)
	{
		return cstring::Tokenize(iTokenizePos, sDelim);
	}

	cstring		m_sVariable;
	lua_cstring TokenizeVariable(const char *sExpression)
	{
		cstring sTok;

		if (iTokenizePos >= 0) {
			int iPrevPos = iTokenizePos;
			iTokenizePos = FindVariableString(m_sVariable, sExpression, iTokenizePos);
			sTok		 = m_sStr.c_str() + iPrevPos;

			if (iTokenizePos >= 0) {
				sTok = m_sStr.c_str() + iPrevPos;
				sTok.erase(iTokenizePos - iPrevPos, -1);
			} else {
				m_sVariable.clear();
			}
		} else {
			m_sVariable.clear();
			sTok.clear();
		}

		return sTok;
	}

	lua_cstring GetVariable(void)
	{
		return m_sVariable;
	}

	int Length(void) const
	{
		return cstring::Length();
	}
	void Set(const char *sStr)
	{
		iTokenizePos = 0;
		cstring::Set(sStr);
	}

	int CheckFileExtensionLua(const char *sExtList)
	{
		if (sExtList) {
			const char	*sDelim = " .,;";
			cstring		 sExts(sExtList);
			list<string> sList;
			{
				// make ext list
				int iPos = 0;

				while (1) {
					cstring sExt = sExts.Tokenize(iPos, sDelim);

					if (iPos > 0) {
						sList.push_back(sExt.c_str());
					} else
						break;
				}
			}

			if (sList.size()) {
				const char **sExtPrivateList = new const char *[sList.size() + 1];
				int			 t				 = 0;

				for (auto &i : sList) {
					sExtPrivateList[t] = i.c_str();
					t++;
				}

				sExtPrivateList[t] = NULL;
				int id			   = CheckFileExtension(sExtPrivateList);
				delete[] sExtPrivateList;
				return id;
			}
		}

		return -1;
	}

	bool GetEnvironment(const char *sKey)
	{
		return cstring::GetEnvironment(sKey);
	}

	void SetEnvironment(const char *sKey)
	{
		cstring::SetEnvironment(sKey);
	}

	void FormatDate(const char *sFormat, int iDayShift)
	{
		char	  sTime[1024];
		time_t	  now = time(0);
		struct tm t	  = *localtime(&now);

		if (iDayShift) {
			t.tm_mday += iDayShift;
			mktime(&t);
		}

		strftime(sTime, sizeof(sTime), sFormat, &t);
		m_sStr = sTime;
	}

	bool ChangeCharset(const char *szSrcCharset, const char *szDstCharset)
	{
		return cstring::ChangeCharset(szSrcCharset, szDstCharset);
	}
	bool ChangeCharsetToUTF8(void)
	{
		return cstring::ChangeCharsetToUTF8();
	}
	bool ChangeCharsetToANSI(void)
	{
		return cstring::ChangeCharsetToANSI();
	}

	void Append(const char *sStr)
	{
		cstring::Append(sStr);
	}
	inline const char *c_str(void) const
	{
		return m_sStr.c_str();
	}
	inline char get(int iPos) const
	{
		if (iPos < 0 || iPos >= m_sStr.length())
			return 0;

		return m_sStr.c_str()[iPos];
	}
	inline int find_ch(char ch, int pos = 0)
	{
		return m_sStr.find(ch, pos);
	}
	inline int find(const char *s, LuaRef pos)
	{
		int ipos = 0;
		if (pos.isNumber()) {
			ipos = (int)pos;
		}
		return m_sStr.find(s, ipos);
	}
	inline int rfind(const char *s)
	{
		return m_sStr.rfind(s);
	}
	inline int rfind_ch(char ch)
	{
		return m_sStr.rfind(ch);
	}
	inline int size(void)
	{
		return m_sStr.size();
	}
	inline int length(void)
	{
		return m_sStr.length();
	}
	inline void clear(void)
	{
		m_sStr.clear();
		iTokenizePos = 0;
	}
	inline void erase(int iPos, int iSize)
	{
		m_sStr.erase(iPos, iSize);
	}
	inline void insert(int iPos, const char *s)
	{
		if (iPos < iTokenizePos)
			iTokenizePos += strlen(s);

		m_sStr.insert(iPos, s);
	}
	inline void replace(int iPos, int iSize, const char *s)
	{
		m_sStr.replace(iPos, iSize, s);
	}
};

class lua_DocNode : public DocXML
{
public:
	lua_DocNode(void) {}
	lua_DocNode(xml_node node) : DocXML(node) {}
	lua_DocNode next_sibling(LuaRef name) const
	{
		return name.isString() ? xml_node::next_sibling(name) : xml_node::next_sibling();
	}
	lua_DocNode previous_sibling(LuaRef name) const
	{
		return name.isString() ? xml_node::previous_sibling(name) : xml_node::previous_sibling();
	}
	string path(void)
	{
		return xml_node::path();
	}

	bool Destroy(int iCount)
	{
		xml_node node, next_node;

		while (iCount) {
			node = *this;

			if (!node.empty()) {
				*this = node.next_sibling();
				node.parent().remove_child(node);
				iCount--;
			} else
				return false;
		}
		return true;
	}

	void AddChildFromBuffer(const char *sBuffer)
	{
		DocXML::AddChildFromBuffer(sBuffer);
	}

	void AddChildAfterFromBuffer(lua_DocNode &node, const char *sBuffer)
	{
		pugi::xml_document *pDoc = new pugi::xml_document;

		if (pDoc) {
			pDoc->load_buffer(sBuffer, strlen(sBuffer), pugi::parse_default | pugi::parse_ws_pcdata);

			for (auto child = pDoc->first_child(); !child.empty(); child = child.next_sibling()) {
				insert_copy_after(child, node);
			}

			delete pDoc;
		}
	}

	void AddChildBeforeFromBuffer(lua_DocNode &node, const char *sBuffer)
	{
		pugi::xml_document *pDoc = new pugi::xml_document;

		if (pDoc) {
			pDoc->load_buffer(sBuffer, strlen(sBuffer), pugi::parse_default | pugi::parse_ws_pcdata);

			for (auto child = pDoc->first_child(); !child.empty(); child = child.next_sibling()) {
				insert_copy_before(child, node);
			}

			delete pDoc;
		}
	}

	lua_DocNode append_child(const char *sName)
	{
		return xml_node::append_child(sName);
	}
	lua_DocNode prepend_child(const char *sName)
	{
		return xml_node::prepend_child(sName);
	}
	lua_DocNode insert_child_after(const char *sName, const lua_DocNode &node)
	{
		return xml_node::insert_child_after(sName, node);
	}
	lua_DocNode insert_child_before(const char *sName, const lua_DocNode &node)
	{
		return xml_node::insert_child_before(sName, node);
	}

	lua_DocNode child(const char *name) const
	{
		return xml_node::child(name);
	}

	lua_DocNode child_in_depth(const char *sName, const char *sValue)
	{
		typedef struct {
			DocXML		node;
			const char *sValue;
		} __private;
		__private p;
		p.sValue = sValue;
		EnumerateInDepth(sName, &p, [](DocXML node, void *pPrivate) -> bool {
			__private *p = (__private *)pPrivate;

			if (p->sValue && !strstr(node.text().as_string(), p->sValue))
				return true;
			p->node = node;
			return false;
		});
		return p.node;
	}

	lua_DocNode first_child(void) const
	{
		return xml_node::first_child();
	}

	lua_DocNode last_child(void) const
	{
		return xml_node::last_child();
	}

	lua_DocNode parent(const char *sName) const
	{
		xml_node parent_node = xml_node::parent();

		if (!sName)
			return parent_node;

		while (!parent_node.empty() && strcmp(parent_node.name(), sName)) {
			parent_node = parent_node.parent();
		}

		return parent_node;
	}

	lua_DocNode root(void) const
	{
		return xml_node::root();
	}

	const char *name(void) const
	{
		return xml_node::name();
	}

	bool set_name(const char *sName)
	{
		return xml_node::set_name(sName);
	}

	const char *text(void) const
	{
		return xml_node::text().get();
	}

	lua_DocNode child_by_attribute(const char *child_name, const char *attr_name, const char *attr_value)
	{
		if (child_name && (strstr(child_name, "/") != NULL) && attr_name && attr_value) { // nested search
			typedef struct {
				const char *attr_name;
				const char *attr_value;
				xml_node	node;
			} FIND_CHILD_DEPTH;
			FIND_CHILD_DEPTH p;
			p.attr_name	 = attr_name;
			p.attr_value = attr_value;
			Enumerate(child_name, &p, [](DocXML node, void *pPrivate) -> bool {
				FIND_CHILD_DEPTH &p = *(FIND_CHILD_DEPTH *)pPrivate;
				if (!strcmp(node.attribute(p.attr_name).as_string(), p.attr_value)) {
					p.node = node;
					return false;
				}
				return true;
			});
			return p.node;
		}

		return child_name ? xml_node::find_child_by_attribute(child_name, attr_name, attr_value)
						  : xml_node::find_child_by_attribute(attr_name, attr_value);
	}

	lua_DocNode child_by_text(const char *sChild, const char *sSecondChild, const char *sText)
	{
		return DocXML::child_by_text(sChild, sSecondChild, sText);
	}

	string children_text(const char *sChild)
	{
		return DocXML::children_text(sChild);
	}

	bool set_string(const char *rhs)
	{
		return xml_node::text().set(rhs);
	}
	bool set_int(int rhs)
	{
		return xml_node::text().set(rhs);
	}
	bool set_long(long rhs)
	{
		return xml_node::text().set(rhs);
	}
	bool set_double(double rhs)
	{
		return xml_node::text().set(rhs);
	}
	bool set_float(float rhs)
	{
		return xml_node::text().set(rhs);
	}
	bool set_bool(bool rhs)
	{
		return xml_node::text().set(rhs);
	}
	const char *as_string(void) const
	{
		return xml_node::text().as_string();
	}
	int as_int(void)
	{
		return xml_node::text().as_int();
	}
	double as_double(void)
	{
		return xml_node::text().as_double();
	}
	bool as_bool(void)
	{
		return xml_node::text().as_bool();
	}
	string as_xml(void)
	{
		stringstream ss;
		xml_node::print(ss);
		return ss.str();
	}

	void set_attribute(const char *sName, const char *sValue)
	{
		pugi::xml_attribute attr = xml_node::attribute(sName);

		if (attr.empty())
			attr = xml_node::append_attribute(sName);

		attr.set_name(sValue);
	}
	const char *get_attribute(const char *sName)
	{
		pugi::xml_attribute attr = xml_node::attribute(sName);

		if (attr.empty())
			return NULL;

		return attr.as_string();
	}
};

#include "htmlparser/html_parser.hpp"
class lua_html_parser;
class lua_html_element
{
public:
	lua_html_element(void)
	{
		m_iIndex = 0;
	}
	~lua_html_element(void) {}

	bool IsEmpty(void)
	{
		return (m_iIndex < 0) || (m_iIndex >= m_list.size());
	}
	size_t Size(void)
	{
		return m_list.size();
	}
	bool Head(void)
	{
		m_iIndex = 0;
		return IsEmpty();
	}

	bool Tail(void)
	{
		m_iIndex = m_list.size() - 1;
		return IsEmpty();
	}

	bool Previous(void)
	{
		m_iIndex--;
		return IsEmpty();
	}
	bool Next(void)
	{
		m_iIndex++;
		return IsEmpty();
	}
	string Attribute(const char *sID)
	{
		if (IsEmpty())
			return "";

		return m_list[m_iIndex]->GetAttribute(string(sID));
	}

	string Name(void)
	{
		if (IsEmpty())
			return "";

		return m_list[m_iIndex]->GetName();
	}

	string Value(void)
	{
		if (IsEmpty())
			return "";

		return m_list[m_iIndex]->GetValue();
	}

	string html(void)
	{
		if (IsEmpty())
			return "";

		return m_list[m_iIndex]->html();
	}

	string text(void)
	{
		if (IsEmpty())
			return "";

		cstring s = m_list[m_iIndex]->text();
		s.Replace("&lt;", "<", true);
		s.Replace("&gt;", ">", true);
		s.Replace("&nbsp;", " ", true);
		s.Replace("&amp;", "&", true);
		s.Replace("&quot;", "\"", true);
		return s.c_string();
	}

	lua_html_element *Current(const char *sName)
	{
		if (!IsEmpty())
			return NULL;

		lua_html_element *pElement = new lua_html_element;
		pElement->m_list.push_back(m_list[m_iIndex]);
		return pElement;
	}

	lua_html_element *GetElementByClassName(const char *sName)
	{
		if (IsEmpty())
			return NULL;

		lua_html_element *pElement = new lua_html_element;
		pElement->m_list		   = m_list[m_iIndex]->GetElementByClassName(sName);
		return pElement;
	}

	lua_html_element *GetElementById(const char *sName)
	{
		if (IsEmpty())
			return NULL;

		lua_html_element	   *pElement = new lua_html_element;
		shared_ptr<HtmlElement> pItem	 = m_list[m_iIndex]->GetElementById(sName);

		if (pItem)
			pElement->m_list.push_back(pItem);

		return pElement;
	}

	lua_html_element *GetElementByTagName(const char *sName)
	{
		if (IsEmpty())
			return NULL;

		lua_html_element *pElement = new lua_html_element;
		pElement->m_list		   = m_list[m_iIndex]->GetElementByTagName(sName);
		return pElement;
	}

private:
	friend class lua_html_parser;
	vector<shared_ptr<HtmlElement>> m_list;
	size_t							m_iIndex;
};

class lua_html_parser
{
public:
	lua_html_parser(void) {}
	~lua_html_parser(void) {}

	bool Open(const char *sFileName)
	{
		TextFile f;

		if (f.Open(sFileName)) {
			cstring sHtml;
			f.GetAll(sHtml);

			while (sHtml.Replace("\n  ", "\n "));

			while (sHtml.Replace("\r  ", "\r "));

			HtmlParser parser;
			m_pDoc = parser.Parse(sHtml.c_string());
			return true;
		}

		return false;
	}

	void Parse(const char *sText)
	{
		cstring	   sContents(sText);
		HtmlParser parser;
		m_pDoc = parser.Parse(sContents.c_string());
	}

	string html(void)
	{
		if (m_pDoc) {
			return m_pDoc->html();
		}

		return NULL;
	}

	lua_html_element *GetElementByClassName(const char *sName)
	{
		if (m_pDoc) {
			lua_html_element *pElement = new lua_html_element;
			pElement->m_list		   = m_pDoc->GetElementByClassName(sName);
			return pElement;
		}

		return NULL;
	}

	lua_html_element *SelectElement(const char *sName)
	{
		if (m_pDoc) {
			lua_html_element *pElement = new lua_html_element;
			pElement->m_list		   = m_pDoc->SelectElement(sName);
			return pElement;
		}

		return NULL;
	}

	lua_html_element *GetElementById(const char *sName)
	{
		if (m_pDoc) {
			lua_html_element	   *pElement = new lua_html_element;
			shared_ptr<HtmlElement> pItem	 = m_pDoc->GetElementById(sName);

			if (pItem)
				pElement->m_list.push_back(pItem);

			return pElement;
		}

		return NULL;
	}

	lua_html_element *GetElementByTagName(const char *sName)
	{
		if (m_pDoc) {
			lua_html_element *pElement = new lua_html_element;
			pElement->m_list		   = m_pDoc->GetElementByTagName(sName);
			return pElement;
		}

		return NULL;
	}

private:
	shared_ptr<HtmlDocument> m_pDoc;
};

#include "clipper/SVGBuilder.h"
#include <qrencode.h>
typedef struct {
	double sx, sy, ex, ey;
} Layer_Rect;

class lua_clipper
{
public:
	Paths m_Paths;

	lua_clipper(void) {

	};

	~lua_clipper(void) {

	};

	void AddRect(int x, int y, int width, int height)
	{
		ClipperLib::Clipper c;
		Path				temp;
		temp << IntPoint(x, y) << IntPoint(x + width, y) << IntPoint(x + width, y + height) << IntPoint(x, y + height);
		c.AddPaths(m_Paths, ClipperLib::ptSubject, true);
		c.AddPath(temp, ClipperLib::ptClip, true);
		ClipperLib::Paths result;
		c.Execute(ClipperLib::ctUnion, result, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
		m_Paths = result;
	}

	void AddPolygon(lua_State *L) // (x,y) table, offset x, offset y
	{
		LuaRef v = LuaRef::fromStack(L, 2);

		double fOffsetX = 0;
		double fOffsetY = 0;
		{ // get offset
			LuaRef vOffsetX = LuaRef::fromStack(L, 3);
			LuaRef vOffsetY = LuaRef::fromStack(L, 4);

			if (vOffsetX.isNumber()) {
				fOffsetX = (double)vOffsetX;
			}

			if (vOffsetY.isNumber()) {
				fOffsetY = (double)vOffsetY;
			}
		}

		if (v.isTable()) {
			int	  iSize = v.length();
			Paths temp(1);

			for (int i = 1; i <= iSize; i++) {
				temp[0].push_back(IntPoint((double)v[i][1] + fOffsetX, (double)v[i][2] + fOffsetY));
			}

			ClipperLib::Clipper c;
			c.AddPaths(temp, ClipperLib::ptSubject, true);
			c.AddPaths(m_Paths, ClipperLib::ptClip, true);
			ClipperLib::Paths result;
			c.Execute(ClipperLib::ctUnion, result, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
			m_Paths = result;
		}
	}

	void AddQRCode(lua_State *L) // string, scale, offset x, offset y
	{
		LuaRef str = LuaRef::fromStack(L, 2);
		if (str.isString()) {
			QRcode		  *qrcode = QRcode_encodeString((const char *)str, 0, QR_ECLEVEL_Q, QR_MODE_8, 1);
			int			   width  = (qrcode->width);
			unsigned char *data	  = qrcode->data;

			double		   fScale	= 1;
			double		   fOffsetX = 0;
			double		   fOffsetY = 0;
			{ // get scale & offset
				LuaRef vScale	= LuaRef::fromStack(L, 3);
				LuaRef vOffsetX = LuaRef::fromStack(L, 4);
				LuaRef vOffsetY = LuaRef::fromStack(L, 5);

				if (vScale.isNumber()) {
					fScale = (double)vScale;
				}
				if (vOffsetX.isNumber()) {
					fOffsetX = (double)vOffsetX;
				}

				if (vOffsetY.isNumber()) {
					fOffsetY = (double)vOffsetY;
				}
			}

			{
				for (size_t y = 0; y < width; y++) {
					for (size_t x = 0; x < width; x++, data++) {
						if ((*data & 1)) {
							AddRect((x + fOffsetX) * fScale, (y + fOffsetY) * fScale, fScale, fScale);
						}
					}
				}
			}
			QRcode_free(qrcode);
		}
	}

	bool Compare(lua_clipper *pClipper)
	{
		ClipperLib::Clipper c;
		c.AddPaths(pClipper->m_Paths, ClipperLib::ptSubject, true);
		c.AddPaths(m_Paths, ClipperLib::ptClip, true);
		ClipperLib::Paths result;
		c.Execute(ClipperLib::ctXor, result, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
		return (result.size() == 0);
	}

	bool CompareInclusive(lua_clipper *pClipper)
	{
		ClipperLib::Clipper c;
		c.AddPaths(m_Paths, ClipperLib::ptSubject, true);
		c.AddPaths(pClipper->m_Paths, ClipperLib::ptClip, true);
		ClipperLib::Paths result;
		c.Execute(ClipperLib::ctDifference, result, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
		return (result.size() == 0);
	}

	unordered_map<string, double> GetBound(void)
	{
		unordered_map<string, double> m;
		ClipperLib::Clipper			  c;
		Layer_Rect					  result;
		IntRect						  rc;
		c.Clear();
		c.AddPaths(m_Paths, ClipperLib::ptClip, true);
		rc			= c.GetBounds();
		m["left"]	= rc.left;
		m["top"]	= rc.top;
		m["right"]	= rc.right;
		m["bottom"] = rc.bottom;
		return m;
	}
};

class lua_SVGBuilder
{
	SVGBuilder m_Builder;

public:
	lua_SVGBuilder(void) {}
	~lua_SVGBuilder(void) {}

	void AppPaths(lua_clipper *pPath, LuaRef style)
	{
		if (pPath) {
			SVGBuilder::StyleInfo s;
			if (style.isTable()) {
				if (style["fill"].isNumber()) {
					s.brushClr = (unsigned int)style["fill"];
				}
				if (style["stroke"].isNumber()) {
					s.penClr = (unsigned int)style["stroke"];
				}
				if (style["stroke_width"].isNumber()) {
					s.penWidth = (double)style["stroke_width"];
				}
				if (style["shadow"].isBool()) {
					s.showCoords = (bool)style["shadow"];
				}
				if (style["x"].isBool()) {
					s.offset.x = (bool)style["x"];
				}
				if (style["y"].isBool()) {
					s.offset.x = (bool)style["y"];
				}
			}
			m_Builder.AddPaths(pPath->m_Paths, &s);
		}
	}

	bool ExportToFile(const char *sFileName, LuaRef style)
	{
		double fMargin = 0;
		double fScale  = 10;
		if (style.isTable()) {
			if (style["margin"].isNumber()) {
				fMargin = (double)style["margin"];
			}
			if (style["scale"].isNumber()) {
				fScale = (double)style["scale"];
			}
		}
		return m_Builder.SaveToFile(sFileName, fScale, fMargin);
	}
};

static bool __PostToDocument(const char *sName, int iMsg, const char *sMsg) // To TestDrive document (windows only)
{
	if (!sName)
		return false;

#ifdef WIN32
	cstring sMemName;

	if (sMemName.GetEnvironment("TESTDRIVE_MEMORY")) {
		// find memory
		sMemName += "_";
		sMemName += sName;
		ITestDriverMemory *pMem = TestDriver_GetMemory(sMemName);

		if (!pMem)
			return false;

		// set extra data
		if (sMsg) {
			strcpy((char *)pMem->GetPointer(), sMsg);
		} else {
			((char *)pMem->GetPointer())[0] = 0;
		}

		{
			// send to document
			HWND hWnd = *(HWND *)(pMem->GetConfig()->UserConfig);
			SendMessage(hWnd, WM_USER, iMsg, 0);
			pMem->Release();
		}
	}

	return true;
#else
	return false;
#endif
}

static void __LOGI(const char *sLog)
{
	cstring s(sLog);
	s.ChangeCharsetToANSI();
	LOGI(s.c_str());
}

static void __LOGE(const char *sLog)
{
	cstring s(sLog);
	s.ChangeCharsetToANSI();
	LOGE(s.c_str());
}

static void __LOGW(const char *sLog)
{
	cstring s(sLog);
	s.ChangeCharsetToANSI();
	LOGW(s.c_str());
}

static void __Pause(void)
{
	printf("Press any key to continue...\n");
	fflush(stdout);
	fflush(stdin);
	getchar();
}

static int __AddLuaPackagePath(lua_State *L, const char *path)
{
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");				// get field "path" from table at top of stack (-1)
	std::string cur_path = lua_tostring(L, -1); // grab path string from top of stack
	cur_path.append(";");						// do your path magic here
	cur_path.append(path);
	cur_path.append("/?.lua");
	lua_pop(L, 1);						 // get rid of the string on the stack we just pushed on line 5
	lua_pushstring(L, cur_path.c_str()); // push the new one
	lua_setfield(L, -2, "path");		 // set the field "path" in table at -2 with value at top of stack
	lua_pop(L, 1);						 // get rid of package table from top of stack
	return 0;							 // all done!
}

static string __exec(const char *cmd)
{
	cstring sCmd(cmd);
	char	buffer[128];
	string	result = "";
	sCmd += " 2>&1"; // redirect catch stderr
	FILE *pipe = popen(sCmd.c_str(), "r");

	if (!pipe)
		throw runtime_error("popen() failed!");

	try {
		while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}

	pclose(pipe);
	return result;
}

static int __execute(const char *cmd)
{
	return system(cmd);
}

static bool __IsWindows(void)
{
#ifdef WIN32
	return true;
#else
	return false;
#endif
}

static bool __key_compare(string a, string b)
{
	int i0 = 0, i1 = 0;
	int len0 = a.length(), len1 = b.length();

	for (int i0 = 0, i1 = 0; i0 < len0 && i1 < len1; i0++, i1++) {
		char a_ch  = a[i0];
		char b_ch  = b[i1];
		bool a_num = isdigit(a_ch);
		bool b_num = isdigit(b_ch);

		if (a_num && b_num) {
			cstring n_a, n_b;
			n_a += a_ch;
			n_b += b_ch;

			for (++i0; i0 < len0 && isdigit(a[i0]); i0++) n_a += a[i0];
			for (++i1; i1 < len1 && isdigit(b[i1]); i1++) n_b += b[i1];

			int num_a = atoi(n_a);
			int num_b = atoi(n_b);

			if (num_a != num_b) {
				return num_a < num_b;
			}

		} else if (a_ch != b_ch) {
			return a_ch < b_ch;
		}
	}

	return len0 < len1;
}

Script::Script(void)
{
	m_pLua = luaL_newstate();

	if (m_pLua) {
		luaL_openlibs(m_pLua);
		luaopen_lfs(m_pLua);
		{
			m_sEnvPath = GetCommonToolPath() + "/bin/codegen/";
			m_sEnvPath.Replace("//", "/");
			/*__AddLuaPackagePath(m_pLua, m_sEnvPath.c_str());
			// add common package path
			cstring sPackagePath;
			sPackagePath.Format("package.path = package.path .. ';%slibs/?.lua'", m_sEnvPath.c_str());
			luaL_dostring(m_pLua, sPackagePath);
#ifdef WIN32
			sPackagePath.Format("package.cpath = package.cpath .. ';%slibs/?.dll'", m_sEnvPath.c_str());
#else
			sPackagePath.Format("package.cpath = package.cpath .. ';%slibs/?.so'", m_sEnvPath.c_str());
#endif
			luaL_dostring(m_pLua, sPackagePath);
			*/
		}
		{
			// setup lua
			getGlobalNamespace(m_pLua)
				.beginClass<TextFile>("TextFile")
				.addConstructor<void (*)(void)>()
				.addFunction("Open", &TextFile::Open)
				.addFunction("Create", &TextFile::Create)
				.addFunction("Close", &TextFile::Close)
				.addFunction("Put", &TextFile::Puts)
				.addFunction("Get", &TextFile::Gets)
				.addFunction(
					"GetAll", std::function<string(TextFile * pFile, bool bUseComment)>([](TextFile *pFile, bool bUseComment) -> string {
						cstring sContents;
						pFile->GetAll(sContents, bUseComment);
						return sContents.c_string();
					}))
				.addFunction("LineNumber", &TextFile::LineNumber)
				.addFunction("IsOpen", &TextFile::IsOpen)
				.addFunction("IsEOF", &TextFile::IsEOF)
				.endClass()
				.beginClass<DocExcelPos>("DocExcelPos")
				.addConstructor<void (*)(void)>()
				.addConstructor<void (*)(const char *sPos)>()
				.addConstructor<void (*)(int iX, int iY)>()
				.addConstructor<void (*)(DocExcelSheet *pSheet)>()
				.addProperty("s", &DocExcelPos::Get, &DocExcelPos::Set)
				.addFunction("Relative", &DocExcelPos::Relative)
				.addProperty("x", &DocExcelPos::x, false)
				.addProperty("y", &DocExcelPos::y, false)
				.endClass()
				.beginClass<pugi::xml_node>("xml_node")
				.endClass()
				.deriveClass<lua_DocNode, pugi::xml_node>("DocNode")
				.addConstructor<void (*)(void)>()
				.addConstructor<void (*)(pugi::xml_node node)>()
				.addFunction("next_sibling", &lua_DocNode::next_sibling)
				.addFunction("previous_sibling", &lua_DocNode::previous_sibling)
				.addFunction("path", &lua_DocNode::path)
				.addFunction("Destroy", &lua_DocNode::Destroy)
				.addFunction("AddChildFromBuffer", &lua_DocNode::AddChildFromBuffer)
				.addFunction("AddChildAfterFromBuffer", &lua_DocNode::AddChildAfterFromBuffer)
				.addFunction("AddChildBeforeFromBuffer", &lua_DocNode::AddChildBeforeFromBuffer)
				.addFunction("append_child", &lua_DocNode::append_child)
				.addFunction("empty", std::function<bool(lua_DocNode * node)>([](lua_DocNode *node) -> bool {
								 return node->empty();
							 }))
				.addFunction("prepend_child", &lua_DocNode::prepend_child)
				.addFunction("insert_child_after", &lua_DocNode::insert_child_after)
				.addFunction("insert_child_before", &lua_DocNode::insert_child_before)
				.addFunction("child", &lua_DocNode::child)
				.addFunction("child_in_depth", &lua_DocNode::child_in_depth)
				.addFunction("first_child", &lua_DocNode::first_child)
				.addFunction("last_child", &lua_DocNode::last_child)
				.addFunction("next_sibling", &lua_DocNode::next_sibling)
				.addFunction("previous_sibling", &lua_DocNode::previous_sibling)
				.addFunction("parent", &lua_DocNode::parent)
				.addFunction("root", &lua_DocNode::root)
				.addFunction("name", &lua_DocNode::name)
				.addFunction("set_name", &lua_DocNode::set_name)
				.addFunction("text", &lua_DocNode::text)
				.addFunction("child_by_attribute", &lua_DocNode::child_by_attribute)
				.addFunction("child_by_text", &lua_DocNode::child_by_text)
				.addFunction("children_text", &lua_DocNode::children_text)
				.addFunction("set_string", &lua_DocNode::set_string)
				.addFunction("set_int", &lua_DocNode::set_int)
				.addFunction("set_long", &lua_DocNode::set_long)
				.addFunction("set_double", &lua_DocNode::set_double)
				.addFunction("set_float", &lua_DocNode::set_float)
				.addFunction("set_bool", &lua_DocNode::set_bool)
				.addFunction("as_string", &lua_DocNode::as_string)
				.addFunction("as_int", &lua_DocNode::as_int)
				.addFunction("as_double", &lua_DocNode::as_double)
				.addFunction("as_bool", &lua_DocNode::as_bool)
				.addFunction("as_xml", &lua_DocNode::as_xml)
				.addFunction("set_attribute", &lua_DocNode::set_attribute)
				.addFunction("get_attribute", &lua_DocNode::get_attribute)
				.endClass()
				.beginClass<DocFile>("DocFile")
				.addFunction("Open", &DocFile::Open)
				.addFunction("Save", &DocFile::Save)
				.addFunction("Close", &DocFile::Close)
				.addFunction("IsOpen", &DocFile::IsOpen)
				.addFunction("FileName", &DocFile::FileName)
				.addFunction("ReplaceFile", &DocFile::ReplaceFile)
				.addFunction("DeleteFile", &DocFile::DeleteFile)
				.addFunction(
					"GetNode", std::function<lua_DocNode(DocFile * pFile, const char *sEntryName, bool bForceToCreate)>(
								   [](DocFile *pFile, const char *sEntryName, bool bForceToCreate) -> lua_DocNode {
									   return *(pFile->GetXML(sEntryName, bForceToCreate));
								   }))
				.endClass()
				.deriveClass<DocWord, DocFile>("DocWord")
				.addConstructor<void (*)(void)>()
				.addFunction("Open", &DocWord::Open)
				.addFunction("ReplaceImage", &DocWord::ReplaceImage)
				.addFunction("ReplaceSubDocument", &DocWord::ReplaceSubDocument)
				.addFunction("SetProperty", &DocWord::SetProperty)
				.addFunction("AddMedia", &DocWord::AddMedia)
				.addFunction("AddSubDocument", &DocWord::AddSubDocument)
				.addFunction("AddHyperlink", &DocWord::AddHyperlink)
				.endClass()
				.beginClass<DocExcelStyle>("DocExcelStyle")
				.addFunction("ID", &DocExcelStyle::ID)
				.addFunction("AlignmentHorizontal", &DocExcelStyle::AlignmentHorizontal)
				.addFunction("BackgroundColor", &DocExcelStyle::BackgroundColor)
				.addFunction("TextRotation", &DocExcelStyle::TextRotation)
				.endClass()
				.beginClass<DocExcelSheet>("DocExcelSheet")
				.addFunction("GetPane", &DocExcelSheet::GetPane)
				.addFunction("GetPanePosX", std::function<int(DocExcelSheet * pSheet)>([](DocExcelSheet *pSheet) -> int {
								 int x, y;
								 pSheet->GetPanePos(x, y);
								 return x;
							 }))
				.addFunction("GetPanePosY", std::function<int(DocExcelSheet * pSheet)>([](DocExcelSheet *pSheet) -> int {
								 int x, y;
								 pSheet->GetPanePos(x, y);
								 return y;
							 }))
				.addFunction("SetPosition", &DocExcelSheet::SetPosition)
				.addFunction("SetPos", &DocExcelSheet::SetPos)
				.addFunction("GetPosition", &DocExcelSheet::GetPosition)
				.addFunction("GetPos", &DocExcelSheet::GetPos)
				.addFunction("GetRow", &DocExcelSheet::GetRow)
				.addFunction("GetColumn", &DocExcelSheet::GetColumn)
				.addFunction("GetValue", &DocExcelSheet::GetValue)
				.addFunction("GetLatestValueColor", &DocExcelSheet::GetLatestValueColor)
				.addFunction("GetLatestValueType", &DocExcelSheet::GetLatestValueTypeString)
				.addFunction("GetColumnWidth", &DocExcelSheet::GetColumnWidth)
				.addFunction("IsMergedCell", &DocExcelSheet::IsMergedCell)
				.addFunction("GetMergeCellPos", std::function<string(DocExcelSheet * pSheet)>([](DocExcelSheet *pSheet) -> string {
								 int tx, ty, width, height;

								 if (pSheet->GetMergeCellPos(tx, ty, width, height)) {
									 cstring sRet;
									 sRet.Format("%d,%d,%d,%d", tx, ty, width, height);
									 return sRet.c_str();
								 }
								 return "";
							 }))
				.addFunction("GetStyle", &DocExcelSheet::GetStyle)
				.addFunction("GetNumberFormat", &DocExcelSheet::GetNumberFormat)
				.addFunction("SetString", &DocExcelSheet::SetString)
				.addFunction("SetInt", &DocExcelSheet::SetInt)
				.addFunction("SetDouble", &DocExcelSheet::SetDouble)
				.addFunction("SetFunction", &DocExcelSheet::SetFunction)
				.addFunction("SetPane", &DocExcelSheet::SetPane)
				.addFunction("SetStyle", &DocExcelSheet::SetStyle)
				.addFunction("SetColumnWidth", &DocExcelSheet::SetColumnWidth)
				.addFunction("SetProtection", &DocExcelSheet::SetProtection)
				.addFunction("SetTabColor", &DocExcelSheet::SetTabColor)
				.addFunction("SetConditionalFormatting", &DocExcelSheet::SetConditionalFormatting)
				.addFunction("Parent", &DocExcelSheet::Parent)
				.addFunction("MergeCells", &DocExcelSheet::MergeCells)
				.addFunction("HideColumn", &DocExcelSheet::HideColumn)
				.addFunction("Hide", &DocExcelSheet::Hide)
				.addProperty("name", &DocExcelSheet::GetName, &DocExcelSheet::SetName)
				.endClass()
				.deriveClass<DocExcel, DocFile>("DocExcel")
				.addConstructor<void (*)(void)>()
				.addFunction("Open", &DocExcel::Open)
				.addFunction("GetSheetCount", &DocExcel::GetSheetCount)
				.addFunction("GetSheet", &DocExcel::GetSheet)
				.addFunction("GetSheetByIndex", &DocExcel::GetSheetByIndex)
				.addFunction("GetStyleByIndex", &DocExcel::GetStyleByIndex)
				.addFunction("CreateSheet", &DocExcel::CreateSheet)
				.addFunction("DeleteSheet", &DocExcel::DeleteSheet)
				.addFunction("StyleFont", &DocExcel::StyleFont)
				.addFunction("StyleFill", &DocExcel::StyleFill)
				.addFunction("StyleBorder", &DocExcel::StyleBorder)
				.addFunction("StyleNumberFormat", &DocExcel::StyleNumberFormat)
				.addFunction("StyleCell", &DocExcel::StyleCell)
				.addFunction("StyleFormat", &DocExcel::StyleFormat)
				.endClass()
				.beginClass<lua_cstring>("String")
				.addConstructor<void (*)(LuaRef s)>()
				.addFunction("Replace", &lua_cstring::Replace)
				.addFunction("ReplaceVariable", &lua_cstring::ReplaceVariable)
				.addProperty("s", &lua_cstring::c_str, &lua_cstring::Set)
				.addFunction("Append", &lua_cstring::Append)
				.addFunction("Length", &lua_cstring::Length)
				.addFunction("Compare", &lua_cstring::Compare)
				.addFunction("CompareFront", &lua_cstring::CompareFront)
				.addFunction("CompareBack", &lua_cstring::CompareBack)
				.addFunction("RetrieveTag", &lua_cstring::RetrieveTag)
				.addFunction("IsEmpty", &lua_cstring::IsEmpty)
				.addFunction("CutFront", &lua_cstring::CutFront)
				.addFunction("CutBack", &lua_cstring::CutBack)
				.addFunction("ReadFile", &lua_cstring::ReadFile)
				.addFunction("Trim", &lua_cstring::Trim)
				.addFunction("TrimLeft", &lua_cstring::TrimLeft)
				.addFunction("TrimRight", &lua_cstring::TrimRight)
				.addFunction("Tokenize", &lua_cstring::TokenizeLua)
				.addFunction("TokenizeVariable", &lua_cstring::TokenizeVariable)
				.addFunction("GetVariable", &lua_cstring::GetVariable)
				.addProperty("TokenizePos", &lua_cstring::GetTokenizeLuaPos, &lua_cstring::SetTokenizeLuaPos)
				.addFunction("DeleteFront", &lua_cstring::DeleteFront)
				.addFunction("DeleteBack", &lua_cstring::DeleteBack)
				.addFunction("DeleteBlock", &lua_cstring::DeleteBlock)
				.addFunction("MakeUpper", &lua_cstring::MakeUpper)
				.addFunction("MakeLower", &lua_cstring::MakeLower)
				.addFunction("CheckFileExtension", &lua_cstring::CheckFileExtensionLua)
				.addFunction("GetEnvironment", &lua_cstring::GetEnvironment)
				.addFunction("SetEnvironment", &lua_cstring::SetEnvironment)
				.addFunction("FormatDate", &lua_cstring::FormatDate)
				.addFunction("ChangeCharset", &lua_cstring::ChangeCharset)
				.addFunction("ChangeCharsetToUTF8", &lua_cstring::ChangeCharsetToUTF8)
				.addFunction("ChangeCharsetToANSI", &lua_cstring::ChangeCharsetToANSI)
				.addFunction("insert", &lua_cstring::insert)
				.addFunction("clear", &lua_cstring::clear)
				.addFunction("find", &lua_cstring::find)
				.addFunction("rfind", &lua_cstring::rfind)
				.addFunction("at", &lua_cstring::get)
				.addFunction("erase", &lua_cstring::erase)
				.addFunction("insert", &lua_cstring::insert)
				.endClass()
				.beginClass<lua_clipper>("Clipper")
				.addConstructor<void (*)(void)>()
				.addFunction("AddRect", &lua_clipper::AddRect)
				.addFunction("AddPolygon", &lua_clipper::AddPolygon)
				.addFunction("AddQRCode", &lua_clipper::AddQRCode)
				.addFunction("Compare", &lua_clipper::Compare)
				.addFunction("CompareInclusive", &lua_clipper::CompareInclusive)
				.endClass()
				.beginClass<lua_SVGBuilder>("SVGBuilder")
				.addConstructor<void (*)(void)>()
				.addFunction("AppPaths", &lua_SVGBuilder::AppPaths)
				.addFunction("ExportToFile", &lua_SVGBuilder::ExportToFile)
				.endClass()
				.beginClass<lua_html_element>("HtmlElement")
				.addConstructor<void (*)(void)>()
				.addFunction("IsEmpty", &lua_html_element::IsEmpty)
				.addFunction("Size", &lua_html_element::Size)
				.addFunction("Head", &lua_html_element::Head)
				.addFunction("Tail", &lua_html_element::Tail)
				.addFunction("Next", &lua_html_element::Next)
				.addFunction("Previous", &lua_html_element::Previous)
				.addFunction("Attribute", &lua_html_element::Attribute)
				.addFunction("Name", &lua_html_element::Name)
				.addFunction("Value", &lua_html_element::Value)
				.addFunction("Current", &lua_html_element::Current)
				.addFunction("GetElementByClassName", &lua_html_element::GetElementByClassName)
				.addFunction("GetElementById", &lua_html_element::GetElementById)
				.addFunction("GetElementByTagName", &lua_html_element::GetElementByTagName)
				.addFunction("html", &lua_html_element::html)
				.addFunction("text", &lua_html_element::text)
				.endClass()
				.beginClass<lua_html_parser>("HtmlParser")
				.addConstructor<void (*)(void)>()
				.addFunction("Open", &lua_html_parser::Open)
				.addFunction("Parse", &lua_html_parser::Parse)
				.addFunction("html", &lua_html_parser::html)
				.addFunction("GetElementByClassName", &lua_html_parser::GetElementByClassName)
				.addFunction("SelectElement", &lua_html_parser::SelectElement)
				.addFunction("GetElementById", &lua_html_parser::GetElementById)
				.addFunction("GetElementByTagName", &lua_html_parser::GetElementByTagName)
				.endClass()
				.beginClass<ArgTable>("ArgTable")
				.addConstructor<void (*)(const char *sDescription)>()
				.addFunction("DoParse", &ArgTable::DoParseDefault)
				.addFunction("AddRemark", &ArgTable::AddRemark)
				.addFunction("AddOption", &ArgTable::AddOption)
				.addFunction("AddOptionInt", &ArgTable::AddOptionInt)
				.addFunction("AddOptionDouble", &ArgTable::AddOptionDouble)
				.addFunction("AddOptionString", &ArgTable::AddOptionString)
				.addFunction("AddOptionFile", &ArgTable::AddOptionFile)
				.addFunction("AddOptionFiles", &ArgTable::AddOptionFiles)
				.addFunction("GetOption", &ArgTable::GetOption)
				.addFunction("GetOptionInt", &ArgTable::GetOptionInt)
				.addFunction("GetOptionDouble", &ArgTable::GetOptionDouble)
				.addFunction("GetOptionString", &ArgTable::GetOptionString)
				.addFunction("GetOptionFile", &ArgTable::GetOptionFile)
				.endClass()
				.beginClass<VL>("VL")
				.addFunction("Add", &VL::AddChild)
				.addFunction("Clear", &VL::Clear)
				.addFunction("AddFront", &VL::AddChildFront)
				.addFunction("Pop", &VL::PopChild)
				.addFunction("Remove", &VL::RemoveChild)
				.addFunction("Find", &VL::FindChild)
				.addFunction("Data", &VL::Data)
				.addFunction("Size", &VL::ChildSize)
				.addFunction("ShowAll", &VL::ShowAll)
				.addProperty("name", &VL::GetName, &VL::SetName)
				.addProperty("data", &VL::GetData, &VL::SetData)
				.endClass()
				.deriveClass<VL_String, VL>("VL_String")
				.addProperty("i", &VL_String::m_sData)
				.endClass()
				.deriveClass<VL_Number, VL>("VL_Number")
				.addProperty("i", &VL_Number::m_fData)
				.addFunction("SetFormat", &VL_Number::SetFormat)
				.endClass()
				.beginNamespace("log")
				.addProperty("error_count", &g_log_error_count)
				.addProperty("warning_count", &g_log_warning_count)
				.addProperty("pause_on_error", &__bPause_on_error)
				.endNamespace()
				.beginNamespace("codegen")
				.addFunction("ArgumentSize", &ArgTable::ArgumentSize)
				.addFunction("GetArgument", &ArgTable::GetArgument)
				.addProperty("current_file", &m_sLuaFileName.c_string(), false)
				.addProperty("work_path", &m_sWorkPath.c_string(), false)
				.addProperty("env_path", &m_sEnvPath.c_string(), false)
				.endNamespace()
				.beginClass<minGit>("Git")
				.addConstructor<void (*)(void)>()
				.addFunction("Clone", &minGit::Clone)
				.addProperty("User", &minGit::sUser)
				.addProperty("Password", &minGit::sPassword)
				.endClass()
				.addFunction("AliasString", &VL_String::Alias)
				.addFunction("AliasNumber", &VL_Number::Alias)
				.addFunction("VL_String", &VL_String::Create)
				.addFunction("CreateFromTable", &VL_String::CreateFromTable)
				.addFunction("CreateFromXLSX", &VL_String::CreateFromXLSX)
				.addFunction("CreateToXLSX", &VL_String::CreateToXLSX)
				.addFunction("CreateFileList", &VL_String::CreateFileList)
				.addFunction("VL_Number", &VL_Number::Create)
				.addFunction("RunScript", &Script::__RunScript)
				.addFunction("RunString", &Script::__RunString)
				.addFunction("ArgumentSize", &ArgTable::ArgumentSize)
				.addFunction("GetArgument", &ArgTable::GetArgument)
				.addFunction("PostToDocument", __PostToDocument)
				.addFunction("LOGI", __LOGI)
				.addFunction("LOGE", __LOGE)
				.addFunction("LOGW", __LOGW)
				.addFunction("exec", __exec)
				.addFunction("run", __execute)
				.addFunction("Sleep", std::function<void(unsigned int ms)>([](unsigned int ms) {
#if defined __WIN32__ || defined WIN32 // windows
								 Sleep(ms);
#else // unix variants
								 usleep(ms * 1000);
#endif
							 }))
				.addFunction("Pause", __Pause)
#if defined __WIN32__ || defined WIN32 // windows
				.addFunction(
					"SendMessage", std::function<void(uint64_t hHandle, int msg, int wparam, int lparam)>(
									   [](uint64_t hHandle, int msg, int wparam, int lparam) {
										   SendMessage((HWND)hHandle, msg, wparam, lparam);
									   }))
#endif
				.addFunction("__key_compare", __key_compare)
				.addFunction("IsWindows", __IsWindows)
				.addFunction("GetCommonToolPath", GetCommonToolPath);
		}
	}
}

Script::~Script(void)
{
	if (m_pLua) {
		lua_close(m_pLua);
		m_pLua = NULL;
	}
}

static void __lua_traceback(lua_State *L, const char *sFileName)
{
	luaL_loadbuffer(L, "print(debug.traceback())", 24, sFileName);
	lua_pcall(L, 0, LUA_MULTRET, 0);
}

void Script::EnableTraceBack(bool bEnable)
{
	__bUseTraceBack = bEnable;
}

bool Script::Run(const char *sFileName)
{
	cstring sLuaFilePath(sFileName);
	bool	bRet		= false;
	Script *pPrevScript = m_pScript;
	cstring sPrevWorkPath(m_sWorkPath);
	cstring sPrevLuaFileName(m_sLuaFileName);
	m_pScript = this;
	// get work path
	m_sWorkPath = sLuaFilePath;

	if (m_sWorkPath.find('/') >= 0 || m_sWorkPath.find('\\') >= 0) {
		if (!m_sWorkPath.CutBack("/"))
			m_sWorkPath.CutBack("\\");

		m_sWorkPath.Append("/");
	} else {
		m_sWorkPath.clear();
	}

	if (sLuaFilePath.CompareFront("@")) {
		__bPause_on_error = true;
		sLuaFilePath.erase(0, 1);
	}

	// access check
	if (access(sLuaFilePath, F_OK)) {			 // can't search on file path
		if (!sLuaFilePath.CompareBack(".lua")) { // add a file extension.
			sLuaFilePath += ".lua";
		}

		if (access(sLuaFilePath, F_OK)) {
			cstring sExtraPath;
			// environmental
			sExtraPath.Format("%s%s", m_sEnvPath.c_str(), sLuaFilePath.c_str());
			m_sWorkPath = sExtraPath;

			if (access(sExtraPath, F_OK)) {
				sExtraPath.Format("%s%s", sPrevWorkPath.c_str(), sLuaFilePath.c_str());
				m_sWorkPath = sExtraPath;
			}

			sLuaFilePath = sExtraPath;
		}

		if (m_sWorkPath.find('/') >= 0 || m_sWorkPath.find('\\') >= 0) {
			if (!m_sWorkPath.CutBack("/"))
				m_sWorkPath.CutBack("\\");

			m_sWorkPath.Append("/");
		} else {
			m_sWorkPath.clear();
		}
	}

	// work path (absolute)
	{
		if (m_sWorkPath.IsEmpty())
			m_sWorkPath = ".";

		filesystem::path p(m_sWorkPath.c_str());
		m_sWorkPath = filesystem::absolute(p).string();
		m_sWorkPath.Replace(_T("\\"), _T("/"), true);

		if (!m_sWorkPath.CompareBack(_T("/")))
			m_sWorkPath += "/";
	}
	// loading & run!
	LuaFile f;
	m_sLuaFileName = sLuaFilePath;
	m_sLuaFileName.CutFront(_T("/"));
	m_sLuaFileName.CutFront(_T("\\"));

	if (f.Load(sLuaFilePath.c_str())) {
		cstring sShortenFilePath = sLuaFilePath;
		sShortenFilePath.Replace(m_sEnvPath, "[TOOL_PATH]/");
		cstring sBuffer(f.Buffer(), f.Size()); // automatic change to UTF-8 format
		sBuffer.ChangeCharsetToUTF8();

		if (luaL_loadbuffer(m_pLua, sBuffer.c_str(), sBuffer.Length(), sShortenFilePath.c_str()) || lua_pcall(m_pLua, 0, LUA_MULTRET, 0)) {
			const char *sError = luaL_checkstring(m_pLua, -1);
			LOGE("Error on running script : %s", sError);
			lua_pop(m_pLua, 1); // pop out error message

			if (__bUseTraceBack)
				__lua_traceback(m_pLua, sShortenFilePath.c_str());
		} else {
			bRet = true;
		}
	} else {
		LOGE("Can't open Lua file : %s", sFileName);
	}

	if (__bPause_on_error && !bRet) {
		printf("Press enter key to continue...\n");
		getchar();
	}

	m_pScript	   = pPrevScript;
	m_sWorkPath	   = sPrevWorkPath;
	m_sLuaFileName = sPrevLuaFileName;
	return bRet;
}

bool Script::RunBuffer(const char *sBuffer, const char *sFileName)
{
	bool bRet = false;

	if (sBuffer != NULL && sFileName != NULL) {
		if (luaL_loadbuffer(m_pLua, sBuffer, strlen(sBuffer), sFileName) || lua_pcall(m_pLua, 0, LUA_MULTRET, 0)) {
			const char *sError = luaL_checkstring(m_pLua, -1);
			LOGE("Error on running script : %s", sError);
			lua_pop(m_pLua, 1); // pop out error message

			if (__bUseTraceBack)
				__lua_traceback(m_pLua, sFileName);
		} else {
			bRet = true;
		}

		if (__bPause_on_error && !bRet) {
			printf("Press enter key to continue...\n");
			getchar();
		}
	}

	return bRet;
}

bool Script::__RunScript(const char *sFileName)
{
	if (m_pScript)
		return m_pScript->Run(sFileName);

	return false;
}

bool Script::__RunString(const char *sCodes, const char *sTag)
{
	if (sTag == NULL)
		sTag = "-";

	if (m_pScript) {
		cstring sTarget;
		sTarget.Format("%s(%s)", m_pScript->m_sLuaFileName.c_str(), sTag);
		return m_pScript->RunBuffer(sCodes, sTarget);
	}

	return false;
}
