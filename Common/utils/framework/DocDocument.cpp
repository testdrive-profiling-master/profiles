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
#include "DocDocument.h"

#include <cctype>

// Hack on pugixml
// We need to write xml to std string (or char *)
// So overload the write function
struct xml_string_writer : pugi::xml_writer {
	string result;

	virtual void write(const void* data, size_t size) {
		result.append(static_cast<const char*>(data), size);
	}
};

duckx::Run::Run(void) {}

duckx::Run::Run(pugi::xml_node parent, pugi::xml_node current)
{
	set_parent(parent);
	set_current(current);
}

void duckx::Run::set_parent(pugi::xml_node node)
{
	m_Parent = node;
	m_Current = m_Parent.child("w:r");
}

void duckx::Run::set_current(pugi::xml_node node)
{
	m_Current = node;
}

string duckx::Run::get_text(void) const
{
	return m_Current.child("w:t").text().get();
}

bool duckx::Run::set_text(const string& text) const
{
	return m_Current.child("w:t").text().set(text.c_str());
}

bool duckx::Run::set_text(const char* text) const
{
	return m_Current.child("w:t").text().set(text);
}

duckx::Run& duckx::Run::next(void)
{
	m_Current = m_Current.next_sibling();
	return *this;
}

bool duckx::Run::has_next(void) const
{
	return m_Current != 0;
}

// Table cells
duckx::TableCell::TableCell(void) {}

duckx::TableCell::TableCell(pugi::xml_node parent, pugi::xml_node current)
{
	set_parent(parent);
	set_current(current);
}

void duckx::TableCell::set_parent(pugi::xml_node node)
{
	m_Parent	= node;
	m_Current	= m_Parent.child("w:tc");
	m_Paragraph.set_parent(m_Current);
}

void duckx::TableCell::set_current(pugi::xml_node node)
{
	m_Current	= node;
}

bool duckx::TableCell::has_next(void) const
{
	return m_Current != 0;
}

duckx::TableCell& duckx::TableCell::next(void)
{
	m_Current = m_Current.next_sibling();
	return *this;
}

duckx::Paragraph& duckx::TableCell::paragraphs(void)
{
	m_Paragraph.set_parent(m_Current);
	return m_Paragraph;
}

// Table rows
duckx::TableRow::TableRow(void) {}

duckx::TableRow::TableRow(pugi::xml_node parent, pugi::xml_node current)
{
	set_parent(parent);
	set_current(current);
}

void duckx::TableRow::set_parent(pugi::xml_node node)
{
	m_Parent	= node;
	m_Current	= m_Parent.child("w:tr");
	m_Cell.set_parent(m_Current);
}

void duckx::TableRow::set_current(pugi::xml_node node)
{
	m_Current	= node;
}

duckx::TableRow& duckx::TableRow::next(void)
{
	m_Current	= m_Current.next_sibling();
	return *this;
}

duckx::TableCell& duckx::TableRow::cells(void)
{
	m_Cell.set_parent(m_Current);
	return m_Cell;
}

bool duckx::TableRow::has_next(void) const
{
	return m_Current != 0;
}

// Tables
duckx::Table::Table(void) {}

duckx::Table::Table(pugi::xml_node parent, pugi::xml_node current)
{
	this->set_parent(parent);
	this->set_current(current);
}

void duckx::Table::set_parent(pugi::xml_node node)
{
	m_Parent	= node;
	m_Current	= m_Parent.child("w:tbl");
	m_Row.set_parent(m_Current);
}

bool duckx::Table::has_next(void) const
{
	return m_Current != 0;
}

duckx::Table& duckx::Table::next(void)
{
	m_Current	= m_Current.next_sibling();
	m_Row.set_parent(m_Current);
	return *this;
}

void duckx::Table::set_current(pugi::xml_node node)
{
	m_Current	= node;
}

duckx::TableRow& duckx::Table::rows(void)
{
	m_Row.set_parent(m_Current);
	return m_Row;
}

