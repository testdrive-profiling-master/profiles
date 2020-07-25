//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : WDM PCIe Driver for TestDrive
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "driver.h"

DRIVER_INFO				g_Driver;

// ����̹� �ʱ�ȭ
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	PAGED_CODE();
	LOGI("DriverEntry : " DEV_NAME " on " __DATE__ " at "__TIME__ "\n");
	memset(&g_Driver, 0, sizeof(DRIVER_INFO));
	g_Driver.info.system.codes							= DRIVER_VENDOR_ID | (DRIVER_DEVICE_ID << 16);
	// Major function ���
	pDriverObject->DriverUnload							= DriverUnload;
	pDriverObject->DriverExtension->AddDevice			= DispatchAddDevice;
	pDriverObject->MajorFunction[IRP_MJ_CREATE]			= DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE]			= DispatchClose;
	pDriverObject->MajorFunction[IRP_MJ_PNP]			= DispatchPNP;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]	= DispatchControl;
	return STATUS_SUCCESS;
}

// ����̹� ����
VOID DriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	PAGED_CODE();
	LOGI("Driver binary is unloaded.\n");
}

NTSTATUS DriverInitialize(IN PDRIVER_OBJECT pDriverObject, IN PDEVICE_EXTENSION	pDevExt)
{
	NTSTATUS			status	= STATUS_SUCCESS;
	PAGED_CODE();

	// ���� naming ���� ����̹� ����
	if(!g_Driver.pDevice) {
		UNICODE_STRING		sDeviceName, sDevLinkName;
		LOGI("Initializing driver...\n");
		RtlInitUnicodeString(&sDeviceName, DEVICE_NAME);
		RtlInitUnicodeString(&sDevLinkName, SYMBOLIC_NAME);
		// ����̹� �ʱ�ȭ
		status = IoCreateDevice(
					 pDriverObject,					// Our Driver Object
					 0,								// Device extension
					 &sDeviceName,					// Device name
					 FILE_DEVICE_UNKNOWN,			// Device type
					 FILE_DEVICE_SECURE_OPEN,		// Device characteristics
					 //FALSE,							// Not an exclusive device
					 TRUE,							// Not an exclusive device
					 &g_Driver.pDevice);			// Returned ptr to Device Object

		if(!NT_SUCCESS(status)) {
			LOGI("Couldn't create the device object\n");
			return status;
		}

		g_Driver.pDevice->Flags |= DO_BUFFERED_IO | DO_DIRECT_IO;	// buffered I/O ����
		g_Driver.pDevice->Flags &= ~DO_DEVICE_INITIALIZING;
		// �ɺ��� ��ũ ����
		status = IoCreateSymbolicLink(&sDevLinkName, &sDeviceName);
		KeInitializeSpinLock(&g_Driver.intr.lock);

		if(!NT_SUCCESS(status)) {
			LOGE("Couldn't create symbolic link\n");
			IoDeleteDevice(g_Driver.pDevice);		// �ٺ��̽� ����
			g_Driver.pDevice	= NULL;
		}
	}

	if(pDevExt) {
		pDevExt->iIndex	= g_Driver.info.card_count;
		g_Driver.pDevExt[pDevExt->iIndex]	= pDevExt;
		g_Driver.info.card_count++;
		{
			// ����̽� ���� ���
			TD_DEVICE_INFO* pDevInfo = &g_Driver.info.device[pDevExt->iIndex];
			unsigned int link_status;
			ReadConfigSpace(pDevExt->pDevice, (PUCHAR)&link_status, 0x144, sizeof(unsigned int));	// config space �� ���ؼ� ����� ������ �ʴ´�. slave bus �� 0x144 �д� �۾��� Font-end driver �� �߰��Ǿ� �ִ�.
			ReadConfigSpace(pDevExt->pDevice, (PUCHAR)&pDevInfo->sub_system.codes, 0x2C, sizeof(unsigned int));
			{
				// power management disable �ϱ�
				unsigned int	reg;
				ReadConfigSpace(pDevExt->pDevice, (PUCHAR)&reg, 0x10, sizeof(unsigned int));
				reg	&= ~3;
				WriteConfigSpace(pDevExt->pDevice, (PUCHAR)&reg, 0x10, sizeof(unsigned int));
			}
			pDevInfo->pcie.gen		= (link_status & 1) + 1;
			pDevInfo->pcie.lanes	= 1 << ((link_status >> 1) & 3);
			LOGI("    Card[%d] : VID(0x%04X), DID(0x%04X), PCIe %d.0 - %d Lanes\n", pDevExt->iIndex,
				 pDevInfo->sub_system.id.vendor,
				 pDevInfo->sub_system.id.device,
				 pDevInfo->pcie.gen,
				 pDevInfo->pcie.lanes);
			{
				unsigned int reg;
				ReadConfigSpace(pDevExt->pDevice, (PUCHAR)&reg, 0x4, sizeof(unsigned int));
				LOGI("REG[4] = 0x%X\n", reg);
				ReadConfigSpace(pDevExt->pDevice, (PUCHAR)&reg, 0x8, sizeof(unsigned int));
				LOGI("REG[4] = 0x%X\n", reg);
			}
		}
	}

	return status;
}

void DriverRelease(IN PDEVICE_EXTENSION	pDevExt)
{
	PAGED_CODE();
	{
		// ����̽� ���� DMA �����
		PDMA_MEMORY	pNode	= g_Driver.pDMALink;

		while(pNode) {
			PDMA_MEMORY	pNext	= pNode->pNext;

			if(pNode->pAdapter == pDevExt->dma.pAdapter) {
				DMAFree(pNode);
			}

			pNode	= pNext;
		}
	}

	// ���ͷ�Ʈ ���� ����
	if(pDevExt->pInterrupt) {
		IO_DISCONNECT_INTERRUPT_PARAMETERS	params;
		RtlZeroMemory(&params, sizeof(IO_DISCONNECT_INTERRUPT_PARAMETERS));
		params.Version								= CONNECT_MESSAGE_BASED;
		params.ConnectionContext.Generic			= pDevExt->pInterrupt;
		IoDisconnectInterruptEx(&params);
	}

	{
		// ����̹� Ȯ�� �����
		int i;

		for(i = 0; i < TD_MAX_DEVICE_COUNT; i++) {
			if(g_Driver.pDevExt[i] == pDevExt) {
				g_Driver.pDevExt[i]	= NULL;
				g_Driver.info.card_count--;
				break;
			}
		}
	}

	// ���� ���� ����̹� ����
	if(g_Driver.pDevice && !g_Driver.info.card_count) {
		UNICODE_STRING	uniWin32NameString;
		// �ɺ��� ��ũ �����
		RtlInitUnicodeString(&uniWin32NameString, SYMBOLIC_NAME);
		IoDeleteSymbolicLink(&uniWin32NameString);
		// ����̹� �����
		IoDeleteDevice(g_Driver.pDevice);
		g_Driver.pDevice	= NULL;
		LOGI("Driver is released.\n");
	}
}
