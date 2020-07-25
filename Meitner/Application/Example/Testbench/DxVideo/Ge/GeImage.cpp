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
#include "GeImage.h"

CGeImage::CGeImage()
{
	m_pTexture		= NULL;
	m_pVB			= NULL;
	memset(&m_Draw, 0, sizeof(m_Draw));
}

CGeImage::~CGeImage()
{
	ReleaseAll();
}

void CGeImage::ReleaseAll()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVB);
}

HRESULT CGeImage::CreateImage(int iWidth, int iHeight, D3DFORMAT format)
{
	ReleaseAll();
	m_pTexture	= TEXTURE_CREATE(iWidth, iHeight, format);

	if(!m_pTexture) return E_FAIL;

	m_pTexture->AddRef();

	if(FAILED(InitVertexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGeImage::LoadImage(const char* image_filename)
{
	ReleaseAll();
	m_pTexture = TEXTURE_LOAD_FILE(image_filename);

	if(!m_pTexture) return E_FAIL;

	m_pTexture->AddRef();

	if(FAILED(InitVertexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGeImage::LoadImage(CGeTexture* pTexture)
{
	ReleaseAll();

	if(!pTexture) return E_FAIL;

	m_pTexture	= pTexture;
	m_pTexture->AddRef();

	if(FAILED(InitVertexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGeImage::InitVertexBuffer()
{
	SAFE_RELEASE(m_pVB);

	if(FAILED(m_pd3dDevice->CreateVertexBuffer(4 * sizeof(VERTEXFORMAT_IMAGE),
			  0, D3DFVF_D3DXIMAGEVERTEX,
			  D3DPOOL_MANAGED , &m_pVB, NULL))) {
		ReleaseAll();
		return E_FAIL;
	}

	return S_OK;
}

void CGeImage::SetUpMatrix(float x, float y, float ex, float ey, float stu, float stv, float etu, float etv)
{
	SetUpMatrix(x, y, ex, y, x, ey, ex, ey, stu, stv, etu, etv);
}

void CGeImage::SetUpMatrix(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float stu, float stv, float etu, float etv)
{
	VERTEXFORMAT_IMAGE* pVertices;

	if(FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0))) return;

	pVertices[0].position = D3DXVECTOR3(x1, y1, 0);
	pVertices[1].position = D3DXVECTOR3(x2, y2, 0);
	pVertices[2].position = D3DXVECTOR3(x3, y3, 0);
	pVertices[3].position = D3DXVECTOR3(x4, y4, 0);
	pVertices[0].tu = stu;
	pVertices[0].tv	= stv;
	pVertices[1].tu = etu;
	pVertices[1].tv	= stv;
	pVertices[2].tu = stu;
	pVertices[2].tv	= etv;
	pVertices[3].tu = etu;
	pVertices[3].tv	= etv;
	m_pVB->Unlock();
}

void CGeImage::SetUpDraw(SCREEN_POINT ref_point, float x, float y, float width, float height)
{
	m_Draw.RefPoint	= ref_point;
	m_Draw.x		= x;
	m_Draw.y		= y;
	m_Draw.width	= width ? width : Width();
	m_Draw.height	= height ? height : Height();
	Restore();
}

void CGeImage::Restore(void)
{
	float sx, sy;

	switch(m_Draw.RefPoint) {
	case SCREEN_POINT_LT:		// left & top
		sx	= -m_iScreenWidth / 2;
		sy	= m_iScreenHeight / 2;
		break;

	case SCREEN_POINT_RT:		// right & top
		sx	= m_iScreenWidth / 2;
		sy	= m_iScreenHeight / 2;
		break;

	case SCREEN_POINT_CENTER:	// center
		sx	= 0;
		sy	= 0;
		break;

	case SCREEN_POINT_LD:		// left & down
		sx	= -m_iScreenWidth / 2;
		sy	= -m_iScreenHeight / 2;
		break;

	case SCREEN_POINT_RD:		// right down
		sx	= m_iScreenWidth / 2;
		sy	= -m_iScreenHeight / 2;
		break;
	}

	sx	+= m_Draw.x;
	sy	-= m_Draw.y;
	SetUpMatrix(sx, sy, sx + m_Draw.width, sy - m_Draw.height);
}

void CGeImage::Present(LPDIRECT3DTEXTURE9 pTex)
{
	if(pTex) m_pd3dDevice->SetTexture(0, pTex);
	else if(m_pTexture) m_pd3dDevice->SetTexture(0, m_pTexture->pTexture);

	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEXFORMAT_IMAGE));
	m_pd3dDevice->SetFVF(D3DFVF_D3DXIMAGEVERTEX);
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

int CGeImage::Width(int iLevel)
{
	if(!m_pTexture) return 0;

	D3DSURFACE_DESC desc;
	m_pTexture->pTexture->GetLevelDesc(iLevel, &desc);
	return desc.Width;
}

int CGeImage::Height(int iLevel)
{
	if(!m_pTexture) return 0;

	D3DSURFACE_DESC desc;
	m_pTexture->pTexture->GetLevelDesc(iLevel, &desc);
	return desc.Height;
}

VERTEXFORMAT_IMAGE* CGeImage::BeginSetupVertex(void)
{
	VERTEXFORMAT_IMAGE* pVertices;

	if(FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0))) return NULL;

	return pVertices;
}

void CGeImage::EndSetupVertex(void)
{
	m_pVB->Unlock();
}
