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
// Title : TestDrive codegen project
// Rev.  : 1/26/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "VL.h"
#include "DocExcel.h"

VL_String::VL_String(const char* sName, const char* sData) : VL(CLASS_STRING)
{
	m_sName			= sName;
	cstring s		= sData;
	{
		// instant include files
		int iPos = 0;
		cstring sFileName;

		while((iPos = s.FindVariableString(sFileName, "$F(*)")) >= 0) {
			TextFile f;

			if(f.Open(sFileName)) {
				cstring sContents;
				f.GetAll(sContents);
				s.insert(iPos, sContents);
			} else {
				LOGE("Can't open file : %s", sFileName);
				exit(1);
			}
		}
	}
	m_sData	= s;
}

VL_String::~VL_String(void)
{
}

VL_String* VL_String::Alias(VL* pNode)
{
	if(pNode && pNode->Class() == CLASS_STRING)
		return (VL_String*)pNode;

	return NULL;
}

VL_String* VL_String::Create(const char* sName, const char* sData)
{
	return new VL_String(sName, sData);
}

VL_String* VL_String::CreateFromTable(const char* sFileName, const char* sChildID)
{
	map<int, string>	id_name;
	map<int, string>	default_val;
	VL_String*			pTop	= NULL;
	TextFile			f;

	if(f.Open(sFileName)) {
		cstring	sLine;
		pTop	= new VL_String(sFileName, "");

		while(f.GetLine(sLine)) {
			VL_String*			pNode	= NULL;
			sLine.Trim(" \r\n");

			if(!sLine.size()) continue;

			if(id_name.size()) {
				pNode	= new VL_String("", "");
				pTop->AddChild(pNode);
			}

			{
				// read line
				int	iPos	= 0;	// string position
				int	iItem	= 0;	// item position
				int iColumn	= 0;	// column position

				while(iPos >= 0) {
					if(sLine[iPos] == '\t') {
						if(iItem == iColumn) {	// empty column
							if(pNode) {			// add default values
								pNode->AddChild(new VL_String(id_name[iItem].c_str(), default_val[iItem].c_str()));
							}

							iItem++;
						}

						iColumn++;
						iPos++;
					} else {
						cstring sData = sLine.Tokenize(iPos, "\t");
						sData.Trim(" ");

						if(iPos > 0 && sData.size() != 0) {
							if(!pNode) {	// ID name
								id_name[iItem]	= sData;
							} else {		// data
								default_val[iItem]	= sData;
								pNode->AddChild(new VL_String(id_name[iItem].c_str(), sData.c_str()));

								if(sChildID && (id_name[iItem] == sChildID)) {
									pNode->Name()	= sData;
								}
							}

							iItem++;
						} else if(iItem == iColumn) {
							pNode->AddChild(new VL_String(id_name[iItem].c_str(), default_val[iItem].c_str()));
						}
					}
				}
			}
		}
	}

	return pTop;
}

VL_String* VL_String::CreateFromXLSX(const char* sFileName, const char* sSheetName, const char* sChildID)
{
	VL_String*	pTop	= NULL;
	DocExcel	doc;

	if(doc.Open(sFileName)) {
		DocExcelSheet*	pSheet	= doc.GetSheet(sSheetName);

		if(pSheet) {
			vector<string>		id_name;
			map<int, string>	default_val;
			pTop	= new VL_String(sFileName, "");
			// get first ID tag list
			{
				pSheet->GetRow();

				for(int i = 0; pSheet->GetColumn(); i++) {
					string	sName	= pSheet->GetValue().c_str();

					if(sName == "") break;

					id_name.push_back(sName);
				}
			}

			while(pSheet->GetRow()) {
				VL_String*			pNode		= new VL_String("", "");
				bool				bEmptyRow	= true;

				// setup new node
				for(int i = 0; i < id_name.size(); i++) {
					bool	bEmptyCell	= (pSheet->GetColumn() == false);
					VL_String*	pCell	= new VL_String(id_name[i].c_str(), "");
					pNode->AddChild(pCell);

					if(bEmptyCell) {
					EMPTY_CELL:
						pCell->AddChild(new VL_String("empty", ""));
						pCell->SetData(default_val[i].c_str());
					} else {
						string sData	= pSheet->GetValue();

						if(sData == "") goto EMPTY_CELL;

						default_val[i]	= sData;
						pCell->SetData(sData.c_str());
						bEmptyRow		= false;
					}

					if(sChildID && (sChildID == id_name[i].c_str())) {
						pNode->Name()	= default_val[i];
					}
				}

				if(bEmptyRow) {
					pNode->Release();
				} else {
					pTop->AddChild(pNode);
				}
			}
		}
	}

	return pTop;
}

