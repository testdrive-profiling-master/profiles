//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Simulation HDL module
// Rev.  : 12/30/2021 Thu (clonextop@gmail.com)
//================================================================================
#include "SimHDL_common.h"
#include "TestDriver.inl"
#include "SimHDL.h"
#include "SimTop.h"
#include <time.h>		// for rand/srand

static UINT64	g_lSimulationTime		= 0;		// global simulation timestamp
SimHDL*			g_pSimHDL				= NULL;
SimControl*		g_pSimControl			= NULL;
SimTop*			g_pSimTop				= NULL;
#ifndef SIMULATION_TOP_EX
static BOOL		__PreINTR				= TRUE;
#endif
#ifdef SIM_TRACE
#ifdef SIM_TRACE_FILE_OUTPUT
VerilatedFstC*	g_pWaveDump				= NULL;
#elif defined(SIM_TRACE_INTERACTIVE)
VerilatedVcdC*	g_pWaveDump				= NULL;
VerilatedVcdGTKWave*	g_pGtkWave		= NULL;
#endif
UINT64			g_lTraceStartTime		= 0;
#endif

#ifndef DEFAULT_HALF_CLOCK_PERIOD
#define DEFAULT_HALF_CLOCK_PERIOD		5000		// 5000ps = 5ns = 10ns clock cycle = 100MHz
#endif

class SimHDL_imp : public SimHDL {
public:
	SimHDL_imp(void) {
	}

	virtual ~SimHDL_imp(void) {
#ifdef SIM_TRACE

		if(g_pWaveDump)
			g_pWaveDump->close();

		SAFE_DELETE(g_pWaveDump);
#ifdef SIM_TRACE_INTERACTIVE
		SAFE_DELETE(g_pGtkWave);
#endif
#endif

		if(g_pSimHDL == this) {
			g_pSimTop->final();
			delete g_pSimTop;
			g_pSimTop		= NULL;
			g_pSimHDL		= NULL;
		}
	}

	virtual BOOL Initialize(void) {	// cppcheck-suppress internalAstError
		if(!g_pSimHDL) {
			srand(time(NULL));			// randomize seed
			Verilated::randReset(2);	// randomize all bits
			g_pSimHDL		= this;
			g_pSimTop		= new SimTop;
		} else {
			LOGI("'SimProcessor' At least one more instances has been created.\n");
			return FALSE;
		}

#ifdef SYSTEM_MEMORY_BASE_ADDRESS
		SetMemoryBaseAddress(SYSTEM_MEMORY_BASE_ADDRESS);
#endif
#ifndef SIMULATION_TOP_EX
		g_pSimTop->nRST		= 0;
		g_pSimTop->MCLK		= 0;
#endif
#ifdef SIM_TRACE
		{
			g_pWaveDump			= NULL;
			g_lTraceStartTime	= 0;
			{
				// set simulation start time
				char sEnv[MAX_PATH];

				if(GetEnvironmentVariable("SIM_TRACE_START_TIME", sEnv, MAX_PATH)) {
					g_lTraceStartTime	= atoi(sEnv);

					if(g_lTraceStartTime) {
						LOGI("Set trace start time : %ld ns\n", g_lTraceStartTime);
						g_lTraceStartTime	*= 1000;
					}
				}
			}
			{
				char sFile[MAX_PATH];
				*sFile		= '\0';
#ifdef SIM_TRACE_FILE_OUTPUT
				// VCD output file
				GetEnvironmentVariable("SIM_OUTPUT_FILE", sFile, MAX_PATH);
				Verilated::traceEverOn(true);
				g_pWaveDump		= new VerilatedFstC();
#elif defined(SIM_TRACE_INTERACTIVE)
				// interactive GTKWave mode
				GetEnvironmentVariable("SIM_DO_FILE", sFile, MAX_PATH);
				Verilated::traceEverOn(true);
				g_pGtkWave	= new VerilatedVcdGTKWave;
				g_pWaveDump	= new VerilatedVcdC(g_pGtkWave);
#endif

				if(g_pWaveDump) {
					// Do simulation
					g_pWaveDump->set_time_unit("1ps");
					g_pWaveDump->set_time_resolution("1ps");
					g_pSimTop->trace(g_pWaveDump, 99);

					if(*sFile) g_pWaveDump->open(sFile);
				}
			}
		}
#else
		Verilated::traceEverOn(false);	// no tracing
#endif
#ifndef SIMULATION_TOP_EX
		{
			// make default clock, reset & busy
			CLOCK_INTERFACE*	pClock	= g_pSimControl->CreateClock(&g_pSimTop->MCLK, &g_pSimTop->nRST);
			pClock->SetParameters(0, DEFAULT_HALF_CLOCK_PERIOD);	// set half clock period
			pClock->DoReset();
			g_pSimControl->SimulationAddBusy(&g_pSimTop->BUSY);
		}
#endif
		return TRUE;
	}

