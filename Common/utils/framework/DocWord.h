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
// Rev.  : 9/14/2024 Sat (clonextop@gmail.com)
//================================================================================
#ifndef __DOC_WORD_H__
#define __DOC_WORD_H__
#include "DocDocument.h"

typedef enum {
	DOC_WORD_RELATIONSHIP_HEADER,
	DOC_WORD_RELATIONSHIP_FOOTER,
	DOC_WORD_RELATIONSHIP_IMAGE,
	DOC_WORD_RELATIONSHIP_FONT,
	DOC_WORD_RELATIONSHIP_STYLES,
	DOC_WORD_RELATIONSHIP_FONTTABLE,
	DOC_WORD_RELATIONSHIP_CUSTOM_XML,
	DOC_WORD_RELATIONSHIP_WEB_SETTINGS,
	DOC_WORD_RELATIONSHIP_SUB_DOCUMENT,
	DOC_WORD_RELATIONSHIP_HIPERLINK,
} DOC_WORD_RELATIONSHIP;

class DocWord : public DocFile
{
public:
	DocWord(void);
	virtual ~DocWord(void);
	virtual bool Open(const char *sFileName);
	void		 Modify(map<string, string> *pMod);
	bool		 ReplaceImage(const char *sDesc, const char *sFileName);
	bool		 ReplaceSubDocument(const char *sPrevName, const char *sFileName);
	bool		 SetProperty(const char *sID, const char *sValue);
	string		 AddMedia(const char *sFileName);
	bool		 AddFont(const char *sFileName, const char *sFontName, const char *sKey, bool bFixed = false); // .odttf file only
	string		 AddSubDocument(const char *sFileName);
	string		 AddHyperlink(const char *sHyperlink);

protected:
	bool		 AddFile(const char *sFileName, DOC_WORD_RELATIONSHIP RelationShip, cstring &sRelationID);
	virtual bool OnOpen(void);
	virtual void OnClose(void);
	virtual bool OnSave(void);

private:
	DocXML				m_Body;
	DocXML				m_Relationships;
	DocXML				m_ContentsType;
	DocXML				m_Properties;
	DocXML				m_FontList;	 // embedded font list
	DocXML				m_FontTable; // font tables
	list<DocXML>		m_HeaderFooters;
	map<string, string> m_ImageMap;
	map<string, string> m_OLEObjectMap;
};

#endif //__DOC_WORD_H__
