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
// Title : Global system configuration
// Rev.  : 6/26/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_CONFIG_MTSP_H__
#define __SYSTEM_CONFIG_MTSP_H__
#include "SystemConfig.h"

#define MTSP_MAGIC_CODE	   0x5053544D // 'MTSP'
#define MTSP_MAX_CORE_SIZE 32
#define MTSP_THREAD_SIZE   8
#define MTSP_GPR_SIZE	   128
#define MTSP_LMB_SIZE	   (MTSP_GPR_SIZE * MTSP_THREAD_SIZE)
#define MTSP_MAX_GMB_SIZE  65536

typedef enum {
	MTSP_COMMAND_MEMORY_DESC_0,
	MTSP_COMMAND_MEMORY_DESC_1,
	MTSP_COMMAND_MEMORY_DESC_2,
	MTSP_COMMAND_MEMORY_DESC_3,
	MTSP_COMMAND_SET_PC,
	MTSP_COMMAND_VIDEO_BASE,
	MTSP_COMMAND_INVOKE_MEMORY_OPERATION = 8,
	MTSP_COMMAND_SIGNAL_ASSERTION,
	MTSP_COMMAND_TREAD_CONTROL,
	MTSP_COMMAND_WAIT_SIGNAL,
} MTSP_COMMAND_ID;

typedef union {
	DWORD all;
	struct {
		DWORD busy			  : 1;
		DWORD mem_command_en  : 1;
		DWORD cmd_fifo_empty  : 1;
		DWORD cmd_fifo_full	  : 1;
		DWORD cmd_fifo_depth  : 4;
		DWORD cmd_fifo_rindex : 8;
		DWORD cmd_fifo_windex : 8;
		DWORD core_size		  : 8;
	};
} MTSP_STATUS;

typedef union {
	DWORD m[4];
	struct {
		// #0
		WORD id;		 // memory ID
		WORD index : 12; // register index
		WORD op	   : 4;	 // memory operation
		// #1
		WORD mask;	 // index mask
		BYTE size;	 // transfer size (n * 16 bytes)
		BYTE stride; // transfer stride (n * 16 bytes)
		// #2
		DWORD p_base; // physical base address
		// #3
		WORD g_base; // GMB base index
		WORD l_base; // LMB base index
	};
} MTSP_MEM_COMMAND;

typedef union {
	uint32_t m;
	struct {
		uint32_t branch_op		  : 4; // branch operation
		uint32_t branch_en		  : 1; // branch enable
		uint32_t intrrupt_enable  : 1; // interrupt assert when process is done
		uint32_t run_count_enable : 1; // run count increase when process is done
		uint32_t icache_clear	  : 1; // clear instruction cache when start
		uint32_t gcache_clear	  : 1; // clear GMB cache when start
		uint32_t lcache_clear	  : 1; // clear LMB cache when start
		uint32_t /*reserved*/	  : 0;
	};
} MTSP_REG_COMMAND;

typedef struct {
	union {
		uint32_t m; // = MTSP_MAGIC_CODE
		char	 c[4];
	} magic_code;

	union {
		uint32_t m;
		struct {
			uint32_t fifo_depth	  : 4;
			uint32_t lmb_depth	  : 4; // orignal depth - 6
			uint32_t gmb_depth	  : 4; // orignal depth - 6
			uint32_t /*reserved*/ : 4;
			uint32_t core_count	  : 16;
		};
	} config;

	union {
		uint32_t m;
		struct {
			uint32_t run_count	  : 16; // current run count value
			uint32_t /*reserved*/ : 14;
			uint32_t intr		  : 1; // interrupt invoked
			uint32_t busy		  : 1; // process is busy
		};
	} status;
} MTSP_REG_INFO;

#ifdef USE_TESTDRIVE
#	define MTSP_COMMAND_RING_SIZE 32

typedef struct {
	bool	   bUpdate;
	uint32_t   uLastUpdateID;
	REG_VALUE4 m[MTSP_GPR_SIZE];
} MTSP_REGMAP_GPRs;

typedef struct {
	bool	   bUpdate;
	uint32_t   uLastUpdateID;
	REG_VALUE4 m[MTSP_MAX_GMB_SIZE];
} MTSP_REGMAP_GMB;

typedef struct {
	bool	 bUpdate;
	uint64_t pc;
	union {
		uint32_t m;
		struct {
			uint32_t bAlive			: 1;
			uint32_t bBusy			: 1;
			uint32_t /* reserved */ : 0;
		};
	} status;

	MTSP_REGMAP_GPRs GPRs;
} MTSP_REGMAP_THREAD;

typedef struct {
	bool			   bUpdate;
	int				   iThreadCurrent;
	uint32_t		   uLMBLastUpdateID;
	MTSP_REGMAP_THREAD thread[MTSP_THREAD_SIZE];
} MTSP_REGMAP_CORE;

typedef struct {
	uint16_t id;
	uint32_t op;
	uint32_t data[4];
	uint64_t elapsed_time;
} MTSP_REGMAP_COMMAND_DATA;

typedef struct {
	bool					 bUpdate;
	uint32_t				 dwCurrent;
	uint32_t				 dwHead;
	MTSP_REGMAP_COMMAND_DATA cmd[MTSP_COMMAND_RING_SIZE];
} MTSP_REGMAP_COMMAND;

typedef struct : public SYSTEM_REGMAP {
	uint64_t lSimulationTime;
	uint64_t lStartupTime;

	struct {
		bool  bUpdate;
		short iCoreCount;
		short iCoreCurrent; // current core ID
		int	  iLMB_Count;
		int	  iGMB_Count;
	} system;

	MTSP_REGMAP_COMMAND command;
	MTSP_REGMAP_GMB		gmb;
	MTSP_REGMAP_CORE	core[MTSP_MAX_CORE_SIZE];
} MTSP_REGMAP;

#endif // USE_TESTDRIVE

#endif //__SYSTEM_CONFIG_MTSP_H__
