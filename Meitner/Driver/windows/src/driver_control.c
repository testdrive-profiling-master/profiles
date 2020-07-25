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

#pragma LOCKEDCODE
VOID OnCancelInterrupt(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(pDeviceObject);
	IoCancelIrp(Irp);
}

#pragma PAGEDCODE
NTSTATUS DispatchControl(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION  irpSp;// Pointer to current stack location
	NTSTATUS            status = STATUS_SUCCESS;// Assume success
	UNREFERENCED_PARAMETER(pDeviceObject);		// Naming driver : DO NOT USE THIS!!!
	PAGED_CODE();
	irpSp			= IoGetCurrentIrpStackLocation(Irp);

	// Determine which I/O control code was specified.
	switch(irpSp->Parameters.DeviceIoControl.IoControlCode) {
	case IOCTL_COMMAND_TRANSACTION_REG: {
		PTD_TRANSACTION_REG pTrans	= (PTD_TRANSACTION_REG)Irp->AssociatedIrp.SystemBuffer;
		ULONG32* pData				= (ULONG32*)MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
		status						= TransactionREG(pTrans, pData);
	}
	break;

	case IOCTL_COMMAND_TRANSACTION_MEM: {
		PTD_TRANSACTION_MEM pTrans	= (PTD_TRANSACTION_MEM)Irp->AssociatedIrp.SystemBuffer;
		ULONG64* pData				= (ULONG64*)MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
		status						= TransactionMEM(pTrans, pData);
	}
	break;

	case IOCTL_COMMAND_DMA_BIND: {
		PTD_DMA_BIND pBind			= (PTD_DMA_BIND)Irp->AssociatedIrp.SystemBuffer;
		PTD_DMA_MEMORY pDMA			= (PTD_DMA_MEMORY)MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
		status						= DMA_Bind(pBind, pDMA);
	}
	break;

	case IOCTL_COMMAND_DMA_ALLOC:
		LOGI("Called IOCTL_DMA_ALLOC\n");
		// http://www.techtalkz.com/microsoft-device-drivers/285462-how-mapping-allocatecommonbuffers-memoey-user-mode.html
		{
			PTD_DMA_MEMORY pUser		= (PTD_DMA_MEMORY)Irp->AssociatedIrp.SystemBuffer;
			pUser->hDMA.pointer			= (PVOID)DMAAlloc(pUser->byte_size);

			if(pUser->hDMA.pointer) {
				PDMA_MEMORY pDMA			= (PDMA_MEMORY)pUser->hDMA.pointer;
				pUser->vir_addr.pointer		= pDMA->pUserVirtual;
				Irp->IoStatus.Information	= sizeof(TD_DMA_MEMORY);
			} else {
				status		= STATUS_INSUFFICIENT_RESOURCES;
			}
		}
		break;

	case IOCTL_COMMAND_DMA_FREE: {
		PTD_DMA_MEMORY pUser = (PTD_DMA_MEMORY)Irp->AssociatedIrp.SystemBuffer;

		if(DMAFree((PDMA_MEMORY)pUser->hDMA.pointer)) {
			pUser->hDMA.pointer			= NULL;
			pUser->vir_addr.pointer		= NULL;
			Irp->IoStatus.Information	= sizeof(TD_DMA_MEMORY);
		} else {
			status		= STATUS_INSUFFICIENT_RESOURCES;
		}
	}
	break;

	case IOCTL_COMMAND_INFOMATION:
		// 정보 복사
		RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &g_Driver.info, sizeof(TD_DRIVER_INFO));
		Irp->IoStatus.Information = sizeof(TD_DRIVER_INFO);
		break;

	case IOCTL_COMMAND_INTERRUPT_LOCK: {
		Irp->IoStatus.Status		= status;
		Irp->IoStatus.Information	= 0;
		{
			SetInterruptEvent(TRUE, Irp, 0);
		}
		IoMarkIrpPending(Irp);
		//IoStartPacket(pDeviceObject, Irp, NULL, OnCancelInterrupt);	// IRP 를 큐잉한다. 나중에 StartIO 가 호출된다. 여기서는 후처리가 필요 없으므로 생략한다.
		status = STATUS_PENDING;
		LOGI("IOCTL_COMMAND_INTERRUPT_LOCK is called\n");
		return STATUS_PENDING;
	}

	case IOCTL_COMMAND_INTERRUPT_FREE: {
		SetInterruptEvent(FALSE, NULL, 0);
		break;
	}
	break;

	default:
		// 지정되지 않은 명령
		status = STATUS_INVALID_DEVICE_REQUEST;
		LOGI("ERROR: unrecognized IOCTL %x\n", irpSp->Parameters.DeviceIoControl.IoControlCode);
		break;
	}

	// I/O 동작을 완료시킨다.
	Irp->IoStatus.Status = status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS ReadWriteConfigSpace(PDEVICE_OBJECT pDevObj, PUCHAR pBuffer, ULONG Offset, ULONG Length, int bRead)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	PIO_STACK_LOCATION nextStackLocation;	// Next IRP stack location.
	IO_STATUS_BLOCK ioStatus;				// I/O status block for the results.
	KEVENT event;							// Event object used to wait for results.
	PIRP pIrp;								// IRP to send to bus driver to get data.
	// Initialize the event object.
	KeInitializeEvent(&event, NotificationEvent, FALSE);
	// Build an IRP to send.
	pIrp = IoBuildSynchronousFsdRequest(IRP_MJ_PNP, pDevExt->pLowerDevice, NULL, 0, NULL, &event, &ioStatus);
	// Get access to the lower driver's parameters.
	nextStackLocation = IoGetNextIrpStackLocation(pIrp);
	// Make sure this IRP is a PNP IRP (from lower drivers' perspective).
	nextStackLocation->MajorFunction = IRP_MJ_PNP;
	// Handle read/write cases.
	nextStackLocation->MinorFunction = bRead ? IRP_MN_READ_CONFIG : IRP_MN_WRITE_CONFIG;
	// Setup the parameters to do the config read.
	nextStackLocation->Parameters.ReadWriteConfig.WhichSpace = 0;
	nextStackLocation->Parameters.ReadWriteConfig.Buffer	 = pBuffer;
	nextStackLocation->Parameters.ReadWriteConfig.Offset	 = Offset;
	nextStackLocation->Parameters.ReadWriteConfig.Length	 = Length;
	// Initialize the status.
	pIrp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	// Call the driver stack to process this request.
	status = IoCallDriver(pDevExt->pLowerDevice, pIrp);

	if(NT_SUCCESS(status)) {
		// Check to see if the lower driver will complete this later.
		if(status == STATUS_PENDING) {
			// Wait for the lower IRP to complete the request.
			KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);
		}

		// Get the IRP completion status from the I/O status block.
		status = ioStatus.Status;

		if(!NT_SUCCESS(status))
			LOGI("A lower driver failed the IRP with status: 0x%08X\n", status);
	} else {
		LOGI("IoCallDriver failed\n");
	}

	return status;
}

