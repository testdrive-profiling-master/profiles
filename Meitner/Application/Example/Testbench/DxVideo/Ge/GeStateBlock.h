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
#ifndef __GE_STATE_BLOCK_H__
#define __GE_STATE_BLOCK_H__
#include "GePrimitive.h"

typedef enum{
	STATE_ALPHA_BLEND_NONE,
	STATE_ALPHA_BLEND_NORMAL,
	STATE_ALPHA_BLEND_LIGHT,
}STATE_ALPHA_BLEND;

typedef enum{
	STATE_TEXTURE_FILTER_NONE,
	STATE_TEXTURE_FILTER_LINEAR,
	STATE_TEXTURE_FILTER_HIGH,
}STATE_TEXTURE_FILTER;

typedef enum{
	STATE_TEXTURE_NONE,
	STATE_TEXTURE_NORMAL,
	STATE_TEXTURE_MODULATE_A,
	STATE_TEXTURE_MODULATE_ARGB,
}STATE_TEXTURE;

class CGeStateBlock : public CGePrimitive
{
public:
	CGeStateBlock(void);
	virtual ~CGeStateBlock(void);

	void ReleaseAll(void);

	void Begin(void);
	void End(void);
	void Capture(void);
	void Apply(void);

	void SetAlphaBlend(STATE_ALPHA_BLEND mode);
	void SetTextureFilter(STATE_TEXTURE_FILTER mode);
	void SetTexture(int iIndex, STATE_TEXTURE mode);
	void SetTextureFactor(DWORD dwFactor);

protected:
	LPDIRECT3DSTATEBLOCK9		m_pStateBlock;

};

#endif//__GE_STATE_BLOCK_H__
