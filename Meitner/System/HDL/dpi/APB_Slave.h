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
// Title : Common DPI
// Rev.  : 11/5/2019 Tue (clonextop@gmail.com)
//================================================================================
#ifndef __APB_SLAVE_H__
#define __APB_SLAVE_H__
#include "dpi_common.h"

class APB_Slave : public SelfDestory
{
public:
	APB_Slave(const char* sTitle, DWORD dwAddrBase, DWORD dwAddrHigh);
	virtual ~APB_Slave(void);

	void BusSignal(
			BYTE nRST,
			BYTE& PSEL, BYTE& PENABLE, BYTE& PWRITE, DWORD& PADDR, DWORD& PWDATA, DWORD& PSTRB,
			DWORD PRDATA, BYTE PREADY, BYTE PSLVERR);

protected:
	typedef enum {
		BUS_STATE_IDLE,
		BUS_STATE_START,
		BUS_STATE_DATA,
	} BUS_STATE;

private:
	BUS_SLAVE_INTERFACE*	m_pSlave;
	BUS_STATE				m_state;
	BUS_SALVE_PACKET*		m_pPacket;
	BOOL					m_bWrite;
	DWORD					m_dwTime;
	SystemLog				Log;
};

#endif//__APB_SLAVE_H__