BOOLEAN GetDeviceAddresses(IN PDEVICE_OBJECT pDevObj)
{
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	int i;

	for(i = 0; i < pDevExt->bar_count; i++) {
		PPCI_BAR	pBar	= &pDevExt->bar[i];

		if(pBar->bMemory) {
			// memory 일 경우
			pBar->pVirtual	= MmMapIoSpace(pBar->PhyAddress, pBar->dwByteLength, MmNonCached);

			if(!pBar->pVirtual) return FALSE;
		} else {
			// I/O port 일 경우
			pBar->pVirtual	= (PVOID)pBar->PhyAddress.LowPart;
		}

		LOGI("    Card[%d].Bar[%d] = Type(%s), Address(0x%p), Size(0x%X)\n",
			 pDevExt->iIndex, i * 2,
			 pBar->bMemory ? "Memory" : "I/O",
			 pBar->pVirtual, pBar->dwByteLength);
	}

	return TRUE;
}

NTSTATUS TransactionREG(PTD_TRANSACTION_REG pTrans, ULONG32* pData)
{
	PDEVICE_EXTENSION	pDevExt		= g_Driver.pDevExt[pTrans->dev_id];
	volatile ULONG32*	pBus		= (volatile ULONG32*)(((unsigned char*)pDevExt->bar[0].pVirtual) + pTrans->phy_address);
	unsigned int			i;
	LOGI("TRAN_REG W(%d), phy(0x%X) size(%d)\n", pTrans->is_write, pTrans->phy_address, pTrans->count);

	if(pTrans->is_write)
		for(i = 0; i < pTrans->count; i++) {
			*pBus = pData[i];	// write

			if(pTrans->is_incremental) pBus++;
		}
	else
		for(i = 0; i < pTrans->count; i++) {
			pData[i] = *pBus;	// read

			if(pTrans->is_incremental) pBus++;
		}

	return STATUS_SUCCESS;
}