duckx::Paragraph::Paragraph(void) {}

duckx::Paragraph::Paragraph(pugi::xml_node parent, pugi::xml_node current)
{
	set_parent(parent);
	set_current(current);
}

void duckx::Paragraph::set_parent(pugi::xml_node node)
{
	m_Parent	= node;
	m_Current	= m_Parent.child("w:p");
	m_Run.set_parent(m_Current);
}

void duckx::Paragraph::set_current(pugi::xml_node node)
{
	m_Current = node;
}

duckx::Paragraph& duckx::Paragraph::next(void)
{
	m_Current = m_Current.next_sibling();
	m_Run.set_parent(m_Current);
	return *this;
}

bool duckx::Paragraph::has_next(void) const
{
	return m_Current != 0;
}

duckx::Run& duckx::Paragraph::runs(void)
{
	m_Run.set_parent(m_Current);
	return m_Run;
}

duckx::Run& duckx::Paragraph::add_run(const string& text, duckx::formatting_flag f)
{
	return add_run(text.c_str(), f);
}

duckx::Run& duckx::Paragraph::add_run(const char* text, duckx::formatting_flag f)
{
	// Add new run
	pugi::xml_node new_run	= m_Current.append_child("w:r");
	// Insert meta to new run
	pugi::xml_node meta		= new_run.append_child("w:rPr");

	if(f & duckx::bold)
		meta.append_child("w:b");

	if(f & duckx::italic)
		meta.append_child("w:i");

	if(f & duckx::underline)
		meta.append_child("w:u").append_attribute("w:val").set_value("single");

	if(f & duckx::strikethrough)
		meta.append_child("w:strike")
		.append_attribute("w:val")
		.set_value("true");

	if(f & duckx::superscript)
		meta.append_child("w:vertAlign")
		.append_attribute("w:val")
		.set_value("superscript");
	else if(f & duckx::subscript)
		meta.append_child("w:vertAlign")
		.append_attribute("w:val")
		.set_value("subscript");

	if(f & duckx::smallcaps)
		meta.append_child("w:smallCaps")
		.append_attribute("w:val")
		.set_value("true");

	if(f & duckx::shadow)
		meta.append_child("w:shadow")
		.append_attribute("w:val")
		.set_value("true");

	pugi::xml_node new_run_text = new_run.append_child("w:t");

	// If the run starts or ends with whitespace characters, preserve them using the xml:space attribute
	if(*text != 0 && (isspace(text[0]) || isspace(text[strlen(text) - 1])))
		new_run_text.append_attribute("xml:space").set_value("preserve");

	new_run_text.text().set(text);
	return *new Run(m_Current, new_run);
}

duckx::Paragraph& duckx::Paragraph::insert_paragraph_after(const string& text, duckx::formatting_flag f)
{
	pugi::xml_node new_para = m_Parent.insert_child_after("w:p", m_Current);
	Paragraph* p = new Paragraph();
	p->set_current(new_para);
	p->add_run(text, f);
	return *p;
}

DocXML::DocXML(void)
{
}

DocXML::DocXML(pugi::xml_node node) : pugi::xml_node(node)
{
}

DocXML::~DocXML(void)
{
}

static bool __EnumerateNode(cstring& sPath, int iPos, DocXML node, void* pPrivate, DOCX_NODE_ENUMERATOR_FUNC func)
{
	cstring sChild = sPath.Tokenize(iPos, "/");

	if(iPos < 0) {
		if(!func(node, pPrivate))
			return false;
	} else {
		for(DocXML child_node = node.child(sChild); !child_node.empty(); child_node = child_node.next_sibling(sChild)) {
			if(!__EnumerateNode(sPath, iPos, child_node, pPrivate, func))
				return false;
		}
	}

	return true;
}

