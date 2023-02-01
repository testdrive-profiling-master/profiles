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

DECLARE_NATIVE_DRIVER(PCIeDriver)

static TD_TRANSACTION_REG	__TranReg;
static TD_TRANSACTION_MEM	__TranMem;

PCIeDriver::PCIeDriver(void)
{
	memset(&__TranReg, 0, sizeof(TD_TRANSACTION_REG));
	memset(&__TranMem, 0, sizeof(TD_TRANSACTION_MEM));
	memset(&m_OverlappedIO, 0, sizeof(OVERLAPPED));
	__TranReg.count		= 1;
	SetSystemDescription("PCIe(WDM) driver");
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
		m_MemBaseAddress		= 0x80000000;			// base address
		m_MemByteSize			= 1024 * 1024 * 128;	// 128MB
		m_OverlappedIO.hEvent	= CreateEvent(NULL, true, true, "");

		if(DeviceIoControl(m_hDriver, IOCTL_COMMAND_INFOMATION, NULL, 0, &info, sizeof(TD_DRIVER_INFO), &dwReadSize, NULL)) {
			printf("��������������������������������������������������������������������������������\n" \
				   "��                          �� Device information. ��                         ��\n" \
				   "�˦�����������������������������������������������������������������������������\n" \
				   "��1. System information                                                       ��\n" \
				   "��    PCI-Express Evaluation board ID : VID(0x%04X), DID(0x%04X)              ��\n" \
				   "��2. Card connection info.                                                    ��\n",
				   info.system.id.vendor, info.system.id.device);

			for(int i = 0; i < info.card_count; i++) {
				char sDesc[1024];
				SetCurrentCard(i);
				DWORD LinkStatus	= RegRead(0x144);
				sprintf(sDesc, "PCIe %d.0 - %d Lanes, %d MSIs",
						(LinkStatus & 1) + 1,
						1 << ((LinkStatus >> 1) & 3),
						info.device[i].pcie.msi_count);
				printf("��    %d/%d Sub-System : VID(0x%04X), DID(0x%04X), %-25s   ��\n",
					   i + 1,
					   info.card_count,
					   info.device[i].sub_system.id.vendor,
					   info.device[i].sub_system.id.device,
					   sDesc);
			}

			SetCurrentCard(0);
			printf("��������������������������������������������������������������������������������\n\n");
			m_dwCardCount	= info.card_count;
		}

		return true;
	} else {
		printf("*E: Device driver is not found.\n");
		m_hDriver	= NULL;
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
	__TranMem.phy_address	= dwAddress - m_MemBaseAddress;
	__TranMem.count			= dwCount;
	DeviceIoControl(m_hDriver, IOCTL_COMMAND_TRANSACTION_MEM, &__TranMem, sizeof(TD_TRANSACTION_MEM), pData, sizeof(DWORD) * 2 * dwCount, &dwReadSize, NULL);
}

void PCIeDriver::MemoryRead(UINT64 dwAddress, BYTE* pData, DWORD dwCount)
{
	DWORD dwReadSize;
	__TranMem.is_write		= 0;
	__TranMem.phy_address	= dwAddress - m_MemBaseAddress;
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
