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
// Title : Driver(PCIe) sub-system
// Rev.  : 1/31/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "PCIeDriver.h"
#include "DriverCommand.h"
#include <assert.h>

DECLARE_NATIVE_DRIVER(PCIeDriver)

PCIeDriver::PCIeDriver(void)
{
	memset(&m_OverlappedIO, 0, sizeof(OVERLAPPED));
	m_dwCurrentCardID		= 0;
	m_iBarID				= 0;
	{
		// vcu118 FPGA : DDR4 4GBx2
		m_TotalMemory.base_address		= 0x8'0000'0000ULL;			// base address
		m_TotalMemory.byte_size			= 0x1'0000'0000ULL;			// 4GB
	}
	SetSystemDescription("TestDrive PCIe driver");
}

PCIeDriver::~PCIeDriver(void)
{
	Release();
}

bool PCIeDriver::Initialize(const char* sDeviceName)
{
	if(SystemDriverInterface::Initialize(DEV_PATH)) {
		TD_DRIVER_INFO	info;
		DWORD dwReadSize;
		m_OverlappedIO.hEvent	= CreateEvent(NULL, true, true, "");

		if(!m_OverlappedIO.hEvent) {
			printf("*E: Failed to create Interrupt event handler.\n");
			return false;
		}

		if(DeviceIoControl(m_hDriver, IOCTL_COMMAND_INFOMATION, &info, sizeof(TD_DRIVER_INFO), &info, sizeof(TD_DRIVER_INFO), &dwReadSize, NULL)) {
			printf("* PCIe Device Driver connection details.\n");
			printf("\t> Driver version : %d.%d (build %d)\n", info.version.major, info.version.minor, info.version.build);

			if(info.version.major != TESTDRIVE_PCIE_DRIVER_VERSION_MAJOR || info.version.minor != TESTDRIVE_PCIE_DRIVER_VERSION_MINOR) {
				printf("*E: Driver version does not matched, this reference driver version = %d.%d\n", TESTDRIVE_PCIE_DRIVER_VERSION_MAJOR, TESTDRIVE_PCIE_DRIVER_VERSION_MINOR);
				return false;
			}

			printf("\t> PCIe Cards connection list.\n");

			for(int i = 0; i < info.card_count; i++) {
				SetCurrentCard(i);
				printf("\t\t #%d/%d Card : Main-System(VID:0x%04X,DID:0x%04X), Sub-System(VID:0x%04X,DID:0x%04X), PCIe %d.0 - %d Lanes\n",
					   i + 1,
					   info.card_count,
					   info.device[i].id.main.vendor,
					   info.device[i].id.main.device,
					   info.device[i].id.sub.vendor,
					   info.device[i].id.sub.device,
					   info.device[i].pcie.gen,
					   info.device[i].pcie.lanes);
			}

			printf("\n");
			SetCurrentCard(0);
			m_dwCardCount	= info.card_count;
		}

		return true;
	} else {
		printf("*E: Device driver is not found.\n");
	}

	return false;
}

void PCIeDriver::Release(void)
{
	if(m_OverlappedIO.hEvent) {
		CloseHandle(m_OverlappedIO.hEvent);
		m_OverlappedIO.hEvent	= NULL;
	}

	SystemDriverInterface::Release();
}

void PCIeDriver::SetCurrentCard(DWORD dwIndex)
{
	m_dwCurrentCardID	= dwIndex;
}

void PCIeDriver::RegWrite(UINT64 dwAddress, DWORD dwData)
{
	DWORD dwReadSize;
	TD_TRANSACTION_REG			reg_transaction;
	reg_transaction.dev_id		= m_dwCurrentCardID;
	reg_transaction.bar_id		= m_iBarID;	// REG(0), XDMA(1), bypass_MEM(2)
	reg_transaction.is_write	= 1;
	reg_transaction.phy_address	= dwAddress;
	reg_transaction.reg_data	= dwData;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_REG, &reg_transaction, sizeof(TD_TRANSACTION_REG), NULL, 0, &dwReadSize, NULL);
}

DWORD PCIeDriver::RegRead(UINT64 dwAddress)
{
	DWORD dwReadSize;
	TD_TRANSACTION_REG			reg_transaction;
	reg_transaction.dev_id		= m_dwCurrentCardID;
	reg_transaction.bar_id		= m_iBarID;	// REG(0), XDMA(1), bypass_MEM(2)
	reg_transaction.is_write	= 0;
	reg_transaction.phy_address	= dwAddress;
	reg_transaction.reg_data	= 0;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_REG, &reg_transaction, sizeof(TD_TRANSACTION_REG), &reg_transaction.reg_data, sizeof(DWORD), &dwReadSize, NULL);
	return reg_transaction.reg_data;
}

