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
// Title : TestDrive System Driver wrapper
// Rev.  : 7/17/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_DRIVER_H__
#define __SYSTEM_DRIVER_H__
#include "SystemDriverInterface.h"
#include "InterruptService.h"
#include "ThreadManager.h"
#include <string>

class SystemDriver : public ISystemImp, public IMemoryManager
{
public:
	SystemDriver(void);
	virtual ~SystemDriver(void);

	// Identify
	virtual const char *GetDescription(void); // get system description

	// life cycle
	virtual bool Initialize(IMemoryImp *pMem); // system create
	virtual void Release(void);				   // system release

	// memory
	virtual uint64_t GetMemoryBase(void); // get memory start address
	virtual uint64_t GetMemorySize(void); // get memory byte size

	// register
	virtual uint32_t RegRead(uint64_t ulAddress);					// register read
	virtual void	 RegWrite(uint64_t ulAddress, uint32_t dwData); // register write

	// system
	virtual void	 RegisterInterruptService(INTRRUPT_SERVICE routine, void *pPrivate); // register ISR
	virtual void	 EnableInterrupt(bool bEnable = true);								 // enable interrupt
	virtual void	 ClearInterruptPending(void);										 // clear interrupt pending bit
	virtual uint32_t DriverCommand(void *pCommand);										 // post driver specific command

	// memory manager
	virtual IMemoryNative *CreateMemory(uint64_t ulByteSize, uint64_t ulByteAlignment);

protected:
	virtual void InvokeISR(void);

private:
	SystemDriverInterface *m_pNativeDriver;
	IMemoryImp			  *m_pMemImp;
	InterruptService	   m_ISR;
	bool				   m_bMustExit;
	std::string			   m_sSystemDesc;
};
#endif //__SYSTEM_DRIVER_H__
