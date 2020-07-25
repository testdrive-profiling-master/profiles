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
#include "GeFont.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
// Custom vertex types for rendering text
//-----------------------------------------------------------------------------
#define MAX_NUM_VERTICES 50*6


struct VERTEXFORMAT_FONT2D {
	D3DXVECTOR4 p;
	DWORD color;
	FLOAT tu, tv;
};
struct VERTEXFORMAT_FONT3D {
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT tu, tv;
};

#define D3DFVF_FONT2DVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FONT3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

inline VERTEXFORMAT_FONT2D InitFont2DVertex(const D3DXVECTOR4& p, D3DCOLOR color,
		FLOAT tu, FLOAT tv)
{
	VERTEXFORMAT_FONT2D v;
	v.p = p;
	v.color = color;
	v.tu = tu;
	v.tv = tv;
	return v;
}

inline VERTEXFORMAT_FONT3D InitFont3DVertex(const D3DXVECTOR3& p, const D3DXVECTOR3& n,
		FLOAT tu, FLOAT tv)
{
	VERTEXFORMAT_FONT3D v;
	v.p = p;
	v.n = n;
	v.tu = tu;
	v.tv = tv;
	return v;
}

//-----------------------------------------------------------------------------
// Name: CGeFont()
// Desc: Font class constructor
//-----------------------------------------------------------------------------
CGeFont::CGeFont(const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags)
{
	_tcscpy(m_strFontName, strFontName);
	m_dwFontHeight			= dwHeight;
	m_dwFontFlags			= dwFlags;
	m_dwSpacing				= 0;
	m_dwTexWidth			= 0;
	m_dwTexHeight			= 0;
	m_pTexture				= NULL;
	m_pVB					= NULL;
	m_pStateBlockSaved		= NULL;
	m_pStateBlockDrawText	= NULL;
	m_fTextScale			= 0;
}