bool VL_String::CreateToXLSX(const char* sFileName, VL* pVL)
{
	bool		bFomula	= false;
	DocExcel	xls;

	if(!sFileName || !pVL) return false;

	{
		typedef struct {
			bool			bFomula;
			DocExcelSheet*	pSheet;
			int				iPanePos;
			int				iStyleColumn;
		} __private;
		__private	p = {false, NULL, 0, 0};
		{
			string	sPath	= GetCommonToolPath() + "bin/lua/DocExcel_template.xlsx";

			if(!xls.Open(sPath.c_str())) return false;

			p.pSheet		= xls.GetSheetByIndex(0);
			p.iStyleColumn	= xls.StyleCell(xls.StyleFont(NULL, 0, true, false, 0), xls.StyleFill(0xBFBFFF), 0, 0, "horizontal=center");
		}
		pVL->EnumerateChild([](VL * pChild, void* pPrivate) -> bool {
			__private* p	= (__private*)pPrivate;

			p->pSheet->GetRow(true);

			pChild->EnumerateChild([](VL * pChild, void* pPrivate) -> bool {
				__private* p	= (__private*)pPrivate;

				if(pChild->Name() == "column")
				{
					int iWidth		= 10;
					VL* pVL_width	= pChild->FindChild("colum_width");

					if(pVL_width) iWidth	= atoi(pVL_width->Data().c_str());

					p->pSheet->GetColumn(true);
					p->pSheet->SetColumnWidth(iWidth);
					p->pSheet->SetString(pChild->Data().c_str());
					p->pSheet->SetStyle(p->iStyleColumn);
					p->iPanePos		= p->pSheet->GetPosY() + 1;
				} else
				{
					switch(pChild->Class()) {
					case CLASS_STRING: {
						const char*	sData = ((VL_String*)pChild)->GetData();
						p->pSheet->GetColumn(true);

						if(*sData == '=') {
							// formula
							p->bFomula	= true;
							p->pSheet->SetInt(0);
						} else {
							p->pSheet->SetString(sData);
						}
					}
					break;

					case CLASS_NUMBER: {
						VL_Number* pNum	= (VL_Number*)pChild;
						p->pSheet->GetColumn(true);

						if(pNum->Name() == "int") {
							p->pSheet->SetInt((int)pNum->GetNumber());
						} else {
							p->pSheet->SetDouble(pNum->GetNumber());
						}
					}
					break;
					}
				}
				return true;
			}, pPrivate);

			return true;
		}, (void*)&p);
		bFomula	= p.bFomula;

		if(p.iPanePos) {
			p.pSheet->SetPane(Excel_PosEncode(0, p.iPanePos).c_str());
		}
	}

	if(bFomula) {	// setup formula
		typedef struct {
			int				x, y;
			DocExcelSheet*	sheet;
		} __private;
		__private	p = {0, 0, xls.GetSheet(pVL->Name().c_str())};
		pVL->EnumerateChild([](VL * pChild, void* pPrivate) -> bool {
			__private* p	= (__private*)pPrivate;
			pChild->EnumerateChild([](VL * pChild, void* pPrivate) -> bool {
				__private*		p		= (__private*)pPrivate;
				DocExcelSheet*	sheet	= p->sheet;

				if(pChild->Class() == CLASS_STRING)
				{
					const char*	sData = ((VL_String*)pChild)->GetData();
					sheet->SetPos(p->x, p->y);
					sheet->GetRow();
					sheet->GetColumn();
					//sheet->append_attribute("t", "str");
					sheet->SetFunction(sData + 1);
				}
				p->x++;
				return true;
			}, pPrivate);

			p->x	= 0;
			p->y++;
			return true;
		}, (void*)&p);
	}

	return xls.Save(sFileName);
}

