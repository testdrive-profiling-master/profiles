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
// Title : System manager
// Rev.  : 7/21/2021 Wed (clonextop@gmail.com)
//================================================================================
#include "System.h"
#include "Utils.h"
#include "testdrive_document.inl"

REGISTER_LOCALED_DOCUMENT(System)

static LPCTSTR __sCompiler[] = {
	_T("gcc"),
	_T("clang"),
	NULL
};

static LPCTSTR __sSimulator[] = {
	_T("verilator"),
	_T("iverilog"),
	NULL
};

static LPCTSTR __sSimWaveMode[] = {
	_T("None"),
	_T("File output"),
	_T("Interactive"),
	NULL
};

static LPCTSTR __sSimCDEFS[] = {
	_T(""),
	_T("-DSIM_TRACE -DSIM_TRACE_FILE_OUTPUT"),
	_T("-DSIM_TRACE -DSIM_TRACE_INTERACTIVE")
};

static LPCTSTR __sSimMultithreading[] = {
	_T("Disable"),
	_T("Enable(HDL)"),
	_T("Enable(HDL+DPI)"),
	NULL
};

static LPCTSTR	__sLastLunchTime		= _T("LAST_LAUNCH_TIME");

void System::MakeDefinitionList(BOOL bAutoFix)
{
	TCHAR	sFirstDefinition[1024 * 32];
	BOOL				bMatch		= FALSE;
	BOOL				bNewTop		= FALSE;

	if(!m_sSimTopFile[0]) {	// if not specified, set first one thing.
		TopDesignItem*	pItem	= m_TopDesignList.Size() ? m_TopDesignList.Item(0) : NULL;

		if(pItem) {
			_tcscpy(m_sSimTopFile.Buffer(), pItem->sTopPath);
			m_pPropertySim[SIM_PROPERTY_TOP]->UpdateData(FALSE);
			m_pPropertySim[SIM_PROPERTY_TOP]->UpdateConfigFile(FALSE);
			bNewTop	= TRUE;
		}
	}

	ITDPropertyData*	pProperty	= m_pPropertySim[SIM_PROPERTY_DEFINITION];
	TopDesignItem*		pTop		= m_TopDesignList.FindDesign(m_sSimTopFile);
	sFirstDefinition[0]	= _T('\0');
	pProperty->AddOption(NULL);

	if(pTop)
		for(list<CString>::iterator i = pTop->OptionList.begin(); i != pTop->OptionList.end(); i++) {
			if(!sFirstDefinition[0]) _tcscpy(sFirstDefinition, (*i));

			if(!m_sSimDefinition.Compare(*i)) bMatch	= TRUE;

			pProperty->AddOption(*i);
		}

	if(bNewTop || (bAutoFix && !bMatch)) {
		if(sFirstDefinition[0])	// definitions are existed and no matched, then set to first definition
			_tcscpy(m_sSimDefinition.Buffer(), sFirstDefinition);
		else
			m_sSimDefinition.SetAt(0, _T('\0'));

		pProperty->UpdateData(FALSE);
		pProperty->UpdateConfigFile(FALSE);
	}
}