	virtual BOOL IsTrace(void) {
#ifdef SIM_TRACE
		return (g_pWaveDump != NULL);
#else
		return FALSE;
#endif
	}

	virtual BOOL Eval(void) {
		// simulation evaluation
		g_pSimTop->eval();
#ifndef SIMULATION_TOP_EX
		{
			if(!__PreINTR && g_pSimTop->INTR)
				g_pSimControl->AwakeInterrupt();

			__PreINTR	= g_pSimTop->INTR;
		}
#endif
#ifdef SIM_TRACE

		if(g_pWaveDump) {
			if(!g_lSimulationTime || (g_lSimulationTime > g_lTraceStartTime))
				g_pWaveDump->dump((vluint64_t)g_lSimulationTime);
		}

#endif
		return !Verilated::gotFinish();
	}

	virtual void Release(void) {
		delete this;
	}
};

SimHDL* CreateSimHDL(SimControl* pControl)
{
	g_pSimControl	= pControl;
	return new SimHDL_imp();
}

//-----------------------------------------------------------------------------------------------
// verilator's function override.
//-----------------------------------------------------------------------------------------------
void vl_finish(const char* filename, int linenum, const char* hier)
{
	LOGI("%s(%d) - Verilog $finish\n", filename, linenum);
	SimulationFlush();
}

void vl_stop(const char* filename, int linenum, const char* hier)
{
	LOGI("%s(%d) - Verilog $stop, Press a 'SPACE' or 'ESC' key to continue.\n", filename, linenum);
	SimulationStop();
}

void vl_fatal(const char* filename, int linenum, const char* hier, const char* msg)
{
	LOGE("Fatal error - %s:%d: %s\n", filename, linenum, msg);
	SimulationFlush();
}

double sc_time_stamp()          // Called by $time in Verilog
{
	return SimulationTime();
}

//-----------------------------------------------------------------------------------------------
// bypass command
BUS_SLAVE_INTERFACE* CreateSlave(DWORD dwAddrBase, DWORD dwAddrHigh)
{
	return g_pSimControl->CreateSlave(dwAddrBase, dwAddrHigh);
}

BUS_SLAVE_INTERFACE* FindSlave(DWORD dwAddress)
{
	return g_pSimControl->FindSlave(dwAddress);
}

CLOCK_INTERFACE* FindClock(BYTE* pCLK)
{
	return g_pSimControl->FindClock(pCLK);
}

void MemoryRead32(int ID, unsigned int ADDR, unsigned int* DATA)
{
	g_pSimControl->MemoryRead32(ID, ADDR, *(DWORD*)DATA);
}

void MemoryWrite32(int ID, unsigned int ADDR, unsigned int DATA)
{
	g_pSimControl->MemoryWrite32(ID, ADDR, DATA);
}

void MemoryRead16(int ID, unsigned int ADDR, unsigned short int* DATA)
{
	g_pSimControl->MemoryRead16(ID, ADDR, *(WORD*)DATA);
}

void MemoryWrite16(int ID, unsigned int ADDR, unsigned short int DATA)
{
	g_pSimControl->MemoryWrite16(ID, ADDR, DATA);
}