void PCIeDriver::MemoryWrite(NativeMemory* pNative, UINT64 dwAddress, UINT64 dwOffset, DWORD dwByteSize)
{
	TD_DMA_MEMORY*	pDMA	= (TD_DMA_MEMORY*)pNative->pDriver;
	DWORD dwReadSize;

	if(pDMA) {
		TD_TRANSACTION_DMA	TranDMA;
		OVERLAPPED			OverlappedIO;
		memset(&OverlappedIO, 0, sizeof(OVERLAPPED));
		OverlappedIO.hEvent				= CreateEvent(NULL, true, true, NULL);
		assert(OverlappedIO.hEvent != NULL);
		TranDMA.dev_id					= m_dwCurrentCardID;
		TranDMA.hDMA.pointer			= pDMA->hDMA.pointer;
		TranDMA.is_write				= 1;
		TranDMA.phy_address.addr64		= dwAddress;
		TranDMA.offset					= dwOffset;
		TranDMA.size					= dwByteSize;
		DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_DMA, &TranDMA, sizeof(TD_TRANSACTION_DMA), NULL, 0, &dwReadSize, &OverlappedIO);
		WaitForSingleObject(OverlappedIO.hEvent, INFINITE);
		CloseHandle(OverlappedIO.hEvent);
	}
}

void PCIeDriver::MemoryRead(NativeMemory* pNative, UINT64 dwAddress, UINT64 dwOffset, DWORD dwByteSize)
{
	TD_DMA_MEMORY*	pDMA	= (TD_DMA_MEMORY*)pNative->pDriver;
	DWORD dwReadSize;

	if(pDMA) {
		TD_TRANSACTION_DMA	TranDMA;
		OVERLAPPED			OverlappedIO;
		memset(&OverlappedIO, 0, sizeof(OVERLAPPED));
		OverlappedIO.hEvent				= CreateEvent(NULL, true, true, NULL);
		assert(OverlappedIO.hEvent != NULL);
		TranDMA.dev_id					= m_dwCurrentCardID;
		TranDMA.hDMA.pointer			= pDMA->hDMA.pointer;
		TranDMA.is_write				= 0;
		TranDMA.phy_address.addr64		= dwAddress;
		TranDMA.offset					= dwOffset;
		TranDMA.size					= dwByteSize;
		DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_DMA, &TranDMA, sizeof(TD_TRANSACTION_DMA), NULL, 0, &dwReadSize, &OverlappedIO);
		WaitForSingleObject(OverlappedIO.hEvent, INFINITE);
		CloseHandle(OverlappedIO.hEvent);
	}
}

void PCIeDriver::MemoryCreate(NativeMemory* pNative, UINT64 dwByteSize, UINT64 dwAlignment)
{
	DWORD dwReadSize;
	TD_DMA_MEMORY*	pDma	= new TD_DMA_MEMORY;
	memset(pDma, 0, sizeof(TD_DMA_MEMORY));
	pDma->byte_size			= dwByteSize;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_DMA_ALLOC, pDma, sizeof(TD_DMA_MEMORY), pDma, sizeof(TD_DMA_MEMORY), &dwReadSize, NULL);

	if(!pDma->vir_addr.pointer) {
		delete pDma;
		LOGE("DMA memory insufficient : %lld bytes", dwByteSize);
	} else {
		pNative->pMem		= (BYTE*)(pDma->vir_addr.pointer);
		pNative->pDriver	= pDma;
	}
}

void PCIeDriver::MemoryFree(NativeMemory* pNative)
{
	DWORD dwReadSize;
	TD_DMA_MEMORY*	pDma	= (TD_DMA_MEMORY*)(pNative->pDriver);

	if(pDma) {
		DeviceIoControl(m_hDriver, IOCTL_COMMAND_DMA_FREE, pDma, sizeof(TD_DMA_MEMORY), pDma, sizeof(TD_DMA_MEMORY), &dwReadSize, NULL);
		delete	pDma;
		pNative->pDriver	= NULL;
		pNative->pMem		= NULL;
	}
}

void PCIeDriver::InterruptLock(void)
{
	DWORD	dwReadSize, dwIntrFlags;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_INTERRUPT_LOCK, NULL, 0, &dwIntrFlags, sizeof(DWORD), &dwReadSize, &m_OverlappedIO);
	WaitForSingleObject(m_OverlappedIO.hEvent, INFINITE);
}

void PCIeDriver::InterruptFree(void)
{
	DWORD dwReadSize;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_INTERRUPT_FREE, NULL, 0, NULL, 0, &dwReadSize, NULL);
}

DWORD PCIeDriver::Command(void* pCommand)
{
	if(pCommand) {
		DRIVER_COMMAND*	pCom	= (DRIVER_COMMAND*)pCommand;

		switch(pCom->command_id) {
		case DRIVER_COMMAND_ID_SELECT_BAR:
			if(pCom->data.select_bar.id < 3) {
				m_iBarID		= pCom->data.select_bar.id;
			} else {
				LOGE("Invalid BAR# : %d", pCom->data.select_bar.id);
				exit(1);
			}

			break;

		default:
			LOGE("Invalid driver command : %d", pCom->command_id);
			break;
		}
	}

	return 0;
}