System::System(ITDDocument* pDoc) :
	m_bBusy(FALSE),
	m_BuildAutomation(pDoc, COMMAND_AUTOMATION_CEHCK, COMMAND_AUTOMATION_BUILD)
{
	m_pDoc				= pDoc;
	m_pSystemConfig		= (BASE_SYSTEM_CONFIG*)g_pSystem->GetMemory(NULL, TRUE)->GetConfig();
	m_bAutoUpdate		= FALSE;
	WriteConfiguration(_T("PROJECT_NAME"),	g_pSystem->GetMemory(NULL, TRUE)->GetName());
	UpdateDefaultSystemConfigHeader();
	m_pSystemConfig->hSystemManager	= pDoc->GetWindowHandle();
	m_SubSystemList.Initialize();

	if(m_SubSystemList.Size()) m_TopDesignList.Initialize();

	// Hide this document, can't close by user until program quit.
	pDoc->Show(FALSE);
	{
		// register properties...
		ITDPropertyData* pProperty;
		m_sSubSystem.GetBuffer(MAX_PATH);

		if(m_SubSystemList.Size()) {
			m_sSubSystem			= m_SubSystemList.Item(0) ? m_SubSystemList.Item(0)->sName.c_str() : _T("");
			pProperty				= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SYSTEM_TYPE, _L(SUB_SYSTEM), (DWORD_PTR)m_sSubSystem.GetBuffer(), _L(DESC_SUB_SYSTEM));
			pProperty->UpdateConfigFile();

			if(!m_SubSystemList.Find(m_sSubSystem)) {
				m_sSubSystem		= m_SubSystemList.Item(0) ? m_SubSystemList.Item(0)->sName.c_str() : _T("");
				g_pSystem->LogInfo(_T("%s"), m_SubSystemList.Item(0)->sName.c_str());
				pProperty->UpdateData(FALSE);
			}

			pProperty->AllowEdit(FALSE);

			for(int i = 0; i < m_SubSystemList.Size(); i++) pProperty->AddOption(m_SubSystemList.Item(i)->sName);
		}

		m_sCompiler.GetBuffer(1024);
		m_sCompiler			= __sCompiler[0];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_COMPILER, _L(COMPILER), (DWORD_PTR)m_sCompiler.GetBuffer(), _L(DESC_COMPILER));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for(int i = 0; __sCompiler[i]; i++) pProperty->AddOption(__sCompiler[i]);

		m_sSimulator.GetBuffer(1024);
		m_sSimulator		= __sSimulator[0];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SIMULATOR, _L(SIMULATOR), (DWORD_PTR)m_sSimulator.GetBuffer(), _L(DESC_SIMULATOR));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for(int i = 0; __sSimulator[i]; i++) pProperty->AddOption(__sSimulator[i]);

		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_BUILD_AUTOMATION, _L(BUILD_AUTOMATION), (DWORD_PTR) & (m_BuildAutomation.AutoBuild()), _L(DESC_BUILD_AUTOMATION));
		pProperty->UpdateConfigFile();
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_USE_DEBUG, _L(DEBUG_INFO), (DWORD_PTR) & (m_BuildAutomation.UseDebug()), _L(DESC_USE_DEBUG));
		pProperty->UpdateConfigFile();
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_AUTO_UPDATE, _L(AUTO_UPDATE), (DWORD_PTR) & (m_bAutoUpdate), _L(DESC_AUTO_UPDATE));
		pProperty->UpdateConfigFile();
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_CODING_A_STYLE, _L(CODING_A_STYLE), (DWORD_PTR)(LPCTSTR)(m_BuildAutomation.AStyle()), _L(DESC_CODING_A_STYLE));
		pProperty->UpdateConfigFile();
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_CODING_I_STYLE, _L(CODING_I_STYLE), (DWORD_PTR)(LPCTSTR)(m_BuildAutomation.iStyle()), _L(DESC_CODING_I_STYLE));
		pProperty->UpdateConfigFile();
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_BOOL, PROPERTY_ID_STRICT_BUILD, _L(STRICT_BUILD), (DWORD_PTR) & (m_BuildAutomation.StrictBuild()), _L(DESC_STRICT_BUILD));
		pProperty->UpdateConfigFile();
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_AUTHOR_INFO, _L(AUTHOR), (DWORD_PTR)m_BuildAutomation.AuthorName().GetBuffer(), _L(DESC_AUTHOR));
		pProperty->UpdateConfigFile();

		if(m_BuildAutomation.AuthorName().IsEmpty())
			g_pSystem->LogWarning(_L(AUTHOR_IS_EMPTY));

		m_sSimTopFile.GetBuffer(1024);
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SIMULATION_TOP, _L(SIM_TOP), (DWORD_PTR)(LPCTSTR)m_sSimTopFile, _L(DESC_SIM_TOP));
		pProperty->UpdateConfigFile();
		m_pPropertySim[SIM_PROPERTY_TOP]	= pProperty;
		{
			for(int i = 0; i < m_TopDesignList.Size(); i++) {
				TopDesignItem* pTop	= m_TopDesignList.Item(i);
				pProperty->AddOption(pTop->sTopPath);
			}
		}
		m_sSimDefinition.GetBuffer(1024 * 32);
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SIMULATION_DEFINITION, _L(SIM_DEFINITION), (DWORD_PTR)(LPCTSTR)m_sSimDefinition, _L(DESC_SIM_DEFINITION));
		pProperty->UpdateConfigFile();
		m_pPropertySim[SIM_PROPERTY_DEFINITION]	= pProperty;
		MakeDefinitionList(FALSE);	// preserve previous configuration.
		m_sSimWaveMode.GetBuffer(MAX_PATH);
		m_sSimWaveMode		= __sSimWaveMode[0];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SIMULATION_WAVE_MODE, _L(WAVE_OUTPUT), (DWORD_PTR)m_sSimWaveMode.GetBuffer(), _L(DESC_WAVE_OUTPUT));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for(int i = 0; __sSimWaveMode[i]; i++) pProperty->AddOption(__sSimWaveMode[i]);

		m_pPropertySim[SIM_PROPERTY_WAVE_MODE]	= pProperty;
		m_sSimMultithreading.GetBuf(1024);
		m_sSimMultithreading	= __sSimMultithreading[0];
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_STRING, PROPERTY_ID_SIMULATION_MULTITHREAD, _L(SIMULATION_MULTITHREADING), (DWORD_PTR)m_sSimMultithreading.GetBuffer(), _L(DESC_SIMULATION_MULTITHREADING));
		pProperty->UpdateConfigFile();
		pProperty->AllowEdit(FALSE);

		for(int i = 0; __sSimMultithreading[i]; i++) pProperty->AddOption(__sSimMultithreading[i]);

		m_pPropertySim[SIM_PROPERTY_MULTITHREAD]	= pProperty;
		m_sSimWaveOutputFile.GetBuffer(1024);
		m_sSimWaveOutputFile	= _T("sim.fst");
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_FILE_PATH, PROPERTY_ID_SIMULATION_WAVE_OUTPUT_FILE, _L(WAVE_OUTPUT_FILENAME), (DWORD_PTR)m_sSimWaveOutputFile.GetBuffer(), _L(DESC_OUTPUT_FILENAME));
		pProperty->UpdateConfigFile();
		m_pPropertySim[SIM_PROPERTY_WAVE_OUTPUT_FILE]	= pProperty;
		m_sSimWaveDoFile.GetBuffer(1024);
		m_sSimWaveDoFile	= _T("sim.do");
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_FILE_PATH, PROPERTY_ID_SIMULATION_WAVE_DO_FILE, _L(DO_FILENAME), (DWORD_PTR)m_sSimWaveDoFile.GetBuffer(), _L(DESC_DO_FILENAME));
		pProperty->UpdateConfigFile();
		m_pPropertySim[SIM_PROPERTY_WAVE_DO_FILE]	= pProperty;
		m_iTraceStartTime	= 0;
		pProperty			= pDoc->AddPropertyData(PROPERTY_TYPE_INT, PROPERTY_ID_TRACE_START_TIME, _L(TRACE_START_TIME), (DWORD_PTR)&m_iTraceStartTime, _L(DESC_TRACE_START_TIME));
		m_pPropertySim[SIM_PROPERTY_TRACE_START_TIME]	= pProperty;
	}
	UpdateEnvironments();
	{
		BOOL	bFirstRunToday	= FALSE;
		BOOL	bMonthlyUpdate	= FALSE;
		{
			// check code analysis
			CString	cur_time;
			{
				int wday;
				cur_time	= GetTodayString(&wday);
				{
					// first run check on today
					CString	last_time;
					last_time.GetBuffer(1024);
					pDoc->GetConfigString(__sLastLunchTime, last_time.GetBuffer(), 1024);
					bFirstRunToday	= (cur_time.Compare(last_time) != 0);

					// check monthly update
					if(last_time.GetLength()) {
						CString	cur_month(cur_time);
						last_time.Replace(_T('/'), _T('\0'));
						cur_month.Replace(_T('/'), _T('\0'));
						bMonthlyUpdate = (cur_month.Compare(last_time) != 0);
					}
				}

				if(bFirstRunToday) {
					pDoc->SetConfigString(__sLastLunchTime, cur_time);

					if(wday == 1) {	// we will run cppcheck once on every monday...
						g_pSystem->RunProfile(_T("%TESTDRIVE_PROFILE%Common\\CodeAnalysis\\main.sp"));
					}
				}

				if(m_bAutoUpdate && bMonthlyUpdate) {
					g_pSystem->LogInfo(_L(MONTHLY_UPDATE));
					g_pSystem->ExecuteFile(_T("%TESTDRIVE_DIR%bin\\upgrade_mingw.bat"), NULL, TRUE, NULL, _T("%TESTDRIVE_DIR%bin"), NULL);
				}
			}
		}
		{
			// Do essential build
			g_pSystem->LogInfo(_T("[%s] %s"), _L(DOCUMENT_TITLE), _L(DO_ESSENTIAL_BUILD));
			g_pSystem->RunProfile(_T("../System/initialize.sp"), TRUE);
			g_pSystem->LogInfo(_T("[%s] %s"), _L(DOCUMENT_TITLE), _L(DONE));
		}

		// Check need to "Build All"
		//if(bFirstRunToday && !IsFileExisted(_T("%PROJECT%\\Program\\SimSystem.dll"))) {
		if(m_SubSystemList.Size() && !IsFileExisted(_T("%PROJECT%\\Program\\SimSystem.dll"))) {
			g_pSystem->LogInfo(_L(INFO_BUILD_ALL));
		}
	}
}

