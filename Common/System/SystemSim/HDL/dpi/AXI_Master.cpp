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
// Title : Common DPI
// Rev.  : 4/24/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "AXI_common.h"
#include "AXI_Master.h"

#define DEBUG_LEVEL(n)	if(m_iDebugLevel>=n)

MAXI::MAXI(const char* sTitle, int iDataWidth, bool bUseAXI4, int iDebugLevel)
{
	Log.SetTitle(*sTitle ? "MAXI('%s')" : "MAXI", sTitle);
	m_bUseAXI4		= bUseAXI4;
	m_iDebugLevel	= iDebugLevel;
	m_pWriteSlave	= NULL;
	m_pReadSlave	= NULL;

	switch(iDataWidth) {
	case 32:
	case 64:
	case 128:
	case 256:
	case 512:
	case 1024:		// max 1024 bit data bus width support
		m_dwDataBytes	= iDataWidth / 8;
		break;

	default:
		LOGE("Requested bit width is not allowed : %d (must be 32~1024)", iDataWidth);
		break;
	}

	m_lAddressMask	= ~((UINT64)m_dwDataBytes - 1);
	m_dwMaxASIZE	= (DWORD)log2(m_dwDataBytes);
	memset(&m_Read, 0, sizeof(m_Read));
	memset(m_Read.RDATA, 0xCC, sizeof(m_Read.RDATA));
	memset(&m_Write, 0, sizeof(m_Write));
	memset(m_Write.WDATA, 0xCC, sizeof(m_Write.WDATA));
	DEBUG_LEVEL(0) LOGI("%d bit AXI%d master bus is created.", iDataWidth, 3 + bUseAXI4);
}

MAXI::~MAXI(void)
{
}

void MAXI::AquireBus(void)
{
	SimulationUnLock();
}

void MAXI::ReleaseBus(void)
{
	SimulationLock(32);
}

void MAXI::TouchAddress(MAXI_DESC* pDesc)
{
	pDesc->BEAT--;

	switch(pDesc->BURST) {
	case 	BURST_FIXED:
		return;

	case BURST_INCR:
		pDesc->ADDR	+= pDesc->SIZE;
		break;

	case BURST_WRAP: {
		UINT64	dwMask	= (pDesc->SIZE * pDesc->LEN) - 1;
		pDesc->ADDR		= ((pDesc->ADDR + pDesc->SIZE) & dwMask) | (pDesc->ADDR & (~dwMask));
	}
	break;
	}
}

bool MAXI::Read(MAXI_DESC* pDesc, BYTE* pData)
{
	BYTE* pMem = (BYTE*)GetMemoryPointer(pDesc->ADDR, pDesc->SIZE, FALSE);
	pData	+= (pDesc->ADDR & (pDesc->SIZE - 1));

	if(!pMem) {
		if(!m_pReadSlave) {
			m_pReadSlave	= FindSlave(pDesc->ADDR);

			if(!m_pReadSlave) LOGE("Unreachable read address : 0x%llX (%d bytes)", pDesc->ADDR, pDesc->SIZE);
			else {
				if(!m_pReadSlave->RequestRead(pDesc->ADDR))
					m_pReadSlave	= NULL;
			}

			return FALSE;
		} else {
			if(m_pReadSlave->WaitRead(*(DWORD*)pData)) {
				m_pReadSlave	= NULL;
			} else return FALSE;
		}
	} else {
		memcpy(pData, pMem, pDesc->SIZE);
		{
			// Observer of MAXI
			ChainList<MAXI_Observer>* pObserver	= MAXI_Observer::Head();

			while(pObserver) {
				pObserver->Item()->OnRead(pDesc, pMem);
				pObserver = pObserver->Next();
			}
		}
		DEBUG_LEVEL(2) {
			char sData[256];
			char* sTok	= sData;

			for(int i = 0; i < pDesc->SIZE; i++) {
				sprintf(sTok, "%02X", pMem[i]);
				sTok += 2;

				if((i & 3) == 3 && i != (pDesc->SIZE - 1)) {
					sTok[0] = '/';
					sTok++;
				}
			}

			LOGI("READ[%08llX, %2d/%d] = %s", pDesc->ADDR, (1 + pDesc->LEN - pDesc->BEAT), pDesc->LEN, sData);
		}
	}

	TouchAddress(pDesc);
	return true;
}

