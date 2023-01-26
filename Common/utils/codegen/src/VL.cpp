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

VL::VL(CLASS Class)
{
	m_Class	= Class;
}

VL::~VL(void)
{
	Clear();
}

void VL::Clear(void)
{
	for(auto& i : m_ChildList) {
		delete i;
	}

	m_ChildList.clear();
}

VL* VL::AddChild(VL* pCode)
{
	if(pCode && pCode != this)
		m_ChildList.push_back(pCode);

	return pCode;
}

VL* VL::AddChildFront(VL* pCode)
{
	if(pCode && pCode != this)
		m_ChildList.push_front(pCode);

	return pCode;
}

VL* VL::PopChild(void)
{
	if(m_ChildList.size()) {
		VL* pVL	= m_ChildList.front();

		if(pVL) m_ChildList.pop_front();

		return pVL;
	}

	return NULL;
}

bool VL::RemoveChild(VL* pCode)
{
	bool bRet	= false;

	if(pCode || pCode != this) {
		auto i = m_ChildList.begin();

		while(i != m_ChildList.end()) {
			VL* pNode	= *i;

			if(pNode == pCode) {
				auto i_next = i;
				i_next++;
				m_ChildList.erase(i);
				i = i_next;
				bRet	= true;
			} else i++;
		}
	}

	return bRet;
}

VL* VL::FindChild(const char* sName)
{
	if(sName) {
		for(VL*& pVL : m_ChildList) {
			if(pVL) {
				if(!pVL->Name().size()) {
					pVL	= pVL->FindChild(sName);

					if(pVL) return pVL;
				} else if(pVL->Name() == sName) {
					return pVL;
				}
			}
		}
	}

	return NULL;
}

void VL::EnumerateChild(ENUMERATE_VL_FUNCTION func, void* pPrivate)
{
	for(auto& pVL : m_ChildList) {
		if(!func(pVL, pPrivate)) break;
	}
}

string VL::Data(void)
{
	cstring sVar, s	= PrivateData();
	int iPos		= 0;

	while((iPos = s.FindVariableString(sVar, "$(*)", iPos)) >= 0) {
		cstring sFormat, sData, sNameList(sVar), sSeparator;
		bool	bParentInheritance	= false;
		bool	bFormatMustApply	= true;
		{
			int iFormatPos = sVar.find(':');

			if(iFormatPos > 0) {
				sFormat	= sVar.c_str() + iFormatPos + 1;
				sNameList.erase(iFormatPos, -1);

				if(sFormat[0] == ':') {
					sFormat.erase(0, 1);
					bFormatMustApply	= false;
				}
			}

			if(sNameList[0] == '^') {	// do parent inheritance
				sNameList.erase(0,  1);
				bParentInheritance	= true;
			}
		}
		{
			// get separator
			int iSeparatorPos	= sNameList.find('[');

			if(iSeparatorPos > 0) {
				sSeparator			= sNameList.c_str() + iSeparatorPos + 1;
				sNameList.erase(iSeparatorPos, -1);
				iSeparatorPos		= sSeparator.find(']');

				if(iSeparatorPos > 0) sSeparator.erase(iSeparatorPos, -1);
			}
		}
		// apply string format
		{
			bool	bFound	= false;
			{
				int iNamePos	= 0;

				while(!bFound) {
					cstring sName	= sNameList.Tokenize(iNamePos, ",");
					sName.Trim(" ");

					if(iNamePos >= 0) {
						for(auto& pChild : m_ChildList) {
							if(sName == pChild->Name().c_str()) {
								bFound		= true;

								if(sFormat.size()) {
									sData.AppendFormat(sFormat.c_str(), pChild->Data().c_str());
								} else {
									sData.Append(pChild->Data().c_str());
								}
							}
						}
					} else break;
				}
			}

			if(!bFound) {
				sData.clear();

				if(bParentInheritance) {
					// not found any variables : restore variable expression
					if(sVar != Name().c_str()) {	// prevent self recursive
						sData.Format("$(%s)", sVar.c_str());
						s.insert(iPos, sData);
					}
				} else if(sFormat.size() && bFormatMustApply) {
					// insert dummy
					sData.Format(sFormat, "");

					if(sSeparator.size())
						sData.DeleteBack(sSeparator);

					s.insert(iPos, sData);
				}

				iPos	+= sData.size();
			} else {
				if(sSeparator.size())
					sData.DeleteBack(sSeparator);

				s.insert(iPos, sData);
			}
		}
	}

	return s.c_str();
}

void VL::ShowAll(void)
{
	static int iTab	= 0;

	for(int i = 0; i < iTab; i++) printf("\t");

	printf("%s = %s\n", Name().c_str(), GetData());
	iTab++;

	for(auto& it : m_ChildList) {
		for(int i = 0; i < iTab; i++) printf("\t");

		it->ShowAll();
	}

	iTab--;
}
