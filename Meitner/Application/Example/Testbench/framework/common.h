//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
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
// Rev.  : 2/20/2023 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __COMMON_H__
#define __COMMON_H__
#include "STDInterface.h"
#include "ddk/SystemDDK.h"
#include "TestDriver.h"
#include "SystemConfig.h"
#include "VirtualDisplayConfig.h"
#include "MmapViewLog.h"
#include "TD_Semaphore.h"

class TestDriveResource {
public:
	TestDriveResource(void);
	virtual ~TestDriveResource(void);

protected:
	typedef struct {
		struct {
			SYSTEM_CONFIG*			pSystem;
			DisplayConfig*			pDisplay;
		} Config;
		struct {
			ITestDriverMemory*		pSystem;
			ITestDriverMemory*		pDisplay;
		} Memory;
	} __RESOURCE;

	static	__RESOURCE		m_TestDrive;

private:
	static DWORD		m_dwTestDriveRefCount;
};

#ifdef WIN32
#define	ELAPSED_TIME		LONGLONG
#else
#define	ELAPSED_TIME		struct timeval
#endif
float GetElapsedTime(ELAPSED_TIME& PrevTime, BOOL bInit = FALSE);
void LOG(const char* sFormat, ...);

#define	printf	LOG

#endif//__COMMON_H__