void DocXML::Enumerate(const char* sChildPath, void* pPrivate, DOCX_NODE_ENUMERATOR_FUNC func)
{
	if(sChildPath) {
		cstring sChildNodePath	= sChildPath;
		__EnumerateNode(sChildNodePath, 0, *this, pPrivate, func);
	}
}

size_t DocXML::Size(const char* sChild)
{
	return std::distance(children(sChild).begin(), children(sChild).end());
}

static void __enum_copy(pugi::xml_node dst, pugi::xml_node src)
{
	for(auto child = src.first_child(); child ; child = child.next_sibling()) {
		const char* sChildName	= child.name();
		//if(!*sChildName) continue;
		pugi::xml_node	node = dst.append_child(sChildName);

		if(!child.text().empty()) {
			// copy value
			node.text().set(child.text().get());
		}

		for(auto attr = child.first_attribute(); attr; attr = attr.next_attribute()) {
			node.append_attribute(attr.name())	= attr.value();
		}

		__enum_copy(node, child);
	}
}

void DocXML::AddChildFromBuffer(const char* sBuffer)
{
	pugi::xml_document*	pDoc	= new pugi::xml_document;

	if(pDoc) {
		pDoc->load_buffer(sBuffer, strlen(sBuffer),  pugi::parse_default | pugi::parse_ws_pcdata);

		for(auto child = pDoc->first_child(); !child.empty() ; child = child.next_sibling())
			append_copy(child);

		delete pDoc;
	}
}

static bool __EnumerateNodeInDepth(cstring& sChild, DocXML node, void* pPrivate, DOCX_NODE_ENUMERATOR_FUNC func)
{
	for(auto& i : node) {
		if(sChild == i.name()) {
			if(!func(i, pPrivate)) break;
		} else {
			if(!__EnumerateNodeInDepth(sChild, i, pPrivate, func))
				break;
		}
	}

	return true;
}

void DocXML::EnumerateInDepth(const char* sChild, void* pPrivate, DOCX_NODE_ENUMERATOR_FUNC func)
{
	if(sChild) {
		cstring	sChildName	= sChild;
		__EnumerateNodeInDepth(sChildName, *this, pPrivate, func);
	}
}

DocFile::DocFile(void)
{
	m_pZipFile	= NULL;
}

DocFile::~DocFile(void)
{
	OnClose();
}

void DocFile::Close(void)
{
	OnClose();

	if(m_Documents.size()) {
		for(auto& i : m_Documents)
			delete i.second;

		m_Documents.clear();
	}

	if(m_pZipFile) {
		zip_close(m_pZipFile);
		m_pZipFile	= NULL;
	}
}

void DocFile::OnClose(void)
{
}

