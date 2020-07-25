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
// Title : Simulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
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
	SimResource(void){}
	virtual ~SimResource(void){}
	static inline SimEngine* Sim(void)	{return m_pSim;}

protected:
	static SimEngine*	m_pSim;
};

class SimInstance : public SimResource	// 시뮬레이션 인스턴스 템플릿
{
	SimInstance*	m_pNext;

protected:
	friend class	SimEngine;

public:
	SimInstance(void);
	virtual ~SimInstance(void);
	virtual BOOL OnRun(void) = 0;
};

//#define ERROR_OCCURED(s)	SetError(__FILE__, __LINE__, __FUNCDNAME__, s);

class SimEngine :
	protected ThreadManager,	// 시뮬레이션 단일 엔진
	public SimControl,
	public SimResource
{
	BOOL				m_bUpdate;

	SimInstance*		m_pInstance;
	DelayLock			m_Lock;

protected:
	friend class SimInstance;

	virtual void MonitorThread(void);
	virtual void OnThreadKill(void);

	// SimControl interface
	STDMETHOD_(BUS_SLAVE_INTERFACE*, CreateSlave)(DWORD dwAddrBase, DWORD dwAddrHigh);
	STDMETHOD_(BUS_SLAVE_INTERFACE*, FindSlave)(DWORD dwAddress);
	STDMETHOD_(CLOCK_INTERFACE*, CreateClock)(BYTE* pCLK, BYTE* pRST);
	STDMETHOD_(BOOL, AwakeInterrupt)(void);	// TRUE(Awaked), FALSE(Pending)
	STDMETHOD_(void, SetMemoryBaseAddress)(DWORD dwAddress);
	STDMETHOD_(void, MemoryRead32)(int iID, DWORD dwAddress, DWORD& dwData);
	STDMETHOD_(void, MemoryWrite32)(int iID, DWORD dwAddress, DWORD dwData);
	STDMETHOD_(void, MemoryRead16)(int iID, DWORD dwAddress, WORD& dwData);
	STDMETHOD_(void, MemoryWrite16)(int iID, DWORD dwAddress, WORD dwData);
	STDMETHOD_(void, MemoryRead8)(int iID, DWORD dwAddress, BYTE& dwData);
	STDMETHOD_(void, MemoryWrite8)(int iID, DWORD dwAddress, BYTE dwData);
	STDMETHOD_(SYSTEM_CONFIG*, GetSystemConfig)(void);
	STDMETHOD_(DisplayConfig*, GetDisplayConfig)(void);
	STDMETHOD_(BYTE*, GetMemoryPointer)(DWORD dwAddress, DWORD dwSize = 0, BOOL bDisplay = FALSE);
	STDMETHOD_(DWORD, GetMemoryBaseAddress)(void);
	STDMETHOD_(BOOL, GetMemory)(const char* sName, void*& pConfig, void*& pMemory);
	STDMETHOD_(void, SimulationLock)(int iDelayTicks = 0);
	STDMETHOD_(void, SimulationUnLock)(void);
	STDMETHOD_(void, SimulationAddBusy)(BYTE* pBusy);
	STDMETHOD_(void, SimulationDebugMode)(BOOL bDebug = TRUE);
	STDMETHOD_(void, SetSystemDescription)(const char* sDesc);

public:
	SimEngine(void);
	virtual ~SimEngine(void);

	BOOL Initialize(void);
	BOOL Clocking(void);

	void Lock(int iDelayTime = 0);
	void Unlock(void);

	BOOL Start(void);
	void Terminate(void);

	inline InterruptService& Interrupt(void)	{return m_Interrupt;}

	const char* GetSystemDescription(void);

private:
	std::string			m_sSystemDesc;
	BOOL				m_bErrorOccured;
	InterruptService	m_Interrupt;
	SimHDL*				m_pSimHDL;
};
#endif//__SIM_ENGINE_H__
