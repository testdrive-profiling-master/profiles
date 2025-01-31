//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 1/31/2025 Fri (clonextop@gmail.com)
//================================================================================
#include "AXI_common.h"
#include "AXI_Master.h"

#define DEBUG_LEVEL(n) if (m_iDebugLevel >= n)

BandwidthLimiter MAXI::m_Bandwidth;

MAXI::MAXI(const char *sTitle, int iDataWidth, bool bUseAXI4, bool bLite, int iDebugLevel)
{
	if (bLite)
		Log.SetTitle(*sTitle ? "MAXI_LITE('%s')" : "MAXI_LITE", sTitle);
	else
		Log.SetTitle(*sTitle ? "MAXI('%s')" : "MAXI", sTitle);

	m_bUseAXI4	  = bUseAXI4;
	m_iDebugLevel = iDebugLevel;
	m_pWriteSlave = NULL;
	m_pReadSlave  = NULL;

	if (bLite) {
		if (iDataWidth != 32)
			LOGE("Requested bit width is not allowed : %d (must be 32 only)", iDataWidth);
	} else {
		switch (iDataWidth) {
		case 32:
		case 64:
		case 128:
		case 256:
		case 512:
		case 1024: // max 1024 bit data bus width support
			m_dwDataBytes = iDataWidth / 8;
			break;

		default:
			LOGE("Requested bit width is not allowed : %d (must be 32~1024)", iDataWidth);
			break;
		}
	}

	m_lAddressMask = ~((uint64_t)m_dwDataBytes - 1);
	m_dwMaxASIZE   = (uint32_t)log2(m_dwDataBytes);
	memset(&m_Read, 0, sizeof(m_Read));
	memset(m_Read.RDATA, 0xCC, sizeof(m_Read.RDATA));
	memset(&m_Write, 0, sizeof(m_Write));
	memset(m_Write.WDATA, 0xCC, sizeof(m_Write.WDATA));
	DEBUG_LEVEL(0) LOGI("%d bit AXI%d master bus is created.", iDataWidth, 3 + bUseAXI4);
}

MAXI::~MAXI(void) {}

void MAXI::AquireBus(void)
{
	SimulationUnLock();
}

void MAXI::ReleaseBus(void)
{
	SimulationLock(100);
}

void MAXI::TouchAddress(MAXI_DESC *pDesc)
{
	pDesc->BEAT--;

	switch (pDesc->BURST) {
	case AXI_BURST_FIXED:
		return;

	case AXI_BURST_INCR: {
		uint64_t prev_addr = pDesc->ADDR;
		pDesc->ADDR += pDesc->SIZE;

		if (pDesc->BEAT) { // 4KB boundary address crossing check
			if ((prev_addr ^ pDesc->ADDR) & (~(uint64_t)(4096 - 1))) {
#ifdef MAXI_4KB_BOUNDARY_CROSSING_AS_ERROR
				LOGE("Burst increase mode - 4KB address(0x%llX) boundary crossing is detected.", pDesc->ADDR);
#else
				LOGW("Burst increase mode - 4KB address(0x%llX) boundary crossing is detected.", pDesc->ADDR);
#endif
			}
		}
	} break;

	case AXI_BURST_WRAP: {
		uint64_t dwMask = (pDesc->SIZE * pDesc->LEN) - 1;
		pDesc->ADDR		= ((pDesc->ADDR + pDesc->SIZE) & dwMask) | (pDesc->ADDR & (~dwMask));
	} break;
	}
}

