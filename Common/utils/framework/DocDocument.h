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
// Rev.  : 12/8/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __DOCX_DOCUMENT_H__
#define __DOCX_DOCUMENT_H__
#include "UtilFramework.h"

#include "pugixml/pugixml.hpp"
#include "minizip/zip.h"

using namespace std;

namespace duckx {

typedef unsigned const int formatting_flag;

// text-formatting flags
formatting_flag none				= 0;
formatting_flag bold				= 1 << 0;
formatting_flag italic				= 1 << 1;
formatting_flag underline			= 1 << 2;
formatting_flag strikethrough		= 1 << 3;
formatting_flag superscript			= 1 << 4;
formatting_flag subscript			= 1 << 5;
formatting_flag smallcaps			= 1 << 6;
formatting_flag shadow				= 1 << 7;

// Run contains runs in a paragraph
class Run {
	pugi::xml_node		m_Parent;		// Store the parent node (a paragraph)
	pugi::xml_node		m_Current;		// And store current node also

public:
	Run(void);
	Run(pugi::xml_node, pugi::xml_node);
	void set_parent(pugi::xml_node);
	void set_current(pugi::xml_node);

	string get_text(void) const;
	bool set_text(const string&) const;
	bool set_text(const char*) const;

	Run& next(void);
	bool has_next(void) const;
};

// Paragraph contains a paragraph and stores runs
class Paragraph {
	pugi::xml_node		m_Parent;		// Store parent node (usually the body node)
	pugi::xml_node		m_Current;		// And store current node also
	Run					m_Run;			// A paragraph consists of runs

public:
	Paragraph(void);
	Paragraph(pugi::xml_node, pugi::xml_node);
	void set_parent(pugi::xml_node);
	void set_current(pugi::xml_node);

	Paragraph& next(void);
	bool has_next(void) const;

	Run& runs(void);
	Run& add_run(const string&, duckx::formatting_flag = duckx::none);
	Run& add_run(const char*, duckx::formatting_flag = duckx::none);
	Paragraph& insert_paragraph_after(const string&,
									  duckx::formatting_flag = duckx::none);
};

// TableCell contains one or more paragraphs
class TableCell {
	pugi::xml_node		m_Parent;
	pugi::xml_node		m_Current;
	Paragraph			m_Paragraph;

public:
	TableCell(void);
	TableCell(pugi::xml_node, pugi::xml_node);

	void set_parent(pugi::xml_node);
	void set_current(pugi::xml_node);

	Paragraph& paragraphs(void);

	TableCell& next(void);
	bool has_next(void) const;
};

// TableRow consists of one or more TableCells
class TableRow {
	pugi::xml_node		m_Parent;
	pugi::xml_node		m_Current;
	TableCell			m_Cell;

public:
	TableRow(void);
	TableRow(pugi::xml_node, pugi::xml_node);
	void set_parent(pugi::xml_node);
	void set_current(pugi::xml_node);

	TableCell& cells(void);

	bool has_next(void) const;
	TableRow& next(void);
};

// Table consists of one or more TableRow objects
class Table {
	pugi::xml_node		m_Parent;
	pugi::xml_node		m_Current;
	TableRow			m_Row;

public:
	Table(void);
	Table(pugi::xml_node, pugi::xml_node);
	void set_parent(pugi::xml_node);
	void set_current(pugi::xml_node);

	Table& next(void);
	bool has_next(void) const;

	TableRow& rows(void);
};

}

class DocXML;
typedef bool (*DOCX_NODE_ENUMERATOR_FUNC)(DocXML node, void* pPrivate);
typedef bool (*DOCX_FILE_ENUMERATOR_FUNC)(const char* sFileName, void* pPrivate);

class DocXML : public pugi::xml_node {
public:
	DocXML(void);
	DocXML(pugi::xml_node node);
	virtual ~DocXML(void);

	void Enumerate(const char* sChildPath, void* pPrivate, DOCX_NODE_ENUMERATOR_FUNC func);
	void EnumerateInDepth(const char* sChild, void* pPrivate, DOCX_NODE_ENUMERATOR_FUNC func);
	size_t Size(const char* sChild);
	void AddChildFromBuffer(const char* sBuffer);
};

class DocFile {
public:
	DocFile(void);
	virtual ~DocFile(void);

	virtual void Close(void);
	virtual bool Open(const char* sFileName);
	virtual bool Save(const char* sFileName = NULL);

	pugi::xml_document* GetXML(const char* sEntryName, bool bForceToCreate = false);
	pugi::xml_document* EnumerateFiles(DOCX_FILE_ENUMERATOR_FUNC func, void* pPrivate);
	bool ReplaceFile(const char* sEntryName, const char* sFileName, const char* sNewEntryName = NULL);
	bool DeleteFile(const char* sEntryName);

	inline bool IsOpen(void) {
		return (m_pZipFile != NULL);
	}

	inline const char* FileName(void) {
		return m_sDocFileName.c_str();
	}

protected:
	virtual bool OnOpen(void);
	virtual void OnClose(void);
	virtual bool OnSave(void);
	virtual bool OnDelete(const char* sEntryName);

	string								m_sDocFileName;
	zip_t*								m_pZipFile;
	map<string, pugi::xml_document*>	m_Documents;
	map<string, string>					m_FileReplacements;
};

#endif//__DOCX_DOCUMENT_H__
