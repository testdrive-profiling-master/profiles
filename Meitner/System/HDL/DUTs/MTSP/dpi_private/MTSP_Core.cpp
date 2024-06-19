//================================================================================
// Copyright (c) 2013 ~ 2024.
// All rights reserved by HyungKi Jeong(clonextop@gmail.com)
// Home  : https://testdrive-profiling-master.github.io/
//
// Title : MTSPU
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "MTSP.h"

MTSP_Core::MTSP_Core(int iCoreID, MTSP_REGMAP_CORE *pReg)
{
	Log.SetTitle("Core#%d", iCoreID);
	m_iCoreID	= iCoreID;
	m_iThreadID = 0;
	m_pReg		= pReg;
	LOGI("Initialized.");
}

MTSP_Core::~MTSP_Core(void) {}

void MTSP_Core::Initialize(void) {}

void MTSP_Core::Thread(int iThreadID, uint32_t status, uint64_t pc)
{
	MTSP_REGMAP_THREAD &thread = m_pReg->thread[iThreadID];
	m_iThreadID				   = iThreadID;
	thread.status.m			   = status;
	thread.pc				   = pc;
	thread.bUpdate			   = true;
	m_pReg->bUpdate			   = true;
}

void MTSP_Core::ShowInstruction(uint64_t pc, uint32_t *pInst)
{
	printf("EX[%d-%d].INST[0x%llX] : %08X %08X %08X %08X\n", m_iCoreID, m_iThreadID, pc, pInst[0], pInst[1], pInst[2],
		   pInst[3]);
}

DPI_FUNCTION void *GetCore(int iCoreID)
{
	return g_pMTSP->Core(iCoreID);
}

static int GetThreadIDFromMask(uint32_t thread_mask)
{
	int thread_id = 0;

	for (int i = 0; i < MTSP_THREAD_SIZE; i++) {
		if (thread_mask & 1) {
			thread_id = i;
			break;
		}

		thread_mask >>= 1;
	}
	return thread_id;
}

DPI_FUNCTION void GetGPRs(int iCoreID, const svBitVecVal *thread_mask, const svBitVecVal *ADDR, svBitVecVal *DATA)
{
	// get thread id
	int			thread_id = GetThreadIDFromMask(*thread_mask);

	REG_VALUE4 *pGPR = &(g_pMTSP->Core(iCoreID)->GPRs(thread_id)->m[(*ADDR) & (MTSP_GPR_SIZE - 1)]);
	DATA[0]			 = pGPR->m[0].udata;
	DATA[1]			 = pGPR->m[1].udata;
	DATA[2]			 = pGPR->m[2].udata;
	DATA[3]			 = pGPR->m[3].udata;

	printf("GetGPRs[%d-%d] = %08X, %08X, %08X, %08X\n", iCoreID, thread_id, DATA[0], DATA[1], DATA[2], DATA[3]);
}

DPI_FUNCTION void SetGPRs(int iCoreID, const svBitVecVal *thread_mask, const svBitVecVal *WE, const svBitVecVal *ADDR,
						  const svBitVecVal *DATA)
{
	// get thread id
	int				  thread_id = GetThreadIDFromMask(*thread_mask);
	uint8_t			  we		= *WE;

	MTSP_REGMAP_GPRs *pGPRs = g_pMTSP->Core(iCoreID)->GPRs(thread_id);
	REG_VALUE4		 *pGPR	= &(pGPRs->m[(*ADDR) & (MTSP_GPR_SIZE - 1)]);
	pGPRs->uLastUpdateID++;

	printf("SetGPRs[%d-%d] GPRs[%d] WE(%X) : %08X_%08X_%08X_%08X\n", iCoreID, thread_id, *ADDR, we, DATA[0], DATA[1],
		   DATA[2], DATA[3]);

	if (we & 1) {
		pGPR->m[0].udata	 = DATA[0];
		pGPR->m[0].uUpdateID = pGPRs->uLastUpdateID;
	}

	if (we & 2) {
		pGPR->m[1].udata	 = DATA[1];
		pGPR->m[1].uUpdateID = pGPRs->uLastUpdateID;
	}

	if (we & 4) {
		pGPR->m[2].udata	 = DATA[2];
		pGPR->m[2].uUpdateID = pGPRs->uLastUpdateID;
	}

	if (we & 8) {
		pGPR->m[3].udata	 = DATA[3];
		pGPR->m[3].uUpdateID = pGPRs->uLastUpdateID;
	}
}

DPI_FUNCTION void InitGPRs(int iCoreID, int iThreadID, int iAddr, const svBitVecVal *DATA)
{
	MTSP_REGMAP_GPRs *pGPRs = g_pMTSP->Core(iCoreID)->GPRs(iThreadID);
	REG_VALUE4		 *pGPR	= &(pGPRs->m[iAddr & (MTSP_GPR_SIZE - 1)]);
	pGPR->m[0].udata		= DATA[0];
	pGPR->m[1].udata		= DATA[1];
	pGPR->m[2].udata		= DATA[2];
	pGPR->m[3].udata		= DATA[3];
}

DPI_FUNCTION void MTSP_LMB(int iCoreID, int iIndex, const svBitVecVal *wmask, const svBitVecVal *data)
{
	MTSP_Core		 *pCore = g_pMTSP->Core(iCoreID);
	MTSP_REGMAP_CORE *pReg	= g_pMTSP->Core(iCoreID)->m_pReg;
	MTSP_REGMAP_GPRs *pGPR	= pCore->GPRs(iIndex & 7);
	REG_VALUE4		 &m		= pGPR->m[(iIndex >> 3) & (MTSP_GPR_SIZE - 1)];
	{
		uint32_t mask = *wmask;

		if (mask) {
			pReg->uLMBLastUpdateID++;

			for (int i = 0; i < 4; i++) {
				if (mask & 1) {
					m.m[i].udata	 = data[i];
					m.m[i].uUpdateID = pReg->uLMBLastUpdateID | 0x80000000;
				}

				mask >>= 1;
			}

			pGPR->bUpdate = true;
		}
	}
}

DPI_FUNCTION void MTSP_Thread(int iCoreID, int iThreadID, const svBitVecVal *status, long long pc)
{
	if (g_pMTSP) {
		g_pMTSP->Core(iCoreID)->Thread(iThreadID, *status, pc);
	}
}

DPI_FUNCTION void ShowInstruction(int iCoreID, const svBitVecVal *pc, const svBitVecVal *inst)
{
	uint64_t PC = *(uint64_t *)pc;
	g_pMTSP->Core(iCoreID)->ShowInstruction(*(uint64_t *)pc, (uint32_t *)inst);
}
