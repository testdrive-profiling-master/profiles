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
// Title : TestDrive codegen project
// Rev.  : 6/7/2024 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __SCRIPT_H__
#define __SCRIPT_H__
#include "UtilFramework.h"
#include "VL.h"
#include "lua.hpp"
#include <filesystem>
// lua bridge
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/Vector.h"
#include "LuaBridge/UnorderedMap.h"
#include "LuaFile.h"
using namespace luabridge;

class Script
{
public:
	Script(void);
	virtual ~Script(void);

	bool		Run(const char *sFileName);
	bool		RunBuffer(const char *sBuffer, const char *sFileName = "inline");
	static void EnableTraceBack(bool bEnable = true);

protected:
	static bool	   __RunScript(const char *sFileName);
	static bool	   __RunString(const char *sCodes, const char *sTag);

	lua_State	  *m_pLua;
	cstring		   m_sEnvPath;
	cstring		   m_sWorkPath;
	cstring		   m_sLuaFileName;
	static Script *m_pScript;
};

#endif //__SCRIPT_H__
