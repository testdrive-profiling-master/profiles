//================================================================================
// Copyright (c) 2013 ~ 2024.
// All rights reserved by HyungKi Jeong(clonextop@gmail.com)
// Home  : https://testdrive-profiling-master.github.io/
//
// Title : MTSPU
// Rev.  : 6/19/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __NPU_DPI_H__
#	include "SystemConfigMTSP.h"
#	include "dpi_common.h"

class MTSP_Core
{
public:
	MTSP_Core(int iCoreID, MTSP_REGMAP_CORE *pReg);
	~MTSP_Core(void);

	void					 Initialize(void);
	void					 Thread(int iThreadID, uint32_t status, uint64_t pc);
	void					 ShowInstruction(uint64_t pc, uint32_t *pInst);
	inline MTSP_REGMAP_GPRs *GPRs(int iThreadID)
	{
		return &(m_pReg->thread[iThreadID].GPRs);
	}
	SystemLog		  Log;
	int				  m_iCoreID;
	int				  m_iThreadID;
	MTSP_REGMAP_CORE *m_pReg;
};

class MTSP : public SelfDestory
{
public:
	MTSP(int iCoreCount, int iGMBCount);
	~MTSP(void);

	void					  GetInstruction(uint64_t addr, uint32_t *inst);
	void					  Main(void);

	void					  CommandPush(int id, uint32_t data);
	void					  CommandStart(void);
	void					  CommandEnd(void);
	MTSP_REGMAP_COMMAND_DATA *CommandCurrent(void);

	inline MTSP_Core		 *Core(int iCoreID)
	{
		return m_pCoreList[iCoreID];
	}

	inline MTSP_REGMAP_GMB *GMB(void)
	{
		return &m_pRegmap->gmb;
	}

private:
	MTSP_Core			*m_pCoreList[MTSP_MAX_CORE_SIZE];
	MTSP_REGMAP			*m_pRegmap;
	MTSP_REGMAP_COMMAND *m_pCmd;
	SystemLog			 Log;
};

extern MTSP *g_pMTSP;

#endif //__NPU_DPI_H__