bool MAXI::Read(MAXI_DESC *pDesc, uint8_t *pData)
{
	uint8_t *pMem = (uint8_t *)GetMemoryPointer(pDesc->ADDR, pDesc->SIZE, FALSE);
	pData += (pDesc->ADDR & (pDesc->SIZE - 1));

	if (pMem) {
		memcpy(pData, pMem, pDesc->SIZE);
		{
			// Observer of MAXI
			ChainList<MAXI_Observer> *pObserver = MAXI_Observer::Head();

			while (pObserver) {
				pObserver->Item()->OnRead(pDesc, pMem);
				pObserver = pObserver->Next();
			}
		}
		DEBUG_LEVEL(2)
		{
			char  sData[256];
			char *sTok = sData;

			for (int i = 0; i < pDesc->SIZE; i++) {
				sprintf(sTok, "%02X", pMem[i]);
				sTok += 2;

				if ((i & 3) == 3 && i != (pDesc->SIZE - 1)) {
					sTok[0] = '/';
					sTok++;
				}
			}

			LOGI("READ[%08llX, %2d/%d] = %s", pDesc->ADDR, (1 + pDesc->LEN - pDesc->BEAT), pDesc->LEN, sData);
		}
	} else
		false;

	TouchAddress(pDesc);
	return true;
}

bool MAXI::Write(MAXI_DESC *pDesc, const uint8_t *pData, const uint32_t *pByteStrob)
{
	uint8_t *pMem = (uint8_t *)GetMemoryPointer(pDesc->ADDR & m_lAddressMask, m_dwDataBytes, FALSE);

	if (pMem) {
		uint32_t dwTransferSize = 0;
		uint32_t dwFirst		= 0;
		uint32_t dwLast			= 0;
		{
			// write transfer
			const uint32_t *pStrobe = pByteStrob;

			for (uint32_t i = 0; i < m_dwDataBytes; i++) {
				uint32_t mask = 1 << (i & 0x1F);

				if ((*pStrobe) & mask) {
					pMem[i] = pData[i];

					if (!dwTransferSize)
						dwFirst = i;

					dwLast = i;
					dwTransferSize++;
				}

				if (mask == 0x80000000)
					pStrobe++;
			}

			{
				// Observer of MAXI
				ChainList<MAXI_Observer> *pObserver = MAXI_Observer::Head();

				while (pObserver) {
					pObserver->Item()->OnWrite(pDesc, pMem);
					pObserver = pObserver->Next();
				}
			}

			DEBUG_LEVEL(2)
			{
				char  sData[256];
				char *sTok = sData;
				pStrobe	   = pByteStrob;

				for (int i = 0; i < m_dwDataBytes; i++) {
					uint32_t mask = 1 << (i & 0x1F);

					if ((*pStrobe) & mask) {
						sprintf(sTok, "%02X", pMem[i]);
					} else {
						sprintf(sTok, "__");
					}

					sTok += 2;

					if ((i & 3) == 3 && i != (m_dwDataBytes - 1)) {
						sprintf(sTok, "/");
						sTok++;
					}
				}

				LOGI("WRITE[%08llX, %2d/%d] = %s", pDesc->ADDR, (1 + pDesc->LEN - pDesc->BEAT), pDesc->LEN, sData);
			}
		}

		if (pDesc->SIZE != m_dwDataBytes && dwTransferSize) { // validation narrow write strobe check
			uint32_t dwShift = pDesc->ADDR & (m_dwDataBytes - 1);

			if (dwTransferSize > pDesc->SIZE || dwFirst < dwShift || dwLast > (dwShift + pDesc->SIZE)) {
				LOGE("Invalid write narrow byte strobe : Address(0x%08llX), Size(%d)", pDesc->ADDR, pDesc->SIZE);
			}
		}
	} else
		return false;

	TouchAddress(pDesc);
	return true;
}
static const char *__sBURST_MODE[] = {
	"FIXED",
	"INCR",
	"WRAP",
	"reserved",
};

DPI_FUNCTION void *CreateMAXI(const char *sTitle, int iDataWidth, int bUseAXI4, int bLite, int iDebugLevel)
{
	return (void *)(new MAXI(sTitle, iDataWidth, bUseAXI4, bLite, iDebugLevel));
}