bool MAXI::Write(MAXI_DESC* pDesc, const BYTE* pData, const DWORD* pByteStrob)
{
	BYTE*	pMem	= (BYTE*)GetMemoryPointer(pDesc->ADDR & m_lAddressMask, m_dwDataBytes, FALSE);

	if(!pMem) {
		if(!m_pWriteSlave) {
			m_pWriteSlave	= FindSlave(pDesc->ADDR);

			if(!m_pWriteSlave) LOGE("Unreachable write address : 0x%llX (%d bytes)", pDesc->ADDR, pDesc->SIZE);
			else {
				if(!m_pWriteSlave->RequestWrite(pDesc->ADDR, *(DWORD*)pData))
					m_pWriteSlave	= NULL;
			}

			return FALSE;
		} else {
			if(m_pWriteSlave->WaitWrite()) {
				m_pWriteSlave	= NULL;
			} else return FALSE;
		}
	} else {
		DWORD	dwTransferSize	= 0;
		DWORD	dwFirst			= 0;
		DWORD	dwLast			= 0;
		{
			// write transfer
			const DWORD* pStrobe = pByteStrob;

			for(DWORD i = 0; i < m_dwDataBytes; i++) {
				DWORD mask	= 1 << (i & 0x1F);

				if((*pStrobe) & mask) {
					pMem[i]	= pData[i];

					if(!dwTransferSize)
						dwFirst = i;

					dwLast	= i;
					dwTransferSize++;
				}

				if(mask == 0x80000000) pStrobe++;
			}

			{
				// Observer of MAXI
				ChainList<MAXI_Observer>* pObserver	= MAXI_Observer::Head();

				while(pObserver) {
					pObserver->Item()->OnWrite(pDesc, pMem);
					pObserver = pObserver->Next();
				}
			}

			DEBUG_LEVEL(2) {
				char sData[256];
				char* sTok	= sData;
				pStrobe = pByteStrob;

				for(int i = 0; i < m_dwDataBytes; i++) {
					DWORD mask	= 1 << (i & 0x1F);

					if((*pStrobe) & mask) {
						sprintf(sTok, "%02X", pMem[i]);
					} else {
						sprintf(sTok, "__");
					}

					sTok += 2;

					if((i & 3) == 3 && i != (m_dwDataBytes - 1)) {
						sprintf(sTok, "/");
						sTok++;
					}
				}

				LOGI("WRITE[%08llX, %2d/%d] = %s", pDesc->ADDR, (1 + pDesc->LEN - pDesc->BEAT), pDesc->LEN, sData);
			}
		}

		if(pDesc->SIZE != m_dwDataBytes && dwTransferSize) {	// validation narrow write strobe check
			DWORD	dwShift	= pDesc->ADDR & (m_dwDataBytes - 1);

			if(dwTransferSize > pDesc->SIZE || dwFirst < dwShift || dwLast > (dwShift + pDesc->SIZE)) {
				LOGE("Invalid write narrow byte strobe : Address(0x%08llX), Size(%d)", pDesc->ADDR, pDesc->SIZE);
			}
		}
	}

	TouchAddress(pDesc);
	return true;
}
static const char* __sBURST_MODE[] = {
	"FIXED",
	"INCR",
	"WRAP",
	"reserved",
};

DPI_FUNCTION void* CreateMAXI(const char* sTitle, int iDataWidth, int bUseAXI4, int iDebugLevel)
{
	return (void*)(new MAXI(sTitle, iDataWidth, bUseAXI4, iDebugLevel));
}

