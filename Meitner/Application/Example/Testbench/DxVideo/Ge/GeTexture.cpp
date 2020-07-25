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
#include "GeTexture.h"

CGeTexture*	CGeTexture::pHead	= NULL;

CGeTexture::CGeTexture(LPDIRECT3DTEXTURE9 pTex)
{
	pTexture	= pTex;
	dwRefCount	= 0;
	pNext		= pHead;
	pPrev		= NULL;
	pHead		= this;
}

CGeTexture::~CGeTexture(void)
{
	SAFE_RELEASE(pTexture);
}

void CGeTexture::AddRef(void)
{
	dwRefCount++;
}

void CGeTexture::Release(void)
{
	dwRefCount--;

	if(!dwRefCount) {
		if(pPrev) pPrev->pNext	= pNext;

		if(pNext) pNext->pPrev	= pPrev;

		if(pHead == this) pHead	= pNext;

		delete this;
	}
}

void* CGeTexture::Lock(DWORD dwLevel)
{
	D3DLOCKED_RECT d3dlr;

	if(!pTexture || FAILED(pTexture->LockRect(dwLevel, &d3dlr, 0, 0)))
		return NULL;

	return d3dlr.pBits;
}

void CGeTexture::Unlock(DWORD dwLevel)
{
	if(pTexture)
		pTexture->UnlockRect(dwLevel);
}

CGeTexture* CGeTexture::CreateTexture(int iWidth, int iHeight, D3DFORMAT format, DWORD dwUsage)
{
	if(!m_pd3dDevice) return NULL;

	LPDIRECT3DTEXTURE9 pTexture;

	if(FAILED(D3DXCreateTexture(m_pd3dDevice, iWidth, iHeight, 0, dwUsage, format, D3DPOOL_MANAGED, &pTexture)))
		return NULL;

	return new CGeTexture(pTexture);
}

CGeTexture* CGeTexture::CreateTextureFromFile(const char* filename, BOOL bMipmap)
{
	if(!m_pd3dDevice || !filename) return NULL;

	LPDIRECT3DTEXTURE9 pTexture;

	/*if(FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, filename, &pTexture)))
		return NULL;*/

	if(FAILED(D3DXCreateTextureFromFileEx(
				  m_pd3dDevice,
				  filename,
				  D3DX_DEFAULT, D3DX_DEFAULT,	// width, height
				  D3DX_DEFAULT,					// MipLevels
				  0,							// Usage
				  D3DFMT_UNKNOWN,				// Format
				  D3DPOOL_MANAGED,				// Pool
				  D3DX_DEFAULT,					// Filter
				  D3DX_DEFAULT,					// MipFilter
				  0,							// ColorKey
				  NULL,							// pSrcInfo
				  NULL,							// pPalette
				  &pTexture)))
		return NULL;

	if(bMipmap) pTexture->GenerateMipSubLevels();

	return new CGeTexture(pTexture);
}