//-----------------------------------------------------------------------------
// Name: ~CGeFont()
// Desc: Font class destructor
//-----------------------------------------------------------------------------
CGeFont::~CGeFont()
{
	InvalidateDeviceObjects();
	DeleteDeviceObjects();
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: Initializes device-dependent objects, including the vertex buffer used
//       for rendering text and the texture map which stores the font image.
//-----------------------------------------------------------------------------
HRESULT CGeFont::InitDeviceObjects()
{
	HRESULT hr;
	// Keep a local copy of the device
	// Establish the font and texture size
	m_fTextScale  = 1.0f; // Draw fonts into texture without scaling

	// Large fonts need larger textures
	if(m_dwFontHeight > 60)
		m_dwTexWidth = m_dwTexHeight = 2048;
	else if(m_dwFontHeight > 30)
		m_dwTexWidth = m_dwTexHeight = 1024;
	else if(m_dwFontHeight > 15)
		m_dwTexWidth = m_dwTexHeight = 512;
	else
		m_dwTexWidth  = m_dwTexHeight = 256;

	// If requested texture is too big, use a smaller texture and smaller font,
	// and scale up when rendering.
	D3DCAPS9 d3dCaps;
	m_pd3dDevice->GetDeviceCaps(&d3dCaps);

	if(m_dwTexWidth > d3dCaps.MaxTextureWidth) {
		m_fTextScale = (FLOAT)d3dCaps.MaxTextureWidth / (FLOAT)m_dwTexWidth;
		m_dwTexWidth = m_dwTexHeight = d3dCaps.MaxTextureWidth;
	}

	// Create a new texture for the font
	hr = m_pd3dDevice->CreateTexture(m_dwTexWidth, m_dwTexHeight, 1,
									 0, D3DFMT_A4R4G4B4,
									 D3DPOOL_MANAGED, &m_pTexture, NULL);

	if(FAILED(hr))
		return hr;

	// Prepare to create a bitmap
	DWORD*      pBitmapBits;
	BITMAPINFO bmi;
	ZeroMemory(&bmi.bmiHeader,  sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = (int)m_dwTexWidth;
	bmi.bmiHeader.biHeight      = -(int)m_dwTexHeight;
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biBitCount    = 32;
	// Create a DC and a bitmap for the font
	HDC     hDC       = CreateCompatibleDC(NULL);
	HBITMAP hbmBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS,
										 (void**)&pBitmapBits, NULL, 0);
	SetMapMode(hDC, MM_TEXT);
	// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
	// antialiased font, but this is not guaranteed.
	INT nHeight    = -MulDiv(m_dwFontHeight,
							 (INT)(GetDeviceCaps(hDC, LOGPIXELSY) * m_fTextScale), 72);
	DWORD dwBold   = (m_dwFontFlags & D3DFONT_BOLD)   ? FW_BOLD : FW_NORMAL;
	DWORD dwItalic = (m_dwFontFlags & D3DFONT_ITALIC) ? TRUE    : FALSE;
	HFONT hFont    = CreateFont(nHeight, 0, 0, 0, dwBold, dwItalic,
								FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,//OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
								VARIABLE_PITCH, m_strFontName);

	if(NULL == hFont) {
		DeleteObject(hbmBitmap);
		return E_FAIL;
	}

	HGDIOBJ hbmOld = SelectObject(hDC, hbmBitmap);
	HGDIOBJ hFontOld = SelectObject(hDC, hFont);
	// Set text properties
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkColor(hDC, 0x00000000);
	SetTextAlign(hDC, TA_TOP);
	// Loop through all printable character and output them to the bitmap..
	// Meanwhile, keep track of the corresponding tex coords for each character.
	DWORD x = 0;
	DWORD y = 0;
	TCHAR str[2] = _T("x");
	SIZE size;
	// Calculate the spacing between characters based on line height
	GetTextExtentPoint32(hDC, TEXT(" "), 1, &size);
	x = m_dwSpacing = (DWORD) ceil(size.cy * 0.3f);

	for(TCHAR c = 32; c < 127; c++) {
		str[0] = c;
		GetTextExtentPoint32(hDC, str, 1, &size);

		if((DWORD)(x + size.cx + m_dwSpacing) > m_dwTexWidth) {
			x  = m_dwSpacing;
			y += size.cy + 1;
		}

		ExtTextOut(hDC, x + 0, y + 0, ETO_OPAQUE, NULL, str, 1, NULL);
		m_fTexCoords[c - 32][0] = ((FLOAT)(x + 0       - m_dwSpacing)) / m_dwTexWidth;
		m_fTexCoords[c - 32][1] = ((FLOAT)(y + 0       + 0)) / m_dwTexHeight;
		m_fTexCoords[c - 32][2] = ((FLOAT)(x + size.cx + m_dwSpacing)) / m_dwTexWidth;
		m_fTexCoords[c - 32][3] = ((FLOAT)(y + size.cy + 0)) / m_dwTexHeight;
		x += size.cx + (2 * m_dwSpacing);
	}

	// Lock the surface and write the alpha values for the set pixels
	D3DLOCKED_RECT d3dlr;
	m_pTexture->LockRect(0, &d3dlr, 0, 0);
	BYTE* pDstRow = (BYTE*)d3dlr.pBits;
	WORD* pDst16;
	BYTE bAlpha; // 4-bit measure of pixel intensity

	for(y = 0; y < m_dwTexHeight; y++) {
		pDst16 = (WORD*)pDstRow;

		for(x = 0; x < m_dwTexWidth; x++) {
			bAlpha = (BYTE)((pBitmapBits[m_dwTexWidth * y + x] & 0xff) >> 4);

			if(bAlpha > 0) {
				*pDst16++ = (WORD)((bAlpha << 12) | 0x0fff);
			} else {
				*pDst16++ = 0x0000;
			}
		}

		pDstRow += d3dlr.Pitch;
	}

	// Done updating texture, so clean up used objects
	m_pTexture->UnlockRect(0);
	SelectObject(hDC, hbmOld);
	SelectObject(hDC, hFontOld);
	DeleteObject(hbmBitmap);
	DeleteObject(hFont);
	DeleteDC(hDC);
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: RestoreDeviceObjects()
// Desc:
//-----------------------------------------------------------------------------
HRESULT CGeFont::RestoreDeviceObjects()
{
	HRESULT hr;
	// Create vertex buffer for the letters
	int vertexSize = max(sizeof(VERTEXFORMAT_FONT2D), sizeof(VERTEXFORMAT_FONT3D));

	if(FAILED(hr = m_pd3dDevice->CreateVertexBuffer(MAX_NUM_VERTICES * vertexSize,
				   D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0,
				   D3DPOOL_DEFAULT, &m_pVB, NULL))) {
		return hr;
	}

	// Create the state blocks for rendering text
	for(UINT which = 0; which < 2; which++) {
		m_pd3dDevice->BeginStateBlock();
		m_pd3dDevice->SetTexture(0, m_pTexture);

		if(D3DFONT_ZENABLE & m_dwFontFlags)
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		else
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF,         0x08);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL);
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE,   D3DFILL_SOLID);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,   D3DCULL_CCW);
		m_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,    FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPING,         TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE,  FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND,      D3DVBF_DISABLE);
		m_pd3dDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE,        FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE,
									 D3DCOLORWRITEENABLE_RED  | D3DCOLORWRITEENABLE_GREEN |
									 D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
		m_pd3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		if(which == 0)
			m_pd3dDevice->EndStateBlock(&m_pStateBlockSaved);
		else
			m_pd3dDevice->EndStateBlock(&m_pStateBlockDrawText);
	}

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InvalidateDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CGeFont::InvalidateDeviceObjects()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pStateBlockSaved);
	SAFE_RELEASE(m_pStateBlockDrawText);
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DeleteDeviceObjects()
// Desc: Destroys all device-dependent objects
//-----------------------------------------------------------------------------
HRESULT CGeFont::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pTexture);
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: GetTextExtent()
// Desc: Get the dimensions of a text string
//-----------------------------------------------------------------------------
HRESULT CGeFont::GetTextExtent(const TCHAR* strText, SIZE* pSize)
{
	if(NULL == strText || NULL == pSize)
		return E_FAIL;

	FLOAT fRowWidth  = 0.0f;
	FLOAT fRowHeight = (m_fTexCoords[0][3] - m_fTexCoords[0][1]) * m_dwTexHeight;
	FLOAT fWidth     = 0.0f;
	FLOAT fHeight    = fRowHeight;

	while(*strText) {
		TCHAR c = *strText++;

		if(c == _T('\n')) {
			fRowWidth = 0.0f;
			fHeight  += fRowHeight;
		}

		if((c - 32) < 0 || (c - 32) >= 128 - 32)
			continue;

		FLOAT tx1 = m_fTexCoords[c - 32][0];
		FLOAT tx2 = m_fTexCoords[c - 32][2];
		fRowWidth += (tx2 - tx1) * m_dwTexWidth - 2 * m_dwSpacing;

		if(fRowWidth > fWidth)
			fWidth = fRowWidth;
	}

	pSize->cx = (int)fWidth;
	pSize->cy = (int)fHeight;
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DrawTextScaled()
// Desc: Draws scaled 2D text.  Note that x and y are in viewport coordinates
//       (ranging from -1 to +1).  fXScale and fYScale are the size fraction
//       relative to the entire viewport.  For example, a fXScale of 0.25 is
//       1/8th of the screen width.  This allows you to output text at a fixed
//       fraction of the viewport, even if the screen or window size changes.
//-----------------------------------------------------------------------------
HRESULT CGeFont::DrawTextScaled(FLOAT x, FLOAT y, FLOAT z,
								FLOAT fXScale, FLOAT fYScale, DWORD dwColor,
								const TCHAR* strText, DWORD dwFlags)
{
	if(m_pd3dDevice == NULL)
		return E_FAIL;

	// Set up renderstate
	m_pStateBlockSaved->Capture();
	m_pStateBlockDrawText->Apply();
	m_pd3dDevice->SetFVF(D3DFVF_FONT2DVERTEX);
	m_pd3dDevice->SetPixelShader(NULL);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEXFORMAT_FONT2D));

	// Set filter states
	if(dwFlags & D3DFONT_FILTERED) {
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	FLOAT fLineHeight = (m_fTexCoords[0][3] - m_fTexCoords[0][1]) * m_dwTexHeight;

	// Center the text block in the viewport
	if(dwFlags & D3DFONT_CENTERED_X) {
		const TCHAR* strTextTmp = strText;
		float xFinal = 0.0f;

		while(*strTextTmp) {
			TCHAR c = *strTextTmp++;

			if(c == _T('\n'))
				break;  // Isn't supported.

			if((c - 32) < 0 || (c - 32) >= 128 - 32)
				continue;

			FLOAT tx1 = m_fTexCoords[c - 32][0];
			FLOAT tx2 = m_fTexCoords[c - 32][2];
			FLOAT w = (tx2 - tx1) * m_dwTexWidth;
			w *= (fXScale * m_iScreenHeight) / fLineHeight;
			xFinal += w - (2 * m_dwSpacing) * (fXScale * m_iScreenHeight) / fLineHeight;
		}

		x = -xFinal / m_iScreenWidth;
	}

	if(dwFlags & D3DFONT_CENTERED_Y) {
		y = -fLineHeight / m_iScreenHeight;
	}

	FLOAT sx  = (x + 1.0f) * m_iScreenWidth / 2;
	FLOAT sy  = (y + 1.0f) * m_iScreenHeight / 2;
	FLOAT sz  = z;
	FLOAT rhw = 1.0f;
	// Adjust for character spacing
	sx -= m_dwSpacing * (fXScale * m_iScreenHeight) / fLineHeight;
	FLOAT fStartX = sx;
	// Fill vertex buffer
	VERTEXFORMAT_FONT2D* pVertices;
	DWORD         dwNumTriangles = 0L;
	m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	while(*strText) {
		TCHAR c = *strText++;

		if(c == _T('\n')) {
			sx  = fStartX;
			sy += fYScale * m_iScreenHeight;
		}

		if((c - 32) < 0 || (c - 32) >= 128 - 32)
			continue;

		FLOAT tx1 = m_fTexCoords[c - 32][0];
		FLOAT ty1 = m_fTexCoords[c - 32][1];
		FLOAT tx2 = m_fTexCoords[c - 32][2];
		FLOAT ty2 = m_fTexCoords[c - 32][3];
		FLOAT w = (tx2 - tx1) * m_dwTexWidth;
		FLOAT h = (ty2 - ty1) * m_dwTexHeight;
		w *= (fXScale * m_iScreenHeight) / fLineHeight;
		h *= (fYScale * m_iScreenHeight) / fLineHeight;

		if(c != _T(' ')) {
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + h - 0.5f, sz, rhw), dwColor, tx1, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + 0 - 0.5f, sz, rhw), dwColor, tx1, ty1);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + h - 0.5f, sz, rhw), dwColor, tx2, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + 0 - 0.5f, sz, rhw), dwColor, tx2, ty1);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + h - 0.5f, sz, rhw), dwColor, tx2, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + 0 - 0.5f, sz, rhw), dwColor, tx1, ty1);
			dwNumTriangles += 2;

			if(dwNumTriangles * 3 > (MAX_NUM_VERTICES - 6)) {
				// Unlock, render, and relock the vertex buffer
				m_pVB->Unlock();
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
				m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);
				dwNumTriangles = 0L;
			}
		}

		sx += w - (2 * m_dwSpacing) * (fXScale * m_iScreenHeight) / fLineHeight;
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();

	if(dwNumTriangles > 0)
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);

	// Restore the modified renderstates
	m_pStateBlockSaved->Apply();
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: DrawText()
// Desc: Draws 2D text. Note that sx and sy are in pixels
//-----------------------------------------------------------------------------
HRESULT CGeFont::DrawText(FLOAT sx, FLOAT sy, DWORD dwColor,
						  const TCHAR* strText, DWORD dwFlags)
{
	if(m_pd3dDevice == NULL)
		return E_FAIL;

	// Setup renderstate
	m_pStateBlockSaved->Capture();
	m_pStateBlockDrawText->Apply();
	m_pd3dDevice->SetFVF(D3DFVF_FONT2DVERTEX);
	m_pd3dDevice->SetPixelShader(NULL);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEXFORMAT_FONT2D));

	// Set filter states
	if(dwFlags & D3DFONT_FILTERED) {
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	// Center the text block in the viewport
	if(dwFlags & D3DFONT_CENTERED_X) {
		const TCHAR* strTextTmp = strText;
		float xFinal = 0.0f;

		while(*strTextTmp) {
			TCHAR c = *strTextTmp++;

			if(c == _T('\n'))
				break;  // Isn't supported.

			if((c - 32) < 0 || (c - 32) >= 128 - 32)
				continue;

			FLOAT tx1 = m_fTexCoords[c - 32][0];
			FLOAT tx2 = m_fTexCoords[c - 32][2];
			FLOAT w = (tx2 - tx1) *  m_dwTexWidth / m_fTextScale;
			xFinal += w - (2 * m_dwSpacing);
		}

		sx = (m_iScreenWidth - xFinal) / 2.0f;
	}

	if(dwFlags & D3DFONT_CENTERED_Y) {
		float fLineHeight = ((m_fTexCoords[0][3] - m_fTexCoords[0][1]) * m_dwTexHeight);
		sy = (m_iScreenHeight - fLineHeight) / 2;
	}

	// Adjust for character spacing
	sx -= m_dwSpacing;
	FLOAT fStartX = sx;
	// Fill vertex buffer
	VERTEXFORMAT_FONT2D* pVertices = NULL;
	DWORD         dwNumTriangles = 0;
	m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	while(*strText) {
		TCHAR c = *strText++;

		if(c == _T('\n')) {
			sx = fStartX;
			sy += (m_fTexCoords[0][3] - m_fTexCoords[0][1]) * m_dwTexHeight;
		}

		if((c - 32) < 0 || (c - 32) >= 128 - 32)
			continue;

		FLOAT tx1 = m_fTexCoords[c - 32][0];
		FLOAT ty1 = m_fTexCoords[c - 32][1];
		FLOAT tx2 = m_fTexCoords[c - 32][2];
		FLOAT ty2 = m_fTexCoords[c - 32][3];
		FLOAT w = (tx2 - tx1) *  m_dwTexWidth / m_fTextScale;
		FLOAT h = (ty2 - ty1) * m_dwTexHeight / m_fTextScale;

		if(c != _T(' ')) {
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + h - 0.5f, 0.9f, 1.0f), dwColor, tx1, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + 0 - 0.5f, 0.9f, 1.0f), dwColor, tx1, ty1);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + h - 0.5f, 0.9f, 1.0f), dwColor, tx2, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + 0 - 0.5f, 0.9f, 1.0f), dwColor, tx2, ty1);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + w - 0.5f, sy + h - 0.5f, 0.9f, 1.0f), dwColor, tx2, ty2);
			*pVertices++ = InitFont2DVertex(D3DXVECTOR4(sx + 0 - 0.5f, sy + 0 - 0.5f, 0.9f, 1.0f), dwColor, tx1, ty1);
			dwNumTriangles += 2;

			if(dwNumTriangles * 3 > (MAX_NUM_VERTICES - 6)) {
				// Unlock, render, and relock the vertex buffer
				m_pVB->Unlock();
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
				pVertices = NULL;
				m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);
				dwNumTriangles = 0L;
			}
		}

		sx += w - (2 * m_dwSpacing);
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();

	if(dwNumTriangles > 0)
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);

	// Restore the modified renderstates
	m_pStateBlockSaved->Apply();
	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Render3DText()
