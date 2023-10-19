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
// Rev.  : 10/19/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "AXI_common.h"

const char* g_sAXI_BRESP[AXI_RESP_SIZE] = {
	"OKAY",
	"EXOKAY",
	"SLVERR",
	"DECERR",
};

const char* g_sAXI_BURST[AXI_BURST_SIZE] = {
	"FIXED",
	"INCR",
	"WRAP",
	"*RESERVED*",
};

#define MAX_BANDWIDTH_QUEUE_COUNT		(1024*256)

BandwidthLimiter::BandwidthLimiter(void) : m_Sema(1)
{
	m_Slot.pData			= NULL;
	Initialize(0);		// initial : no limit
}

BandwidthLimiter::~BandwidthLimiter(void)
{
	Initialize(0);
}

void BandwidthLimiter::Initialize(uint32_t kb_size)
{
	m_dwLimitBandwidth		= kb_size;
	m_Slot.uElapsedTime		= 0;
	m_Slot.iIndex			= 0;

	if(kb_size) {
		if(!m_Slot.pData) {
			m_Slot.pData	= new TIME_SLOT[MAX_BANDWIDTH_QUEUE_COUNT];
		}

		memset(m_Slot.pData, 0, sizeof(TIME_SLOT) * MAX_BANDWIDTH_QUEUE_COUNT);
	} else {
		SAFE_DELETE_ARRAY(m_Slot.pData);
	}

	m_Slot.pCurrent			= m_Slot.pData;
	m_dwConsumedByte		= 0;
	m_bValid				= true;
}

void BandwidthLimiter::Transaction(uint32_t byte_size)
{
	if(m_Slot.pData) {
		uint64_t	uCurTime	= SimulationTime();
		m_Sema.Down();
		{
			if(m_Slot.pCurrent->uTime == uCurTime) {
				m_Slot.pCurrent->bytes		+= byte_size;
				m_dwConsumedByte			+= byte_size;
			} else {
				m_Slot.iIndex++;

				if(m_Slot.iIndex >= MAX_BANDWIDTH_QUEUE_COUNT) {
					m_Slot.iIndex = 0;
					m_Slot.pCurrent		= m_Slot.pData;
				} else {
					m_Slot.pCurrent++;
				}

				m_Slot.uElapsedTime			= uCurTime - m_Slot.pCurrent->uTime;
				m_dwConsumedByte			+= (byte_size - m_Slot.pCurrent->bytes);
				m_Slot.pCurrent->bytes		= byte_size;
				m_Slot.pCurrent->uTime		= uCurTime;
			}

			if(m_Slot.uElapsedTime) {
				m_bValid	= (((uint64_t)m_dwConsumedByte * 976562500ULL) / m_Slot.uElapsedTime) < m_dwLimitBandwidth;	// 976562500 = 1000000000000 / 1024 = (1/1ps) / 1KB
			}
		}
		m_Sema.Up();
	}
}
