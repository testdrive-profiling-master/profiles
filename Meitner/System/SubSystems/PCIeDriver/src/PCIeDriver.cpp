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
// Title : Driver(PCIe) sub-system
// Rev.  : 2/1/2023 Wed (clonextop@gmail.com)
//================================================================================
#include "PCIeDriver.h"
#include "driver_testdrive.h"

static TD_TRANSACTION_REG	__TranReg;
static TD_TRANSACTION_MEM	__TranMem;

PCIeDriver*	g_pDriver	= NULL;

PCIeDriver::PCIeDriver(void)
{
	m_hDriver			= NULL;
	m_dwCardCount		= 0;
	m_PhyBaseAddress	= 0x80000000;			//@FIXME : temporary setting
	m_PhyByteSize		= 1024 * 1024 * 128;	//@FIXME : temporary setting
	memset(&__TranReg, 0, sizeof(TD_TRANSACTION_REG));
	memset(&__TranMem, 0, sizeof(TD_TRANSACTION_MEM));
	memset(&m_OverlappedIO, 0, sizeof(OVERLAPPED));
	__TranReg.count		= 1;
	g_pDriver			= this;
	SetSystemDescription("PCIe(WDM) driver");
}

PCIeDriver::~PCIeDriver(void)
{
	g_pDriver		= NULL;
	Release();
}

bool PCIeDriver::Initialize(void)
{
	Release();
	m_hDriver	= CreateFile(DEV_PATH, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	m_OverlappedIO.hEvent	= CreateEvent(NULL, true, true, "");

	if(m_hDriver != INVALID_HANDLE_VALUE) {
		TD_DRIVER_INFO	info;
		DWORD dwReadSize;

		if(DeviceIoControl(m_hDriver, IOCTL_COMMAND_INFOMATION, NULL, 0, &info, sizeof(TD_DRIVER_INFO), &dwReadSize, NULL)) {
			printf("旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬\n" \
				   "早                          Ⅹ Device information. Ⅹ                         早\n" \
				   "竹收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收羊\n" \
				   "弛1. System information                                                       弛\n" \
				   "弛    PCI-Express Evaluation board ID : VID(0x%04X), DID(0x%04X)              弛\n" \
				   "弛2. Card connection info.                                                    弛\n",
				   info.system.id.vendor, info.system.id.device);

			for(int i = 0; i < info.card_count; i++) {
				char sDesc[1024];
				SetCurrentCard(i);
				DWORD LinkStatus	= RegRead(0x144);
				sprintf(sDesc, "PCIe %d.0 - %d Lanes, %d MSIs",
						(LinkStatus & 1) + 1,
						1 << ((LinkStatus >> 1) & 3),
						info.device[i].pcie.msi_count);
				printf("弛    %d/%d Sub-System : VID(0x%04X), DID(0x%04X), %-25s   弛\n",
					   i + 1,
					   info.card_count,
					   info.device[i].sub_system.id.vendor,
					   info.device[i].sub_system.id.device,
					   sDesc);
			}

			SetCurrentCard(0);
			printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n\n");
			m_dwCardCount	= info.card_count;
		}
	} else {
		printf("*E: Device driver is not found.\n");
		m_hDriver	= NULL;
	}

	return IsInitialized();
}

void PCIeDriver::Release(void)
{
	if(m_hDriver) {
		CloseHandle(m_hDriver);
		m_hDriver	= NULL;
	}
}

bool PCIeDriver::IsInitialized(void)
{
	return (m_hDriver != NULL);
}

void PCIeDriver::SetCurrentCard(DWORD dwIndex)
{
	__TranReg.dev_id	= dwIndex;
	__TranMem.dev_id	= dwIndex;
}

void PCIeDriver::RegWrite(UINT64 dwAddress, DWORD dwData)
{
	DWORD dwReadSize;
	__TranReg.is_write		= 1;
	__TranReg.phy_address	= dwAddress;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_REG, &__TranReg, sizeof(TD_TRANSACTION_REG), &dwData, sizeof(DWORD), &dwReadSize, NULL);
}

DWORD PCIeDriver::RegRead(UINT64 dwAddress)
{
	DWORD dwReadSize, dwData;
	__TranReg.is_write		= 0;
	__TranReg.phy_address	= dwAddress;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_REG, &__TranReg, sizeof(TD_TRANSACTION_REG), &dwData, sizeof(DWORD), &dwReadSize, NULL);
	return dwData;
}

void PCIeDriver::MemoryWrite(UINT64 dwAddress, BYTE* pData, DWORD dwCount)
{
	DWORD dwReadSize;
	__TranMem.is_write		= 1;
	__TranMem.phy_address	= dwAddress;
	__TranMem.count			= dwCount;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_MEM, &__TranMem, sizeof(TD_TRANSACTION_MEM), pData, sizeof(DWORD) * 2 * dwCount, &dwReadSize, NULL);
}

void PCIeDriver::MemoryRead(UINT64 dwAddress, BYTE* pData, DWORD dwCount)
{
	DWORD dwReadSize;
	__TranMem.is_write		= 0;
	__TranMem.phy_address	= dwAddress;
	__TranMem.count			= dwCount;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_MEM, &__TranMem, sizeof(TD_TRANSACTION_MEM), pData, sizeof(DWORD) * 2 * dwCount, &dwReadSize, NULL);
}

void PCIeDriver::InterruptLock(void)
{
	DWORD	dwReadSize;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_INTERRUPT_LOCK, NULL, 0, NULL, 0, &dwReadSize, &m_OverlappedIO);
	WaitForSingleObject(m_OverlappedIO.hEvent, INFINITE);
}

void PCIeDriver::InterruptFree(void)
{
	DWORD dwReadSize;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_INTERRUPT_FREE, NULL, 0, NULL, 0, &dwReadSize, NULL);
}
