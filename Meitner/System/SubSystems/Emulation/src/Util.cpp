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
// Title : Emulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "Util.h"

//-----------------------------------------------------------
// Semaphore
Semaphore::Semaphore(int iInitValue)
{
#if defined(__linux__)
	m_Sema = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	EGL_ASSERT(m_Sema != -1);
	semctl(m_Sema, 0, SETVAL, iInitValue);
#else
	m_Sema	= CreateSemaphore(NULL, iInitValue, LONG_MAX, NULL);
#endif
}

#if defined(__linux__)
struct sembuf	Semaphore::m_Down	= { 0, -1, SEM_UNDO };
struct sembuf	Semaphore::m_Up		= { 0,  1, SEM_UNDO };
#endif

Semaphore::~Semaphore(void)
{
#if defined(__linux__)
	int arg = 1;
	semctl(m_Sema, 0, IPC_RMID, arg);
#else
	CloseHandle(m_Sema);
#endif
}

int Semaphore::Down(void)
{
#if defined(__linux__)
	return semop(m_Sema, &m_Down, 1);
#else
	return WaitForSingleObject(m_Sema, INFINITE);
#endif
}

int Semaphore::Up(void)
{
#if defined(__linux__)
	return semop(m_Sema, &m_Up, 1);
#else
	return ReleaseSemaphore(m_Sema, 1, NULL);
#endif
}

void LOGI(char* fmt, ...)
{
	printf("[SimSystem] ");
	{
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
	fflush(stdout);
}
