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
#include "GeCamera.h"

CGeCamera::CGeCamera(void)
{
	m_pTexture	= NULL;
	m_pPost		= NULL;
	m_iWidth	= 0;
	m_iHeight	= 0;
}

CGeCamera::~CGeCamera(void)
{
	ReleaseAll();
}

BOOL CGeCamera::Create(int iWidth, int iHeight)
{
	if(!m_Source.Create(iWidth, iHeight)) return FALSE;

	m_iWidth	= iWidth;
	m_iHeight	= iHeight;

	if(FAILED(D3DXCreateTexture(m_pd3dDevice, iWidth, iHeight, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &m_pTexture)))
		return FALSE;

	m_pTexture->SetAutoGenFilterType(D3DTEXF_LINEAR);
	m_Source.Start(this);
	return TRUE;
}

void CGeCamera::ReleaseAll(void)
{
	RegisterPost(NULL);
	m_Source.Stop();
	Lock();
	SAFE_RELEASE(m_pTexture);
	Unlock();
}

void CGeCamera::RegisterPost(CameraPost* pPost)
{
	m_pPost	= pPost;
}

void CGeCamera::SampleVideoSource(long lSize, BYTE* pBuffer)
{
	if(!m_pTexture || m_bDeviceLost || !m_bActive) return;

	D3DLOCKED_RECT d3dlr;
	Lock();

	if(SUCCEEDED(m_pTexture->LockRect(0, &d3dlr, 0, 0))) {
		memcpy(d3dlr.pBits, pBuffer, lSize);
		m_pTexture->UnlockRect(0);
		m_pTexture->GenerateMipSubLevels();

		if(m_pPost) m_pPost->CameraPostProcess(this, lSize, pBuffer);
	}

	Unlock();
}
