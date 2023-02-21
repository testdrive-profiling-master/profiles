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
// Title : Simulation sub-system
// Rev.  : 2/1/2023 Wed (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "BusSlave.h"

BusSlave*	BusSlave::m_pHead	= NULL;
Semaphore	BusSlave::m_LockSW(1);

//----------------------------------------------------------------------------------------------------
// Slave bus method
//----------------------------------------------------------------------------------------------------
BusSlave::BusSlave(DWORD dwAddrBase, DWORD dwAddrHigh) :
	m_LockBusAck(0),
	m_LockBus(1),
	m_dwAddrBase(dwAddrBase),
	m_dwAddrHigh(dwAddrHigh)
{
	// link chain
	m_pNext			= m_pHead;
	m_pHead			= this;
	memset(&m_Read,  0, sizeof(m_Read));
	memset(&m_Write, 0, sizeof(m_Write));
}

BusSlave::~BusSlave(void)
{
	// unlink chain
	if(m_pHead == this) {
		m_pHead	= m_pNext;
	} else {
		BusSlave* pSlave	= m_pHead;

		while(pSlave) {
			if(pSlave->m_pNext == this) {
				pSlave->m_pNext	= m_pNext;
				break;
			}

			pSlave	= pSlave->m_pNext;
		}
	}
}

bool BusSlave::OnRun(void)
{
	return m_Read.bEnable | m_Write.bEnable;
}

void BusSlave::Write(DWORD dwAddress, DWORD dwData)
{
	m_LockSW.Down();
	{
	RETRY:
		m_LockBus.Down();

		if(m_Write.bEnable || m_Write.bWait) {
			m_LockBus.Up();
			goto RETRY;
		}

		m_Write.packet.dwAddr	= dwAddress;
		m_Write.packet.dwData	= dwData;
		m_Write.bEnable			= true;
		m_Write.bWait			= false;
		m_LockBus.Up();
		TRACE_UNLOCK
		m_pSim->Unlock();
		m_LockBusAck.Down();
	}
	m_LockSW.Up();
}

DWORD BusSlave::Read(DWORD dwAddress)
{
	DWORD	dwData;
	m_LockSW.Down();
	{
	RETRY:
		m_LockBus.Down();

		if(m_Read.bEnable || m_Write.bWait) {
			m_LockBus.Up();
			goto RETRY;
		}

		m_Read.packet.dwAddr	= dwAddress;
		m_Read.bEnable			= true;
		m_Read.bWait			= false;
		m_LockBus.Up();
		TRACE_UNLOCK
		m_pSim->Unlock();
		m_LockBusAck.Down();
		dwData			= m_Read.packet.dwData;
	}
	m_LockSW.Up();
	return dwData;
}

bool BusSlave::RequestWrite(DWORD dwAddr, DWORD dwData)
{
	bool bRet	= false;
	m_LockBus.Down();

	if(!m_Write.bEnable && !m_Write.bWait) {
		bRet					= true;
		m_Write.bEnable			= true;
		m_Write.bWait			= true;
		m_Write.packet.dwAddr	= dwAddr;
		m_Write.packet.dwData	= dwData;
		m_pSim->Unlock();
	}

	m_LockBus.Up();
	return bRet;
}

bool BusSlave::WaitWrite(void)
{
	bool bRet	= false;
	m_LockBus.Down();

	if(!m_Write.bEnable && m_Write.bWait) {
		bRet					= true;
		m_Write.bWait			= false;
	}

	m_LockBus.Up();
	return bRet;
}

bool BusSlave::RequestRead(DWORD dwAddr)
{
	bool bRet	= false;
	m_LockBus.Down();

	if(!m_Read.bEnable && !m_Read.bWait) {
		bRet					= true;
		m_Read.bEnable			= true;
		m_Read.bWait			= true;
		m_Read.packet.dwAddr	= dwAddr;
		m_pSim->Unlock();
	}

	m_LockBus.Up();
	return bRet;
}

bool BusSlave::WaitRead(DWORD& dwData)
{
	bool bRet	= false;
	m_LockBus.Down();

	if(!m_Read.bEnable && m_Read.bWait) {
		bRet					= true;
		m_Read.bWait			= false;
		dwData					= m_Read.packet.dwData;
	}

	m_LockBus.Up();
	return bRet;
}

BUS_SALVE_PACKET* BusSlave::GetWrite(void)
{
	return m_Write.bEnable ? &m_Write.packet : NULL;
}

void BusSlave::WriteAck(void)
{
	m_LockBus.Down();
	m_Write.bEnable		= false;

	if(!m_Write.bWait) m_LockBusAck.Up();

	m_LockBus.Up();
	TRACE_LOCK
	m_pSim->Lock(32);
}
BUS_SALVE_PACKET* BusSlave::GetRead(void)
{
	return m_Read.bEnable ? &m_Read.packet : NULL;
}

void BusSlave::ReadAck(void)
{
	m_LockBus.Down();
	m_Read.bEnable		= false;

	if(!m_Read.bWait) m_LockBusAck.Up();

	m_LockBus.Up();
	TRACE_LOCK
	m_pSim->Lock(32);
}

bool BusSlave::IsValidAddress(DWORD dwAddress)
{
	return (dwAddress >= m_dwAddrBase) && (dwAddress <= m_dwAddrHigh);
}

BusSlave* BusSlave::FindSlave(DWORD dwAddress)
{
	BusSlave* pSlave	= m_pHead;

	while(pSlave) {
		if(pSlave->IsValidAddress(dwAddress)) break;

		pSlave	= pSlave->m_pNext;
	}

	return pSlave;
}

BUS_SLAVE_INTERFACE* SimEngine::CreateSlave(UINT64 dwAddrBase, UINT64 dwAddrHigh)
{
	return new BusSlave(dwAddrBase, dwAddrHigh);
}

BUS_SLAVE_INTERFACE* SimEngine::FindSlave(UINT64 dwAddress)
{
	return BusSlave::FindSlave(dwAddress);
}
