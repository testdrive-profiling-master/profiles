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
// Title : Common profiles
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __VIRTUAL_DISPLAY_CONFIG_H__
#define __VIRTUAL_DISPLAY_CONFIG_H__
#include "STDInterface.h"

typedef union{
	DWORD		color;
	struct{
		BYTE	r,g,b,a;
	};
}DisplayColor;

typedef enum{
	DISPLAY_FORMAT_ALPHA,			// A8
	DISPLAY_FORMAT_LUMINANCE,		// L8
	DISPLAY_FORMAT_LUMINANCE_ALPHA,	// L8A8
	DISPLAY_FORMAT_RGB_565,			// R5G6B5
	DISPLAY_FORMAT_RGBA_4444,		// R4G4B4A4
	DISPLAY_FORMAT_RGBA_5551,		// R5G5B5A1
	DISPLAY_FORMAT_RGB_888,			// R8G8B8
	DISPLAY_FORMAT_ABGR_8888,		// A8B8G8R8
	DISPLAY_FORMAT_RGBA_8888,		// R8G8B8A8
	DISPLAY_FORMAT_ARGB_8888,		// A8R8G8B8
}DISPLAY_FORMAT;

typedef enum{
	DISPLAY_CMD_SAVE_TO_FILE,		// param : {filename}
	DISPLAY_CMD_LOAD_FROM_FILE,		// param : {format, base_addres, byte_stride, filename}, return BOOLEAN('TRUE') if successful
	DISPLAY_CMD_SET_FOREGROUND,		// param : none
}DISPLAY_CMD;

typedef struct{
	DWORD			dwAddress;			// Offset address
	volatile BOOL	bUpdate;			// Update screen
}DisplayBuffer;

#ifdef TESTDRIVE_DOCUMENT
typedef BOOL (*DisplayDecoder)(void* pScreen, const void* pMem);	// framebuffer decoder function template
#endif

typedef struct{
	const DWORD		dwMemorySize;		// Allocated memory byte size (do not fix it.)
	HWND			hWndHandle;			// Virtual Display document window handle (64bit is not support.)
	int				iWidth;				// width
	int				iHeight;			// height
	DISPLAY_FORMAT	ColorFormat;		// color format
	DWORD			dwByteStride;		// width byte stride
	BOOL			bReverse;			// reverse screen
	BOOL			bShowFrontBuffer;	// Front(TRUE)/Back(FALSE) buffer's status (do not fix it.)
	BOOL			bFramePause;		// stop frame buffer change
	BOOL			bMovieSave;			// Saving to movie file... (do not fix it.)

	DisplayBuffer	Front;				// Front buffer
	DisplayBuffer	Back;				// Back buffer

#ifdef TESTDRIVE_DOCUMENT
	DisplayDecoder	decoder;			// virtual screen decoding override function pointer (Use TestDrive document implementation only.)
#endif
}DisplayConfig;

#endif//__VIRTUAL_DISPLAY_CONFIG_H__
