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
// Title : Private dynamic document
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "SimALU.h"

SimALU::SimALU(void)
{
	m_pSimALU	= NULL;
	m_hModule	= NULL;
}

SimALU::~SimALU(void)
{
	Release();
}

void SimALU::Release(void)
{
	if(m_pSimALU) {
		m_pSimALU->Release();
		m_pSimALU	= NULL;
	}

	if(m_hModule) {
		FreeLibrary(m_hModule);
		m_hModule	= NULL;
	}
}

BOOL SimALU::Initialize(LPCTSTR sModulePath)
{
	Release();
	m_hModule	= LoadLibrary(sModulePath);

	if(!m_hModule) {
		g_pSystem->LogWarning(_L(SIMULATION_MODULE_IS_NOT_FOUND), sModulePath);
		return FALSE;
	}

	CREATE_SIM_ALU_FUNCTION CreateSimALU	= (CREATE_SIM_ALU_FUNCTION)GetProcAddress(m_hModule, "CreateSimALU");

	if(!CreateSimALU) {
		Release();
		g_pSystem->LogError(_L(IS_NOT_SIMULATION_MODULE), sModulePath);
		return FALSE;
	}

	m_pSimALU	= CreateSimALU();

	if(!m_pSimALU) {
		Release();
		g_pSystem->LogError(_L(CANT_INITAILIZE_SIMULATION_MODULE), sModulePath);
	}

	return TRUE;
}

BOOL SimALU::SimulateALU(float DIN, float& DOUT, float& DOUT_GOLDEN)
{
	if(m_pSimALU) return m_pSimALU->SimulateALU(DIN, DOUT, DOUT_GOLDEN);

	return FALSE;
}
