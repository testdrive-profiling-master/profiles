//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : __PROJECT__ application
// Rev.  : 10/17/2026 Sat (clonextop@gmail.com)
//================================================================================
#include "common.h"

void TargetDeviceTest(EFI_PCI_IO_PROTOCOL *pci_io)
{
	EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *bar;
	for (int i = 0; i < 3; i++) {
		pci_io->GetBarAttributes(pci_io, i, NULL, (VOID **)&bar);
		LOG("    BAR#%d : address:0x%llX, size:0x%llX\n", i, bar->AddrRangeMin, bar->AddrLen);
		if (i == 2) { // BAR#2
			unsigned int *pMem = (unsigned int *)bar->AddrRangeMin;
			// read
			LOG("        0x%08X 0x%08X 0x%08X 0x%08X\n", pMem[0], pMem[1], pMem[2], pMem[3]);
			// write
			pMem[0] = 0xDEAD0001;
			pMem[1] = 0xDEAD0002;
			pMem[2] = 0xDEAD0003;
			pMem[3] = 0xDEAD0004;
		}
		FreePool(bar);
	}
}

void ShowDevices(void)
{
	LOGI("Enumerate devices.");

	EFI_STATUS			 Status;
	UINTN				 HandleCount;
	EFI_HANDLE			*HandleBuffer;
	UINTN				 Index;
	EFI_PCI_IO_PROTOCOL *pci_io;
	UINTN				 Segment, Bus, Device, Function;
	UINT16				 VendorId;
	UINT16				 DeviceId;
	UINT8				 ClassCode[3];

	// Get EFI_PCI_IO_PROTOCOL handle
	Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiPciIoProtocolGuid, NULL, &HandleCount, &HandleBuffer);

	if (EFI_ERROR(Status)) {
		LOGE("Failed to locate PCI I/O protocols! Status(%r)\n", Status);
	} else {
		LOG("Total %d PCI/PCIe device(s) found.\n\n", HandleCount);
		LOG("B/D/F      VID    DID    Class Code\n");
		LOG("-----------------------------------\n");

		for (Index = 0; Index < HandleCount; Index++) {
			if (EFI_ERROR(gBS->HandleProtocol(HandleBuffer[Index], &gEfiPciIoProtocolGuid, (VOID **)&pci_io)))
				continue;

			// Get PCI BDF position
			pci_io->GetLocation(pci_io, &Segment, &Bus, &Device, &Function);

			// Read Vendor/Device ID from PCI configuration.
			pci_io->Pci.Read(pci_io, EfiPciIoWidthUint16, 0x00, 1, &VendorId);
			pci_io->Pci.Read(pci_io, EfiPciIoWidthUint16, 0x02, 1, &DeviceId);
			pci_io->Pci.Read(pci_io, EfiPciIoWidthUint8, 0x09, 3, ClassCode);

			LOG("%02X:%02X:%d   0x%04X 0x%04X   0x%02X%02X%02X\n", (UINT8)Bus, (UINT8)Device, (UINT8)Function, VendorId, DeviceId,
				ClassCode[2], // Base Class
				ClassCode[1], // Sub Class
				ClassCode[0]  // Prog IF
			);
			if (VendorId == 0x1234 && DeviceId == 0xABCD) // target device
				TargetDeviceTest(pci_io);
		}

		gBS->FreePool(HandleBuffer); // Free handle
	}

	PAUSE
}

void TestReadFile(void)
{
	EFI_STATUS						 Status;
	UINTN							 HandleCount;
	EFI_HANDLE						*HandleBuffer;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SfProtocol;
	EFI_FILE_PROTOCOL				*RootVolume;
	EFI_FILE_PROTOCOL				*FileHandle;

	LOGI("File System test : search 'test.txt'\n");

	// Get EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID handle
	Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &HandleCount, &HandleBuffer);

	if (EFI_ERROR(Status)) {
		LOGE("Failed to locate Simple File System protocols! Status(%r)\n", Status);
	} else {
		LOGI("Found %d file system drive(s).\n", HandleCount);

		// search all drives
		for (int i = 0; i < HandleCount; i++) {
			// try to open drive
			if (EFI_ERROR(gBS->HandleProtocol(HandleBuffer[i], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&SfProtocol)))
				continue;

			// try to open volume's root directory
			if (EFI_ERROR(SfProtocol->OpenVolume(SfProtocol, &RootVolume)))
				continue;

			// try to open target file("test.txt")
			if (!EFI_ERROR(RootVolume->Open(RootVolume, &FileHandle, L"test.txt", EFI_FILE_MODE_READ, 0))) {
				LOGI("Successfully opened 'test.txt' on drive fs#%d:!\n", i);

				// read file
				UINT8 Buffer[256];
				UINTN BufferSize = sizeof(Buffer) - 1;
				if (!EFI_ERROR(FileHandle->Read(FileHandle, &BufferSize, Buffer))) {
					Buffer[BufferSize] = '\0';
					LOGI("- Content[%d bytes]:\n\n%a", BufferSize, (const char *)Buffer);
				} else {
					LOGE("\nCan't read file...\n");
				}

				// close file
				FileHandle->Close(FileHandle);
			}
			RootVolume->Close(RootVolume);
		}

		// 핸들 Buffer 메모리 해제
		gBS->FreePool(HandleBuffer);
	}

	PAUSE
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	g_SysTable = SystemTable;

	{
		BOOLEAN			   bRun	  = TRUE;
		static const char *Menu[] = {
			"Show device list.",
			"File-system test.",
			"Boot next Operate System.",
			"Power off.",
		};

		while (bRun) {
			switch (DO_MENU(Menu, "* __PROJECT__ Application v" __PROJECT_UPPER___VERSION " (" __PROJECT_UPPER___VERSION_DATE ")", "* Use [Up/Down] to move, [Enter] to select.")) {
			case 0: // show devices
				ShowDevices();
				break;
			case 1: // file system test
				TestReadFile();
				break;
			case 2: // boot to next
				LOG(" >> Reboot next system << ");
				Sleep(2000);
				bRun = FALSE;
				break;
			case 3: // shutdown
				LOG(" >> System will be shutdown << ");
				Sleep(2000);
				bRun = FALSE;
				gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
				break;
			default:
				LOGI("Invalid operation.\n");
				break;
			}
		}
	}

	return EFI_SUCCESS;
}
