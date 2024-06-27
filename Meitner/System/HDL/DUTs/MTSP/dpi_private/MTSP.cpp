//================================================================================
// Copyright (c) 2013 ~ 2024.
// All rights reserved by HyungKi Jeong(clonextop@gmail.com)
// Home  : https://testdrive-profiling-master.github.io/
//
// Title : MTSPU
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "MTSP.h"
MTSP *g_pMTSP = NULL;

MTSP::MTSP(int iCoreCount, int iGMBCount) : Log("MTSP")
{
	m_pRegmap = (MTSP_REGMAP *)GetMemoryPointer(0, 0, true);
	m_pCmd	  = &m_pRegmap->command;
	memset(m_pRegmap, 0, sizeof(MTSP_REGMAP));
	m_pRegmap->system.iCoreCount = iCoreCount;
	m_pRegmap->system.iGMB_Count = iGMBCount;
	m_pRegmap->magic_code		 = MTSP_MAGIC_CODE;
	m_pRegmap->system.bUpdate	 = true;
	LOGI("Initialized. (REGMAP memory consumption : %.2f MBytes)", ((double)sizeof(MTSP_REGMAP)) / (1024 * 1024));
	memset(m_pCoreList, 0, sizeof(m_pCoreList));

	for (int i = 0; i < iCoreCount; i++) {
		m_pCoreList[i] = new MTSP_Core(i, &m_pRegmap->core[i]);
	}
}

MTSP::~MTSP(void)
{
	g_pMTSP = NULL;

	for (int i = 0; i < MTSP_MAX_CORE_SIZE; i++) {
		SAFE_DELETE(m_pCoreList[i]);
	}
}

void MTSP::Main(void)
{
	m_pRegmap->lSimulationTime = g_ulSimulationTime;
}

void MTSP::GetInstruction(uint64_t addr, uint32_t *inst)
{
	DWORD *pInst  = (DWORD *)GetMemoryPointer(addr, 16);
	DWORD  offset = (((DWORD)addr) >> 2) & 0x3;

	if (pInst) {
		for (int i = 0; i < 4; i++) {
			inst[(i + offset) & 3] = pInst[i];
		}
	} else {
		LOGE("Invalid instruction PC : 0x%llX", *(UINT64 *)addr);
	}
}

void MTSP::CommandPush(int id, uint32_t data)
{
	static uint32_t __command_data[4];

	if (!id) {
		for (int i = 0; i < 3; i++) __command_data[i + 1] = __command_data[i];

		__command_data[0] = data;
	} else {
		MTSP_REGMAP_COMMAND_DATA *cmd = &m_pCmd->cmd[m_pCmd->dwHead];
		cmd->id						  = id;
		cmd->op						  = data;
		cmd->elapsed_time			  = 0;
		memcpy(cmd->data, __command_data, sizeof(uint32_t) * 4);
		m_pCmd->dwHead++;
		m_pCmd->bUpdate = true;
	}
}

MTSP_REGMAP_COMMAND_DATA *MTSP::CommandCurrent(void)
{
	return &(m_pCmd->cmd[m_pCmd->dwCurrent & (MTSP_COMMAND_RING_SIZE - 1)]);
}

void MTSP::CommandStart(void)
{
	MTSP_REGMAP_COMMAND_DATA *pCmd = CommandCurrent();

	// update register update ID when start-up process
	if (pCmd->id == 2) {
		m_pRegmap->lStartupTime = g_ulSimulationTime;
		// clear all update count
		REG_VALUE4 *pM;
		// GMB
		pM = m_pRegmap->gmb.m;

		for (int i = 0; i < m_pRegmap->system.iGMB_Count; i++) {
			for (int t = 0; t < 4; t++) pM->m[t].uUpdateID = 0;
		}

		m_pRegmap->gmb.uLastUpdateID = 0;
		m_pRegmap->gmb.bUpdate		 = true;

		for (int iCore = 0; iCore < m_pRegmap->system.iCoreCount; iCore++) {
			MTSP_REGMAP_CORE *pCore = &m_pRegmap->core[iCore];
			pCore->uLMBLastUpdateID = 0;

			// GPRs
			for (int iThread = 0; iThread < MTSP_THREAD_SIZE; iThread++) {
				MTSP_REGMAP_THREAD *pThread = &pCore->thread[iThread];
				pM							= pThread->GPRs.m;

				for (int i = 0; i < MTSP_GPR_SIZE; i++) {
					for (int t = 0; t < 4; t++) pM->m[t].uUpdateID = 0;
				}

				pThread->GPRs.uLastUpdateID = 0;
				pThread->GPRs.bUpdate		= true;
			}
		}
	}

	pCmd->elapsed_time = g_ulSimulationTime;
}

void MTSP::CommandEnd(void)
{
	MTSP_REGMAP_COMMAND_DATA *pCmd = CommandCurrent();
	pCmd->elapsed_time			   = g_ulSimulationTime - pCmd->elapsed_time;
	m_pCmd->dwCurrent++;
	m_pCmd->bUpdate = true;
}

DPI_FUNCTION void MTSP_Initialize(int iCoreCount, int iGMBCount)
{
	if (!g_pMTSP)
		g_pMTSP = new MTSP(iCoreCount, iGMBCount);
}

DPI_FUNCTION void GetInstruction(const svBitVecVal *addr, svBitVecVal *inst)
{
	g_pMTSP->GetInstruction(*(UINT64 *)addr, inst);
}

DPI_FUNCTION void MTSP_Main(void)
{
	g_pMTSP->Main();
}

DPI_FUNCTION void MTSP_Command_Push(const svBitVecVal *id, unsigned int data)
{
	g_pMTSP->CommandPush(*id, data);
}

DPI_FUNCTION void MTSP_Command_Start(void)
{
	g_pMTSP->CommandStart();
}

DPI_FUNCTION void MTSP_Command_End(void)
{
	g_pMTSP->CommandEnd();
}

DPI_FUNCTION void MTSP_GMB(int iIndex, const svBitVecVal *wmask, const svBitVecVal *data)
{
	MTSP_REGMAP_GMB *mb = g_pMTSP->GMB();
	REG_VALUE4		&m	= mb->m[iIndex];
	{
		uint32_t mask = *wmask;

		if (mask) {
			mb->uLastUpdateID++;

			for (int i = 0; i < 4; i++) {
				if (mask & 1) {
					m.m[i].udata	 = data[i];
					m.m[i].uUpdateID = mb->uLastUpdateID;
				}

				mask >>= 1;
			}
		}
	}
	mb->bUpdate = true;
}
