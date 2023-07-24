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
#ifndef __BASE_SYSTEM_CONFIG_H__
#define __BASE_SYSTEM_CONFIG_H__
#include "STDInterface.h"

#ifdef USE_TESTDRIVE
//-----------------------------------------------------------------
// Base System
//-----------------------------------------------------------------
	typedef enum{
		SYSTEM_TYPE_SIMULATOR,
		SYSTEM_TYPE_EMULATOR,
		SYSTEM_TYPE_DRIVER,
		SYSTEM_TYPE_SIZE
	}SYSTEM_TYPE;
	
	typedef enum{
		TESTDRIVE_CMD_SET_TITLE,
		TESTDRIVE_CMD_CLEAR_LOG,
		TESTDRIVE_CMD_RUN_PROFILE,
		TESTDRIVE_CMD_GET_DOCUMENT_HANDLE,
	}TESTDRIVE_CMD;
	
	typedef struct{
		DWORD					dwMemorySize;
		SYSTEM_TYPE				dwSystemType;
		HWND					hSystemManager;
	}BASE_SYSTEM_CONFIG;
	
//-----------------------------------------------------------------
// System chart
//-----------------------------------------------------------------
	typedef enum{
		CHART_CMD_CREATE_SERIE,
		CHART_CMD_REMOVE_SERIE,
		CHART_CMD_CLEAR_SERIE,
		CHART_CMD_SET_NAME,
		CHART_CMD_ADD_POINT,
		CHART_CMD_ADD_POINT_ARRAY,
		CHART_CMD_SET_LABEL,
		CHART_CMD_SET_WIDTH,
		CHART_CMD_SET_SMOOTH,
		CHART_CMD_SET_FOREGROUND,
	}CHART_CMD;
	
	typedef struct{
		DWORD					dwMemorySize;
		HWND					hWnd;
	}SYSTEM_CHART_CONFIG;
#endif

#endif//__BASE_SYSTEM_CONFIG_H__
