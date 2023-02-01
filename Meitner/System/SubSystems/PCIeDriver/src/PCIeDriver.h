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
// Title : Driver(PCIe) sub-system
// Rev.  : 2/1/2023 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __PCIE_DRIVER_H__
#define __PCIE_DRIVER_H__
#include "SystemDriverInterface.h"

class PCIeDriver : public SystemDriverInterface {
public:
	PCIeDriver(void);
	virtual ~PCIeDriver(void);

	// native driver interfaces
	virtual bool Initialize(const char* sDeviceName = NULL);
	virtual void Release(void);
	virtual void SetCurrentCard(DWORD dwIndex);
	virtual void RegWrite(UINT64 dwAddress, DWORD dwData);
	virtual DWORD RegRead(UINT64 dwAddress);
	virtual void MemoryWrite(UINT64 dwAddress, BYTE* pData, DWORD dwCount);	// 64bit write operation
	virtual void MemoryRead(UINT64 dwAddress, BYTE* pData, DWORD dwCount);	// 64bit read operation
	virtual void InterruptLock(void);
	virtual void InterruptFree(void);

private:
	OVERLAPPED			m_OverlappedIO;
};

#endif//__PCIE_DRIVER_H__
