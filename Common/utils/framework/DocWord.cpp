//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 1/26/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "DocWord.h"

DocWord::DocWord(void)
{
}

DocWord::~DocWord(void)
{
	OnClose();
}

bool DocWord::Open(const char* sFileName)
{
	return DocFile::Open(sFileName);
}

bool DocWord::OnOpen(void)
{
	m_Body			= GetXML("word/document.xml")->child("w:document").child("w:body");
	m_Relationships	= GetXML("word/_rels/document.xml.rels")->child("Relationships");
	m_ContentsType	= GetXML("[Content_Types].xml")->child("Types");
	m_Properties	= GetXML("docProps/custom.xml")->child("Properties");
	// open header and footers
	{
		typedef struct {
			DocWord*		pDoc;
			list<DocXML>*	pList;
		} __DOCX_EXTRA;
		__DOCX_EXTRA	extra = {this, &m_HeaderFooters};
		EnumerateFiles([](const char* sFileName, void* pPrivate) -> bool {
			cstring sName(sFileName);
			__DOCX_EXTRA* pExtra = (__DOCX_EXTRA*)pPrivate;

			if(sName.CompareBack(".xml"))
			{
				if(sName.CompareFront("word/header")) {
					pExtra->pList->push_back(pExtra->pDoc->GetXML(sFileName)->child("w:hdr").child("w:p"));
				} else if(sName.CompareFront("word/footer")) {
					pExtra->pList->push_back(pExtra->pDoc->GetXML(sFileName)->child("w:ftr").child("w:p"));
				}
			}

			return true;
		}, &extra);
	}
	// make image id list
	m_Body.EnumerateInDepth("w:drawing", &m_ImageMap, [](DocXML node, void* pPrivate) -> bool {
		string sDesc, sID;
		node.EnumerateInDepth("wp:docPr", &sDesc, [](DocXML node, void* pPrivate) -> bool {
			* ((string*)pPrivate)	= node.attribute("descr").value();
			return false;
		});

		if(sDesc.size())
		{
			node.EnumerateInDepth("a:blip", &sID, [](DocXML node, void* pPrivate) -> bool {
				* ((string*)pPrivate)	= node.attribute("r:embed").value();
				return false;
			});
			(*((map<string, string>*)pPrivate))[sDesc]	= sID;
		}

		return true;
	});
	// make OLE object list
	m_Body.EnumerateInDepth("w:object", &m_OLEObjectMap, [](DocXML node, void* pPrivate) -> bool {
		string sDesc, sID;
		node.EnumerateInDepth("v:shape", &sDesc, [](DocXML node, void* pPrivate) -> bool {
			* ((string*)pPrivate)	= node.attribute("alt").value();
			return false;
		});

		if(sDesc.size())
		{
			node.EnumerateInDepth("v:imagedata", &sID, [](DocXML node, void* pPrivate) -> bool {
				* ((string*)pPrivate)	= node.attribute("r:id").value();
				return false;
			});
			node.EnumerateInDepth("o:OLEObject", &sID, [](DocXML node, void* pPrivate) -> bool {
				* ((string*)pPrivate)	+= ",";
				* ((string*)pPrivate)	+= node.attribute("r:id").value();
				return false;
			});
			(*((map<string, string>*)pPrivate))[sDesc]	= sID;
		}

		return true;
	});
	// default contents type
	{
		typedef struct {
			const char* sExt;
			const char* sType;
		} __content_types;
		static const __content_types	content_types[] = {
			{"jpeg", "image/jpeg"},
			{"jpg", "image/jpeg"},
			{"png", "image/png"},
			{"gif", "image/gif"},
			{"tif", "image/tiff"},
			{"bmp", "image/bmp"},
			{"svg", "image/svg+xml"},
			{"wmf", "image/x-wmf"},
			{"odttf", "application/vnd.openxmlformats-officedocument.obfuscatedFont"},
			{NULL, NULL},
		};

		for(int i = 0; content_types[i].sExt; i++) {
			if(!m_ContentsType.find_child_by_attribute("Default", "Extension", content_types[i].sExt)) {
				DocXML	node	= m_ContentsType.append_child("Default");
				node.append_attribute("Extension").set_value(content_types[i].sExt);
				node.append_attribute("ContentType").set_value(content_types[i].sType);
			}
		}
	}
	return true;
}

void DocWord::OnClose(void)
{
	m_ImageMap.clear();
	m_OLEObjectMap.clear();
}

bool DocWord::OnSave(void)
{
	return true;
}

