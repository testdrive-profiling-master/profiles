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
#ifndef __VIDEO_SOURCE_H__
#define __VIDEO_SOURCE_H__
#include "STDInterface.h"
#include "TD_Semaphore.h"

typedef enum{
	SOURCE_TYPE_ANY,
	SOURCE_TYPE_I420,
	SOURCE_TYPE_YV12,
	SOURCE_TYPE_IYUV,
	SOURCE_TYPE_YUY2,
	SOURCE_TYPE_UYVY,
	SOURCE_TYPE_H264,
	SOURCE_TYPE_MJPG,
	SOURCE_TYPE_RGB8,
	SOURCE_TYPE_RGB565,
	SOURCE_TYPE_RGB555,
	SOURCE_TYPE_RGB24,
	SOURCE_TYPE_RGB32,
}SOURCE_TYPE;

interface VideoSourceCallBack{
	STDMETHOD_(void, SampleVideoSource)(long lSize, BYTE* pBuffer) PURE;
};

class VideoSource{
public:
	VideoSource(void);
	virtual ~VideoSource(void);

	BOOL Create(int iWidth = 0, int iHeight = 0, SOURCE_TYPE type = SOURCE_TYPE_ANY);
	void Release(void);
	BOOL IsValid(void);

	BOOL Start(VideoSourceCallBack* pCB);
	BOOL Stop(void);
	inline void Lock(void)		{m_Sema.Down();}
	inline void Unlock(void)	{m_Sema.Up();}

private:
	Semaphore				m_Sema;
	static DWORD			m_dwRefCount;
};

#endif//__VIDEO_SOURCE_H__