NTSTATUS TransactionMEM(PTD_TRANSACTION_MEM pTrans, ULONG64* pData)
{
	PDEVICE_EXTENSION	pDevExt		= g_Driver.pDevExt[pTrans->dev_id];
	volatile ULONG64*	pBus		= (volatile ULONG64*)(((unsigned char*)pDevExt->bar[1].pVirtual) + pTrans->phy_address);
	unsigned int			i;
	LOGI("TRAN_MEM W(%d), phy(0x%X) size(%d)\n", pTrans->is_write, pTrans->phy_address, pTrans->count);

	if(pTrans->is_write)
		for(i = 0; i < pTrans->count; i++) pBus[i] = pData[i];	// write
	else
		for(i = 0; i < pTrans->count; i++) pData[i] = pBus[i];	// read

	return STATUS_SUCCESS;
}

NTSTATUS DMA_Bind(PTD_DMA_BIND pBind, PTD_DMA_MEMORY pData)
{
	// 	UNREFERENCED_PARAMETER(pBind);
	// 	UNREFERENCED_PARAMETER(pData);
	PDEVICE_EXTENSION	pDevExt		= g_Driver.pDevExt[pBind->dev_id];
	PDMA_MEMORY			pDMA		= (PDMA_MEMORY)pData->hDMA.pointer;
	/*--------------------------------------------------------------------------------------------------------
	// Xilinx DMA 맵핑 기술 (TestDrive 프로파일 뷰, Documents/Xilinx vc707 evaluation board/AXI bridge for PCIe Express 참조)
	// PCI 쪽 4개의 AXIBAR2PICEBAR
	// Bar 번호 주소 영역               사이즈   PCIE 레지스터 설정 영역
	// BAR #0   (0xD0000000~0xD3FFFFFF) 64MB   : 0x208(LowPart) 0x20C(HighPart)
	// BAR #1   (0xD4000000~0xD7FFFFFF) 64MB   : 0x210(LowPart) 0x214(HighPart)
	// BAR #2   (0xD8000000~0xDBFFFFFF) 64MB   : 0x218(LowPart) 0x21C(HighPart)
	// BAR #3   (0xDC000000~0xDFFFFFFF) 64MB   : 0x220(LowPart) 0x224(HighPart)
	// BAR #0/1 과 #2/3 을 한 세트로 취급한다.
	--------------------------------------------------------------------------------------------------------*/
	// ATT(address translation table) binding
	PHYSICAL_ADDRESS		page		= pDMA->PhysicalAddress;
	volatile unsigned int*	pATT		= (volatile unsigned int*)(((unsigned char*)pDevExt->bar[0].pVirtual) + 0x208 + (pBind->dma_num * 16));
	// DMA 메모리가 경계에 걸처 할당될 수 있으므로 최소 2개의 ATT 가 필요하다.
	pATT[0] = page.LowPart;
	pATT[1] = page.HighPart;
	page.QuadPart += 0x1000000;	// +64MB
	pATT[2] = page.LowPart;
	pATT[3] = page.HighPart;
	KeFlushIoBuffers(pDMA->pMDL, pBind->is_write ? FALSE : TRUE, TRUE);
	/*
	pDMA->pAdapter->DmaOperations->FlushAdapterBuffers(
		pDMA->pAdapter,
		pDMA->pMDL,
		NULL,
		pDMA->VirtualAddress,
		pDMA->Length,
		pFlush->is_write);					// Write(TRUE)/Read(FALSE)
	*/
	return STATUS_SUCCESS;
}

