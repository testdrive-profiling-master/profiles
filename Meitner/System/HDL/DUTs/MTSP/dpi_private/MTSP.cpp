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
// Title : Meitner processor v1.1
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "MTSP.h"

SystemLog	Log("MTSP");
MTSP		g_MTSP;

MTSP::MTSP(void)
{
	m_pDisplay	= NULL;
	m_pReg		= NULL;
	m_pGMB		= NULL;
}

MTSP::~MTSP(void)
{
}

BOOL MTSP::Initialize(int iCoreSize, int iGMBSize, int iLMBSize, int iGPRsSize)
{
	if(iCoreSize <= 1 || iCoreSize >  MAX_CORE_SIZE) {
		LOGE("Invalid core size : %d (1<?<=%d)", iCoreSize, MAX_CORE_SIZE);
		return FALSE;
	}

	if(iGMBSize < 16 || iGMBSize >  MAX_GMB_SIZE || iGMBSize != (1 << (int)log2(iGMBSize))) {
		LOGE("Invalid GMB size : %d (16<=?<=%d, must be 2^n)", iGMBSize, MAX_GMB_SIZE);
		return FALSE;
	}

	if(iLMBSize < 16 || iLMBSize >  MAX_LMB_SIZE || iLMBSize != (1 << (int)log2(iLMBSize))) {
		LOGE("Invalid LMB size : %d (16<=?<=%d, must be 2^n)", iLMBSize, MAX_LMB_SIZE);
		return FALSE;
	}

	if(iGPRsSize <= 0 || iGPRsSize > MAX_GPR_SIZE || iGPRsSize != (1 << (int)log2(iGPRsSize))) {
		LOGE("Invalid GPR size : %d (0<?<=%d, must be 2^n)", iGPRsSize, MAX_GPR_SIZE);
		return FALSE;
	}

	m_pDisplay					= GetDisplayConfig();
	m_pReg						= (MTSP_REGMAP*)GET_SYSTEM_REGMAP;
	m_pGMB						= &(m_pReg->GMB);
	m_pReg->magic_code			= SYSTEM_MAGIC_CODE;
	m_pReg->system.iCoreSize	= iCoreSize;
	m_pReg->system.iGPRsSize	= iGPRsSize;
	m_pReg->system.iLMBSize		= iLMBSize;
	m_pReg->system.iGMBSize		= iGMBSize;
	// command
	m_pReg->command.iPos		= 0;
	m_pReg->command.dwCount		= 0;
	m_pReg->command.bUpdate		= TRUE;

	for(int t = 0; t < iCoreSize; t++) {
		m_pReg->core[t].bUpdate	= TRUE;

		for(int i = 0; i < THREAD_SIZE; i++) {
			THREAD_T*	pThread	= &m_pReg->core[t].thread[i];
			pThread->status.all	= 0;
			pThread->bUpdate	= TRUE;
		}
	}

	if(m_pReg->system.iCoreID >= m_pReg->system.iCoreSize)
		m_pReg->system.iCoreID	= 0;

	m_pGMB->uLastUpdateTime		= -1;
	m_pGMB->bUpdate				= TRUE;

	for(int i = 0; i < iGMBSize; i++) m_pGMB->reg[i].uUpdateTime	= 0;

	m_pReg->uUpdateTime			= 0;
	m_pReg->system.bUpdate		= TRUE;
	return TRUE;
}

void MTSP::WriteCommand(int iCommand, unsigned int dwData)
{
	int pos	= m_pReg->command.iPos;
	m_pReg->command.history[pos].id		= iCommand;
	m_pReg->command.history[pos].dwData	= dwData;
	m_pReg->command.history[pos].uiTime	= SimulationTime();
	m_pReg->command.iPos				= (pos + 1) & (MAX_COMMAND_HISTORY_SIZE - 1);
	m_pReg->command.dwCount++;
	m_pReg->command.bUpdate				= TRUE;
}

