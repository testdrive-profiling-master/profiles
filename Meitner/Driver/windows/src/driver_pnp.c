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

NTSTATUS DispatchAddDevice(IN PDRIVER_OBJECT pDriverObject, IN PDEVICE_OBJECT pPhysDevObj)
{
	NTSTATUS            status		= STATUS_SUCCESS;
	PDEVICE_OBJECT		pDevObj		= NULL;
	PDEVICE_EXTENSION	pDevExt		= NULL;
	PAGED_CODE();
	LOGI("%s entered.\n", __FUNCTION__);
	{
		// http://ridl.cfd.rit.edu/docs/Leach%20controller/Voodoo/source%20code%20for%20driver/astropci.cpp
		status	= IoCreateDevice(
					  pDriverObject,
					  sizeof(DEVICE_EXTENSION),
					  NULL,
					  FILE_DEVICE_UNKNOWN,
					  FILE_DEVICE_SECURE_OPEN,
					  FALSE,
					  &pDevObj);

		if(NT_SUCCESS(status)) {
			pDevObj->Flags			|= DO_DIRECT_IO;
			// Clear the Device Initializing bit since the FDO was created
			pDevObj->Flags			&= ~(DO_DEVICE_INITIALIZING | DO_DEVICE_HAS_NAME);	// outside of DriverEntry.
			pDevExt					= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
			memset(pDevExt, 0, sizeof(DEVICE_EXTENSION));
			pDevExt->pDevice		= pDevObj;
			pDevExt->pPhyDevice		= pPhysDevObj;
			pDevExt->pLowerDevice	= IoAttachDeviceToDeviceStack(pDevObj, pPhysDevObj);

			if(!pDevExt->pLowerDevice) {
				status	= STATUS_DEVICE_REMOVED;
			}

			// register DPC for ISR
			IoInitializeDpcRequest(pDevObj, DpcForIsr);
		}

		if(!NT_SUCCESS(status)) {
			// fails, must delete device object
			if(pDevObj) {
				IoDeleteDevice(pDevObj);
				pDevObj	= NULL;
			}
		}
	}

	if(NT_SUCCESS(status))
		status	= DriverInitialize(pDriverObject, pDevExt);

	return status;
}

VOID FreeDeviceResources(IN PDEVICE_OBJECT pDevObj)
{
	PDEVICE_EXTENSION	pDevExt	= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	int i;
	// 인터럽트 해제
	SetInterruptEvent(FALSE, NULL, 0);	// release interrupt

	// I/O sapce 지우기
	for(i = 0; i < pDevExt->bar_count; i++) {
		PPCI_BAR	pBar	= &pDevExt->bar[i];

		if(pBar->pVirtual && pBar->bMemory)
			MmUnmapIoSpace(pBar->pVirtual, pBar->dwByteLength);

		pBar->pVirtual	= NULL;
	}

	if(pDevExt->dma.pAdapter) {
		pDevExt->dma.pAdapter->DmaOperations->PutDmaAdapter(pDevExt->dma.pAdapter);
		pDevExt->dma.pAdapter	= NULL;
	}
}

