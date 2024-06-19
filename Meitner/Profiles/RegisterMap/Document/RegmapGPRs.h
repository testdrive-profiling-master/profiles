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
// Title : Meitner processor register map document
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __REGMAP_GPRs_H__
#define __REGMAP_GPRs_H__
#include "RegmapCommon.h"

typedef enum {
	DATA_TYPE_AUTO,
	DATA_TYPE_INT,
	DATA_TYPE_FLOAT,
	DATA_TYPE_HEX,
	DATA_TYPE_BOOLEAN,
	DATA_TYPE_SIZE
} DATA_TYPE;

class RegmapGPRs : public Regmap
{
public:
	RegmapGPRs(void);
	virtual ~RegmapGPRs(void);

private:
	virtual BOOL OnUpdate(void);
	virtual BOOL OnCommand(LPCTSTR lpszURL);
	virtual void OnBroadcast(LPVOID pData = NULL);
	bool		 IsValidID(uint32_t id);

	void		 UpdateTagName(void);
	void		 UpdateTagAll(void);
	void		 UpdateVectorAll(void);
	void		 UpdateVector(int iIndex);
	void		 UpdateData(int iIndex, int iElement);

	DATA_TYPE	 m_DataType[MTSP_GPR_SIZE][4];
};
#endif //__REGMAP_GPRs_H__
