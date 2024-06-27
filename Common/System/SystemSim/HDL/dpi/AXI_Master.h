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
#ifndef __AXI_MASTER_H__
#define __AXI_MASTER_H__
#include "dpi_lib.h"

#define MAXI_TIMEOUT_COUNT 100000

typedef enum {
	MAXI_STATUS_INIT,
	MAXI_STATUS_SLAVE_REQUEST,
	MAXI_STATUS_SLAVE_WAIT,
	MAXI_STATUS_MEMORY,
	MAXI_STATUS_DONE,
} MAXI_STATUS;

typedef struct {
	uint16_t	ID;
	uint16_t	SIZE;	 // byte size
	uint64_t	ADDR;	 // address
	uint16_t	BEAT;	 // beat count
	uint16_t	LEN;	 // length
	uint32_t	BURST;	 // burst mode
	MAXI_STATUS status;	 // current status
	uint32_t	TimeOut; // timeout count
} MAXI_DESC;

typedef RoundQueue<MAXI_DESC, 4> MAXI_QUEUE;

class MAXI_Observer : public ChainList<MAXI_Observer>
{
public:
	MAXI_Observer(void) : ChainList<MAXI_Observer>(this) {}
	virtual ~MAXI_Observer(void) {}

	virtual bool OnRead(MAXI_DESC *pDesc, uint8_t *pData)
	{
		return true;
	}
	virtual bool OnWrite(MAXI_DESC *pDesc, uint8_t *pData)
	{
		return true;
	}
};

class MAXI : public SelfDestory
{
	// access main memory
	MAXI_QUEUE m_WriteQ;
	MAXI_QUEUE m_ReadQ;

public:
	MAXI(const char *sTitle, int iDataWidth, bool bUseAXI4, bool bLite, int iDebugLevel = 0);
	virtual ~MAXI(void);

	bool			   Read(MAXI_DESC *pDesc, uint8_t *pData);
	bool			   Write(MAXI_DESC *pDesc, const uint8_t *pData, const uint32_t *pByteStrob);

	inline MAXI_QUEUE &ReadQ(void)
	{
		return m_ReadQ;
	}
	inline MAXI_QUEUE &WriteQ(void)
	{
		return m_WriteQ;
	}

	void AquireBus(void);
	void ReleaseBus(void);

	void BusWriteRequest(
		uint8_t nRST, int AWID, uint64_t AWADDR, uint32_t AWLEN, uint32_t AWSIZE, uint32_t AWBURST, uint8_t AWLOCK, uint32_t AWCACHE,
		uint32_t AWPROT, uint32_t AWREGION, uint32_t AWQOS, uint8_t AWVALID, uint8_t &AWREADY);
	void BusWriteRequestLite(uint8_t nRST, uint64_t AWADDR, uint8_t AWVALID, uint8_t &AWREADY);
	void BusWriteData(
		uint8_t nRST, int WID, const uint32_t *WDATA, const uint32_t *WSTRB, uint8_t WLAST, uint8_t WVALID, uint8_t &WREADY, int &BID,
		uint32_t &BRESP, uint8_t &BVALID, uint8_t BREADY);
	void BusReadRequest(
		uint8_t nRST, int ARID, uint64_t ARADDR, uint32_t ARLEN, uint32_t ARSIZE, uint32_t ARBURST, uint8_t ARLOCK, uint32_t ARCACHE,
		uint32_t ARPROT, uint32_t ARREGION, uint32_t ARQOS, uint8_t ARVALID, uint8_t &ARREADY);
	void BusReadRequestLite(uint8_t nRST, uint64_t ARADDR, uint8_t ARVALID, uint8_t &ARREADY);
	void BusReadData(uint8_t nRST, int &RID, uint32_t *RDATA, uint32_t &RRESP, uint8_t &RLAST, uint8_t &RVALID, uint8_t RREADY);

protected:
	void TouchAddress(MAXI_DESC *pDesc);

private:
	uint32_t				m_dwDataBytes;
	uint64_t				m_lAddressMask;
	uint32_t				m_dwMaxASIZE;
	bool					m_bUseAXI4;
	int						m_iDebugLevel;
	BUS_SLAVE_INTERFACE	   *m_pWriteSlave;
	BUS_SLAVE_INTERFACE	   *m_pReadSlave;
	static BandwidthLimiter m_Bandwidth;

	struct {
		MAXI_DESC *pDesc;
		uint8_t	   ARREADY;
		uint8_t	   RVALID;
		uint32_t   RDATA[32];
		uint8_t	   RLAST;
	} m_Read;

	struct {
		MAXI_DESC *pDesc;
		uint8_t	   AWREADY;
		uint8_t	   WREADY;
		uint8_t	   BID;
		uint32_t   BRESP;
		uint32_t   WDATA[32];
		uint32_t   WSTRB[4];
		uint8_t	   BVALID;
		uint32_t   BVALID_TimeOut;
	} m_Write;
	SystemLog Log;
};

#endif //__AXI_MASTER_H__
