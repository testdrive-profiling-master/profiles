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
// Title : Common profiles
// Rev.  : 1/27/2023 Fri (clonextop@gmail.com)
//================================================================================
#include "SystemHALConfig.h"
#include "MemoryHeap.h"
#include <stdio.h>
#include <assert.h>

#ifdef WIN32
BOOL GetConfiguration(LPCTSTR sKeyName, LPTSTR sValue)
{
	if(!GetEnvironmentVariable(sKeyName, sValue, MAX_PATH)) {
		char	sConfigFIlePath[4096];
		GetFullPathName(".TestDrive", 4096, sConfigFIlePath, NULL);
		GetPrivateProfileString("Configuration", sKeyName, "", sValue, MAX_PATH, sConfigFIlePath);
	}

	return (*sValue) != NULL;
}
#endif

SystemHALConfig::SystemHALConfig(void)
{
#ifdef WIN32
	m_hSystemImpModule	= NULL;
#endif
	Initialize();
}

SystemHALConfig::~SystemHALConfig(void)
{
	Release();
}

void SystemHALConfig::Initialize(void)
{
}

void SystemHALConfig::Release(void)
{
#ifdef WIN32

	if(m_hSystemImpModule) {
		FreeLibrary(m_hSystemImpModule);
		m_hSystemImpModule	= NULL;
	}

#endif
}

ISystem* SystemHALConfig::CreateSystem(void)
{
	ISystemImp* pImp	= NULL;
#ifdef WIN32

	// open sub-module
	if(!m_hSystemImpModule) {
		char sLibraryPath[MAX_PATH];

		if(!GetConfiguration("SUB_SYSTEM_PATH", sLibraryPath)) {
			strcpy(sLibraryPath, "SimSystem.dll");	// set default system path
		}

		m_hSystemImpModule = LoadLibrary(sLibraryPath);

		if(!m_hSystemImpModule) {
			LOGI("Sub-system is not found : %s\n", sLibraryPath);
			return NULL;
		}
	}

	{
		// Create sub-system
		CreateSystemImplementationFunction	CreateSystemImplementation		= (CreateSystemImplementationFunction)GetProcAddress(m_hSystemImpModule, "CreateSystemImplementation");

		if(CreateSystemImplementation) {
#endif
			pImp = CreateSystemImplementation();

			if(pImp) {
				// Initialize sub-system
				if(!pImp->Initialize(&g_MemoryImplementation)) {
					pImp->Release();
					pImp	= NULL;
					g_MemoryImplementation.Release();
				}
			}

#ifdef WIN32
		} else {
			LOGI("*E : Can't find 'CreateSystemImplementation' implementation in sub-system module.\n");
		}
	}

#endif
	return pImp;
}

static SystemHALConfig	__HALConfig;

SYSTEM_API ISystem* CreateSystem(void)
{
	return __HALConfig.CreateSystem();
}