bool DocWord::ReplaceImage(const char* sDesc, const char* sFileName)
{
	if(!sDesc || !sFileName) return false;

	cstring sInputExt(sFileName);
	string str_desc(sDesc);
	cstring sOrgEntryPath;
	cstring	sNewEntryPath;
	bool	bImage	= false;
	{
		sInputExt.CutFront(".", true);
		sInputExt.MakeLower();

		// fix some contents type
		if(sInputExt == "jpg") sInputExt = "jpeg";

		// image format check
		static const char* __sImageExtList[] = {
			"png", "jpeg", "bmp", "gif", "tiff", NULL
		};
		bImage	= sInputExt.RetrieveTag(__sImageExtList) >= 0;
	}

	if(m_ImageMap.count(sDesc)) {
		string	sID			= m_ImageMap[str_desc];
		DocXML	node		= m_Relationships.find_child_by_attribute("Id", sID.c_str());
		cstring	sEntryName	= node.attribute("Target").value();
		sOrgEntryPath.Format("word/%s", sEntryName.c_str());
		{
			// checking file format extension is not same
			cstring sEntryExt(sEntryName);
			sEntryExt.CutFront(".", true);
			sEntryExt.MakeLower();

			if(!bImage) {
				LOGE("Image format conversion from '%s' to '%s' does not support.", sEntryName.c_str(), sFileName);
				return false;
			}

			if(sEntryExt != sInputExt) {
				// fix entry name
				sNewEntryPath	= sOrgEntryPath;
				sNewEntryPath.CutBack(".");
				sNewEntryPath.AppendFormat(".%s", sInputExt.c_str());
				// fix XML parsing name
				sEntryName.CutBack(".");
				sEntryName.AppendFormat(".%s", sInputExt.c_str());
				node.attribute("Target").set_value(sEntryName);

				// if not existed, add contents type
				if(m_ContentsType.find_child_by_attribute("Extension", sInputExt).empty()) {
					cstring content_type;
					DocXML file_type	= m_ContentsType.append_child("Default");
					content_type.Format("image/%s", sInputExt.c_str());
					file_type.append_attribute("Extension").set_value(sInputExt);
					file_type.append_attribute("ContentType").set_value(content_type);
				}
			}
		}

		if(ReplaceFile(sOrgEntryPath.c_str(), sFileName, sNewEntryPath))
			return true;
	}

	if(m_OLEObjectMap.count(sDesc)) {
		cstring	sID			= m_OLEObjectMap[str_desc];

		if(sInputExt == "emf") {
			sID.CutBack(",");	// image data ID
		} else {
			sID.CutFront(",");	// OLE data ID
		}

		DocXML	node		= m_Relationships.find_child_by_attribute("Id", sID.c_str());
		cstring	sEntryName	= node.attribute("Target").value();
		sOrgEntryPath.Format("word/%s", sEntryName.c_str());
		{
			// checking file format extension is not same
			cstring sEntryExt(sEntryName);
			sEntryExt.CutFront(".", true);
			sEntryExt.MakeLower();

			if(bImage) {
				LOGE("OLE format conversion from '%s' to '%s' does not support.", sEntryName.c_str(), sFileName);
				return false;
			}

			if(sEntryExt != sInputExt) {
				LOGW("OLE object file extension is not same with '%s' and '%s'.", sEntryName.c_str(), sFileName);
			}
		}

		if(ReplaceFile(sOrgEntryPath.c_str(), sFileName, NULL))
			return true;
	}

	LOGW("Image[%s] can not not found.", sDesc);
	return false;
}

bool DocWord::ReplaceSubDocument(const char* sPrevName, const char* sFileName)
{
	if(!sPrevName || !sFileName) return false;

	typedef struct {
		const char* sPrevName;
		const char* sFileName;
	} __Private;
	__Private	private_data = {sPrevName, sFileName};
	m_Relationships.Enumerate("Relationship", &private_data, [](DocXML node, void* pPrivate) -> bool {
		cstring		sType(node.attribute("Type").value());

		if(sType.CompareBack("subDocument"))
		{
			__Private*	pPrivateData	= (__Private*)pPrivate;
			cstring	sName(node.attribute("Target").value());
			sName.CutFront("/", true);
			sName.CutFront("\\", true);
			sName.CutBack(".", true);

			if(sName == pPrivateData->sPrevName) {
				sName	= pPrivateData->sFileName;
				sName.Replace("\\", "/", true);
				node.attribute("Target").set_value(sName);
				return false;
			}
		}

		return true;
	});
	return true;
}

bool DocWord::SetProperty(const char* sID, const char* sValue)
{
	DocXML node = m_Properties.find_child_by_attribute("name", sID);

	if(!node.empty()) {
		node.child("vt:lpwstr").text().set(sValue);
		return true;
	} else {
		LOGW("Property '%s' is not existed.", sID);
	}

	return false;
}