void MAXI::BusReadRequest(
	BYTE nRST,
	int ARID, UINT64 ARADDR, DWORD ARLEN, DWORD ARSIZE, DWORD ARBURST, BYTE ARLOCK, DWORD ARCACHE, DWORD ARPROT, DWORD& ARREGION, DWORD& ARQOS,
	BYTE ARVALID, BYTE& ARREADY
)
{
	if(!nRST) {
		m_Read.ARREADY		= 0;
	} else {
		if(m_Read.ARREADY) {
			if(ARVALID) {
				MAXI_DESC*	pReqDesc	= ReadQ().Push();
				pReqDesc->ID			= ARID;
				pReqDesc->SIZE			= 1 << ARSIZE;
				pReqDesc->ADDR			= ARADDR;
				pReqDesc->LEN			= ARLEN + 1;
				pReqDesc->BEAT			= ARLEN + 1;	// 1~16	: 0 => 1, 15 => 16 number of transfers
				pReqDesc->BURST			= ARBURST;
				pReqDesc->TimeOut		= MAXI_TIMEOUT_COUNT;
				m_Read.ARREADY			= 0;

				// size check
				if(!m_bUseAXI4 && ARLEN >= 16)
					LOGE("ARLEN(=%d) must be smaller then 16 on AXI3.", ARLEN);

				if(ARSIZE > m_dwMaxASIZE)
					LOGE("ARSIZE(=%d) must be less or equal than %d(%d bit).", ARSIZE, m_dwMaxASIZE, 8 * (1 << m_dwMaxASIZE));

				if(((1 << ARSIZE) - 1) & ARADDR)
					LOGE("ARADDR(=0x%08llX) is not met in memory alignment.", ARADDR);

				if(ARBURST == BURST_WRAP && (ARSIZE < 1 || ARSIZE > 4)) {
					LOGE("In Read wrap burst's length(%d) must be 2, 4, 8 or 16.", pReqDesc->SIZE);
				}

				if(ARBURST == BURST_RESERVED) {
					LOGE("ARBURST(%d) is not a specified operation.", ARBURST);
				}

				if(!ARLEN && (ARBURST == BURST_FIXED)) {
					LOGE("When 'ReadOnce' operation, ARBURST(%d) must be INCR(0) or WRAP(2).", ARBURST);
				}

				DEBUG_LEVEL(1) if(((ARADDR ^ (ARADDR + pReqDesc->SIZE * ARLEN)) & ~0xFFFULL)) {
					LOGW("A burst must not cross a 4KB address boundary (Read 0x%08llX ~ 0x%08llX)", ARADDR, ARADDR + pReqDesc->SIZE * (ARLEN + 1) - 1);
				}

				DEBUG_LEVEL(1) LOGI("Request(Read : 0x%08llX), ID #%d, %d bit, %d beat, %s mode", ARADDR, ARID, pReqDesc->SIZE * 8, pReqDesc->LEN, __sBURST_MODE[pReqDesc->BURST]);
				AquireBus();
			}
		} else {
			if(!ReadQ().IsFull())
				m_Read.ARREADY			= 1;
		}
	}

	ARREGION		= 0;
	ARQOS			= 0;
	ARREADY			= m_Read.ARREADY;
}

void MAXI::BusReadData(
	BYTE nRST,
	int& RID, DWORD* RDATA, DWORD& RRESP, BYTE& RLAST, BYTE& RVALID, BYTE RREADY
)
{
	if(!nRST) {
		m_Read.pDesc	= NULL;
		RID				= 0;
		m_Read.RLAST	= 0;
		m_Read.RVALID	= 0;
		m_Read.RLAST	= 0;
	} else {
		if(m_Read.pDesc) {					// activation status
			RID	= m_Read.pDesc->ID;			// transaction ID

			if(m_Read.RLAST && RREADY) {
				// no more transaction
				m_Read.pDesc			= NULL;					// terminate transaction
				m_Read.RLAST			= 0;					// clear last
				m_Read.RVALID			= 0;					// clear valid
				ReleaseBus();									// release bus
				DEBUG_LEVEL(2) LOGI("Read done.");

				for(int i = 0; i < (m_dwDataBytes >> 2); i++) m_Read.RDATA[i] = 0xDEADC0DE;
			} else {
				bool bNewTransaction	= !m_Read.RVALID || (m_Read.RVALID && RREADY);

				if(m_Read.pDesc->BEAT && bNewTransaction) {	// new transaction is requested...
					if(Read(m_Read.pDesc, (BYTE*)m_Read.RDATA)) {		// next transaction data
						m_Read.RVALID			= 1;					// next valid transaction
						m_Read.RLAST			= !m_Read.pDesc->BEAT;
						m_Read.pDesc->TimeOut	= MAXI_TIMEOUT_COUNT;
					} else {
						m_Read.RVALID			= 0;					// next valid transaction
					}
				}

				m_Read.pDesc->TimeOut--;

				if(!m_Read.pDesc->TimeOut)
					LOGE("Read timeout.");
			}
		} else {
			// wait new transaction
			RID				= 0;
			m_Read.pDesc	= ReadQ().Pop();
			m_Read.RLAST	= 0;
			m_Read.RVALID	= 0;
		}
	}

	RVALID			= m_Read.RVALID;
	RLAST			= m_Read.RLAST;
	RRESP			= 0;
	memcpy(RDATA, m_Read.RDATA, m_dwDataBytes);
}

