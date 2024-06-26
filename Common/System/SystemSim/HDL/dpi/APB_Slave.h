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
// Title : Common DPI
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __APB_SLAVE_H__
#define __APB_SLAVE_H__
#include "dpi_lib.h"

class APB_Slave : public SelfDestory
{
public:
	APB_Slave(const char *sTitle, uint64_t lAddrBase, uint64_t lAddrHigh, uint32_t dwTimeout);
	virtual ~APB_Slave(void);

	void BusSignal(
		uint8_t nRST, uint8_t &PSEL, uint8_t &PENABLE, uint8_t &PWRITE, uint64_t &PADDR, uint32_t &PWDATA, uint32_t &PSTRB, uint32_t PRDATA,
		uint8_t PREADY, uint8_t PSLVERR);

protected:
	typedef enum {
		BUS_STATE_IDLE,
		BUS_STATE_START,
		BUS_STATE_DATA,
		BUS_STATE_ACK,
	} BUS_STATE;

private:
	BUS_SLAVE_INTERFACE *m_pSlave;
	BUS_STATE			 m_state;
	BUS_SALVE_PACKET	*m_pPacket;
	bool				 m_bWrite;
	uint32_t			 m_dwTime;
	uint32_t			 m_dwTimeout;
	SystemLog			 Log;
};

#endif //__APB_SLAVE_H__
