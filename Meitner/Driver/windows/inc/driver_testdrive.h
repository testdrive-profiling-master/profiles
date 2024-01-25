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
// Title : WDM PCIe Driver for TestDrive
// Rev.  : 1/25/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __DRIVER_TESTDRIVE_H__
#define __DRIVER_TESTDRIVE_H__
#ifndef _DDK_DRIVER_
#include <windows.h>
#include <winioctl.h>
#endif
#include "driver_version.inl"

#define DEV_NAME					"testdrive_pcie"			// driver name
#define DEV_PATH					"\\\\.\\" DEV_NAME
#define TD_MAX_DEVICE_COUNT			64						// maximum device stack count

// I/O command list
typedef enum {
	IOCTL_COMMAND_INFOMATION			= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_DMA_ALLOC				= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_DMA_FREE				= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS),
	IOCTL_COMMAND_TRANSACTION_REG		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_TRANSACTION_MEM		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_TRANSACTION_DMA		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x806, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_INTERRUPT_LOCK		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x807, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
	IOCTL_COMMAND_INTERRUPT_FREE		= CTL_CODE(FILE_DEVICE_UNKNOWN, 0x808, METHOD_OUT_DIRECT, FILE_ANY_ACCESS),
} IOCTL_COMMAND;

// device IDs
typedef union TD_SYSTEM_ID_t {
	unsigned int		codes;
	struct {
		unsigned short		vendor;
		unsigned short		device;
	};
} TD_SYSTEM_ID;

// kernel address
typedef union TD_KADDRESS_t {
	ULONG64		addr64;
	struct {
		unsigned int	low;
		unsigned int	hi;
	} addr;
	PVOID		pointer;
} TD_KADDRESS;

// device information
typedef struct TD_DEVICE_INFO_t {
	struct {
		TD_SYSTEM_ID	main;
		TD_SYSTEM_ID	sub;
	} id;

	union {
		unsigned int		m;
		struct {
			unsigned int	gen				: 4;	// gen number
			unsigned int	lanes			: 4;	// lane count
			unsigned int	bars			: 3;	// BAR count
			unsigned int	/*reserved*/	: 0;
		};
	} pcie;
} TD_DEVICE_INFO;

// driver information
typedef struct TD_DRIVER_INFO_t {
	struct {
		union {
			unsigned int	all;
			struct {
				unsigned short	major;
				unsigned short	minor;
			};
		};
		unsigned int	build;
	} version;
	int				card_count;
	TD_DEVICE_INFO	device[TD_MAX_DEVICE_COUNT];
} TD_DRIVER_INFO;

// DMA memory structure
typedef struct TD_DMA_MEMORY_t {
	TD_KADDRESS		hDMA;
	TD_KADDRESS		vir_addr;
	unsigned int	byte_size;
} TD_DMA_MEMORY;

// reg/memory/DMA transaction structures
typedef struct TD_TRANSACTION_REG_t {
	struct {
		unsigned int	dev_id			: 8;	// device ID
		unsigned int	bar_id			: 2;	// bar ID
		unsigned int	is_write		: 1;	// write operation
		unsigned int	/*reserved*/	: 0;
	};
	unsigned int	phy_address;			// inner physical address
	unsigned int	reg_data;				// read write data
} TD_TRANSACTION_REG;

typedef struct TD_TRANSACTION_MEM_t {
	struct {
		unsigned int	dev_id			: 8;	// device ID
		unsigned int	is_write		: 1;	// write operation
		unsigned int	/*reserved*/	: 0;
	};
	unsigned int	byte_size;				// byte size
	unsigned int	phy_address;			// inner physical address
} TD_TRANSACTION_MEM;

typedef struct TD_TRANSACTION_DMA_t {
	struct {
		unsigned int	dev_id			: 8;	// device ID
		unsigned int	is_write		: 1;	// write operation
		unsigned int	/*reserved*/	: 0;
	};
	TD_KADDRESS		hDMA;					// DMA object
	TD_KADDRESS		phy_address;			// target physical address
	unsigned int	offset;					// DMA memory address offset
	unsigned int	size;					// transaction size
} TD_TRANSACTION_DMA;

#endif//__DRIVER_TESTDRIVE_H__
