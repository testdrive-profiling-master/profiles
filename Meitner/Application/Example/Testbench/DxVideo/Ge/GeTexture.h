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
#ifndef __GE_TEXTURE_H__
#define __GE_TEXTURE_H__
#include "GePrimitive.h"

struct VERTEXFORMAT_IMAGE
{
	D3DXVECTOR3 position; // The position
	FLOAT       tu, tv;   // The texture coordinates
};
#define D3DFVF_D3DXIMAGEVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

class CGeTexture : public CGePrimitive{
public:
	CGeTexture(LPDIRECT3DTEXTURE9 pTex = NULL);
	virtual ~CGeTexture(void);
	void AddRef(void);
	void Release(void);

	void* Lock(DWORD dwLevel = 0);
	void Unlock(DWORD dwLevel = 0);

	static CGeTexture* CreateTexture(int iWidth, int iHeight, D3DFORMAT format = D3DFMT_X8R8G8B8, DWORD dwUsage = 0);
	static CGeTexture* CreateTextureFromFile(const char *filename, BOOL bMipmap = TRUE);

	LPDIRECT3DTEXTURE9		pTexture;

private:
	DWORD					dwRefCount;
	CGeTexture*				pPrev;
	CGeTexture*				pNext;
	static CGeTexture*		pHead;
};

typedef CGeTexture* LPGETEXTRUE;

#define TEXTURE_LOAD_FILE				CGeTexture::CreateTextureFromFile
#define TEXTURE_CREATE					CGeTexture::CreateTexture

#endif//__GE_TEXTURE_H__