bool DocWord::AddFile(const char* sFileName, DOC_WORD_RELATIONSHIP RelationShip, cstring& sRelationID)
{
	// get new file Id
	for(int i = 1;; i++) {
		sRelationID.Format("rId%d", i);

		if(!m_Relationships.find_child_by_attribute("Id", sRelationID)) break;
	}

	// make all path/entry name
	cstring	sTarget(sFileName), sEntryPath, sRelationShipType;
	bool	bExternal	= false;
	{
		typedef struct {
			const char* sType;
			const char* sTargetPath;
			const char* sEntryPath;
		} __REL_PATH;
		static const __REL_PATH	__rel_path[] = {
			{"header",		"",					"word/"},
			{"footer",		"",					"word/"},
			{"image",		"media/",			"word/media/"},
			{"styles",		"",					"word/"},
			{"fontTable",	"",					"word/"},
			{"customXml",	"../customXml/",	"customXml/"},
			{"webSettings",	"",					"word/"},
			{"subDocument",	NULL,				NULL},
			{"hyperlink",	NULL,				NULL},
		};
		// make others
		sRelationShipType.Format("http://schemas.openxmlformats.org/officeDocument/2006/relationships/%s", __rel_path[(int)RelationShip].sType);

		if(__rel_path[(int)RelationShip].sTargetPath) {
			sTarget.CutFront("\\", true);
			sTarget.CutFront("/", true);
			sTarget.insert(0, "_");
			sTarget.insert(0, sRelationID);
			sEntryPath.Format("%s%s", __rel_path[(int)RelationShip].sEntryPath, sTarget.c_str());
			sTarget.insert(0, __rel_path[(int)RelationShip].sTargetPath);
		} else {
			bExternal	= true;
		}
	}
	// append new relationship
	DocXML	node	= m_Relationships.append_child("Relationship");
	node.append_attribute("Id").set_value(sRelationID);
	node.append_attribute("Type").set_value(sRelationShipType);
	node.append_attribute("Target").set_value(sTarget);

	if(bExternal) node.append_attribute("TargetMode").set_value("External");

	return bExternal ? true : ReplaceFile(sEntryPath, sFileName);
}

string DocWord::AddMedia(const char* sFileName)
{
	cstring	sID;
	AddFile(sFileName, DOC_WORD_RELATIONSHIP_IMAGE, sID);
	return sID.c_str();
}

string DocWord::AddSubDocument(const char* sFileName)
{
	cstring	sID;
	AddFile(sFileName, DOC_WORD_RELATIONSHIP_SUB_DOCUMENT, sID);
	return sID.c_str();
}

string DocWord::AddHyperlink(const char* sHyperlink)
{
	cstring	sID;
	AddFile(sHyperlink, DOC_WORD_RELATIONSHIP_HIPERLINK, sID);
	return sID.c_str();
}

void DocWord::Modify(map<string, string>* pMod)
{
	if(!pMod || !pMod->size()) return;

	auto modify_func = [](DocXML node, void* pPrivate) -> bool {
		map<string, string>& ModMap	= *((map<string, string>*)pPrivate);

		for(;;)
		{
			// paragraph text
			cstring sParagraph, sVariable;
			node.Enumerate("w:r", &sParagraph, [](DocXML node, void* pPrivate) -> bool {
				((cstring*)pPrivate)->Append(node.child("w:t").text().get());
				return true;
			});
			// search variables
			int iPos	= sParagraph.FindVariableString(sVariable, "@$(*)");

			if(iPos >= 0) {
				cstring	sReplace	= (ModMap.count(sVariable.c_str())) ? ModMap[sVariable.c_str()].c_str() : "";
				typedef struct {
					DocXML		parent;
					cstring		sReplace;
					int			iPos, iSize;
				} TEXT_REPLACE;
				TEXT_REPLACE	mod = {node, sReplace, iPos, sVariable.size() + 4};
				node.Enumerate("w:r", &mod, [](DocXML node, void* pPrivate) -> bool {
					TEXT_REPLACE*	pMod = (TEXT_REPLACE*)pPrivate;
					cstring s = node.child("w:t").text().get();

					if(s.size() <= pMod->iPos)
					{
						pMod->iPos	-= s.size();
					} else
					{
						int iRestSize	= s.size() - pMod->iPos;
						int iDelSize	= (pMod->iSize > iRestSize) ? iRestSize : pMod->iSize;
						s.erase(pMod->iPos, iDelSize);

						if(pMod->sReplace.size()) {
							s.insert(pMod->iPos, pMod->sReplace.c_str());
							pMod->sReplace.clear();
						}

						pMod->iPos		= 0;
						pMod->iSize		-= iDelSize;

						if(!s.size()) {
							pMod->parent.remove_child(node);
						} else {
							if(*s.c_str() == ' ' || s.c_str()[strlen(s.c_str()) - 1] == ' ')
								node.child("w:t").append_attribute("xml:space").set_value("preserve");

							node.child("w:t").text().set(s.c_str());
						}
					}
					return (pMod->iSize != 0);
				});
			} else break;
		}

		return true;
	};
	m_Body.EnumerateInDepth("w:p", pMod, modify_func);

	for(auto& i : m_HeaderFooters) {
		i.EnumerateInDepth("w:p", pMod, modify_func);
	}
}