void MAXI::BusReadRequest(
	uint8_t nRST, int ARID, uint64_t ARADDR, uint32_t ARLEN, uint32_t ARSIZE, uint32_t ARBURST, uint8_t ARLOCK, uint32_t ARCACHE,
	uint32_t ARPROT, uint32_t ARREGION, uint32_t ARQOS, uint8_t ARVALID, uint8_t &ARREADY)
{
	if (!nRST) {
		m_Read.ARREADY = 0;
	} else {
		if (m_Read.ARREADY) {
			if (ARVALID) {
				MAXI_DESC *pReqDesc = ReadQ().Push();
				pReqDesc->ID		= ARID;
				pReqDesc->SIZE		= 1 << ARSIZE;
				pReqDesc->ADDR		= ARADDR;
				pReqDesc->LEN		= ARLEN + 1;
				pReqDesc->BEAT		= ARLEN + 1; // 1~16	: 0 => 1, 15 => 16 number of transfers
				pReqDesc->BURST		= ARBURST;
				pReqDesc->status	= MAXI_STATUS_INIT;
				pReqDesc->TimeOut	= MAXI_TIMEOUT_COUNT;
				m_Read.ARREADY		= 0;

				// size check
				if (!m_bUseAXI4 && ARLEN >= 16)
					LOGE("ARLEN(=%d) must be smaller then 16 on AXI3.", ARLEN);

				if (ARSIZE > m_dwMaxASIZE)
					LOGE("ARSIZE(=%d) must be less or equal than %d(%d bit).", ARSIZE, m_dwMaxASIZE, 8 * (1 << m_dwMaxASIZE));

				if (((1 << ARSIZE) - 1) & ARADDR)
					LOGE("ARADDR(=0x%08llX) is not met in memory alignment.", ARADDR);

				if (ARBURST == AXI_BURST_WRAP && (ARSIZE < 1 || ARSIZE > 4)) {
					LOGE("In Read wrap burst's length(%d) must be 2, 4, 8 or 16.", pReqDesc->SIZE);
				}

				if (ARBURST == AXI_BURST_RESERVED) {
					LOGE("ARBURST(%d) is not a specified operation.", ARBURST);
				}

				if (!ARLEN && (ARBURST == AXI_BURST_FIXED)) {
					LOGE("When 'ReadOnce' operation, ARBURST(%d) must be INCR(0) or WRAP(2).", ARBURST);
				}

				DEBUG_LEVEL(1) if (((ARADDR ^ (ARADDR + pReqDesc->SIZE * ARLEN)) & ~0xFFFULL))
				{
					LOGW(
						"A burst must not cross a 4KB address boundary (Read 0x%08llX ~ 0x%08llX)", ARADDR,
						ARADDR + pReqDesc->SIZE * (ARLEN + 1) - 1);
				}

				DEBUG_LEVEL(1)
				LOGI(
					"Request(Read : 0x%08llX), ID #%d, %d bit, %d beat, %s mode", ARADDR, ARID, pReqDesc->SIZE * 8, pReqDesc->LEN,
					__sBURST_MODE[pReqDesc->BURST]);
				AquireBus();
			}
		} else {
			if (!ReadQ().IsFull())
				m_Read.ARREADY = 1;
		}
	}

	ARREADY = m_Read.ARREADY;
}

void MAXI::BusReadRequestLite(uint8_t nRST, uint64_t ARADDR, uint8_t ARVALID, uint8_t &ARREADY)
{
	if (!nRST) {
		m_Read.ARREADY = 0;
	} else {
		if (m_Read.ARREADY) {
			if (ARVALID) {
				MAXI_DESC *pReqDesc = ReadQ().Push();
				pReqDesc->ID		= 0;
				pReqDesc->SIZE		= 4;
				pReqDesc->ADDR		= ARADDR;
				pReqDesc->LEN		= 1;
				pReqDesc->BEAT		= 1;
				pReqDesc->BURST		= 0;
				pReqDesc->status	= MAXI_STATUS_INIT;
				pReqDesc->TimeOut	= MAXI_TIMEOUT_COUNT;
				m_Read.ARREADY		= 0;

				// 4 byte alignment check
				if (ARADDR & (4 - 1)) {
					LOGE("AXI-LITE 4 bytes alignment is failed. (Read 0x%08llX)", ARADDR);
				}

				DEBUG_LEVEL(1) LOGI("Request(Read : 0x%08llX)", ARADDR);
				AquireBus();
			}
		} else {
			if (!ReadQ().IsFull())
				m_Read.ARREADY = 1;
		}
	}

	ARREADY = m_Read.ARREADY;
}

