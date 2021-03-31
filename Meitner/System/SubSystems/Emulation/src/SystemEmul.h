//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 3/31/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_EMUL_H__
#define __SYSTEM_EMUL_H__
#include "common.h"
#include "InterruptService.h"
#include "Emulation.h"
#include "ThreadManager.h"
#include <string>

class SystemEmul :
		public ISystemImp,
		public IMemoryManager,
		protected Emulation,
		public ThreadManager
{
public:
	SystemEmul(void);
	virtual ~SystemEmul(void);

	// Identify
	virtual const char* GetDescription(void);							// get system description

	// life cycle
	virtual BOOL Initialize(IMemoryImp* pMem);							// system create
	virtual void Release(void);											// system release
	
	// memory
	virtual DWORD GetMemoryBase(void);									// get memory start address
	virtual void* GetMemoryPointer(DWORD dwPhyAddress, DWORD dwByteSize = 0);	// get virtual pointer from physical address

	// register
	virtual DWORD RegRead(DWORD dwAddress);								// register read
	virtual void RegWrite(DWORD dwAddress, DWORD dwData);					// register write

	// system
	virtual void RegisterInterruptService(INTRRUPT_SERVICE routine);	// register ISR
	virtual void EnableInterrupt(BOOL bEnable = TRUE);					// enable interrupt
	virtual void ClearInterruptPending(void);							// clear interrupt pending bit

	// memory manager
	virtual IMemoryNative* CreateMemory(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE);

protected:
	virtual void MonitorThread(void);
	virtual void OnThreadKill(void);
	virtual void InvokeMainLoop(void);
	virtual void InvokeISR(void);

private:
	IMemoryImp*			m_pMemImp;
	InterruptService	m_ISR;
	Semaphore			m_ProcessLock;
	BOOL				m_bMustExit;
	std::string			m_sSystemDesc;
};
#endif//__SYSTEM_EMUL_H__
