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
#ifndef __AXI_SLAVE_LITE_H__
#define __AXI_SLAVE_LITE_H__
#include "AXI_common.h"
#include "RoundQueue.h"

class SAXI_Lite : public SelfDestory
{
public:
	SAXI_Lite(const char *sTitle, uint32_t dwAddrBase, uint32_t dwAddrHigh);
	virtual ~SAXI_Lite(void);

	void BusWrite(
		uint8_t nRST, uint64_t &AWADDR, uint8_t &AWVALID, uint8_t AWREADY, uint32_t &WDATA, uint32_t &WSTRB, uint8_t &WVALID, uint8_t WREADY,
		uint32_t BRESP, uint8_t BVALID, uint8_t &BREADY);
	void
	BusRead(uint8_t nRST, uint64_t &ARADDR, uint8_t &ARVALID, uint8_t ARREADY, uint32_t RDATA, uint32_t RRESP, uint8_t RVALID, uint8_t &RREADY);

protected:
	typedef enum {
		BUS_STATE_RESET,
		BUS_STATE_IDLE,
		BUS_STATE_CONTROL,
		BUS_STATE_DATA,
		BUS_STATE_RESP,
	} BUS_STATE;

private:
	BUS_SLAVE_INTERFACE *m_pSlave;
	struct {
		BUS_STATE		  state;
		BUS_SALVE_PACKET *packet;
		uint32_t		  dwTime;
	} m_Read, m_Write;
	union {
		uint32_t all;
		struct {
			uint8_t control;
			uint8_t data;
		};
	} m_WFlag; // write flag

	SystemLog Log;
};

#endif //__AXI_SLAVE_LITE_H__
