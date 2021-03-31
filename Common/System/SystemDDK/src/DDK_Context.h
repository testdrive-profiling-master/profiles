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
// Title : Common profiles
// Rev.  : 3/31/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __DDK_CONTEXT_H__
#define __DDK_CONTEXT_H__
#include "DDK_Common.h"

class DDKContext : public DDK
{
	virtual ~DDKContext(void);
public:
	DDKContext(void);

	inline BOOL IsInitialized(void)		{return m_pSystem != NULL;}

	// Identify
	virtual const char* GetSystemDescription(void);

	// life cycle
	virtual void Release(void);		// Release display object

	// memory
	virtual DWORD GetMemoryBase(void);
	virtual void* GetMemoryPointer(DWORD dwPhyAddress, DWORD dwByteSize = 0);
	virtual BOOL MakeMemoryDump(const char* sFileName = NULL);
	virtual BOOL LoadMemoryDump(const char* sFileName = NULL);

	// register
	virtual DWORD RegRead(DWORD dwAddress);
	virtual void RegWrite(DWORD dwAddress, DWORD dwData);

	// system
	virtual void RegisterInterruptService(DDK_INTRRUPT_SERVICE routine, void* pPrivate = NULL);
	virtual void EnableInterrupt(BOOL bEnable = TRUE);
	virtual void ClearInterruptPending(BOOL bReleaseWait = FALSE);
	virtual void WaitInterruptDone(void);

private:
	ISystem*	m_pSystem;
	Semaphore	m_Semaphore;
};

#endif//__DDK_CONTEXT_H__
