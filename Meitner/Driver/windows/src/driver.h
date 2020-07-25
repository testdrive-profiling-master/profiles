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
#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <ntddk.h>          // various NT definitions
#include <string.h>

#include "driver_testdrive.h"

// device hardware information
#define DRIVER_VENDOR_ID		0x10EE
#define DRIVER_DEVICE_ID		0x0505
#define DRIVER_CLASS_ID			0x0380

#define DEVICE_NAME			L"\\Device\\testdrive"
#define SYMBOLIC_NAME		L"\\DosDevices\\testdrive"
#define MAX_DMA_LENGTH		1024*1024*16				// 최대 1MB 단위로 전송됨.

#if DBG
#define LOGI(fmt, ...)		{DbgPrint("[TESTDRIVE] " fmt, __VA_ARGS__);}
#define LOGE(fmt, ...)		{DbgPrint("*E [TESTDRIVE] " fmt, __VA_ARGS__);}
#else
#define LOGI(...)
#define LOGE(...)
#endif

typedef struct PCI_BAR_t{
	PVOID				pVirtual;
	PHYSICAL_ADDRESS	PhyAddress;
	unsigned int		dwByteLength;
	unsigned int		bMemory;		// memory or i/o port
}PCI_BAR, *PPCI_BAR;

typedef struct DMA_MEMORY_t{
	PMDL					pMDL;
	ULONG					Length;
	PVOID					pUserVirtual;
	PVOID					VirtualAddress;
	PHYSICAL_ADDRESS		PhysicalAddress;
	PDMA_ADAPTER	  		pAdapter;
	PEPROCESS				pCurProcess;
	struct DMA_MEMORY_t*	pNext;
}DMA_MEMORY, *PDMA_MEMORY;

typedef struct DEVICE_EXTENSION_t{
	PDEVICE_OBJECT		pDevice;
	PDEVICE_OBJECT		pPhyDevice;
	PDEVICE_OBJECT		pLowerDevice;
	PIO_INTERRUPT_MESSAGE_INFO	pInterrupt;				// interrupt object
	int					iIndex;							// current PCIe-card index
	PKSEMAPHORE			pIrqSema;
	int					bar_count;
	PCI_BAR				bar[2];
	struct{
		PDMA_ADAPTER	  	pAdapter;
		ULONG				CommonBufferRegisterCount;		// Number of mapped DMA registers
	}dma;
}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

typedef struct DRIVER_INFO_t{
	PDEVICE_OBJECT		pDevice;

	TD_DRIVER_INFO		info;

	struct{
		KSPIN_LOCK			lock;
		PIRP				Irp;
		int					iCount;
	}intr;
	PDEVICE_EXTENSION	pDevExt[TD_MAX_DEVICE_COUNT];
	PDMA_MEMORY			pDMALink;
}DRIVER_INFO, *PDRIVER_INFO;

extern DRIVER_INFO		g_Driver;

DRIVER_INITIALIZE		DriverEntry;
DRIVER_UNLOAD			DriverUnload;
__drv_dispatchType(IRP_MJ_CREATE)			DRIVER_DISPATCH		DispatchCreate;
__drv_dispatchType(IRP_MJ_CLOSE)			DRIVER_DISPATCH		DispatchClose;
__drv_dispatchType(IRP_MJ_PNP)				DRIVER_DISPATCH		DispatchPNP;
__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)	DRIVER_DISPATCH		DispatchControl;
DRIVER_ADD_DEVICE		DispatchAddDevice;
DRIVER_DISPATCH			DefaultPnpHandler;
DRIVER_DISPATCH			DispatchPNPStartDevice;
DRIVER_DISPATCH			DispatchPNPStopDevice;
DRIVER_DISPATCH			DispatchPNPRemoveDevice;

NTSTATUS DriverInitialize(IN PDRIVER_OBJECT pDriverObject, IN PDEVICE_EXTENSION	pDevExt);
void DriverRelease(IN PDEVICE_EXTENSION	pDevExt);
NTSTATUS ReadWriteConfigSpace(PDEVICE_OBJECT pDevObj, PUCHAR pBuffer, ULONG Offset, ULONG Length, int bRead);
#define ReadConfigSpace(pDevObj, pBuffer, Offset, Length)	ReadWriteConfigSpace(pDevObj, pBuffer, Offset, Length, 1)
#define WriteConfigSpace(pDevObj, pBuffer, Offset, Length)	ReadWriteConfigSpace(pDevObj, pBuffer, Offset, Length, 0)
BOOLEAN GetDeviceAddresses(IN PDEVICE_OBJECT pDevObj);
NTSTATUS TransactionREG(PTD_TRANSACTION_REG pTrans, ULONG32* pData);
NTSTATUS TransactionMEM(PTD_TRANSACTION_MEM pTrans, ULONG64* pData);
NTSTATUS DMA_Bind(PTD_DMA_BIND pBind, PTD_DMA_MEMORY pData);
NTSTATUS SetupDmaAdapter(IN PDEVICE_OBJECT pDevObj);
PDMA_MEMORY DMAAlloc(unsigned int dwByteSize);
BOOLEAN DMAFree(PDMA_MEMORY pDMA);

VOID SetInterruptEvent(BOOLEAN bLock, PIRP Irp, ULONG MessageID);
IO_DPC_ROUTINE				DpcForIsr;
KMESSAGE_SERVICE_ROUTINE	ISR_Messaged;
KSERVICE_ROUTINE			ISR_FallBack;
BOOLEAN ISR_Messaged(IN struct _KINTERRUPT *Interrupt, IN PVOID ServiceContext, IN ULONG MessageID);
BOOLEAN ISR_FallBack(IN struct _KINTERRUPT *Interrupt, IN PVOID ServiceContext);

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, DriverUnload)
#pragma alloc_text(PAGE, DriverInitialize)
#pragma alloc_text(PAGE, DriverRelease)

#pragma alloc_text(PAGE, DispatchPNP)
#pragma alloc_text(PAGE, DefaultPnpHandler)
#pragma alloc_text(PAGE, DispatchCreate)
#pragma alloc_text(PAGE, DispatchClose)
#pragma alloc_text(PAGE, DispatchAddDevice)
#pragma alloc_text(PAGE, DispatchControl)
#pragma alloc_text(PAGE, DispatchPNPStartDevice)
#pragma alloc_text(PAGE, DispatchPNPStopDevice)
#pragma alloc_text(PAGE, DispatchPNPRemoveDevice)
#endif // ALLOC_PRAGMA

#endif//__DRIVER_H__
