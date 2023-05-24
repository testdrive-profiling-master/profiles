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
// Rev.  : 5/24/2023 Wed (clonextop@gmail.com)
//================================================================================
#include "APB_Slave.h"

APB_Slave::APB_Slave(const char* sTitle, UINT64 lAddrBase, UINT64 lAddrHigh, DWORD dwTimeout)
{
	Log.SetTitle(*sTitle ? "APB_Slave('%s')" : "APB_Slave", sTitle);
	m_pSlave		= CreateSlave(lAddrBase, lAddrHigh);
	m_state			= BUS_STATE_IDLE;
	m_pPacket		= NULL;
	m_bWrite		= FALSE;
	m_dwTime		= 0;
	m_dwTimeout		= dwTimeout;
	LOGI("Bus created - 0x%08llX ~ 0x%08llX", lAddrBase, lAddrHigh);
}

APB_Slave::~APB_Slave(void)
{
}

void APB_Slave::BusSignal(
	BYTE nRST,
	BYTE& PSEL, BYTE& PENABLE, BYTE& PWRITE, UINT64& PADDR, DWORD& PWDATA, DWORD& PSTRB,
	DWORD PRDATA, BYTE PREADY, BYTE PSLVERR)
{
	if(!nRST) {
		m_state		= BUS_STATE_IDLE;
		m_bWrite	= FALSE;
	ON_RESET:
		PSEL		= 0;
		PENABLE		= 0;
		PWRITE		= 0;
		PADDR		= 0;
		PWDATA		= 0xDEADC0DE;
		PSTRB		= 0;
	} else {
		switch(m_state) {
		case BUS_STATE_IDLE:
			m_pPacket	= m_pSlave->GetRead();

			if(!m_pPacket) {
				m_pPacket	= m_pSlave->GetWrite();
				m_bWrite	= true;
			} else m_bWrite	= FALSE;

			if(m_pPacket) {
				m_state		= BUS_STATE_START;
				PSEL		= 1;
				PENABLE		= 0;
				PWRITE		= m_bWrite;
				PADDR		= m_pPacket->lAddr;
				PWDATA		= m_pPacket->dwData;
				PSTRB		= 0xF;
			} else goto ON_RESET;

			break;

		case BUS_STATE_START:
			m_dwTime	= 0;
			m_state		= BUS_STATE_DATA;
		ON_DATA:
			PSEL		= 1;
			PENABLE		= 1;
			PWRITE		= m_bWrite;
			PADDR		= m_pPacket->lAddr;
			PWDATA		= m_pPacket->dwData;
			PSTRB		= 0xF;
			break;

		case BUS_STATE_DATA:
			m_dwTime++;

			if(PSLVERR)
				LOGE("PSLVERR is occurred.");

			if(m_dwTimeout && (m_dwTime > m_dwTimeout))
				LOGE("Timeout is occurred.");

			if(PREADY) {
				m_state	= BUS_STATE_ACK;

				if(!m_bWrite) {
					m_pPacket->dwData	= PRDATA;
				}

				goto ON_RESET;
			} else goto ON_DATA;

			break;

		case BUS_STATE_ACK:
			m_state	= BUS_STATE_IDLE;
			PSEL		= 0;
			PENABLE		= 0;
			PWRITE		= 0;
			PADDR		= 0;
			PWDATA		= 0xDEADC0DE;
			PSTRB		= 0;

			if(m_bWrite) {
				m_pSlave->WriteAck();
			} else {
				m_pSlave->ReadAck();
			}

			break;
		}
	}
}

DPI_FUNCTION void APB_Slave_Interface(
	void* hSAPB,
	unsigned char nRST,
	unsigned char* PSEL, unsigned char* PENABLE, unsigned char* PWRITE, unsigned long long* PADDR, svBitVecVal* PWDATA, svBitVecVal* PSTRB, const svBitVecVal* PRDATA, unsigned char PREADY, unsigned char PSLVERR)
{
	APB_Slave* pAPB		= reinterpret_cast<APB_Slave*>(hSAPB);
	pAPB->BusSignal(
		nRST,
		*(BYTE*)PSEL, *(BYTE*)PENABLE, *(BYTE*)PWRITE,
		*(UINT64*)PADDR, *(DWORD*)PWDATA, *(DWORD*)PSTRB,
		*(DWORD*)PRDATA, PREADY, PSLVERR);
}

DPI_FUNCTION void* CreateAPBSlave(const char* sTitle, unsigned long long lAddrBase, unsigned int dwAddrBits, unsigned int dwTimeout)
{
	return (void*)(new APB_Slave(sTitle, lAddrBase, lAddrBase + (1 << dwAddrBits) - 1, dwTimeout));
}
