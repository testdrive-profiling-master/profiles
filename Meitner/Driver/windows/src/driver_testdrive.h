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
#ifndef __DRIVER_TESTDRIVE_H__
#define __DRIVER_TESTDRIVE_H__
#ifndef _DDK_DRIVER_
#include <windows.h>
#include <winioctl.h>
#endif

#define DEV_NAME					"testdrive"
#define DEV_PATH					"\\\\.\\" DEV_NAME
#define TD_MAX_DEVICE_COUNT			32

// I/O command list
typedef enum{
	IOCTL_COMMAND_INFOMATION			= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_DMA_ALLOC				= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_DMA_FREE				= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_DMA_BIND				= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_TRANSACTION_REG		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_TRANSACTION_MEM		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_INTERRUPT_LOCK		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_INTERRUPT_FREE		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
}IOCTL_COMMAND;

typedef union TD_SYSTEM_ID_t{
	unsigned int		codes;
	struct{
		unsigned short		vendor;
		unsigned short		device;
	}id;
}TD_SYSTEM_ID;

typedef union TD_KADDRESS_t{
	ULONG64		addr64;
	struct{
		unsigned int	low;
		unsigned int	hi;
	}addr;
	PVOID		pointer;
}TD_KADDRESS;

// driver information
typedef struct TD_DEVICE_INFO_t{
	TD_SYSTEM_ID	sub_system;
	struct{
		unsigned short	gen;
		unsigned short	lanes;
		unsigned short	msi_count;
		unsigned short	reserved[5];
	}pcie;
}TD_DEVICE_INFO;

typedef struct TD_DRIVER_INFO_t{
	TD_SYSTEM_ID	system;
	unsigned int	card_count;
	TD_DEVICE_INFO	device[TD_MAX_DEVICE_COUNT];
}TD_DRIVER_INFO;

// DMA struct
typedef struct TD_DMA_MEMORY_t {
	TD_KADDRESS		hDMA;
	TD_KADDRESS		vir_addr;
	unsigned int	byte_size;
//	unsigned long	phy_addr;
}TD_DMA_MEMORY, *PTD_DMA_MEMORY;

typedef struct TD_DMA_BIND_t {
	unsigned char	dev_id;					// device ID
	unsigned char	dma_num;				// DMA number
	unsigned char	is_write;				// write operation
	unsigned char	count;					// binding count
} TD_DMA_BIND, *PTD_DMA_BIND;

// transaction structs
typedef struct TD_TRANSACTION_REG_t {
	unsigned char	dev_id;					// device ID
	unsigned char	is_write;				// write operation
	unsigned char	is_incremental;			// incremental address
	unsigned char	count;					// transaction count (32bit)
	unsigned int	phy_address;			// inner physical address
} TD_TRANSACTION_REG, *PTD_TRANSACTION_REG;

typedef struct TD_TRANSACTION_MEM_t {
	unsigned char	dev_id;					// device ID
	unsigned char	is_write;				// write operation
	unsigned int	count;					// transaction count (64bit)
	unsigned int	phy_address;			// inner physical address
} TD_TRANSACTION_MEM, *PTD_TRANSACTION_MEM;


typedef struct TD_TRANSACTION_DMA_t {
	unsigned char	dev_id;					// device ID
	unsigned char	bar_id;					// bar ID
	unsigned char	is_write;				// write operation
	unsigned int	phy_address, offset;	// inner physical address & offset
	unsigned int	size;					// transaction size
} TD_TRANSACTION_DMA, *PTD_TRANSACTION_DMA;

typedef union TD_INTERRUPT_t {
	ULONG64			m64;
	HANDLE			hSema;
} TD_INTERRUPT, *PTD_INTERRUPT;

#endif//__DRIVER_TESTDRIVE_H__