void MAXI::BusReadData(uint8_t nRST, int &RID, uint32_t *RDATA, uint32_t &RRESP, uint8_t &RLAST, uint8_t &RVALID, uint8_t RREADY)
{
	if (!nRST) {
		m_Read.pDesc  = NULL;
		RID			  = 0;
		m_Read.RVALID = 0;
		m_Read.RLAST  = 0;
	} else {
		if (m_Read.pDesc) { // activation status
			MAXI_DESC *pDesc = m_Read.pDesc;
			RID				 = pDesc->ID; // transaction ID

			switch (pDesc->status) {
			case MAXI_STATUS_INIT: {
				pDesc->status =
					GetMemoryPointer(pDesc->ADDR & m_lAddressMask, m_dwDataBytes, FALSE) ? MAXI_STATUS_MEMORY : MAXI_STATUS_SLAVE_REQUEST;

				if (pDesc->status == MAXI_STATUS_SLAVE_REQUEST) {
					if (pDesc->BEAT != 1) {
						LOGE(
							"Multiple 'burst' transaction to slave bus from MAXI. : ARID:#%d, ARADDR:0x%08llX, ARLEN:%d", RID, pDesc->ADDR,
							pDesc->LEN - 1);
						m_Read.RLAST = 1;
						ReleaseBus();
						break;
					}

					if (m_dwDataBytes != 4) { // 32bit bus only
						LOGE("Only 32bit MAXI to Slave is supported.");
						ReleaseBus();
						break;
					}

					m_pReadSlave = FindSlave(pDesc->ADDR);

					if (!m_pReadSlave) {
						LOGE("Unreachable slave read address : 0x%llX (%d bytes)", pDesc->ADDR, pDesc->SIZE);
						m_Read.RLAST = 1;
						ReleaseBus();
						break;
					}
				}
			} break;

			case MAXI_STATUS_SLAVE_REQUEST: {
				pDesc->TimeOut--;

				if (!pDesc->TimeOut)
					LOGE("Read timeout on request to slave.");

				if (m_pReadSlave->RequestRead(pDesc->ADDR)) {
					pDesc->status = MAXI_STATUS_SLAVE_WAIT;
				}
			} break;

			case MAXI_STATUS_SLAVE_WAIT: {
				pDesc->TimeOut--;

				if (!pDesc->TimeOut)
					LOGE("Read timeout on wait for slave transaction.");

				if (m_pReadSlave->WaitWrite()) {
					m_pReadSlave  = NULL;
					pDesc->status = MAXI_STATUS_DONE;
					m_Read.RVALID = 1;
					m_Read.RLAST  = 1;
					ReleaseBus();
				}
			} break;

			case MAXI_STATUS_MEMORY: {
				if (m_Read.RLAST && RREADY) {
					// no more transaction
					m_Read.pDesc  = NULL; // terminate transaction
					m_Read.RLAST  = 0;	  // clear last
					m_Read.RVALID = 0;	  // clear valid
					ReleaseBus();		  // release bus
					DEBUG_LEVEL(2) LOGI("Read done.");

					for (int i = 0; i < (m_dwDataBytes >> 2); i++) m_Read.RDATA[i] = 0xDEADC0DE;
				} else {
					bool bTransaction = !m_Read.RVALID || (m_Read.RVALID && RREADY);

					if (bTransaction) { // new transaction is available...
						if (m_Bandwidth.IsValid()) {
							if (Read(pDesc, (uint8_t *)m_Read.RDATA)) { // next transaction data
								m_Read.RVALID  = 1;						// next valid transaction
								m_Read.RLAST   = !pDesc->BEAT;
								pDesc->TimeOut = MAXI_TIMEOUT_COUNT;
							} else {
								LOGE("Unreachable memory address on read : 0x%08llX", pDesc->ADDR);
								ReleaseBus();
								break;
							}
						} else {
							m_Read.RVALID = 0;
							bTransaction  = false;
						}
					}

					m_Bandwidth.Transaction(bTransaction ? m_dwDataBytes : 0);
					{
						// check timeout
						pDesc->TimeOut--;

						if (!pDesc->TimeOut)
							LOGE("Read timeout on memory transaction.");
					}
				}
			} break;
			}
		} else {
			// wait new transaction
			RID			  = 0;
			m_Read.pDesc  = ReadQ().Pop();
			m_Read.RLAST  = 0;
			m_Read.RVALID = 0;
		}
	}

	RVALID = m_Read.RVALID;
	RLAST  = m_Read.RLAST;
	RRESP  = 0;
	memcpy(RDATA, m_Read.RDATA, m_dwDataBytes);
}