DPI_FUNCTION void MAXIR_Interface(
	void* hMAXI,
	unsigned char nRST,
	int ARID, unsigned long long ARADDR, const svBitVecVal* ARLEN, const svBitVecVal* ARSIZE, const svBitVecVal* ARBURST, svBit ARLOCK, const svBitVecVal* ARCACHE, const svBitVecVal* ARPROT, svBitVecVal* ARREGION, svBitVecVal* ARQOS,
	unsigned char ARVALID, unsigned char* ARREADY,
	int* RID, svBitVecVal* RDATA, svBitVecVal* RRESP, unsigned char* RLAST, unsigned char* RVALID, unsigned char RREADY)
{
	MAXI*	pMAXI		= reinterpret_cast<MAXI*>(hMAXI);
	// read data transaction
	pMAXI->BusReadData(
		nRST,
		*RID, (DWORD*)RDATA, *(DWORD*)RRESP, *RLAST, *RVALID, RREADY
	);
	// read request
	pMAXI->BusReadRequest(
		nRST,
		ARID, ARADDR, *ARLEN, *ARSIZE, *ARBURST, ARLOCK, *ARCACHE, *ARPROT, *(DWORD*)ARREGION, *(DWORD*)ARQOS,
		ARVALID, *(BYTE*)ARREADY
	);
}

void MAXI::BusWriteRequest(
	BYTE nRST,
	int AWID, UINT64 AWADDR, DWORD AWLEN, DWORD AWSIZE, DWORD AWBURST, BYTE AWLOCK, DWORD AWCACHE, DWORD AWPROT, DWORD AWREGION, DWORD AWQOS,
	BYTE AWVALID, BYTE& AWREADY
)
{
	if(!nRST) {
		m_Write.AWREADY		= 0;
	} else {
		if(m_Write.AWREADY) {
			if(AWVALID) {	// request
				MAXI_DESC*	pReqDesc	= WriteQ().Push();
				pReqDesc->ID			= AWID;
				pReqDesc->SIZE			= 1 << AWSIZE;
				pReqDesc->ADDR			= AWADDR;
				pReqDesc->LEN			= AWLEN + 1;
				pReqDesc->BEAT			= AWLEN + 1;	// 1~16	: 0 => 1, 15 => 16 number of transfers
				pReqDesc->BURST			= AWBURST;
				pReqDesc->TimeOut		= MAXI_TIMEOUT_COUNT;
				m_Write.AWREADY			= 0;

				// size check
				if(!m_bUseAXI4 && AWLEN >= 16)
					LOGE("AWLEN(=%d) must be smaller then 16 on AXI3.", AWLEN);

				if(AWSIZE > m_dwMaxASIZE)
					LOGE("AWSIZE(=%d) must be less or equal than %d(%d bit).", AWSIZE, m_dwMaxASIZE, 8 * (1 << m_dwMaxASIZE));

				if(((1 << AWSIZE) - 1) & AWADDR)
					LOGE("AWADDR(=0x%08llX) is not met in memory alignment.", AWADDR);

				if(AWBURST == BURST_WRAP && (AWSIZE < 1 || AWSIZE > 4)) {
					LOGE("In Write wrap burst's length(%d) must be 2, 4, 8 or 16.", pReqDesc->SIZE);
				}

				if(AWBURST == BURST_RESERVED) {
					LOGE("AWBURST(%d) is not a specified operation.", AWBURST);
				}

				if(!AWLEN && (AWBURST == BURST_FIXED)) {
					LOGE("When 'WriteUnique' operation, AWBURST(%d) must be INCR(0) or WRAP(2).", AWBURST);
				}

				DEBUG_LEVEL(1) if(((AWADDR ^ (AWADDR + pReqDesc->SIZE * AWLEN)) & ~0xFFFULL)) {
					LOGW("A burst must not cross a 4KB address boundary (Write 0x%08llX ~ 0x%08llX)", AWADDR, AWADDR + pReqDesc->SIZE * (AWLEN + 1) - 1);
				}

				DEBUG_LEVEL(1) LOGI("Request(Write : 0x%08llX), ID #%d, %d bit, %d beat, %s mode", AWADDR, AWID, pReqDesc->SIZE * 8, pReqDesc->LEN, __sBURST_MODE[pReqDesc->BURST]);
				AquireBus();
			}
		} else {
			if(!WriteQ().IsFull())
				m_Write.AWREADY		= 1;
		}
	}

	AWREADY		= m_Write.AWREADY;
}

