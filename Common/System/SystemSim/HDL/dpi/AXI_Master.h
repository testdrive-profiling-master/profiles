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
// Rev.  : 2/2/2024 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __AXI_MASTER_H__
#define __AXI_MASTER_H__
#include "dpi_lib.h"

#define	MAXI_TIMEOUT_COUNT		100000

typedef enum {
	MAXI_STATUS_INIT,
	MAXI_STATUS_SLAVE_REQUEST,
	MAXI_STATUS_SLAVE_WAIT,
	MAXI_STATUS_MEMORY,
	MAXI_STATUS_DONE,
} MAXI_STATUS;

typedef struct {
	WORD			ID;
	WORD			SIZE;		// byte size
	UINT64			ADDR;		// address
	WORD			BEAT;		// beat count
	WORD			LEN;		// length
	DWORD			BURST;		// burst mode
	MAXI_STATUS		status;		// current status
	DWORD			TimeOut;	// timeout count
} MAXI_DESC;

typedef RoundQueue<MAXI_DESC, 4>		MAXI_QUEUE;

class MAXI_Observer : public ChainList<MAXI_Observer> {
public:
	MAXI_Observer(void) : ChainList<MAXI_Observer>(this) {}
	virtual ~MAXI_Observer(void) {}

	virtual bool OnRead(MAXI_DESC* pDesc, BYTE* pData) {
		return true;
	}
	virtual bool OnWrite(MAXI_DESC* pDesc, BYTE* pData) {
		return true;
	}
};

class MAXI : public SelfDestory {
	// access main memory
	MAXI_QUEUE	m_WriteQ;
	MAXI_QUEUE	m_ReadQ;
public:
	MAXI(const char* sTitle, int iDataWidth, bool bUseAXI4,  bool bLite, int iDebugLevel = 0);
	virtual ~MAXI(void);

	bool Read(MAXI_DESC* pDesc, BYTE* pData);
	bool Write(MAXI_DESC* pDesc, const BYTE* pData, const DWORD* pByteStrob);

	inline MAXI_QUEUE& ReadQ(void)	{
		return m_ReadQ;
	}
	inline MAXI_QUEUE& WriteQ(void)	{
		return m_WriteQ;
	}

	void AquireBus(void);
	void ReleaseBus(void);

	void BusWriteRequest(
		BYTE nRST,
		int AWID, UINT64 AWADDR, DWORD AWLEN, DWORD AWSIZE, DWORD AWBURST, BYTE AWLOCK, DWORD AWCACHE, DWORD AWPROT, DWORD AWREGION, DWORD AWQOS,
		BYTE AWVALID, BYTE& AWREADY
	);
	void BusWriteRequestLite(
		BYTE nRST,
		UINT64 AWADDR, BYTE AWVALID, BYTE& AWREADY
	);
	void BusWriteData(
		BYTE nRST,
		int WID, const DWORD* WDATA, const DWORD* WSTRB, BYTE WLAST, BYTE WVALID, BYTE& WREADY,
		int& BID, DWORD& BRESP, BYTE& BVALID, BYTE BREADY
	);
	void BusReadRequest(
		BYTE nRST,
		int ARID, UINT64 ARADDR, DWORD ARLEN, DWORD ARSIZE, DWORD ARBURST, BYTE ARLOCK, DWORD ARCACHE, DWORD ARPROT, DWORD ARREGION, DWORD ARQOS,
		BYTE ARVALID, BYTE& ARREADY
	);
	void BusReadRequestLite(
		BYTE nRST,
		UINT64 ARADDR, BYTE ARVALID, BYTE& ARREADY
	);
	void BusReadData(
		BYTE nRST,
		int& RID, DWORD* RDATA, DWORD& RRESP, BYTE& RLAST, BYTE& RVALID, BYTE RREADY
	);
protected:
	void TouchAddress(MAXI_DESC* pDesc);

private:
	DWORD		m_dwDataBytes;
	UINT64		m_lAddressMask;
	DWORD		m_dwMaxASIZE;
	bool		m_bUseAXI4;
	int			m_iDebugLevel;
	BUS_SLAVE_INTERFACE*	m_pWriteSlave;
	BUS_SLAVE_INTERFACE*	m_pReadSlave;
	static BandwidthLimiter	m_Bandwidth;

	struct {
		MAXI_DESC*		pDesc;
		BYTE			ARREADY;
		BYTE			RVALID;
		DWORD			RDATA[32];
		BYTE			RLAST;
	} m_Read;

	struct {
		MAXI_DESC*		pDesc;
		BYTE			AWREADY;
		BYTE			WREADY;
		BYTE			BID;
		DWORD			BRESP;
		DWORD			WDATA[32];
		DWORD			WSTRB[4];
		BYTE			BVALID;
		DWORD			BVALID_TimeOut;
	} m_Write;
	SystemLog				Log;
};

#endif//__AXI_MASTER_H__
