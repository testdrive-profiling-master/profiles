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
#include "AXI_Slave_Lite.h"

SAXI_Lite::SAXI_Lite(const char *sTitle, uint32_t dwAddrBase, uint32_t dwAddrHigh)
{
	Log.SetTitle(*sTitle ? "SAXI_Lite('%s')" : "SAXI_Lite", sTitle);
	m_pSlave = CreateSlave(dwAddrBase, dwAddrHigh);
	memset(&m_Read, 0, sizeof(m_Read));
	memset(&m_Write, 0, sizeof(m_Write));
	m_WFlag.all = 0;
	LOGI("Bus created - 0x%08X ~ 0x%08X", dwAddrBase, dwAddrHigh);
}

SAXI_Lite::~SAXI_Lite(void) {}

//----------------------------------------------------------------------------------------------------
// Write bus
//----------------------------------------------------------------------------------------------------
void SAXI_Lite::BusWrite(
	uint8_t nRST, uint64_t &AWADDR, uint8_t &AWVALID, uint8_t AWREADY, uint32_t &WDATA, uint32_t &WSTRB, uint8_t &WVALID, uint8_t WREADY,
	uint32_t BRESP, uint8_t BVALID, uint8_t &BREADY)
{
	AWADDR	= 0xDEADC0DE;
	AWVALID = 0;
	WDATA	= 0;
	WSTRB	= 0;
	WVALID	= 0;
	BREADY	= 0;

	if (!nRST) {
		m_Write.state = BUS_STATE_RESET;
	} else {
		switch (m_Write.state) {
		case BUS_STATE_RESET:
			if (AWREADY) {
				LOGW("S_AWREADY is must be initialized with zero on start.");
			}

			m_Write.state = BUS_STATE_IDLE;
			break;

		case BUS_STATE_IDLE:
			if ((m_Write.packet = m_pSlave->GetWrite())) {
				// address transaction
				AWVALID			= 1;
				WVALID			= 1;
				WSTRB			= 0xF;
				AWADDR			= m_Write.packet->lAddr;
				WDATA			= m_Write.packet->dwData;
				m_Write.dwTime	= 0;
				m_WFlag.control = 1;
				m_WFlag.data	= 1;
				m_Write.state	= BUS_STATE_CONTROL;
			}

			break;

		case BUS_STATE_CONTROL:
			m_Write.dwTime++;

			// address control
			if (m_WFlag.control) {
				if (AWREADY)
					m_WFlag.control = 0;
				else
					AWVALID = 1;
			}

			// data transaction
			if (m_WFlag.data) {
				if (WREADY) {
					m_WFlag.data = 0;

					if (m_WFlag.control)
						LOGW("WREADY is asserted too early before the AWREADY.");
				} else
					WVALID = 1;
			}

			if (m_WFlag.all) {
				WSTRB  = 0xF;
				AWADDR = m_Write.packet->lAddr;
				WDATA  = m_Write.packet->dwData;
			} else {
				m_pSlave->WriteAck();
				BREADY		   = 1;
				m_Write.dwTime = 0;
				m_Write.state  = BUS_STATE_RESP;
			}

			if (m_Write.dwTime > 5000)
				LOGE("Too long waited on write control transaction.");

			break;

		case BUS_STATE_RESP:
			m_Write.dwTime++;

			if (BVALID) {
				if (BRESP) {
					LOGE("BRESP('%s') is asserted.", g_sAXI_BRESP[BRESP]);
				}

				m_Write.state = BUS_STATE_IDLE;
			}

			if (m_Write.dwTime > 5000)
				LOGE("Too long waited for BVALID signal.");

			break;
		}
	}
}

DPI_FUNCTION void SAXIW_Interface(
	void *hSAXIL, unsigned char nRST, svBitVecVal *AWADDR, unsigned char *AWVALID, unsigned char AWREADY, svBitVecVal *WDATA, svBitVecVal *WSTRB,
	unsigned char *WVALID, unsigned char WREADY, const svBitVecVal *BRESP, unsigned char BVALID, unsigned char *BREADY)
{
	SAXI_Lite *pSAXI = reinterpret_cast<SAXI_Lite *>(hSAXIL);
	pSAXI->BusWrite(
		nRST, *(uint64_t *)AWADDR, *AWVALID, AWREADY, *(uint32_t *)WDATA, *(uint32_t *)WSTRB, *WVALID, WREADY, *BRESP, BVALID, *BREADY);
}

//----------------------------------------------------------------------------------------------------
// Read bus
//----------------------------------------------------------------------------------------------------
void SAXI_Lite::BusRead(
	uint8_t nRST, uint64_t &ARADDR, uint8_t &ARVALID, uint8_t ARREADY, uint32_t RDATA, uint32_t RRESP, uint8_t RVALID, uint8_t &RREADY)
{
	ARADDR	= 0;
	ARVALID = 0;
	RREADY	= 0;

	if (!nRST) {
		m_Read.state = BUS_STATE_RESET;
	} else {
		switch (m_Read.state) {
		case BUS_STATE_RESET:
			if (RVALID) {
				LOGW("S_RVALID is must be initialized with zero on start.");
			}

			m_Read.state = BUS_STATE_IDLE;
			break;

		case BUS_STATE_IDLE:
			if ((m_Read.packet = m_pSlave->GetRead())) {
				// address transaction
				ARVALID		  = 1;
				ARADDR		  = m_Read.packet->lAddr;
				m_Read.dwTime = 0;
				m_Read.state  = BUS_STATE_CONTROL;
			}

			break;

		case BUS_STATE_CONTROL:
			m_Read.dwTime++;

			if (ARREADY) { // address ready check
				// data transaction
				RREADY		  = 1;
				m_Read.dwTime = 0;
				m_Read.state  = BUS_STATE_DATA;
			} else {
				ARVALID = 1;
				ARADDR	= m_Read.packet->lAddr;
			}

			if (m_Read.dwTime > 5000)
				LOGE("Too long waited on read control transaction.");

			break;

		case BUS_STATE_DATA:
			m_Read.dwTime++;

			if (RVALID) {
				// data is ok
				m_Read.packet->dwData = RDATA;
				m_pSlave->ReadAck();
				m_Read.state = BUS_STATE_IDLE;

				if (RRESP) {
					LOGE("RRESP('%s') is asserted.", g_sAXI_BRESP[RRESP]);
				}
			} else {
				RREADY = 1;
			}

			if (m_Read.dwTime > 5000)
				LOGE("Too long waited on RVALID signal.");

			break;
		}
	}
}

DPI_FUNCTION void SAXIR_Interface(
	void *hSAXIL, unsigned char nRST, unsigned long long *ARADDR, unsigned char *ARVALID, unsigned char ARREADY, const svBitVecVal *RDATA,
	const svBitVecVal *RRESP, unsigned char RVALID, unsigned char *RREADY)
{
	SAXI_Lite *pSAXI = reinterpret_cast<SAXI_Lite *>(hSAXIL);
	pSAXI->BusRead(nRST, *(uint64_t *)ARADDR, *ARVALID, ARREADY, *RDATA, *RRESP, RVALID, *RREADY);
}

DPI_FUNCTION void *CreateSAXILite(const char *sTitle, unsigned long long lAddrBase, unsigned int dwAddrBits)
{
	return (void *)(new SAXI_Lite(sTitle, lAddrBase, lAddrBase + (1 << dwAddrBits) - 1));
}
