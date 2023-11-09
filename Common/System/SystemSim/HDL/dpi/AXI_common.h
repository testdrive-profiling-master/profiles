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
// Rev.  : 11/9/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __AXI_COMMON_H__
#define __AXI_COMMON_H__
#include "dpi_lib.h"

typedef enum {
	AXI_RESP_OKAY,
	AXI_RESP_EXOKAY,
	AXI_RESP_SLVERR,
	AXI_RESP_DECERR,
	AXI_RESP_SIZE
} AXI_RESP;

typedef enum {
	AXI_BURST_FIXED,
	AXI_BURST_INCR,
	AXI_BURST_WRAP,
	AXI_BURST_RESERVED,
	AXI_BURST_SIZE
} AXI_BURST;

extern const char* g_sAXI_BRESP[AXI_RESP_SIZE];
extern const char* g_sAXI_BURST[AXI_BURST_SIZE];

#ifndef MAXI_BANDWIDTH
#define	MAXI_BANDWIDTH		0	// default bandwidth(KB) = 0 (no limit)
#endif//MAXI_BANDWIDTH

class BandwidthLimiter {
public:
	BandwidthLimiter(void);
	virtual ~BandwidthLimiter(void);
	void Initialize(uint32_t kb_size);
	void Transaction(uint32_t byte_size);

	inline bool IsValid(void) {
		return m_bValid;
	}

protected:
	Semaphore			m_Sema;
	uint64_t			m_dwLimitBandwidth;	// (KB)
	volatile uint32_t	m_dwConsumedByte;
	volatile bool		m_bValid;

private:
	typedef struct {
		uint64_t		uTime;
		uint32_t		bytes;
	} TIME_SLOT;

	struct {
		TIME_SLOT*			pData;
		volatile TIME_SLOT*	pCurrent;
		volatile int		iIndex;
		volatile uint64_t	uElapsedTime;
	} m_Slot;
};

#endif//__AXI_COMMON_H__