NTSTATUS DispatchPNPStartDevice(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION	pIrpStack	= IoGetCurrentIrpStackLocation(pIrp);
	PDEVICE_EXTENSION	pDevExt		= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PCM_RESOURCE_LIST				pResourceList;
	PCM_FULL_RESOURCE_DESCRIPTOR	pFullDescriptor;
	PCM_PARTIAL_RESOURCE_LIST		pPartialList;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR	pPartialDescriptor;
	int i;
	//PBASE_ADDRESS		baseAddress	= pDevExt->fBaseAddresses;
	PAGED_CODE();
	LOGI("%s entered.\n", __FUNCTION__);
	// 이미 추출된 기본 리소스 얻기
	pResourceList	= pIrpStack->Parameters.StartDevice.AllocatedResourcesTranslated;
	pFullDescriptor	= pResourceList->List;
	pPartialList	= &pFullDescriptor->PartialResourceList;

	for(i = 0; i < (int)pPartialList->Count; i++) {
		pPartialDescriptor	= &pPartialList->PartialDescriptors[i];

		switch(pPartialDescriptor->Type) {
		case CmResourceTypeInterrupt:
			LOGI("    [CmResourceTypeInterrupt] Level = %d, Vector = %d\n", pPartialDescriptor->u.Interrupt.Level, pPartialDescriptor->u.Interrupt.Vector);
			{
				// 인터럽트 연결
				IO_CONNECT_INTERRUPT_PARAMETERS	params;
				RtlZeroMemory(&params, sizeof(IO_CONNECT_INTERRUPT_PARAMETERS));
				pDevExt->pInterrupt = NULL;
				params.Version									= CONNECT_MESSAGE_BASED;		// MSI 인터럽트 설정 (Vista 이상 OS 요구됨.)
				params.MessageBased.PhysicalDeviceObject		= pDevExt->pPhyDevice;
				params.MessageBased.MessageServiceRoutine		= ISR_Messaged;
				params.MessageBased.ServiceContext				= (PVOID)pDevExt;
				params.MessageBased.SpinLock					= NULL;
				params.MessageBased.SynchronizeIrql				= (KIRQL)(pPartialDescriptor->u.Interrupt.Level);
				params.MessageBased.FloatingSave				= FALSE;						// FP 레지스터(SSE등...) 사용을 context switching 을 위한 이전 FP 레지스터 저장 사용 안함.
				params.MessageBased.FallBackServiceRoutine		= ISR_FallBack;
				params.MessageBased.ConnectionContext.Generic	= &pDevExt->pInterrupt;
				status = IoConnectInterruptEx(&params);

				if(NT_SUCCESS(status)) {
					// working...^^, windows Vista 이상의 OS 임. 동작 중.
					g_Driver.info.device[pDevExt->iIndex].pcie.msi_count = (unsigned short)((*(params.MessageBased.ConnectionContext.InterruptMessageTable))->MessageCount);
					LOGI("MSI Interrupt connection is successful.\n");
				} else {
					// OS 가 MSI 를 지원하지 않음. (다른 우회 방법이 있으나... 무시!)
					LOGE("MSI Interrupt connection is failed.(VERSION = 0x%X)\n", params.Version);
				}
			}
			break;

		case CmResourceTypeDma:
			LOGI("    [CmResourceTypeDma]\n");
			// 아무것도 하지 않음.
			break;

		case CmResourceTypePort:
			LOGI("    [CmResourceTypePort] : Base = %08X, Length = %08X\n",
				 pPartialDescriptor->u.Memory.Start.LowPart,
				 (int)pPartialDescriptor->u.Memory.Length);
			// 아무것도 하지 않음.
			// I/O 포트는 pPartialDescriptor->u.Port.Start 의 하위 32 비트 만을 사용한다.
			break;

		case CmResourceTypeMemory:
			LOGI("    [CmResourceTypeMemory] : Base = %08X : %08X, Length = %08X\n",
				 pPartialDescriptor->u.Memory.Start.HighPart,
				 pPartialDescriptor->u.Memory.Start.LowPart,
				 (int)pPartialDescriptor->u.Memory.Length
				);
			{
				// Bar 메모리 할당.
				PPCI_BAR	pBar	= &pDevExt->bar[pDevExt->bar_count];	// 0 또는 1 뿐임.
				pBar->bMemory		= TRUE;
				pBar->PhyAddress	= pPartialDescriptor->u.Memory.Start;
				pBar->dwByteLength	= pPartialDescriptor->u.Memory.Length;
				pDevExt->bar_count++;
			}
			break;
		}
	}

	LOGI("    Device Initializing....\n");
	GetDeviceAddresses(pDevObj);
	SetupDmaAdapter(pDevObj);
	status	= DefaultPnpHandler(pDevObj, pIrp);
	return status;
}

NTSTATUS DispatchPNPStopDevice(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PAGED_CODE();
	status	= DefaultPnpHandler(pDevObj, pIrp);
	return status;
}

NTSTATUS DispatchPNPRemoveDevice(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS			status	= STATUS_SUCCESS;
	PDEVICE_EXTENSION	pDevExt	= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PAGED_CODE();
	LOGI("%s entered.\n", __FUNCTION__);

	if(!pDevExt) {
		LOGI(__FUNCTION__ "Unexpected error : Invalid DeviceExtension...\n");
		status	= STATUS_INSUFF_SERVER_RESOURCES;
	} else {
		FreeDeviceResources(pDevObj);
		status	= DefaultPnpHandler(pDevObj, pIrp);

		if(pDevExt->pLowerDevice) {
			IoDetachDevice(pDevExt->pLowerDevice);
			pDevExt->pLowerDevice	= NULL;
		}

		IoDeleteDevice(pDevObj);
		// 전역 드라이버 지우기
		DriverRelease(pDevExt);
	}

	return status;
}

NTSTATUS DefaultPnpHandler(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS			status	= STATUS_SUCCESS;
	PDEVICE_EXTENSION	pDevExt	= (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PAGED_CODE();
	//ASSERT(pDevExt->pLowerDevice != NULL);
	IoSkipCurrentIrpStackLocation(pIrp);

	if(pDevExt->pLowerDevice)
		status	= IoCallDriver(pDevExt->pLowerDevice, pIrp);

	return status;
}

NTSTATUS DispatchPNP(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	NTSTATUS            status		= STATUS_SUCCESS;
	PIO_STACK_LOCATION	stack		= IoGetCurrentIrpStackLocation(pIrp);
	PAGED_CODE();

	switch(stack->MinorFunction) {
	case IRP_MN_START_DEVICE:
		status	= DispatchPNPStartDevice(pDevObj, pIrp);
		break;

	case IRP_MN_STOP_DEVICE:
		status	= DispatchPNPStopDevice(pDevObj, pIrp);
		break;

	case IRP_MN_REMOVE_DEVICE:
		status	= DispatchPNPRemoveDevice(pDevObj, pIrp);
		break;

	default:
		status	= DefaultPnpHandler(pDevObj, pIrp);
		break;
	}

	return status;
}