void MemoryRead8(int ID, unsigned int ADDR, unsigned char* DATA)
{
	g_pSimControl->MemoryRead8(ID, ADDR, *(BYTE*)DATA);
}

void MemoryWrite8(int ID, unsigned int ADDR, unsigned char DATA)
{
	g_pSimControl->MemoryWrite8(ID, ADDR, DATA);
}

SYSTEM_CONFIG* GetSystemConfig(void)
{
	if(!g_pSimControl) return NULL;

	return g_pSimControl->GetSystemConfig();
}

DisplayConfig* GetDisplayConfig(void)
{
	if(!g_pSimControl) return NULL;

	return g_pSimControl->GetDisplayConfig();
}

BYTE* GetMemoryPointer(DWORD dwAddress, DWORD dwSize, BOOL bDisplay)
{
	if(!g_pSimControl) return NULL;

	return g_pSimControl->GetMemoryPointer(dwAddress, dwSize, bDisplay);
}

DWORD GetMemoryBaseAddress(void)
{
	if(!g_pSimControl) return 0;

	return g_pSimControl->GetMemoryBaseAddress();
}

BOOL GetMemory(const char* sName, void*& pConfig, void*& pMemory)
{
	if(!g_pSimControl) return NULL;

	return g_pSimControl->GetMemory(sName, pConfig, pMemory);
}

void SimulationQuit(bool bError)
{
	if(g_pSimControl) g_pSimControl->SetError();

	Verilated::gotFinish(true);
}

void SimulationStop(void)
{
	SimulationFlush();

	while(GetKeyState(VK_SPACE) >= 0 && GetKeyState(VK_ESCAPE) >= 0) Sleep(10);	// wait key down

	while(GetKeyState(VK_SPACE) < 0 || GetKeyState(VK_ESCAPE) < 0) Sleep(10);	// wait key up
}

void SimulationFlush(void)
{
	Verilated::flushCall();
	fflush(stdout);
}

UINT64 SimulationTime(void)
{
	return g_lSimulationTime;
}

void AdvenceSimulationTime(DWORD dwTime)
{
	g_lSimulationTime	+= dwTime;
}

void SimulationDebugMode(BOOL bDebug)
{
	if(!g_pSimControl) return;

	g_pSimControl->SimulationDebugMode(bDebug);
}

void SimulationLock(int iDelayTicks)
{
	if(!g_pSimControl) return;

	g_pSimControl->SimulationLock(iDelayTicks);
}

void SimulationUnLock(void)
{
	if(!g_pSimControl) return;

	g_pSimControl->SimulationUnLock();
}

void SetSystemDescription(const char* sDesc)
{
	if(!g_pSimControl) return;

	return g_pSimControl->SetSystemDescription(sDesc);
}

void* SimulationClockCreate(BYTE* pCLK, BYTE* pRST)
{
	return (void*)g_pSimControl->CreateClock(pCLK, pRST);
}

void SimulationCreateBusy(BYTE* pBUSY)
{
	g_pSimControl->SimulationAddBusy(pBUSY);
}

DPI_FUNCTION int SimulationAwakeInterrupt(void)
{
	return g_pSimControl->AwakeInterrupt();
}

DPI_FUNCTION void SimulationSetClock(void* hHandle, unsigned int dwID, unsigned int dwPeriod, unsigned char InitValue, unsigned int dwPhase, unsigned char ClockPolarity, unsigned char ResetPolarity)
{
	CLOCK_INTERFACE* pClock	= (CLOCK_INTERFACE*)hHandle;
	pClock->SetParameters(dwID, dwPeriod, InitValue, dwPhase, ClockPolarity, ResetPolarity);
}

DPI_FUNCTION void SimulationSetReset(void* hHandle, unsigned int dwCycles)
{
	CLOCK_INTERFACE* pClock	= (CLOCK_INTERFACE*)hHandle;
	pClock->DoReset(dwCycles);
}