System::~System(void)
{
	m_pSystemConfig->hSystemManager	= NULL;
}

void System::UpdateEnvironments(void)
{
	for(int i = 0; i < m_SubSystemList.Size(); i++) {
		if(!(m_SubSystemList.Item(i)->sName.Compare(m_sSubSystem))) {
			m_pSystemConfig->dwSystemType	= (SYSTEM_TYPE)i;
			break;
		}
	}

	// Set environment variables
	WriteConfiguration(_T("MAIN_COMPILER"),		m_sCompiler);
	WriteConfiguration(_T("MAIN_SIMULATOR"),	m_sSimulator);
	WriteConfiguration(_T("SUB_SYSTEM_NAME"),	m_sSubSystem);
	WriteConfiguration(_T("SUB_SYSTEM_PATH"),	m_SubSystemList.Find(m_sSubSystem) ? (LPCTSTR)(m_SubSystemList.Find(m_sSubSystem)->sFile) : _T(""));
	WriteConfiguration(_T("AUTHOR"),			m_BuildAutomation.AuthorName());
	WriteConfiguration(_T("SIM_TOP_FILE"),		m_sSimTopFile);
	{
		CString sTopPath;	// simulation top path
		sTopPath	= (LPCTSTR)m_sSimTopFile;
		sTopPath.Replace(_T('\\'), _T('/'));
		{
			// set do/output simulation file
			CString sOutputPath, sDoPath;
			{
				int iPointPos	= sTopPath.ReverseFind('.');
				int iSlashPos	= sTopPath.ReverseFind('/');
				CString	sSimPath(sTopPath);

				if(iPointPos > 0 && iSlashPos > 0 && iPointPos > iSlashPos)
					sSimPath.SetAt(iPointPos, _T('\0'));

				if(iSlashPos > 0) sTopPath.SetAt(iSlashPos, _T('\0'));

				sOutputPath.Format(_T("../System/HDL/%s.%s"), (LPCTSTR)sSimPath, (LPCTSTR)m_sSimWaveOutputFile);
				sDoPath.Format(_T("../System/HDL/%s.%s"), (LPCTSTR)sSimPath, (LPCTSTR)m_sSimWaveDoFile);
				WriteConfiguration(_T("SIM_TOP_PATH"), (iSlashPos == -1) ? _T("") : (LPCTSTR)sTopPath);
			}
			// set simulation file path
			WriteConfiguration(_T("SIM_OUTPUT_FILE"), sOutputPath);
			WriteConfiguration(_T("SIM_DO_FILE"), sDoPath);
		}
		{
			int pos	= 0;
			CString sEnv;		// verilator's additional definitions
			sEnv.Format(_T("-I%s"), sTopPath);	//add top include path

			for(;;) {
				CString sTok	= m_sSimDefinition.Tokenize(_T(";, "), pos);

				if(pos >= 0) {
					sEnv.AppendFormat((sTok.GetAt(0) == _T('-')) ? _T(" %s") : _T(" -D%s"), (LPCTSTR)sTok);
				} else break;
			}

			WriteConfiguration(_T("SIM_DEFINITION"), sEnv);
		}
		{
			// simulation C++ defines
			int id = 0;

			for(; __sSimWaveMode[id] && _tcscmp(m_sSimWaveMode, __sSimWaveMode[id]); id++);

			WriteConfiguration(_T("SIM_CDEFS"),	__sSimCDEFS[id]);
		}
		{
			// normal C++ defines
			if(m_BuildAutomation.UseDebug())
				WriteConfiguration(_T("EXTRA_CFLAGS"),	m_BuildAutomation.UseDebug() ? _T("-g -O0") : _T("-O3 -DNDEBUG"));
		}
	}
	WriteConfiguration(_T("SIM_MULTITHREADING"), m_sSimMultithreading);
	WriteConfiguration(_T("SIM_WAVE_MODE"), m_sSimWaveMode);
	{
		// set trace start time (us)
		CString sEnv;
		sEnv.Format(_T("%d"), m_iTraceStartTime);
		WriteConfiguration(_T("SIM_TRACE_START_TIME"), (LPCTSTR)sEnv);
	}
	{
		// show simulation mode only select simulation subsystem is selected..
		BOOL	bSim	= m_SubSystemList.Size() ? (m_pSystemConfig->dwSystemType == SYSTEM_TYPE_SIMULATOR) : FALSE;

		for(int i = 0; i < SIM_PROPERTY_SIZE; i++)
			m_pPropertySim[i]->Show(bSim);
	}
}

