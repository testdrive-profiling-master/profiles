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
// Title : System remote
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "ThreadManager.h"
#include <process.h>

ThreadManager::ThreadManager(void)
{
	m_Thread			= NULL;
	m_bThreadRunning	= FALSE;
}

ThreadManager::~ThreadManager(void)
{
	KillThread();
}

DWORD WINAPI ThreadManager::thBootStrap(ThreadManager* pManager)
{
	pManager->m_bThreadRunning = TRUE;
	pManager->MonitorThread();
	pManager->m_bThreadRunning = FALSE;
	return 0;
}

BOOL ThreadManager::RunThread(void)
{
	if(m_bThreadRunning) return FALSE;

	m_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thBootStrap, this, 0, NULL);
	return m_Thread != NULL;
}

void ThreadManager::KillThread(void)
{
	if(!m_Thread) return;

	OnThreadKill();
	WaitForSingleObject(m_Thread, 10000);	// wait 10 seconds
	CloseHandle(m_Thread);
	m_Thread = NULL;
}
