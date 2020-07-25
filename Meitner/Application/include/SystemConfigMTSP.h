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
// Title : Global system configuration
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_CONFIG_MTSP_H__
#define __SYSTEM_CONFIG_MTSP_H__
#include "SystemConfig.h"

#define	SYSTEM_MAGIC_CODE					0x4D545350	// System magic code : "MTSP"
#define	GPRs_SIZE							32			// GPRs(general purpose registers)
#define THREAD_SIZE							12			// thread size per a core
#define	MAX_GPR_SIZE						128
#define	MAX_LMB_SIZE						2048
#define	MAX_GMB_SIZE						8196
#define	MAX_CORE_SIZE						32
#define	MAX_THREAD_TRACE_SIZE				4096
#define MAX_COMMAND_HISTORY_SIZE			256
#define MAX_SCRATCH_COUNTER_LOCAL_SIZE		2

typedef enum{
	MTSP_COMMAND_MEMORY_DESC_0,
	MTSP_COMMAND_MEMORY_DESC_1,
	MTSP_COMMAND_MEMORY_DESC_2,
	MTSP_COMMAND_MEMORY_DESC_3,
	MTSP_COMMAND_SET_PC,
	MTSP_COMMAND_VIDEO_BASE,
	MTSP_COMMAND_INVOKE_MEMORY_OPERATION	= 8,
	MTSP_COMMAND_SIGNAL_ASSERTION,
	MTSP_COMMAND_TREAD_CONTROL,
	MTSP_COMMAND_WAIT_SIGNAL,
}MTSP_COMMAND_ID;

typedef union{
	DWORD	all;
	struct{
		DWORD	busy			: 1;
		DWORD	mem_command_en	: 1;
		DWORD	cmd_fifo_empty	: 1;
		DWORD	_reserved_		: 5;
		DWORD	cmd_fifo_depth	: 8;
		DWORD	cmd_fifo_rindex	: 8;
		DWORD	cmd_fifo_windex	: 8;
	};
}MTSP_STATUS;

typedef union{
	DWORD	m[4];
	struct{
		// #0
		WORD		id;						// memory ID
		WORD		index		: 12;		// register index
		WORD		op			: 4;		// memory operation
		// #1
		WORD		mask;					// index mask
		BYTE		size;					// transfer size (n * 16 bytes)
		BYTE		stride;					// transfer stride (n * 16 bytes)
		// #2
		DWORD		p_base;					// physical base address
		// #3
		WORD		g_base;					// GMB base index
		WORD		l_base;					// LMB base index
	};
}MTSP_MEM_COMMAND;

typedef struct{
	UINT64		uiTime;
	BYTE		id;
	DWORD		dwData;
}COMMAND_TRACE;

#ifdef USE_TESTDRIVE
	typedef struct{
		BOOL			bUpdate;
		UINT64			uLastUpdateTime;
		REG_VALUE4		reg[MAX_GPR_SIZE];
	}GPRs_T;

	typedef struct{
		BOOL			bUpdate;
		DWORD			dwRecentUsed;		// latest recent used
		UINT64			uLastUpdateTime;
		REG_VALUE8		reg[MAX_LMB_SIZE];
	}LMB_T;

	typedef struct{
		BOOL			bUpdate;
		DWORD			dwRecentUsed;		// latest recent used
		UINT64			uLastUpdateTime;
		REG_VALUE8		reg[MAX_GMB_SIZE];
	}GMB_T;

	typedef struct{
		BOOL	bUpdate;
		struct{
			DWORD	counter[4];
			DWORD	limit[4];
		}element[MAX_SCRATCH_COUNTER_LOCAL_SIZE];
	}SC_LOCAL_T;

	typedef struct{
		BOOL	bUpdate;
		struct{
			DWORD	counter[4];
			DWORD	limit[4];
		}element;
	}SC_GLOBAL_T;

	typedef struct{
		BOOL		bUpdate;
		union{
			DWORD	all;
			struct{
				DWORD	bBusy		: 1;
				DWORD	/*dummy*/	: 0;
			};
		}status;
		GPRs_T		GPRs;
	}THREAD_T;

	typedef struct{
		LMB_T				LMB;
		SC_LOCAL_T			scratch;
		BOOL				bUpdate;
		THREAD_T			thread[THREAD_SIZE];
	}CORE_T;

	typedef struct{
		BOOL	bUpdate;
		DWORD	dwSize;
		int		iHead;
		struct{
			DWORD	addr[MAX_THREAD_TRACE_SIZE];
		}thread[THREAD_SIZE];
	}TRACE_T;

	typedef struct{
		BOOL			bUpdate;
		DWORD			dwCount;
		int				iPos;
		COMMAND_TRACE	history[MAX_COMMAND_HISTORY_SIZE];
	}COMMAND_T;

	typedef struct : public SYSTEM_REGMAP {
		UINT64			uUpdateTime;
		
		GMB_T			GMB;
		SC_GLOBAL_T		scratch;

		struct{
			BOOL		bUpdate;
			int			iCoreSize;
			int			iCoreID;
			int			iThreadID;
			int			iGPRsSize;
			int			iLMBSize;
			int			iGMBSize;
		}system;

		COMMAND_T	command;
		CORE_T		core[MAX_CORE_SIZE];

	}MTSP_REGMAP;

#endif

#endif//__SYSTEM_CONFIG_MTSP_H__
