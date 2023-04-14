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
// Rev.  : 4/14/2023 Fri (clonextop@gmail.com)
//================================================================================
#include "VirtualSlave.h"

VirtualSlave::VirtualSlave(const char* sTitle, UINT64 lAddrBase, UINT64 lAddrHigh)
{
	Log.SetTitle(*sTitle ? "Virtual_Slave('%s')" : "Virtual_Slave", sTitle);
	m_pSlave		= CreateSlave(lAddrBase, lAddrHigh);
	m_pReadPacket	= NULL;
	m_bReadWait		= FALSE;
	LOGI("Bus created - 0x%08llX ~ 0x%08llX", lAddrBase, lAddrHigh);
}

VirtualSlave::~VirtualSlave(void)
{
}

//----------------------------------------------------------------------------------------------------
// Write bus
//----------------------------------------------------------------------------------------------------
void VirtualSlave::BusWrite(
	BYTE nRST,
	BYTE& EN, UINT64& ADDR, DWORD& DATA)
{
	if(!nRST) {
		EN					= 0;
		ADDR				= 0;
		DATA				= 0xDEADC0DE;
	} else {
		BUS_SALVE_PACKET*	packet	= m_pSlave->GetWrite();

		if(packet) {
			EN				= 1;
			ADDR			= packet->lAddr;
			DATA			= packet->dwData;
			m_pSlave->WriteAck();
		} else {
			EN				= 0;
			ADDR			= 0;
			DATA			= 0xDEADC0DE;
		}
	}
}

DPI_FUNCTION void VirtualSlave_Write(
	void* hSVirtual,
	unsigned char nRST,
	unsigned char* EN, unsigned long long* ADDR, svBitVecVal* DATA)
{
	VirtualSlave* pSAXI	= reinterpret_cast<VirtualSlave*>(hSVirtual);
	pSAXI->BusWrite(
		nRST,
		*(BYTE*)EN, *(UINT64*)ADDR, *(DWORD*)DATA);
}

//----------------------------------------------------------------------------------------------------
// Read bus
//----------------------------------------------------------------------------------------------------
void VirtualSlave::BusRead(
	BYTE nRST,
	BYTE& EN, UINT64& ADDR, DWORD DATA)
{
	if(!nRST) {
		EN					= 0;
		ADDR				= 0;
	} else {
		if(!m_pReadPacket) {
			m_pReadPacket		= m_pSlave->GetRead();

			if(m_pReadPacket) {
				EN				= 1;
				ADDR			= m_pReadPacket->lAddr;
			} else {
				EN				= 0;
				ADDR			= 0;
			}
		} else {
			if(m_bReadWait) {
				m_bReadWait	= FALSE;
				m_pReadPacket->dwData	= DATA;
				m_pReadPacket			= NULL;
				m_pSlave->ReadAck();
			} else {
				m_bReadWait	= true;
			}

			EN					= 0;
			ADDR				= 0;
		}
	}
}

DPI_FUNCTION void VirtualSlave_Read(
	void* hSVirtual,
	unsigned char nRST,
	unsigned char* EN, unsigned long long* ADDR, const svBitVecVal* DATA)
{
	VirtualSlave* pSAXI	= reinterpret_cast<VirtualSlave*>(hSVirtual);
	pSAXI->BusRead(
		nRST,
		*(BYTE*)EN, *(UINT64*)ADDR, *DATA);
}

DPI_FUNCTION void* CreateVirtualSlave(const char* sTitle, unsigned long long lAddrBase, UINT64 dwAddrBits)
{
	return (void*)(new VirtualSlave(sTitle, lAddrBase, lAddrBase + (1 << dwAddrBits) - 1));
}
