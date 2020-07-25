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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __GE_IMAGE_H__
#define __GE_IMAGE_H__
#include "GeTexture.h"

typedef enum{
	SCREEN_POINT_LT,		// left & top
	SCREEN_POINT_RT,		// right & top
	SCREEN_POINT_CENTER,	// center
	SCREEN_POINT_LD,		// left & down
	SCREEN_POINT_RD			// right down
}SCREEN_POINT;

class CGeImage : public CGePrimitive
{
public:
	CGeImage();
	virtual ~CGeImage();

	virtual void Present(LPDIRECT3DTEXTURE9 pTex = NULL);
	//virtual void SetUpMatrix(float x, float y, float ex, float ey, float stu=0.001f, float stv=0.999f, float etu=0.999f, float etv=0.001f);
	virtual void SetUpMatrix(float x, float y, float ex, float ey, float stu=0.001f, float stv=0.001f, float etu=0.999f, float etv=0.999f);
	virtual void SetUpMatrix(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float stu, float stv, float etu, float etv);
	void SetUpDraw(SCREEN_POINT ref_point, float x, float y, float width = 0.f, float height = 0.f);
	void Restore(void);
	HRESULT CreateImage(int iWidth, int iHeight, D3DFORMAT format = D3DFMT_X8R8G8B8);
	HRESULT LoadImage(const char *image_filename);
	HRESULT LoadImage(CGeTexture* pTexture);
	virtual HRESULT InitVertexBuffer(void);
	void SetTexture(LPDIRECT3DTEXTURE9 tex);
	virtual void ReleaseAll(void);

	int Width(int iLevel = 0);
	int Height(int iLevel = 0);

	inline CGeTexture* Texture(void)	{return m_pTexture;}

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	CGeTexture*					m_pTexture;

public:
	VERTEXFORMAT_IMAGE* BeginSetupVertex(void);
	void EndSetupVertex(void);

private:
	struct{
		SCREEN_POINT			RefPoint;
		float					x, y, width, height;
	}m_Draw;
};

#endif//__GE_IMAGE_H__
