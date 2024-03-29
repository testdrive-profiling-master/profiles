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

VL_Number::VL_Number(const char* sName, double fData) : VL(CLASS_NUMBER)
{
	m_sName			= sName;
	m_fData			= fData;
}

VL_Number::~VL_Number(void)
{
}

VL_Number* VL_Number::Alias(VL* pNode)
{
	if(pNode && pNode->Class() == CLASS_NUMBER)
		return (VL_Number*)pNode;

	return NULL;
}

VL_Number* VL_Number::Create(const char* sName, double fData)
{
	return new VL_Number(sName, fData);
}

string VL_Number::PrivateData(void)
{
	cstring s;

	if(m_sFormat.size()) {
		if(m_sFormat[0] == '#')
			s.Format(m_sFormat.c_str() + 1, (long long)m_fData);
		else
			s.Format(m_sFormat.c_str(), m_fData);
	} else {
		if((m_fData - (long long)m_fData) == 0)
			s.Format("%lld", (long long)m_fData);
		else
			s.Format("%f", m_fData);
	}

	return s.c_str();
}

void VL_Number::SetFormat(const char* sFormat)
{
	m_sFormat	= sFormat;
}