DPI_FUNCTION void MAXIR_Interface(
	void *hMAXI, unsigned char nRST, int ARID, unsigned long long ARADDR, const svBitVecVal *ARLEN, const svBitVecVal *ARSIZE,
	const svBitVecVal *ARBURST, const svBitVecVal *ARLOCK, const svBitVecVal *ARCACHE, const svBitVecVal *ARPROT, const svBitVecVal *ARREGION,
	const svBitVecVal *ARQOS, unsigned char ARVALID, unsigned char *ARREADY, int *RID, svBitVecVal *RDATA, svBitVecVal *RRESP,
	unsigned char *RLAST, unsigned char *RVALID, unsigned char RREADY)
{
	MAXI *pMAXI = reinterpret_cast<MAXI *>(hMAXI);
	// read data transaction
	pMAXI->BusReadData(nRST, *RID, (uint32_t *)RDATA, *(uint32_t *)RRESP, *RLAST, *RVALID, RREADY);
	// read request
	pMAXI->BusReadRequest(
		nRST, ARID, ARADDR, *ARLEN, *ARSIZE, *ARBURST, *ARLOCK, *ARCACHE, *ARPROT, *(uint32_t *)ARREGION, *(uint32_t *)ARQOS, ARVALID,
		*(uint8_t *)ARREADY);
}

DPI_FUNCTION void MAXIR_LITE_Interface(
	void *hMAXI, unsigned char nRST, unsigned long long ARADDR, unsigned char ARVALID, unsigned char *ARREADY, svBitVecVal *RDATA,
	svBitVecVal *RRESP, unsigned char *RVALID, unsigned char RREADY)
{
	MAXI   *pMAXI = reinterpret_cast<MAXI *>(hMAXI);
	int		rid;
	uint8_t last;
	// read data transaction
	pMAXI->BusReadData(nRST, rid, (uint32_t *)RDATA, *(uint32_t *)RRESP, last, *RVALID, RREADY);
	// read request
	pMAXI->BusReadRequestLite(nRST, ARADDR, ARVALID, *(uint8_t *)ARREADY);
}

