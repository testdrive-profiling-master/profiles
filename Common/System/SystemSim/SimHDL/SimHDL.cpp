//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 2/20/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "SimHDL_common.h"
#include "TestDriver.inl"
#include "SimHDL.h"
#include "SimTop.h"
#include <time.h>		// for rand/srand

static UINT64				__lSimulationTime		= 0;		// global simulation timestamp
static SimHDL*				__pSimHDL				= NULL;
static SimControl*			__pSimControl			= NULL;
static VerilatedContext*	__pContext				= NULL;
static SimTop*				__pSimTop				= NULL;
bool (*DPI_Initialize)(void)						= NULL;
void (*DPI_Finalize)(void)							= NULL;
#ifndef SIMULATION_TOP_EX
#endif
#ifdef SIM_TRACE
#ifdef SIM_TRACE_FILE_OUTPUT
static VerilatedFstC*		__pWaveDump				= NULL;
#elif defined(SIM_TRACE_INTERACTIVE)
static VerilatedVcdC*		__pWaveDump				= NULL;
static VerilatedVcdGTKWave*	__pGtkWave				= NULL;
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

		if(__pWaveDump)
			__pWaveDump->close();

		SAFE_DELETE(__pWaveDump);
#ifdef SIM_TRACE_INTERACTIVE
		SAFE_DELETE(__pGtkWave);
#endif
#endif

		if(__pSimHDL == this) {
			__pSimTop->final();
			delete __pSimTop;
			delete __pContext;
			__pSimTop		= NULL;
			__pSimHDL		= NULL;
			__pContext		= NULL;
		}
	}

	virtual bool Initialize(void) {	// cppcheck-suppress internalAstError
		if(!__pSimHDL) {
			srand(time(NULL));			// randomize seed
			__pSimHDL		= this;
			__pContext		= new VerilatedContext;
			__pContext->randReset(2);	// randomize all bits
			__pSimTop		= new SimTop(__pContext);
		} else {
			LOGI("'SimProcessor' At least one more instances has been created.\n");
			return false;
		}

#ifdef SYSTEM_MEMORY_BASE_ADDRESS
		SetMemoryBaseAddress(SYSTEM_MEMORY_BASE_ADDRESS);
#endif
#ifndef SIMULATION_TOP_EX
		__pSimTop->INTR		= 0;
		__pSimTop->nRST		= 0;
		__pSimTop->MCLK		= 0;
#endif
#ifdef SIM_TRACE
		{
			__pWaveDump			= NULL;
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
				__pContext->traceEverOn(true);
				__pWaveDump		= new VerilatedFstC();
#elif defined(SIM_TRACE_INTERACTIVE)
				// interactive GTKWave mode
				GetEnvironmentVariable("SIM_DO_FILE", sFile, MAX_PATH);
				__pContext->traceEverOn(true);
				__pGtkWave	= new VerilatedVcdGTKWave;
				__pWaveDump	= new VerilatedVcdC(__pGtkWave);
#endif

				if(__pWaveDump) {
					// Do simulation
					__pWaveDump->set_time_unit("1ps");
					__pWaveDump->set_time_resolution("1ps");
					__pSimTop->trace(__pWaveDump, 99);

					if(*sFile) __pWaveDump->open(sFile);
				}
			}
		}
#else
		__pContext->traceEverOn(false);	// no tracing
#endif
#ifndef SIMULATION_TOP_EX
		{
			// make default clock, reset & busy
			CLOCK_INTERFACE*	pClock	= __pSimControl->CreateClock(&__pSimTop->MCLK, &__pSimTop->nRST);
			pClock->SetParameters(0, DEFAULT_HALF_CLOCK_PERIOD);	// set half clock period
			pClock->DoReset();
			__pSimControl->SimulationAddBusy(&__pSimTop->BUSY);
		}
#endif

		if(DPI_Initialize) return DPI_Initialize();

		return true;
	}

	virtual bool IsTrace(void) {
#ifdef SIM_TRACE
		return (__pWaveDump != NULL);
#else
		return false;
#endif
	}

	virtual bool Eval(void) {
		// simulation evaluation
		if(__pContext) {
			__pSimTop->eval();
#ifndef SIMULATION_TOP_EX
			{
				if(__pSimTop->INTR)
					__pSimControl->AwakeInterrupt();
			}
#endif
#ifdef SIM_TRACE

			if(__pWaveDump) {
				if(!__lSimulationTime || (__lSimulationTime > g_lTraceStartTime))
					__pWaveDump->dump((vluint64_t)__lSimulationTime);
			}

#endif
			return !__pContext->gotFinish();
		}

		return false;
	}

	virtual void Release(void) {
		if(DPI_Finalize) DPI_Finalize();

		delete this;
	}
};

