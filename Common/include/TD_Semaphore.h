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
#ifndef __TD_SEMAPHORE_H__
#define __TD_SEMAPHORE_H__
#include "STDInterface.h"
#if defined(__linux__)
#include <assert.h>
#endif

class Semaphore{
#if defined(__linux__)
	int					m_Sema;
#else
	HANDLE				m_Sema;
#endif

public:
	Semaphore(int iInitValue){
	#if defined(__linux__)
		m_Sema = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
		assert(m_Sema != -1);
		semctl(m_Sema, 0, SETVAL, iInitValue);
	#else
		m_Sema	= CreateSemaphore(NULL, iInitValue, LONG_MAX, NULL);
	#endif
	}
	virtual ~Semaphore(void){
	#if defined(__linux__)
		int arg = 1;
		semctl(m_Sema, 0, IPC_RMID, arg);
	#else
		CloseHandle(m_Sema);
	#endif
	}
	int Down(void){
	#if defined(__linux__)
		static struct sembuf	Semaphore::__Down	= { 0, -1, SEM_UNDO };
		return semop(m_Sema, &__Down, 1);
	#else
		return WaitForSingleObject(m_Sema, INFINITE);
	#endif
	}
	int Up(void){
	#if defined(__linux__)
		static struct sembuf	Semaphore::__Up		= { 0,  1, SEM_UNDO };
		return semop(m_Sema, &__Up, 1);
	#else
		return ReleaseSemaphore(m_Sema, 1, NULL);
	#endif
	}
};

#endif//__TD_SEMAPHORE_H__