void MAXI::BusWriteRequest(
	uint8_t nRST, int AWID, uint64_t AWADDR, uint32_t AWLEN, uint32_t AWSIZE, uint32_t AWBURST, uint8_t AWLOCK, uint32_t AWCACHE,
	uint32_t AWPROT, uint32_t AWREGION, uint32_t AWQOS, uint8_t AWVALID, uint8_t &AWREADY)
{
	if (!nRST) {
		m_Write.AWREADY = 0;
	} else {
		if (m_Write.AWREADY) {
			if (AWVALID) { // request
				MAXI_DESC *pReqDesc = WriteQ().Push();
				pReqDesc->ID		= AWID;
				pReqDesc->SIZE		= 1 << AWSIZE;
				pReqDesc->ADDR		= AWADDR;
				pReqDesc->LEN		= AWLEN + 1;
				pReqDesc->BEAT		= AWLEN + 1; // 1~16	: 0 => 1, 15 => 16 number of transfers
				pReqDesc->BURST		= AWBURST;
				pReqDesc->status	= MAXI_STATUS_INIT;
				pReqDesc->TimeOut	= MAXI_TIMEOUT_COUNT;
				m_Write.AWREADY		= 0;

				// size check
				if (!m_bUseAXI4 && AWLEN >= 16)
					LOGE("AWLEN(=%d) must be smaller then 16 on AXI3.", AWLEN);

				if (AWSIZE > m_dwMaxASIZE)
					LOGE("AWSIZE(=%d) must be less or equal than %d(%d bit).", AWSIZE, m_dwMaxASIZE, 8 * (1 << m_dwMaxASIZE));

				if (((1 << AWSIZE) - 1) & AWADDR)
					LOGE("AWADDR(=0x%08llX) is not met in memory alignment.", AWADDR);

				if (AWBURST == AXI_BURST_WRAP && (AWSIZE < 1 || AWSIZE > 4)) {
					LOGE("In Write wrap burst's length(%d) must be 2, 4, 8 or 16.", pReqDesc->SIZE);
				}

				if (AWBURST == AXI_BURST_RESERVED) {
					LOGE("AWBURST(%d) is not a specified operation.", AWBURST);
				}

				if (!AWLEN && (AWBURST == AXI_BURST_FIXED)) {
					LOGE("When 'WriteUnique' operation, AWBURST(%d) must be INCR(0) or WRAP(2).", AWBURST);
				}

				DEBUG_LEVEL(1) if (((AWADDR ^ (AWADDR + pReqDesc->SIZE * AWLEN)) & ~0xFFFULL))
				{
					LOGW(
						"A burst must not cross a 4KB address boundary (Write 0x%08llX ~ 0x%08llX)", AWADDR,
						AWADDR + pReqDesc->SIZE * (AWLEN + 1) - 1);
				}

				DEBUG_LEVEL(1)
				LOGI(
					"Request(Write : 0x%08llX), ID #%d, %d bit, %d beat, %s mode", AWADDR, AWID, pReqDesc->SIZE * 8, pReqDesc->LEN,
					__sBURST_MODE[pReqDesc->BURST]);
				AquireBus();
			}
		} else {
			if (!WriteQ().IsFull())
				m_Write.AWREADY = 1;
		}
	}

	AWREADY = m_Write.AWREADY;
}

void MAXI::BusWriteRequestLite(uint8_t nRST, uint64_t AWADDR, uint8_t AWVALID, uint8_t &AWREADY)
{
	if (!nRST) {
		m_Write.AWREADY = 0;
	} else {
		if (m_Write.AWREADY) {
			if (AWVALID) { // request
				MAXI_DESC *pReqDesc = WriteQ().Push();
				pReqDesc->ID		= 0;
				pReqDesc->SIZE		= 4;
				pReqDesc->ADDR		= AWADDR;
				pReqDesc->LEN		= 1;
				pReqDesc->BEAT		= 1;
				pReqDesc->BURST		= 0;
				pReqDesc->status	= MAXI_STATUS_INIT;
				pReqDesc->TimeOut	= MAXI_TIMEOUT_COUNT;
				m_Write.AWREADY		= 0;

				// 4 byte alignment check
				if (AWADDR & (4 - 1)) {
					LOGE("AXI-LITE 4 bytes alignment is failed. (Write 0x%08llX)", AWADDR);
				}

				DEBUG_LEVEL(1) LOGI("Request(Write : 0x%08llX)", AWADDR);
				AquireBus();
			}
		} else {
			if (!WriteQ().IsFull())
				m_Write.AWREADY = 1;
		}
	}

	AWREADY = m_Write.AWREADY;
}