BOOL System::OnPropertyUpdate(ITDPropertyData* pProperty)
{
	if(m_pDoc->IsLocked()) {
		g_pSystem->LogWarning(_L(SYSTEM_IS_LOCKED));
		pProperty->UpdateData(FALSE);
		return TRUE;
	}

	pProperty->UpdateData(TRUE);

	if(pProperty->GetID() == PROPERTY_ID_SIMULATION_TOP)
		MakeDefinitionList();

	pProperty->UpdateConfigFile(FALSE);
	UpdateEnvironments();

	if(pProperty->GetID() == PROPERTY_ID_SIMULATION_TOP || pProperty->GetID() == PROPERTY_ID_SIMULATION_DEFINITION || pProperty->GetID() == PROPERTY_ID_SIMULATION_WAVE_MODE || pProperty->GetID() == PROPERTY_ID_SIMULATION_MULTITHREAD) {
		m_BuildAutomation.SetDirtySystem();
		m_pDoc->SetTimer(COMMAND_AUTOMATION_BUILD + 0, 50);
	}

	return TRUE;
}

static void RunMMapView(LPCTSTR sArg)
{
	CString sFile		= g_pSystem->RetrieveFullPath(_T("%TESTDRIVE_DIR%bin\\HexEdit.exe"));
	CString sPrjectPath	= g_pSystem->RetrieveFullPath(_T("%PROJECT%Program\\"));
	ShellExecute(NULL, NULL, sFile, sArg, sPrjectPath, SW_SHOW);
}