bool DocFile::Open(const char* sFileName)
{
	Close();

	if(sFileName) {
		m_sDocFileName		= sFileName;
		m_pZipFile			= zip_open(sFileName, ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

		if(IsOpen()) {
			if(!OnOpen()) {
				Close();
			}
		} else {
			LOGE("Can't open '%s'.", sFileName);
		}
	}

	return IsOpen();
}

bool DocFile::OnOpen(void)
{
	// dummy
	return true;
}

pugi::xml_document* DocFile::GetXML(const char* sEntryName, bool bForceToCreate)
{
	pugi::xml_document*	pDoc	= NULL;

	if(m_pZipFile) {
		if(m_Documents.count(sEntryName)) {
			return m_Documents[sEntryName];
		}

		if(!zip_entry_open(m_pZipFile, sEntryName)) {	// read from zip entries
			if(!zip_entry_isdir(m_pZipFile)) {
				void*		buf		= NULL;
				size_t		bufsize;
				zip_entry_read(m_pZipFile, &buf, &bufsize);
				zip_entry_close(m_pZipFile);
				pugi::xml_document*	pNode	= new pugi::xml_document;
				pNode->load_buffer(buf, bufsize,  pugi::parse_default | pugi::parse_ws_pcdata | pugi::parse_declaration);
				free(buf);
				m_Documents[sEntryName]		= pNode;
				pDoc	= pNode;
			}
		} else if(bForceToCreate) {
			pugi::xml_document*	pNode	= new pugi::xml_document;
			m_Documents[sEntryName]		= pNode;
			pDoc						= pNode;
			pugi::xml_node decl			= pNode->prepend_child(pugi::node_declaration);
			decl.append_attribute("version")	= "1.0";
			decl.append_attribute("encoding")	= "UTF-8";
			decl.append_attribute("standalone")	= "yes";
		}
	}

	return pDoc;
}

pugi::xml_document* DocFile::EnumerateFiles(DOCX_FILE_ENUMERATOR_FUNC func, void* pPrivate)
{
	pugi::xml_document*	pDoc	= NULL;

	if(m_pZipFile) {
		int entry_count	= zip_total_entries(m_pZipFile);

		for(int i = 0; i < entry_count; i++) {
			zip_entry_openbyindex(m_pZipFile, i);

			if(!zip_entry_isdir(m_pZipFile)) {
				const char* sName = zip_entry_name(m_pZipFile);

				if(!func(sName, pPrivate)) entry_count = 0;
			}

			zip_entry_close(m_pZipFile);
		}
	}

	return pDoc;
}


bool DocFile::ReplaceFile(const char* sEntryName, const char* sFileName, const char* sNewEntryName)
{
	if(access(sFileName, F_OK) == -1) {
		LOGE("'%s' File is not existed.", sFileName);
		return false;
	}

	if(m_pZipFile && sEntryName && sFileName) {
		if(m_FileReplacements.count(sEntryName)) {
			LOGE("Replacement is already assigned. '%s' -> %s", sEntryName, sFileName);
			return false;
		}

		cstring	sTarget(sFileName);

		if(sNewEntryName && *sNewEntryName)
			sTarget.AppendFormat(",%s", sNewEntryName);

		m_FileReplacements[sEntryName]	= sTarget;
		return true;
	}

	return false;
}

bool DocFile::DeleteFile(const char* sEntryName)
{
	if(OnDelete(sEntryName)) {
		m_Documents.erase(sEntryName);
		m_FileReplacements[sEntryName].clear();
		return true;
	}

	return false;
}

static bool __create_zip_entry_from_buffer(zip_t* pZip, const char* sEntryName, const void* pBuff = NULL, size_t dwByteSize = 0)
{
	if(pZip && sEntryName) {
		if(!zip_entry_open(pZip, sEntryName)) {
			if(pBuff && dwByteSize) zip_entry_write(pZip, pBuff, dwByteSize);

			zip_entry_close(pZip);
			return true;
		}
	}

	return false;
}

static bool __create_zip_entry_from_file(zip_t* pZip, const char* sEntryName, const char* sFileName)
{
	bool	bRet	= false;

	if(sFileName) {
		cstring	sNewFileName(sFileName);
		cstring	sNewEntryName(sEntryName);
		{
			// filename & entry parsing replacement
			const char*	sDelim			= ",;";
			int			iPos			= 0;
			cstring		sTokFileName	= sNewFileName.Tokenize(iPos, sDelim);
			cstring		sTokEntryName	= sNewFileName.Tokenize(iPos, sDelim);

			if(iPos >= 0) {
				sNewFileName	= sTokFileName;
				sNewEntryName	= sTokEntryName;
			}
		}

		if(!sNewFileName.size()) return true;	// deleted entry.

		FILE*	fp		= fopen(sNewFileName, "rb");

		if(fp) {
			fseek(fp, 0, SEEK_END);
			size_t	file_size	= ftell(fp);
			BYTE*	pBuff		= NULL;

			if(file_size) {
				pBuff		= new BYTE[file_size];
				fseek(fp, 0, SEEK_SET);
				fread(pBuff, 1, file_size, fp);
			}

			bRet	= __create_zip_entry_from_buffer(pZip, sNewEntryName, pBuff, file_size);

			if(pBuff) delete [] pBuff;

			fclose(fp);
		} else {
			LOGE("Can't create entry('%s') from file('%s') \n", sNewEntryName.c_str(), sNewFileName.c_str());
		}
	}

	return bRet;
}

bool DocFile::Save(const char* sFileName)
{
	if(!m_pZipFile || !OnSave()) return false;

	bool	bOverwrite		= (!sFileName) || (m_sDocFileName == sFileName);
	string	sSaveFileName	= bOverwrite ? (m_sDocFileName + ".doc_tmp") : sFileName;
	// Create the new file
	zip_t*	new_zip			= zip_open(sSaveFileName.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

	if(new_zip) {
		bool	bRet			= true;
		{
			// build document container files
			map<string, bool>	saved_entries;

			// replace entry or copy from original
			for(int i = 0; i < zip_total_entries(m_pZipFile); i++) {
				zip_entry_openbyindex(m_pZipFile, i);
				string					entry_name	= zip_entry_name(m_pZipFile);
				pugi::xml_document*		pNode		= m_FileReplacements.count(entry_name) ? NULL : (m_Documents.count(entry_name) ? m_Documents.at(entry_name) : NULL);
				saved_entries[entry_name]			= true;

				if(!pNode) {	// from external file or unmanaged file
					if(m_FileReplacements.count(entry_name)) {
						// file replacement if target path is existed, or delete it.
						if(m_FileReplacements[entry_name].size() && !__create_zip_entry_from_file(new_zip, entry_name.c_str(), m_FileReplacements[entry_name].c_str())) {
							bRet	= false;
							break;
						}
					} else {
						// just copy from original document
						void*		entry_buf;
						size_t		entry_buf_size	= 0;
						zip_entry_read(m_pZipFile, &entry_buf, &entry_buf_size);

						if(!__create_zip_entry_from_buffer(new_zip, entry_name.c_str(), entry_buf, entry_buf_size)) {
							free(entry_buf);
							bRet	= false;
							break;
						}

						free(entry_buf);
					}
				} else {		// managed XML file
					xml_string_writer	writer;
					pNode->print(writer, PUGIXML_TEXT("\t"), pugi::format_default | pugi::parse_ws_pcdata | pugi::format_write_bom);
					const char* buf = writer.result.c_str();

					if(!__create_zip_entry_from_buffer(new_zip, entry_name.c_str(), buf, strlen(buf))) {
						bRet	= false;
						break;
					}
				}

				zip_entry_close(m_pZipFile);
			}

			// new entry from internal DocXML object
			for(auto& i : m_Documents) if(!saved_entries.count(i.first)) {
					pugi::xml_document*		pNode	= i.second;
					xml_string_writer	writer;
					pNode->print(writer, PUGIXML_TEXT("\t"), pugi::format_default | pugi::parse_ws_pcdata | pugi::format_write_bom);
					const char* buf = writer.result.c_str();

					if(!__create_zip_entry_from_buffer(new_zip, i.first.c_str(), buf, strlen(buf))) {
						bRet	= false;
						break;
					}

					saved_entries[i.first]			= true;
				}

			// new entry from external file
			for(auto& i : m_FileReplacements) if(!saved_entries.count(i.first)) {
					if(!__create_zip_entry_from_file(new_zip, i.first.c_str(), i.second.c_str())) {
						bRet	= false;
						break;
					}
				}
		}
		zip_close(new_zip);

		if(!bRet) {				// remove broken new document file
			remove(sSaveFileName.c_str());
		} else if(bOverwrite) {	// remove original & rename to original
			Close();
			remove(m_sDocFileName.c_str());
			rename(sSaveFileName.c_str(), m_sDocFileName.c_str());
		}

		return bRet;
	}

	return false;
}

bool DocFile::OnSave(void)
{
	return true;
}

bool DocFile::OnDelete(const char* sEntryName)
{
	return true;
}

