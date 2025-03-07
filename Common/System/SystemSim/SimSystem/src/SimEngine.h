//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common profiles
// Rev.  : 1/31/2025 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __SIM_ENGINE_H__
#define __SIM_ENGINE_H__
#include "ThreadManager.h"
#include "../SimHDL/SimHDL.h"
#include "InterruptService.h"
#include "DelayLock.h"
#include <string>

class SimEngine;

class SimResource
{
public:
	SimResource(void) {}
	virtual ~SimResource(void) {}
	static inline SimEngine *Sim(void)
	{
		return m_pSim;
	}

protected:
	static SimEngine *m_pSim;
};

class SimInstance : public SimResource
{ // simulation instance template
	SimInstance *m_pNext;

protected:
	friend class SimEngine;

public:
	SimInstance(void);
	virtual ~SimInstance(void);
	virtual bool OnRun(void) = 0;
};

//#define ERROR_OCCURED(s)	SetError(__FILE__, __LINE__, __FUNCDNAME__, s);

class SimEngine : // single-turn simulation engine
				  protected ThreadManager,
				  public SimControl,
				  public SimResource
{
	bool		 m_bUpdate;

	SimInstance *m_pInstance;
	DelayLock	 m_Lock;

protected:
	friend class SimInstance;

	virtual void MonitorThread(void);
	virtual void OnThreadKill(bool bForced);

	// SimControl interface
	virtual BUS_SLAVE_INTERFACE *CreateSlave(uint64_t ulAddrBase, uint64_t ulAddrHigh);
	virtual BUS_SLAVE_INTERFACE *FindSlave(uint64_t ulAddress);
	virtual CLOCK_INTERFACE		*CreateClock(uint8_t *pCLK, uint8_t *pRST);
	virtual CLOCK_INTERFACE		*FindClock(uint8_t *pCLK);
	virtual bool				 AwakeInterrupt(void); // true(Awaked), false(Pending)
	virtual void				 SetMemoryBaseAddress(uint64_t ulAddress);
	virtual void				 MemoryRead32(int iID, uint64_t ulAddress, uint32_t &dwData);
	virtual void				 MemoryWrite32(int iID, uint64_t ulAddress, uint32_t dwData);
	virtual void				 MemoryRead16(int iID, uint64_t ulAddress, uint16_t &dwData);
	virtual void				 MemoryWrite16(int iID, uint64_t ulAddress, uint16_t dwData);
	virtual void				 MemoryRead8(int iID, uint64_t ulAddress, uint8_t &dwData);
	virtual void				 MemoryWrite8(int iID, uint64_t ulAddress, uint8_t dwData);
	virtual SYSTEM_CONFIG		*GetSystemConfig(void);
	virtual DisplayConfig		*GetDisplayConfig(void);
	virtual uint8_t				*GetMemoryPointer(uint64_t ulAddress, uint64_t ulSize = 0, bool bDisplay = false);
	virtual uint64_t			 GetMemoryBaseAddress(void);
	virtual bool				 GetMemory(const char *sName, void *&pConfig, void *&pMemory);
	virtual void				 SimulationLock(int iDelayTicks = 0);
	virtual void				 SimulationUnLock(void);
	virtual void				 SimulationAddBusy(uint8_t *pBusy);
	virtual void				 SimulationDebugMode(bool bDebug = true);
	virtual void				 SetSystemDescription(const char *sDesc);
	virtual void				 SetError(bool bError = true);
	virtual void				 RegisterDriverCommand(DRIVER_COMMAND func);

public:
	SimEngine(void);
	virtual ~SimEngine(void);

	bool		Initialize(void);
	bool		Clocking(void);

	inline void Lock(int iDelayTime = 0)
	{
		m_Lock.Lock(iDelayTime);
	}

	inline void Unlock(void)
	{
		m_Lock.UnLock();
	}

	bool					 Start(void);
	void					 Terminate(void);

	inline InterruptService &Interrupt(void)
	{
		return m_Interrupt;
	}

	uint32_t	DriverCommand(void *pCommand);

	const char *GetSystemDescription(void);
	inline bool IsGotError(void)
	{
		return m_bErrorOccured;
	}

private:
	std::string		 m_sSystemDesc;
	bool			 m_bErrorOccured;
	volatile bool	 m_bForceToExit;
	InterruptService m_Interrupt;
	SimHDL			*m_pSimHDL;
	DRIVER_COMMAND	 m_DriverCommand;
};
#endif //__SIM_ENGINE_H__