SimHDL* CreateSimHDL(SimControl* pControl)
{
	__pSimControl	= pControl;
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
BUS_SLAVE_INTERFACE* CreateSlave(UINT64 dwAddrBase, UINT64 dwAddrHigh)
{
	return __pSimControl->CreateSlave(dwAddrBase, dwAddrHigh);
}

BUS_SLAVE_INTERFACE* FindSlave(UINT64 dwAddress)
{
	return __pSimControl->FindSlave(dwAddress);
}

CLOCK_INTERFACE* FindClock(BYTE* pCLK)
{
	return __pSimControl->FindClock(pCLK);
}

void RegisterDriverCommand(DRIVER_COMMAND func)
{
	__pSimControl->RegisterDriverCommand(func);
}

void MemoryRead32(int ID, unsigned int ADDR, unsigned int* DATA)
{
	__pSimControl->MemoryRead32(ID, ADDR, *(DWORD*)DATA);
}

void MemoryWrite32(int ID, unsigned int ADDR, unsigned int DATA)
{
	__pSimControl->MemoryWrite32(ID, ADDR, DATA);
}

void MemoryRead16(int ID, unsigned int ADDR, unsigned short int* DATA)
{
	__pSimControl->MemoryRead16(ID, ADDR, *(WORD*)DATA);
}

void MemoryWrite16(int ID, unsigned int ADDR, unsigned short int DATA)
{
	__pSimControl->MemoryWrite16(ID, ADDR, DATA);
}

void MemoryRead8(int ID, unsigned int ADDR, unsigned char* DATA)
{
	__pSimControl->MemoryRead8(ID, ADDR, *(BYTE*)DATA);
}

void MemoryWrite8(int ID, unsigned int ADDR, unsigned char DATA)
{
	__pSimControl->MemoryWrite8(ID, ADDR, DATA);
}

SYSTEM_CONFIG* GetSystemConfig(void)
{
	if(!__pSimControl) return NULL;

	return __pSimControl->GetSystemConfig();
}

DisplayConfig* GetDisplayConfig(void)
{
	if(!__pSimControl) return NULL;

	return __pSimControl->GetDisplayConfig();
}

BYTE* GetMemoryPointer(DWORD dwAddress, DWORD dwSize, bool bDisplay)
{
	if(!__pSimControl) return NULL;

	return __pSimControl->GetMemoryPointer(dwAddress, dwSize, bDisplay);
}

DWORD GetMemoryBaseAddress(void)
{
	if(!__pSimControl) return 0;

	return __pSimControl->GetMemoryBaseAddress();
}

bool GetMemory(const char* sName, void*& pConfig, void*& pMemory)
{
	if(!__pSimControl) return false;

	return __pSimControl->GetMemory(sName, pConfig, pMemory);
}

void SimulationQuit(bool bError)
{
	if(__pSimControl) __pSimControl->SetError();

	__pContext->gotFinish(true);
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
	return __lSimulationTime;
}

void AdvenceSimulationTime(DWORD dwTime)
{
	__lSimulationTime	+= dwTime;
}

void SimulationDebugMode(bool bDebug)
{
	if(!__pSimControl) return;

	__pSimControl->SimulationDebugMode(bDebug);
}

void SimulationLock(int iDelayTicks)
{
	if(!__pSimControl) return;

	__pSimControl->SimulationLock(iDelayTicks);
}

void SimulationUnLock(void)
{
	if(!__pSimControl) return;

	__pSimControl->SimulationUnLock();
}

void SetSystemDescription(const char* sDesc)
{
	if(!__pSimControl) return;

	return __pSimControl->SetSystemDescription(sDesc);
}

void* SimulationClockCreate(BYTE* pCLK, BYTE* pRST)
{
	return (void*)__pSimControl->CreateClock(pCLK, pRST);
}

void SimulationCreateBusy(BYTE* pBUSY)
{
	__pSimControl->SimulationAddBusy(pBUSY);
}

DPI_FUNCTION int SimulationAwakeInterrupt(void)
{
	return __pSimControl->AwakeInterrupt();
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