// Desc: Renders 3D text
//-----------------------------------------------------------------------------
HRESULT CGeFont::Render3DText(const TCHAR* strText, DWORD dwFlags)
{
	if(m_pd3dDevice == NULL)
		return E_FAIL;

	// Setup renderstate
	m_pStateBlockSaved->Capture();
	m_pStateBlockDrawText->Apply();
	m_pd3dDevice->SetFVF(D3DFVF_FONT3DVERTEX);
	m_pd3dDevice->SetPixelShader(NULL);
	m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEXFORMAT_FONT3D));

	// Set filter states
	if(dwFlags & D3DFONT_FILTERED) {
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}

	// Position for each text element
	FLOAT x = 0.0f;
	FLOAT y = 0.0f;

	// Center the text block at the origin (not the viewport)
	if(dwFlags & D3DFONT_CENTERED_X) {
		SIZE sz;
		GetTextExtent(strText, &sz);
		x = -(((FLOAT)sz.cx) / 10.0f) / 2.0f;
	}

	if(dwFlags & D3DFONT_CENTERED_Y) {
		SIZE sz;
		GetTextExtent(strText, &sz);
		y = -(((FLOAT)sz.cy) / 10.0f) / 2.0f;
	}

	// Turn off culling for two-sided text
	if(dwFlags & D3DFONT_TWOSIDED)
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Adjust for character spacing
	x -= m_dwSpacing / 10.0f;
	FLOAT fStartX = x;
	TCHAR c;
	// Fill vertex buffer
	VERTEXFORMAT_FONT3D* pVertices;
	DWORD         dwNumTriangles = 0L;
	m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	while((c = *strText++) != 0) {
		if(c == '\n') {
			x = fStartX;
			y -= (m_fTexCoords[0][3] - m_fTexCoords[0][1]) * m_dwTexHeight / 10.0f;
		}

		if((c - 32) < 0 || (c - 32) >= 128 - 32)
			continue;

		FLOAT tx1 = m_fTexCoords[c - 32][0];
		FLOAT ty1 = m_fTexCoords[c - 32][1];
		FLOAT tx2 = m_fTexCoords[c - 32][2];
		FLOAT ty2 = m_fTexCoords[c - 32][3];
		FLOAT w = (tx2 - tx1) * m_dwTexWidth  / (10.0f * m_fTextScale);
		FLOAT h = (ty2 - ty1) * m_dwTexHeight / (10.0f * m_fTextScale);

		if(c != _T(' ')) {
			*pVertices++ = InitFont3DVertex(D3DXVECTOR3(x + 0, y + 0, 0), D3DXVECTOR3(0, 0, -1), tx1, ty2);
			*pVertices++ = InitFont3DVertex(D3DXVECTOR3(x + 0, y + h, 0), D3DXVECTOR3(0, 0, -1), tx1, ty1);
			*pVertices++ = InitFont3DVertex(D3DXVECTOR3(x + w, y + 0, 0), D3DXVECTOR3(0, 0, -1), tx2, ty2);
			*pVertices++ = InitFont3DVertex(D3DXVECTOR3(x + w, y + h, 0), D3DXVECTOR3(0, 0, -1), tx2, ty1);
			*pVertices++ = InitFont3DVertex(D3DXVECTOR3(x + w, y + 0, 0), D3DXVECTOR3(0, 0, -1), tx2, ty2);
			*pVertices++ = InitFont3DVertex(D3DXVECTOR3(x + 0, y + h, 0), D3DXVECTOR3(0, 0, -1), tx1, ty1);
			dwNumTriangles += 2;

			if(dwNumTriangles * 3 > (MAX_NUM_VERTICES - 6)) {
				// Unlock, render, and relock the vertex buffer
				m_pVB->Unlock();
				m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);
				m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);
				dwNumTriangles = 0L;
			}
		}

		x += w - (2 * m_dwSpacing) / 10.0f;
	}

	// Unlock and render the vertex buffer
	m_pVB->Unlock();

	if(dwNumTriangles > 0)
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, dwNumTriangles);

	// Restore the modified renderstates
	m_pStateBlockSaved->Apply();
	return S_OK;
}
