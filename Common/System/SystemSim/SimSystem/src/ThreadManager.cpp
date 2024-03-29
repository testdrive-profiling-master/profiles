//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Simulation sub-system
// Rev.  : 11/9/2022 Wed (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "ThreadManager.h"
#include <chrono>

ThreadManager::ThreadManager(void)
{
	m_bBreakable		= true;
	m_bThreadRunning	= false;
}

ThreadManager::~ThreadManager(void)
{
	assert(!m_Thread.joinable());
}

void ThreadManager::SetThreadBreakable(bool bBreakable)
{
	m_bBreakable		= bBreakable;
}

void ThreadManager::ThreadMain(void)
{
	MonitorThread();
	m_bThreadRunning	= false;
}

bool ThreadManager::RunThread(void)
{
	if(m_bThreadRunning) return false;

	m_bThreadRunning	= true;
	m_Thread			= thread{&ThreadManager::ThreadMain, this};
	return m_Thread.joinable();
}

void ThreadManager::KillThread(void)
{
	if(!m_Thread.joinable()) return;

	if(IsRunning()) {
		OnThreadKill(false);
		this_thread::yield();

		for(int i = 0; i < 600; i++) {	// wait for 5 seconds
			if(!IsRunning()) break;

			if(GetKeyState(VK_ESCAPE) < 0) break;

			this_thread::sleep_for(chrono::milliseconds(10));

			if(!((i + 1) % 100) && (i / 100) != 5) {
				LOGI("S/W is down, but Simulation is still busy. Automatically shutdown in %d sec. or press 'ESC' key to exit.\n", 5 - (i / 100));
			}
		}

		if(IsRunning()) {
			OnThreadKill(true);
			this_thread::yield();
		}
	}

	m_Thread.join();
}

bool ThreadManager::IsRunning(void)
{
	return m_bThreadRunning;
}