PDMA_MEMORY DMAAlloc(unsigned int dwByteSize)
{
	PDMA_MEMORY		pDMA	= NULL;

	if(!dwByteSize || !g_Driver.pDevExt[0]) return NULL;

	{
		// 사용자 메모리 할당
		PDMA_ADAPTER	pAdapter	= g_Driver.pDevExt[0]->dma.pAdapter;	// 임의로 선택
		pDMA		= (PDMA_MEMORY)ExAllocatePoolWithTag(NonPagedPool, sizeof(DMA_MEMORY), 0);

		if(!pDMA) return NULL;

		RtlZeroMemory(pDMA, sizeof(DMA_MEMORY));
		// DMA 메모리 할당
		pDMA->Length				= (dwByteSize + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1));	// 최소 페이지 할당량에 맞게 조정한다.
		pDMA->pCurProcess		= IoGetCurrentProcess();
		pDMA->pAdapter			= pAdapter;
		pDMA->VirtualAddress	= pAdapter->DmaOperations->AllocateCommonBuffer(
									  pAdapter,
									  pDMA->Length,
									  &pDMA->PhysicalAddress,
									  FALSE);

		if(!pDMA->VirtualAddress) {	// 할당할 수 없다면 해제 후 종료.
			DMAFree(pDMA);
			return NULL;
		}

		pDMA->pMDL		= IoAllocateMdl(pDMA->VirtualAddress,
										pDMA->Length,
										FALSE,
										FALSE,
										NULL);

		if(!pDMA->pMDL) {
			DMAFree(pDMA);
			return NULL;
		}

		// Finish building the MDL -- Fill in the "page portion"
		MmBuildMdlForNonPagedPool(pDMA->pMDL);
		// Map the buffer into user space.
		_try{
			pDMA->pUserVirtual =
			MmMapLockedPagesSpecifyCache(pDMA->pMDL,			// MDL
			UserMode,				// Mode
			MmCached,				// Caching Should be MmNonCached?
			NULL,					// Address
			FALSE,					// Bugcheck
			NormalPagePriority);	// Priority
		}
		_except(EXCEPTION_EXECUTE_HANDLER) {
			pDMA->pUserVirtual = NULL;
		}

		if(!pDMA->pUserVirtual) {
			DMAFree(pDMA);
			return NULL;
		}

		// Link to head
		{
			pDMA->pNext			= g_Driver.pDMALink;
			g_Driver.pDMALink	= pDMA;
		}
	}
	return pDMA;
}

BOOLEAN DMAFree(PDMA_MEMORY pDMA)
{
	if(!pDMA || !pDMA->pAdapter) return FALSE;

	{
		// Unlink
		PDMA_MEMORY	pNode	= g_Driver.pDMALink;

		if(pNode == pDMA) {
			g_Driver.pDMALink	= pDMA->pNext;
		} else {
			while(pNode) {
				if(pNode->pNext == pDMA) {
					pNode->pNext	= pDMA->pNext;
					break;
				}

				pNode	= pNode->pNext;
			}
		}
	}
	{
		// DMA 메모리 해제
		PDMA_ADAPTER	pAdapter	= pDMA->pAdapter;

		if(pDMA->pMDL) {
			if(pDMA->pUserVirtual) {
				MmUnmapLockedPages(pDMA->pUserVirtual, pDMA->pMDL);
				pDMA->pUserVirtual	= NULL;
			}

			IoFreeMdl(pDMA->pMDL);
			pDMA->pMDL	= NULL;
		}

		if(pDMA->VirtualAddress) {
			pAdapter->DmaOperations->FreeCommonBuffer(
				pAdapter,
				pDMA->Length,
				pDMA->PhysicalAddress,
				pDMA->VirtualAddress,
				FALSE);
		}
	}
	// 사용자 메모리 해제
	ExFreePool((PVOID)pDMA);
	return TRUE;
}

NTSTATUS SetupDmaAdapter(IN PDEVICE_OBJECT pDevObj)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	DEVICE_DESCRIPTION deviceDescription;
//	PALLOCATE_COMMON_BUFFER AllocateCommonBuffer;
	// http://www.microsoft.com/whdc/driver/tips/DMA.mspx DMA 32bit로 제약하는 방법
	LOGI("In SetupDmaAdapter.\n");
	// Zero out the device description sructure.
	RtlZeroMemory(&deviceDescription, sizeof(DEVICE_DESCRIPTION));
	// Set the length of the buffer, but we may not get this much.
	pDevExt->dma.CommonBufferRegisterCount = MAX_DMA_LENGTH;
	// Fill the device description structure.
	deviceDescription.Version			= DEVICE_DESCRIPTION_VERSION1;
	deviceDescription.Master			= TRUE;
	deviceDescription.ScatterGather		= FALSE;
	deviceDescription.InterfaceType		= PCIBus;
	deviceDescription.MaximumLength		= (ULONG) - 1;
	deviceDescription.Dma64BitAddresses	= TRUE;
	//deviceDescription.IgnoreCount		= TRUE;
	// ... The rest aren't used.
	// Compute the maximum number of mapping registers needed for the maximum transfer size.
	// One is added to allow the maximum transfer to span a page.
	pDevExt->dma.CommonBufferRegisterCount = (pDevExt->dma.CommonBufferRegisterCount / PAGE_SIZE) + 1;
	pDevExt->dma.pAdapter = IoGetDmaAdapter(pDevExt->pPhyDevice,
											&deviceDescription,
											&(pDevExt->dma.CommonBufferRegisterCount));

	// Fail if the adapter can't be assigned.
	if(!pDevExt->dma.pAdapter) {
		LOGI("Can't allocate enough resources for DMA adapter.\n");
		status = STATUS_INSUFFICIENT_RESOURCES;
	}

	//LOGI("Got DMA register count: %d\n", pDevExt->dma.CommonBufferRegisterCount);
	/*	In application
			call AllocateUserPhysicalPages to allocate physical pages
			call VirtualAlloc to reserve virtual pages
			call MapUserPhysicalPages to create maps.
	*/
	return status;
}