void MAXI::BusWriteData(
	BYTE nRST,
	int WID, const DWORD* WDATA, const DWORD* WSTRB, BYTE WLAST, BYTE WVALID, BYTE& WREADY,
	int& BID, DWORD& BRESP, BYTE& BVALID, BYTE BREADY
)
{
	if(!nRST) {
		m_Write.pDesc		= NULL;
		m_Write.WREADY		= 0;
		m_Write.BID			= 0;
		m_Write.BRESP		= 0;
		m_Write.BVALID		= 0;
	} else {
		if(m_Write.pDesc) {
			if(!m_Write.WREADY) {
				if(Write(m_Write.pDesc, (const BYTE*)m_Write.WDATA, m_Write.WSTRB)) {
					if(m_Write.pDesc->BEAT)
						m_Write.WREADY				= 1;
				}
			} else {
				bool bTransaction	= (m_Write.WREADY && WVALID);

				if(bTransaction) {
					if(Write(m_Write.pDesc, (const BYTE*)WDATA, WSTRB)) {
						m_Write.pDesc->TimeOut		= MAXI_TIMEOUT_COUNT;
					} else {
						m_Write.WREADY				= 0;
						memcpy(m_Write.WDATA, WDATA, m_dwDataBytes);
						memcpy(m_Write.WSTRB, WSTRB, (m_dwDataBytes + 31) / 32);
					}

					if(WID != m_Write.pDesc->ID)
						LOGE("Sorry! non-sequential data transaction is not supported yet. WID(%d) != %d", WID, m_Write.pDesc->ID);

					if(m_Write.pDesc->BEAT && WLAST)
						LOGE("Early termination is not supported on a 'Burst mode'. 'WLAST' is asserted on middle of burst transaction(WID:#%d,NEXT_ADDR:0x%08llX,BEAT:%d/%d).", WID, m_Write.pDesc->ADDR, m_Write.pDesc->LEN - m_Write.pDesc->BEAT, m_Write.pDesc->LEN);
				}
			}

			m_Write.pDesc->TimeOut--;

			if(!m_Write.pDesc->TimeOut)
				LOGE("Write timeout.");

			if(!m_Write.pDesc->BEAT) {
				// no more transaction
				m_Write.pDesc			= NULL;		// terminate transaction
				m_Write.BVALID_TimeOut	= MAXI_TIMEOUT_COUNT;
				m_Write.WREADY			= 0;
				m_Write.BVALID			= 1;
				ReleaseBus();
				DEBUG_LEVEL(2) LOGI("Write done.");

				if(!WLAST) LOGE("'WLAST' is not asserted at the last data transaction(WID:#%d).", WID);
			}
		} else {
			if(m_Write.BVALID) {
				m_Write.BVALID_TimeOut--;

				if(BREADY)
					m_Write.BVALID	= 0;

				if(!m_Write.BVALID_TimeOut)
					LOGE("BREADY/BVALID timeout.");
			} else {
				m_Write.pDesc		= WriteQ().Pop();

				if(m_Write.pDesc) {
					m_Write.BID		= m_Write.pDesc->ID;
					m_Write.WREADY	= 1;
				}
			}
		}
	}

	WREADY		= m_Write.WREADY;
	BID			= m_Write.BID;
	BRESP		= m_Write.BRESP;
	BVALID		= m_Write.BVALID;
}

DPI_FUNCTION void MAXIW_Interface(
	void* hMAXI,
	unsigned char nRST,
	int AWID, unsigned long long AWADDR, const svBitVecVal* AWLEN, const svBitVecVal* AWSIZE, const svBitVecVal* AWBURST, svBit AWLOCK, const svBitVecVal* AWCACHE, const svBitVecVal* AWPROT, const svBitVecVal* AWREGION, const svBitVecVal* AWQOS, unsigned char AWVALID, unsigned char* AWREADY,
	int WID, const svBitVecVal* WDATA, const svBitVecVal* WSTRB, unsigned char WLAST, unsigned char WVALID, unsigned char* WREADY,
	int* BID, svBitVecVal* BRESP, unsigned char* BVALID, unsigned char BREADY)
{
	MAXI*	pMAXI		= reinterpret_cast<MAXI*>(hMAXI);
	// write data transaction
	pMAXI->BusWriteData(
		nRST,
		WID, (const DWORD*)WDATA, (const DWORD*)WSTRB, WLAST, WVALID, *WREADY,
		*BID, *(DWORD*)BRESP, *BVALID, BREADY
	);
	// write request
	pMAXI->BusWriteRequest(
		nRST,
		AWID, AWADDR, *AWLEN, *AWSIZE, *AWBURST, AWLOCK, *AWCACHE, *AWPROT, *AWREGION, *AWQOS,
		AWVALID, *AWREADY
	);
}
