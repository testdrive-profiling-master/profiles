//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 8/7/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "common.h"
#include "MmapViewLog.inl"

DWORD						  TestDriveResource::m_dwTestDriveRefCount = 0;

TestDriveResource::__RESOURCE TestDriveResource::m_TestDrive = {{NULL, NULL}, {NULL, NULL}};

TestDriveResource::TestDriveResource(void)
{
	if (!m_dwTestDriveRefCount) {
		if (m_TestDrive.Memory.pSystem = TestDriver_GetMemory(TESTDRIVE_MEMORY_SYSTEM_NAME))
			m_TestDrive.Config.pSystem = (SYSTEM_CONFIG *)m_TestDrive.Memory.pSystem->GetConfig();

		if (m_TestDrive.Memory.pDisplay = TestDriver_GetMemory(TESTDRIVE_MEMORY_DISPLAY_NAME))
			m_TestDrive.Config.pDisplay = (DisplayConfig *)m_TestDrive.Memory.pDisplay->GetConfig();
	}

	m_dwTestDriveRefCount++;
}

TestDriveResource::~TestDriveResource(void)
{
	m_dwTestDriveRefCount--;

	if (!m_dwTestDriveRefCount) {
		m_TestDrive.Config.pSystem	= NULL;
		m_TestDrive.Config.pDisplay = NULL;
		SAFE_RELEASE(m_TestDrive.Memory.pSystem);
		SAFE_RELEASE(m_TestDrive.Memory.pDisplay);
	}
}

float GetElapsedTime(ELAPSED_TIME &PrevTime, BOOL bInit)
{
	float fElapsedTime;
#ifdef WIN32
	{
		static LONGLONG llQPFTicksPerSec = 0;
		LONGLONG		CurTime;

		if (!llQPFTicksPerSec) {
			QueryPerformanceFrequency((LARGE_INTEGER *)&llQPFTicksPerSec);
		}

		QueryPerformanceCounter((LARGE_INTEGER *)&CurTime);
		fElapsedTime = (float)(CurTime - PrevTime) / llQPFTicksPerSec;

		if (bInit)
			PrevTime = CurTime;
	}
#else
	{
		static struct timeval cur;
		gettimeofday(&cur, NULL);
		fElapsedTime = (cur.tv_sec - PrevTime.tv_sec) + (cur.tv_usec - PrevTime.tv_usec) / 1000000.f;

		if (bInit)
			PrevTime = CurTime;
	}
#endif
	return fElapsedTime;
}

void LOG(const char *sFormat, ...)
{
	va_list ap;
	va_start(ap, sFormat);
	vprintf(sFormat, ap);
	va_end(ap);
	fflush(stdout); // flushing console output buffer always
}
