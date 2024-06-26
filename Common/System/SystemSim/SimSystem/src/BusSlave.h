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
// Title : Common profiles
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __BUS_SLAVE_H__
#define __BUS_SLAVE_H__
#include "SimEngine.h"
#include "dpi_interfaces.h"

class BusSlave : public SimInstance, public BUS_SLAVE_INTERFACE
{
public:
	BusSlave(uint64_t lAddrBase, uint64_t lAddrHigh);
	virtual ~BusSlave(void);

	//// S/W master interface
	virtual bool OnRun(void);
	void		 Write(uint64_t lAddrBase, uint32_t dwData);
	uint32_t	 Read(uint64_t lAddrBase);

	//// H/W master interface
	virtual bool RequestWrite(uint64_t lAddr, uint32_t dwData);
	virtual bool WaitWrite(void);
	virtual bool RequestRead(uint64_t lAddr);
	virtual bool WaitRead(uint32_t &dwData);

	// H/W slave interface
	virtual BUS_SALVE_PACKET *GetWrite(void); // get write
	virtual void			  WriteAck(void); // write ack to S/W
	virtual BUS_SALVE_PACKET *GetRead(void);  // get read
	virtual void			  ReadAck(void);  // read ack to S/W

	bool					  IsValidAddress(uint64_t lAddress);

	static BusSlave			 *FindSlave(uint64_t lAddress);

private:
	static BusSlave *m_pHead;
	BusSlave		*m_pNext;

	static Semaphore m_LockSW;
	Semaphore		 m_LockBus;
	Semaphore		 m_LockBusAck;

	// address range
	uint64_t m_lAddrBase;
	uint64_t m_lAddrHigh;

	struct {
		bool			 bEnable;
		bool			 bWait;
		BUS_SALVE_PACKET packet;
	} m_Read, m_Write;
};
#endif //__BUS_SLAVE_H__
