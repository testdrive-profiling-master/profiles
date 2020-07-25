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
// Title : Simulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_SIM_H__
#define __SYSTEM_SIM_H__
#include "TestDriver.h"
#include "SimEngine.h"
#include "ddk/SystemHAL.h"
#include "VirtualDisplayConfig.h"
#include "NativeMemory.h"

class SystemSim :
		public ISystemImp,
		public IMemoryManager
{
public:
	SystemSim(void);
	virtual ~SystemSim(void);

	// Identify
	STDMETHOD_(const char*, GetDescription)(void);							// get system description

	// life cycle
	STDMETHOD_(BOOL, Initialize)(IMemoryImp* pMem);							// system initialize
	STDMETHOD_(void, Release)(void);											// release all system resources
	
	// memory
	STDMETHOD_(DWORD, GetMemoryBase)(void);									// get memory start address
	STDMETHOD_(void*, GetMemoryPointer)(DWORD dwPhyAddress, DWORD dwByteSize = 0);		// get virtual pointer from physical address

	// register
	STDMETHOD_(DWORD, RegRead)(DWORD dwAddress);								// register read
	STDMETHOD_(void, RegWrite)(DWORD dwAddress, DWORD dwData);					// register write

	// system
	STDMETHOD_(void, RegisterInterruptService)(INTRRUPT_SERVICE routine);	// 인터럽트 서비스 루틴 등록
	STDMETHOD_(void, EnableInterrupt)(BOOL bEnable = TRUE);					// 인터럽트 서비스 루틴 활성화
	STDMETHOD_(void, ClearInterruptPending)(void);							// 인터럽트 서비스 팬딩 클리어

	// memory manager
	STDMETHOD_(IMemoryNative*, CreateMemory)(DWORD dwByteSize, DWORD dwByteAlignment, BOOL bDMA = FALSE);

private:
	SimEngine*				m_pSim;
	IMemoryImp*				m_pMemImp;
};
#endif//__SYSTEM_SIM_H__
