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
//=======================================================================
// CloneX developments. Copyright 2013~2014. ALL RIGHTS RESERVED
// Title : Simulation HDL module
// Rev.  : 4/24/2014 Tue (clonextop@gmail.com)
//=======================================================================
#ifndef __SYSTEM_HAL_H__
#define __SYSTEM_HAL_H__
#include "STDInterface.h"

interface ISimALU{
	// Life cycle
	STDMETHOD_(void, Release)(void) PURE;

	// simulation
	STDMETHOD_(BOOL, SimulateALU)(float DIN, float& DOUT, float& DOUT_GOLDEN) PURE;
};

#ifdef SIM_HDL_EXPORTS
	#define SIMHDL_API extern "C" __declspec(dllexport)

	SIMHDL_API ISimALU* CreateSimALU(void);
#else
	extern "C"{
		typedef ISimALU* (*CREATE_SIM_ALU_FUNCTION)(void);
	}
#endif

#endif//__SYSTEM_HAL_H__