void MTSP::SetGMB(DWORD dwIndex, const DWORD* pData)
{
	dwIndex					&= m_pReg->system.iGMBSize - 1;
	m_pGMB->uLastUpdateTime	= SimulationTime();
	m_pGMB->dwRecentUsed	= dwIndex;
	REG_VALUE8* pV			= &m_pGMB->reg[dwIndex];

	for(int i = 0; i < 8; i++)
		pV->udata[i]			= pData[i];

	pV->uUpdateTime			= m_pGMB->uLastUpdateTime;
	m_pGMB->bUpdate			= TRUE;
}

void MTSP::SetLMB(DWORD dwCoreID, DWORD dwIndex, const DWORD* pData)
{
	LMB_T*	pLMB			= &(m_pReg->core[dwCoreID].LMB);
	dwIndex					&= m_pReg->system.iLMBSize - 1;
	pLMB->uLastUpdateTime	= SimulationTime();
	pLMB->dwRecentUsed		= dwIndex;
	REG_VALUE8* pV			= &pLMB->reg[dwIndex];

	for(int i = 0; i < 8; i++)
		pV->udata[i]			= pData[i];

	pV->uUpdateTime			= pLMB->uLastUpdateTime;
	pLMB->bUpdate			= TRUE;
}

void MTSP::SetGPR(DWORD dwCoreID, DWORD dwThreadID, DWORD dwIndex, DWORD dwCompoment, DWORD dwData)
{
	THREAD_T*	pThread				= &(m_pReg->core[dwCoreID].thread[dwThreadID]);
	REG_VALUE*	pGPR				= &(pThread->GPRs.reg[dwIndex].m[dwCompoment]);
	pGPR->udata						= dwData;
	pThread->GPRs.uLastUpdateTime	= pGPR->uUpdateTime		= SimulationTime();
	pThread->GPRs.bUpdate			= TRUE;
}

void MTSP::GetInstruction(DWORD ThreadPC, DWORD* pUINSTx4)
{
	DWORD	dwPC	= ThreadPC << 2;
	DWORD*	pBase	= (DWORD*)GetMemoryPointer(dwPC, sizeof(DWORD) * 4);

	if(pBase) {
		for(int i = 0; i < 4; i++) {
			pUINSTx4[(3 - (ThreadPC + i)) & 0x3] = pBase[i];
		}
	} else {
		LOGE("Invalid PC address : 0x%08X\n", dwPC);
	}
}

void MTSP::SetFrontBufferAddress(DWORD dwAddress)
{
	m_pDisplay->Front.dwAddress	= dwAddress;
	m_pDisplay->Front.bUpdate	= TRUE;
}

DPI_FUNCTION int MTSP_Initialize(int iCoreSize, int iGMBSize, int iLMBSize, int iGPRsSize)
{
	return g_MTSP.Initialize(iCoreSize, iGMBSize, iLMBSize, iGPRsSize);
}

DPI_FUNCTION void MTSP_WriteCommand(int iCommand, unsigned int dwData)
{
	g_MTSP.WriteCommand(iCommand, dwData);
}

DPI_FUNCTION void MTSP_set_GMB(unsigned int dwIndex, const svBitVecVal* DATA)
{
	g_MTSP.SetGMB(dwIndex, (const DWORD*)DATA);
}

DPI_FUNCTION void MTSP_set_LMB(unsigned int dwCoreID, unsigned int dwIndex, const svBitVecVal* DATA)
{
	g_MTSP.SetLMB(dwCoreID, dwIndex, (const DWORD*)DATA);
}

DPI_FUNCTION void SetGPR(DWORD dwCoreID, DWORD dwThreadID, DWORD dwIndex, DWORD dwComponent, const svBitVecVal* DATA)
{
	g_MTSP.SetGPR(dwCoreID, dwThreadID, dwIndex, dwComponent, *(const DWORD*)DATA);
}

DPI_FUNCTION void GetInstruction(unsigned int ThreadPC, svBitVecVal* UINSTx4)
{
	g_MTSP.GetInstruction(ThreadPC, (DWORD*)UINSTx4);
}