BOOL System::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam)
{
	switch(command) {
	case(TD_EXTERNAL_COMMAND-1):
		UpdateEnvironments();
		break;

	case TD_EXTERNAL_COMMAND:		// sendmessage (WM_USER,--)
		ExternalCommand((TESTDRIVE_CMD)LOWORD(wParam), HIWORD(wParam), lParam);
		break;

	case COMMAND_ID_MMAPVIEW:		// open main memory map viewer
		RunMMapView((LPCTSTR)wParam);
		break;

	default:
		m_BuildAutomation.OnCommand(command, wParam, lParam);
		break;
	}

	return TRUE;
}

CString System::GetStringFromSystemMemory(DWORD offset)
{
	char*	str_ansi	= (char*)g_pSystem->GetMemory()->GetPointer(offset);
	int		str_size	= MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, NULL, NULL);
	TCHAR*	str_unicode	= new TCHAR[str_size];
	MultiByteToWideChar(CP_ACP, 0, str_ansi, strlen(str_ansi) + 1, str_unicode, str_size);
	CString	str(str_unicode);
	delete [] str_unicode;
	return str;
}

void System::ExternalCommand(TESTDRIVE_CMD cmd, WORD op, DWORD lParam)
{
	CString	sParam	= GetStringFromSystemMemory(lParam);

	switch(cmd) {
	case TESTDRIVE_CMD_SET_TITLE:
		g_pSystem->SetTitle((ITDSystem::TITLE_ID)op, (LPCTSTR)sParam);
		break;

	case TESTDRIVE_CMD_CLEAR_LOG:
		g_pSystem->ClearLog();
		break;

	case TESTDRIVE_CMD_RUN_PROFILE:
		g_pSystem->RunProfile((LPCTSTR)sParam);
		break;

	case TESTDRIVE_CMD_GET_DOCUMENT_HANDLE: {
		ITDDocument* pDoc	= g_pSystem->GetDocument((LPCTSTR)sParam);
		HWND*		 pHwnd	= (HWND*)g_pSystem->GetMemory()->GetPointer(lParam);
		*pHwnd				= pDoc ? pDoc->GetWindowHandle() : NULL;
	}
	break;

	default:
		g_pSystem->LogWarning(_T("Undefined external command!"));
	}
}