void MAXI::BusWriteData(
	uint8_t nRST, int WID, const uint32_t *WDATA, const uint32_t *WSTRB, uint8_t WLAST, uint8_t WVALID, uint8_t &WREADY, int &BID,
	uint32_t &BRESP, uint8_t &BVALID, uint8_t BREADY)
{
	if (!nRST) {
		m_Write.pDesc  = NULL;
		m_Write.WREADY = 0;
		m_Write.BID	   = 0;
		m_Write.BRESP  = 0;
		m_Write.BVALID = 0;
	} else {
		if (m_Write.pDesc) { // handling with write transaction
			MAXI_DESC *pDesc = m_Write.pDesc;

			switch (pDesc->status) {
			case MAXI_STATUS_INIT: {
				pDesc->status =
					GetMemoryPointer(pDesc->ADDR & m_lAddressMask, m_dwDataBytes, FALSE) ? MAXI_STATUS_MEMORY : MAXI_STATUS_SLAVE_REQUEST;

				if (pDesc->status == MAXI_STATUS_SLAVE_REQUEST) {
					if (pDesc->BEAT != 1) {
						LOGE(
							"Multiple 'burst' transaction to slave bus from MAXI. : AWID:#%d, AWADDR:0x%08llX, AWLEN:%d", WID, pDesc->ADDR,
							pDesc->LEN - 1);
						m_Write.BRESP = 1;
						ReleaseBus();
						break;
					}

					if (m_dwDataBytes != 4) { // 32bit bus only
						LOGE("Only 32bit MAXI to Slave is supported.");
						ReleaseBus();
						break;
					}

					m_pWriteSlave = FindSlave(pDesc->ADDR);

					if (!m_pWriteSlave) {
						LOGE("Unreachable slave write address : 0x%llX (%d bytes)", pDesc->ADDR, pDesc->SIZE);
						m_Write.BRESP = 1;
						ReleaseBus();
						break;
					}
				}
			} break;

			case MAXI_STATUS_SLAVE_REQUEST: {
				pDesc->TimeOut--;

				if (!pDesc->TimeOut)
					LOGE("Write timeout on request to slave.");

				if (WVALID) {
					if ((*WSTRB) & 0xF != 0xF) {
						LOGE("Only 32bit MAXI to Slave write is supported.");
						ReleaseBus();
						break;
					}

					if (m_pWriteSlave->RequestWrite(pDesc->ADDR, *(uint32_t *)WDATA)) {
						m_Write.WREADY = 1;

						if (!WLAST)
							LOGE("'WLAST' is not asserted at the last data transaction(WID:#%d).", WID);

						pDesc->status = MAXI_STATUS_SLAVE_WAIT;
					}
				}
			} break;

			case MAXI_STATUS_SLAVE_WAIT: {
				m_Write.WREADY = 0;
				pDesc->TimeOut--;

				if (!pDesc->TimeOut)
					LOGE("Write timeout on wait for slave transaction.");

				if (m_pWriteSlave->WaitWrite()) {
					m_pWriteSlave = NULL;
					pDesc->status = MAXI_STATUS_DONE;
					ReleaseBus();
				}
			} break;

			case MAXI_STATUS_MEMORY: {
				bool bTransaction = m_Write.WREADY && WVALID;

				if (bTransaction) {
					if (Write(pDesc, (const uint8_t *)WDATA, WSTRB)) {
						pDesc->TimeOut = MAXI_TIMEOUT_COUNT;
					} else {
						LOGE("Unreachable memory address on write : 0x%08llX", pDesc->ADDR);
						ReleaseBus();
					}

					if (WID != pDesc->ID)
						LOGE("Non-sequential data transaction is not supported yet. WID(%d) != Current order of WID(%d)", WID, pDesc->ID);

					if (pDesc->BEAT && WLAST)
						LOGE(
							"Early termination is not supported on a 'Burst mode'. 'WLAST' is asserted on middle of burst "
							"transaction(WID:#%d,NEXT_ADDR:0x%08llX,BEAT:%d/%d).",
							WID, pDesc->ADDR, pDesc->LEN - pDesc->BEAT, pDesc->LEN);
				}

				m_Write.WREADY = m_Bandwidth.IsValid();
				m_Bandwidth.Transaction(bTransaction ? m_dwDataBytes : 0);
				{
					// check timeout
					pDesc->TimeOut--;

					if (!pDesc->TimeOut)
						LOGE("Write timeout on memory transaction.");
				}

				if (!pDesc->BEAT) {
					// no more transaction
					m_Write.pDesc		   = NULL; // terminate transaction
					m_Write.BVALID_TimeOut = MAXI_TIMEOUT_COUNT;
					m_Write.WREADY		   = 0;
					m_Write.BVALID		   = 1;
					ReleaseBus();
					DEBUG_LEVEL(2) LOGI("Write done.");

					if (!WLAST)
						LOGE("'WLAST' is not asserted at the last data transaction(WID:#%d).", WID);
				}
			} break;
			}
		} else { // bus termination
			if (m_Write.BVALID) {
				m_Write.BVALID_TimeOut--;

				if (BREADY)
					m_Write.BVALID = 0;

				if (!m_Write.BVALID_TimeOut)
					LOGE("BREADY/BVALID timeout.");
			} else {
				m_Write.pDesc = WriteQ().Pop();

				if (m_Write.pDesc) {
					m_Write.BID = m_Write.pDesc->ID;
				}
			}
		}
	}

	WREADY = m_Write.WREADY;
	BID	   = m_Write.BID;
	BRESP  = m_Write.BRESP;
	BVALID = m_Write.BVALID;
}

