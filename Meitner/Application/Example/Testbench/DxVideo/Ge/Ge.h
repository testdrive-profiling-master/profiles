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
#ifndef __GE_H__
#define __GE_H__
#include "Resource.h"
#include "GePrimitive.h"
#include "GeBlockWinKey.h"
#include "GeCamera.h"
//#include "GeCameraView.h"
#include "GeImage.h"
#include "GeStateBlock.h"
#include "GeFont.h"
#include "GeRenderview.h"
#include "GeSystemMessage.h"

const static char* G_ENGINE_NAME_SPACE = "Ge";

typedef enum{
	STATE_BLOCK_BACKGROUND,
	STATE_BLOCK_IMAGE,
	STATE_BLOCK_IMAGE_BLNED,
	STATE_BLOCK_IMAGE_MODULATE,
	STATE_BLOCK_LIGHT,
	STATE_BLOCK_LIGHT_BLEND,
	STATE_BLOCK_CAMERA,
	STATE_BLOCK_SIZE
}STATE_BLOCK;

typedef enum{
	DNN_TYPE_1,
	DNN_TYPE_2,
	DNN_TYPE_SIZE
}DNN_TYPE;

void SysLog(DWORD dwColor, const char* sFormat, ...);

class CGe : public CGePrimitive, public CameraPost
{
public:
	CGe(void);
	virtual ~CGe(void);

public:		// �������̽�
	inline CGeSystemMessage& Log(void)	{return m_cSystemMsg;}
	HRESULT CaptureScreenShot(void);	// ��ũ�� ��
	HRESULT ToggleFullScreen(BOOL bToggle=TRUE);		// ��� Ǯ��ũ��
	HRESULT Render3DEnvironment(void);			// ������
	LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	// �޽��� ó��
	HRESULT Create(void);	// ����
	void ReleaseAll(void);	// ����

protected:	// ������
	HRESULT Render(void);			// ������ ����
	BOOL Main(void);				// ���� ���� ����

	HRESULT InitDeviceObjects(void);
	HRESULT DeleteDeviceObjects(void);
	HRESULT InvalidateDeviceObjects(void);
	HRESULT RestoreDeviceObjects(void);
	HRESULT Reset3DEnvironment(void);

	STDMETHOD_(BOOL, CameraPostProcess)(CGeCamera* pCamera, long lSize, BYTE* pBuffer);

protected:	// �ɹ�
	CGeSystemMessage	m_cSystemMsg;
	CGeStateBlock		m_StateBlock[STATE_BLOCK_SIZE];
	CGeImage			m_imTitle;
	CGeImage			m_imSidePannel[DNN_TYPE_SIZE];
	CGeImage			m_imWhiteBox;
	CGeImage			m_imPreview;
	CGeCamera			m_imCamera;
	CGeImage			m_imFrameFullScreen;
	CGeImage			m_imFrameProcess;
	CGeImage			m_imRedLight;
	CGeImage			m_imTarget;
	CGeImage			m_imTargets;
	CGeImage			m_imBoard;
	D3DXMATRIX			m_matW, m_matIW, m_matIdentity;
	CGeFont*			m_pFont;
	CGeFont*			m_pResultFont;
	CGeFont*			m_pRankFont;
	float				m_fChangeImpact;
	DNN_TYPE			m_DNNType;
	float				m_fDNNMax;
	float				m_fProcessingTime;
	BOOL				m_bFound;
	DWORD				m_ColorBarTable[1024];		// ����Ʈ�� �÷� ���̺�

	RECT				m_rcWindowBounds;			// ������ ũ��
	BOOL				m_bDeviceObjectsRestored;	// ��ġ �������� ����
	BOOL				m_bDeviceObjectsInited;		// ��ġ�� �ʱ�ȭ �Ǿ����� ����
	CGeBlockWinKey		m_NoWinKey;					// ������ Ű �Է��� �����մϴ�
};

#endif//__GE_H__
