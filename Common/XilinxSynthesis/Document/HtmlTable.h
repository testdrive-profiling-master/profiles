//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : Xilinx synthesis
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __HTML_TABLE_H__
#define __HTML_TABLE_H__
#include "common.h"

typedef enum{
	CELL_TD,
	CELL_TH
}TABLE_CELL;

typedef enum{
	TABLE_ALIGN_LEFT,
	TABLE_ALIGN_RIGHT,
	TABLE_ALIGN_CENTER
}TABLE_ALIGN;

typedef enum{
	TABLE_BOARDER_LEFT,
	TABLE_BOARDER_RIGHT,
	TABLE_BOARDER_TOP,
	TABLE_BOARDER_BOTTOM
}TABLE_BOARDER;

typedef enum{
	TABLE_SPAN_COL,
	TABLE_SPAN_ROW
}TABLE_SPAN;

extern LPCTSTR		sBLANK_FILED;

class HtmlTable{
public:
	HtmlTable(void);
	virtual ~HtmlTable(void);

	ITDHtml* Create(ITDDocument* pDoc, LPCTSTR sFileName, int sx, int sy, int width, int height, ITDHtmlManager* pManager = NULL);

	void Clear(void);
	void NewRow(void);
	void NewCell(TABLE_CELL type, LPCTSTR sFormat = NULL, ...);
	void NewTH(void);
	void NewTD(void);
	void SetBody(LPCTSTR sFormat, ...);
	void SetField(LPCTSTR sID, LPCTSTR sFormat, ...);
	void SetTextAlignment(TABLE_ALIGN alignment);
	void SetBoarderWidth(TABLE_BOARDER side, int iWidth = 0);
	void SetSpan(TABLE_SPAN span, int iSize);
	void JScript(LPCTSTR sFormat, ...);

	operator ITDHtml* () const	{return m_pHtml;}

private:
	ITDHtml*			m_pHtml;
};
#endif//__HTML_TABLE_H__