static bool __MakeFileList(VL* pVL, string sSearchPath, int iDepth, bool bOnlyFile = false, map<string, bool>* ext_list = NULL, string sSubPath = "")
{
	struct stat statinfo;
	memset(&statinfo, 0, sizeof(statinfo));
	lstat(sSearchPath.c_str(), &statinfo);

	if(!S_ISDIR(statinfo.st_mode)) {
		LOGE("%s is not directory.", sSearchPath.c_str());
		return false;
	}

	DIR* dir;
	struct dirent* entry;

	if((dir = opendir(sSearchPath.c_str())) == NULL) {
		LOGE("Can't access this directory : %s.", sSearchPath.c_str());
		return false;
	}

	while((entry = readdir(dir)) != NULL) {
		memset(&statinfo, 0, sizeof(statinfo));
		string strFilePath = sSearchPath + "/" + entry->d_name;

		if(*entry->d_name == '.') continue;	// bypassing hidden/current/previous file or directory

		while(strFilePath.find("//") != string::npos)
			strFilePath.replace(strFilePath.find("//"), 2, "/");

		lstat(strFilePath.c_str(), &statinfo);

		if(S_ISDIR(statinfo.st_mode)) { // Is directory?
			if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;

			{
				string sNextSubPath		= sSubPath + entry->d_name;

				if(!bOnlyFile) pVL->AddChild(new VL_String("dir", sNextSubPath.c_str()));

				if(iDepth != 0) {
					string sNextSearchPath	= sSearchPath + "/" + entry->d_name;
					sNextSubPath			+= "/";
					__MakeFileList(pVL, sNextSearchPath, iDepth - 1, bOnlyFile, ext_list, sNextSubPath);
				}
			}
		} else {
			bool	bExtMatched	= true;

			if(ext_list) {
				cstring sExt(entry->d_name);
				sExt.CutFront(".", true);

				if(!(*ext_list)[sExt.c_str()]) {
					bExtMatched	= false;
				}
			}

			if(bExtMatched) {
				cstring sData(sSubPath.c_str());
				sData	+= entry->d_name;
				pVL->AddChild(new VL_String("file", sData));
			}
		}
	}

	//���� ���丮 �ݱ�
	closedir(dir);
	return true;
}

VL_String* VL_String::CreateFileList(const char* sPath, int iDepth, bool bOnlyFile, const char* sExtList)
{
	map<string, bool>	ext_list;
	VL_String* pVL = new VL_String("list", sPath);

	if(sExtList) {	// make file extension list
		int	iPos	= 0;
		cstring sExtToken(sExtList);

		for(;;) {
			cstring sExt	= sExtToken.Tokenize(iPos, " .,;");

			if(iPos < 0) break;

			ext_list[sExt.c_str()]	= true;
		}
	}

	if(!__MakeFileList(pVL, sPath, iDepth, bOnlyFile, sExtList ? &ext_list : NULL)) {
		SAFE_RELEASE(pVL);
	}

	return pVL;
}

string VL_String::PrivateData(void)
{
	VL* pOverride	= FindChild("@");	// override

	if(pOverride) {
		cstring s = pOverride->Data();
		s.Replace("@", m_sData.c_str());
	}

	return m_sData;
}
