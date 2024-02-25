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
// Title : System manager
// Rev.  : 2/26/2024 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "BaseSystemConfig.h"
#include "BuildAutomation.h"
#include "SubSystemList.h"
#include "TopDesignList.h"
#include "testdrive_document.h"

typedef enum {
	PROPERTY_ID_SYSTEM_TYPE,
	PROPERTY_ID_COMPILER,
	PROPERTY_ID_SIMULATOR,
	PROPERTY_ID_BUILD_AUTOMATION,
	PROPERTY_ID_USE_DEBUG,
	PROPERTY_ID_AUTO_UPDATE,
	PROPERTY_ID_CODING_STYLE,
	PROPERTY_ID_CODING_I_STYLE,
	PROPERTY_ID_STRICT_BUILD,
	PROPERTY_ID_AUTHOR_INFO,

	PROPERTY_ID_SIMULATION_TOP,
	PROPERTY_ID_SIMULATION_DEFINITION,
	PROPERTY_ID_SIMULATION_WAVE_MODE,
	PROPERTY_ID_SIMULATION_MULTITHREAD,
	PROPERTY_ID_SIMULATION_BAKED_MODEL,
	PROPERTY_ID_SIMULATION_BUILD_JOBS,
	PROPERTY_ID_SIMULATION_WAVE_OUTPUT_FILE,
	PROPERTY_ID_SIMULATION_WAVE_DO_FILE,
	PROPERTY_ID_TRACE_START_TIME,
	PROPERTY_ID_SIZE
} PROPERTY_ID;

typedef enum {
	SIM_PROPERTY_TOP,
	SIM_PROPERTY_DEFINITION,
	SIM_PROPERTY_WAVE_MODE,
	SIM_PROPERTY_MULTITHREAD,
	SIM_PROPERTY_BAKED_MODEL,
	SIM_PROPERTY_BUILD_JOBS,
	SIM_PROPERTY_WAVE_OUTPUT_FILE,
	SIM_PROPERTY_WAVE_DO_FILE,
	SIM_PROPERTY_TRACE_START_TIME,
	SIM_PROPERTY_SIZE
} SIM_PROPERTY;

typedef enum {
	COMMAND_AUTOMATION_CEHCK,
	COMMAND_AUTOMATION_BUILD	 = COMMAND_AUTOMATION_CEHCK + 128,
	COMMAND_AUTOMATION_BUILD_END = COMMAND_AUTOMATION_BUILD + 128,
	COMMAND_ID_MMAPVIEW			 = 0x10000,
} COMMAND_ID;

class System : public TDImplDocumentBase
{
public:
	System(ITDDocument *pDoc);
	virtual ~System(void);

	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData *pProperty);
	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);

	void UpdateEnvironments(void);
	void WriteConfiguration(LPCTSTR sKeyName, LPCTSTR sValue);
	void ExternalCommand(TESTDRIVE_CMD cmd, WORD op, DWORD lParam);

protected:
	CString GetStringFromSystemMemory(DWORD offset);
	void	MakeDefinitionList(BOOL bAutoFix = TRUE);
	void	UpdateDefaultSystemConfigHeader(void);

private:
	ITDDocument		   *m_pDoc;
	BASE_SYSTEM_CONFIG *m_pSystemConfig;
	BuildAutomation		m_BuildAutomation;
	SubSystemList		m_SubSystemList;
	TopDesignList		m_TopDesignList;

	CString				m_sSubSystem;
	CString				m_sCompiler;
	CString				m_sSimulator;
	CString				m_sSimTopFile;
	CString				m_sSimDefinition;
	CString				m_sSimWaveMode;
	CString				m_sSimWaveOutputFile;
	CString				m_sSimWaveDoFile;
	CString				m_sSimMultithreading;
	int					m_iSimBuildJobCount;
	int					m_iTraceStartTime;
	BOOL				m_bAutoUpdate;
	BOOL				m_bBakedModel;

	// properties
	ITDPropertyData *m_pPropertySim[SIM_PROPERTY_SIZE];

	BOOL			 m_bBusy;
};
#endif //__SYSTEM_H__
