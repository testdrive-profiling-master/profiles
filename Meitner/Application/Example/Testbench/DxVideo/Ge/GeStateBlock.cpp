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
#include "GeStateBlock.h"

CGeStateBlock::CGeStateBlock(void)
{
	m_pStateBlock		= NULL;
}

CGeStateBlock::~CGeStateBlock(void)
{
	ReleaseAll();
}

void CGeStateBlock::ReleaseAll(void)
{
	SAFE_RELEASE(m_pStateBlock);
}

void CGeStateBlock::Begin(void)
{
	ReleaseAll();

	if(m_pd3dDevice) {
		m_pd3dDevice->BeginStateBlock();
		// default setting
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPING,			TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE,			FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,		FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE,			FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING,			FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE,			D3DZB_FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,		FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
		m_pd3dDevice->SetRenderState(D3DRS_VERTEXBLEND,		D3DVBF_DISABLE);
		SetAlphaBlend(STATE_ALPHA_BLEND_NONE);
		SetTextureFilter(STATE_TEXTURE_FILTER_NONE);
		SetTexture(0, STATE_TEXTURE_NORMAL);
	}
}
void CGeStateBlock::End(void)
{
	if(m_pd3dDevice)
		m_pd3dDevice->EndStateBlock(&m_pStateBlock);
}

void CGeStateBlock::Capture(void)
{
	if(m_pStateBlock) m_pStateBlock->Capture();
}

void CGeStateBlock::Apply(void)
{
	if(m_pStateBlock) m_pStateBlock->Apply();
}

void CGeStateBlock::SetAlphaBlend(STATE_ALPHA_BLEND mode)
{
	if(m_pd3dDevice)
		switch(mode) {
		case STATE_ALPHA_BLEND_NONE:
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			break;

		case STATE_ALPHA_BLEND_NORMAL:
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			break;

		case STATE_ALPHA_BLEND_LIGHT:
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			break;
		}
}

void CGeStateBlock::SetTextureFilter(STATE_TEXTURE_FILTER mode)
{
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	if(m_pd3dDevice)
		switch(mode) {
		case STATE_TEXTURE_FILTER_NONE:
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			break;

		case STATE_TEXTURE_FILTER_LINEAR:
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			break;

		case STATE_TEXTURE_FILTER_HIGH:
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			break;
		}
}

void CGeStateBlock::SetTexture(int iIndex, STATE_TEXTURE mode)
{
	if(m_pd3dDevice)
		switch(mode) {
		case STATE_TEXTURE_NONE:
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLOROP,   D3DTOP_DISABLE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
			break;

		case STATE_TEXTURE_NORMAL:
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			break;

		case STATE_TEXTURE_MODULATE_A:
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			//m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAOP,   D3DTOP_BLENDFACTORALPHA);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			SetTextureFactor(0xFFFFFFFF);
			break;

		case STATE_TEXTURE_MODULATE_ARGB:
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_COLORARG2, D3DTA_TFACTOR);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			m_pd3dDevice->SetTextureStageState(iIndex, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
			SetTextureFactor(0xFFFFFFFF);
			break;
		}
}

void CGeStateBlock::SetTextureFactor(DWORD dwFactor)
{
	if(m_pd3dDevice)
		m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, dwFactor);
}