DPI_FUNCTION void MAXIW_Interface(
	void *hMAXI, unsigned char nRST, int AWID, unsigned long long AWADDR, const svBitVecVal *AWLEN, const svBitVecVal *AWSIZE,
	const svBitVecVal *AWBURST, const svBitVecVal *AWLOCK, const svBitVecVal *AWCACHE, const svBitVecVal *AWPROT, const svBitVecVal *AWREGION,
	const svBitVecVal *AWQOS, unsigned char AWVALID, unsigned char *AWREADY, int WID, const svBitVecVal *WDATA, const svBitVecVal *WSTRB,
	unsigned char WLAST, unsigned char WVALID, unsigned char *WREADY, int *BID, svBitVecVal *BRESP, unsigned char *BVALID, unsigned char BREADY)
{
	MAXI *pMAXI = reinterpret_cast<MAXI *>(hMAXI);
	// write data transaction
	pMAXI->BusWriteData(
		nRST, WID, (const uint32_t *)WDATA, (const uint32_t *)WSTRB, WLAST, WVALID, *WREADY, *BID, *(uint32_t *)BRESP, *BVALID, BREADY);
	// write request
	pMAXI->BusWriteRequest(nRST, AWID, AWADDR, *AWLEN, *AWSIZE, *AWBURST, *AWLOCK, *AWCACHE, *AWPROT, *AWREGION, *AWQOS, AWVALID, *AWREADY);
}

DPI_FUNCTION void MAXIW_LITE_Interface(
	void *hMAXI, unsigned char nRST, unsigned long long AWADDR, unsigned char AWVALID, unsigned char *AWREADY, const svBitVecVal *WDATA,
	const svBitVecVal *WSTRB, unsigned char WVALID, unsigned char *WREADY, svBitVecVal *BRESP, unsigned char *BVALID, unsigned char BREADY)
{
	MAXI *pMAXI = reinterpret_cast<MAXI *>(hMAXI);
	int	  bid;
	// write data transaction
	pMAXI->BusWriteData(nRST, 0, (const uint32_t *)WDATA, (const uint32_t *)WSTRB, 1, WVALID, *WREADY, bid, *(uint32_t *)BRESP, *BVALID, BREADY);
	// write request
	pMAXI->BusWriteRequestLite(nRST, AWADDR, AWVALID, *AWREADY);
}
