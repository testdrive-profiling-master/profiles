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
#ifndef __BUS_SLAVE_H__
#define __BUS_SLAVE_H__
#include "SimEngine.h"
#include "dpi_interfaces.h"

class BusSlave :
		public SimInstance,
		public BUS_SLAVE_INTERFACE
{
public:
	BusSlave(DWORD dwAddrBase, DWORD dwAddrHigh);
	virtual ~BusSlave(void);

	//// S/W master interface
	virtual BOOL OnRun(void);
	void Write(DWORD dwAddress, DWORD dwData);
	DWORD Read(DWORD dwAddress);

	//// H/W master interface
	STDMETHOD_(BOOL, RequestWrite)(DWORD dwAddr, DWORD dwData);
	STDMETHOD_(BOOL, WaitWrite)(void);
	STDMETHOD_(BOOL, RequestRead)(DWORD dwAddr);
	STDMETHOD_(BOOL, WaitRead)(DWORD& dwData);

	// H/W slave interface
	STDMETHOD_(BUS_SALVE_PACKET*, GetWrite)(void);		// get write
	STDMETHOD_(void, WriteAck)(void);					// write ack to S/W
	STDMETHOD_(BUS_SALVE_PACKET*, GetRead)(void);		// get read
	STDMETHOD_(void, ReadAck)(void);					// read ack to S/W

	BOOL IsValidAddress(DWORD dwAddress);

	static BusSlave* FindSlave(DWORD dwAddress);

private:
	static BusSlave*	m_pHead;
	BusSlave*			m_pNext;

	static Semaphore	m_LockSW;
	Semaphore			m_LockBus;
	Semaphore			m_LockBusAck;

	// address range
	DWORD				m_dwAddrBase;
	DWORD				m_dwAddrHigh;

	struct{
		BOOL				bEnable;
		BOOL				bWait;
		BUS_SALVE_PACKET	packet;
	}m_Read, m_Write;
};
#endif//__BUS_SLAVE_H__