#pragma LOCKEDCODE
VOID SetInterruptEvent(BOOLEAN bLock, PIRP Irp, ULONG MessageID)
{
	KIRQL oldirql;
	UNREFERENCED_PARAMETER(MessageID);
	KeAcquireSpinLock(&g_Driver.intr.lock, &oldirql);

	if(bLock) {	// lock
		if(g_Driver.intr.Irp) {	// 이미 다른 루틴?에 의해 락이 걸리면, 해당 ISR을 호출
			IoCompleteRequest(g_Driver.intr.Irp, IO_NO_INCREMENT);
			g_Driver.intr.Irp		= NULL;
			g_Driver.intr.iCount	= 0;
		}

		if(g_Driver.intr.iCount > 0) {	// 인터럽트가 이전에 뜬 상태이면, 현재 ISR을 호출
			IoCompleteRequest(Irp, IO_NO_INCREMENT);
			g_Driver.intr.iCount	= 0;
		} else {	// 인터럽트가 뜬 적이 없다면 IRP 만 등록
			g_Driver.intr.Irp		= Irp;
		}
	} else {	// release
		if(g_Driver.intr.Irp) {	// 등록된 IRP 가 있을 경우, ISR을 호출
			IoCompleteRequest(g_Driver.intr.Irp, IO_NO_INCREMENT);
			g_Driver.intr.Irp		= NULL;
			g_Driver.intr.iCount	= 0;
		} else {	// 없을 경우엔 인터럽트 참조 카운트만 증가
			g_Driver.intr.iCount++;
		}
	}

	KeReleaseSpinLock(&g_Driver.intr.lock, oldirql);
}

VOID DpcForIsr(PKDPC Dpc, struct _DEVICE_OBJECT* DeviceObject, struct _IRP* Irp, PVOID Context)
{
	// DISPATCH_LEVEL
	PDEVICE_EXTENSION	pDevExt		= (PDEVICE_EXTENSION)Context;
	ULONG				MessageID	= (ULONG)((UINT64)Irp);
	UNREFERENCED_PARAMETER(Dpc);			// not used
	UNREFERENCED_PARAMETER(Irp);			// Irp = NULL
	UNREFERENCED_PARAMETER(DeviceObject);	// Not used
	LOGI("DpcForIsr is occured #%d\n", MessageID);
	SetInterruptEvent(FALSE, NULL, MessageID);
	//IoCompleteRequest(pDevExt->pDevice->CurrentIrp, IO_NO_INCREMENT);	// pDevExt->pDevice->CurrentIrp = NULL
	IoStartNextPacket(pDevExt->pDevice, TRUE);			// next dpc go on.
}

BOOLEAN ISR_Messaged(IN struct _KINTERRUPT* Interrupt, IN PVOID ServiceContext, IN ULONG MessageID)
{
	// DIRQL level
	PDEVICE_EXTENSION	pDevExt = (PDEVICE_EXTENSION)ServiceContext;
	UNREFERENCED_PARAMETER(Interrupt);
	UNREFERENCED_PARAMETER(ServiceContext);
	UNREFERENCED_PARAMETER(MessageID);
	LOGI("ISR_Messaged is occured. #%d\n", MessageID);
	IoRequestDpc(pDevExt->pDevice, (struct _IRP*)MessageID, ServiceContext);
	return TRUE;
}

BOOLEAN ISR_FallBack(IN struct _KINTERRUPT* Interrupt, IN PVOID ServiceContext)
{
	// DIRQL level
	PDEVICE_EXTENSION	pDevExt = (PDEVICE_EXTENSION)ServiceContext;
	UNREFERENCED_PARAMETER(Interrupt);
	UNREFERENCED_PARAMETER(ServiceContext);
	LOGI("ISR_FallBack is occured.\n");
	IoRequestDpc(pDevExt->pDevice, NULL, ServiceContext);
	return TRUE;
}
