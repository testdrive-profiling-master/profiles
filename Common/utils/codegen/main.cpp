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
// Title : make code application
// Rev.  : 1/26/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "Script.h"
#include "ArgTable.h"
#include "code_gen_version.h"

bool				__bSilence		= false;
const char*			__sLuaFileName	= NULL;

cstring GetTitle(void)
{
	cstring	sTitle;
	sTitle.Format("TestDrive code generator. v%d.%d (build #%d date : " __DATE__ ")", CODE_GEN_VERSION_MAJOR, CODE_GEN_VERSION_MINOR, CODE_GEN_BUILD_COUNT);
	return sTitle;
}

int main(int argc, const char* argv[])
{
	int			iRet	= 1;
	int			arg_size;
	ArgTable	main_arg_table(GetTitle());
	main_arg_table.SetExtraArguments("...");
	ArgTable::SetDefaultArguments(argc, argv);
	main_arg_table.AddOption("encrypt", "e", "encrypt", "Encrypt Lua script file");
	main_arg_table.AddOption("decrypt", "d", "decrypt", "Decrypt Lua script file");
	main_arg_table.AddOption("trace", NULL, "trace", "Enable traceback on Lua script");
	main_arg_table.AddOptionString("key", "code_gen_default", "k", "key", "key_code", "Set key code for Encrypt/Decrypt");
	main_arg_table.AddOptionFile("lua_file", NULL, NULL, NULL, "lua_file", "Lua script file");
	// calculate maintain ArgumentSize
	{
		bool	bBypass	= false;

		for(arg_size = 1; arg_size < argc; arg_size++) if(!bBypass) {
				if(!strcmp(argv[arg_size], "-k") || !strcmp(argv[arg_size], "--key")) {	// for secondary argument bypass
					bBypass	= true;
					continue;
				}

				if(*argv[arg_size] != '-') {
					arg_size++;
					break;
				}
			} else bBypass = false;
	}

	if(!main_arg_table.DoParse(arg_size, argv))
		return 1;

#ifdef WIN32
	// set UTF-8 mode
	SetConsoleOutputCP(65001);
#endif

	// get options
	__sLuaFileName	= main_arg_table.GetOptionFile("lua_file");
	LuaFile::SetKeyCode(main_arg_table.GetOptionString("key"));

	if(!__sLuaFileName) {
		LOGE("Need to specify a Lua script file.");
		return 1;
	}

	{
		// encrypt or decrypt
		bool	bEncrypt	= main_arg_table.GetOption("encrypt");
		bool	bDecrypt	= main_arg_table.GetOption("decrypt");
		Script::EnableTraceBack(main_arg_table.GetOption("trace"));

		if(bEncrypt || bDecrypt) {
			if(arg_size != argc) {
				LOGE("invalid extra argument(s)...");
				printf("\t=>");

				for(int i = arg_size; i < argc; i++)
					printf(" %s", argv[i]);

				printf("\n");
				return 1;
			}

			if(bEncrypt && bDecrypt) {
				LOGE("You must use one option encrypt or decrypt, not both.");
				return 1;
			}

			LuaFile f;

			if(!f.Load(__sLuaFileName)) {
				LOGE("Can't open Lua file : %s", __sLuaFileName);
				return 1;
			}

			if(bDecrypt && *LuaFile::GetKeyCode() == '@') {
				LOGW("Decryption is not allowed for this file.");
				return 1;
			}

			if(f.WasEncrypted() == bEncrypt) {
				LOGI("Already this Lua file is %s : %s", bEncrypt ? "encrypted" : "decrypted", __sLuaFileName);
				return 0;
			}

			if(!f.Save(__sLuaFileName, bEncrypt)) {
				LOGE("Can't save to Lua file : %s", __sLuaFileName);
				return 1;
			}

			return 0;
		}
	}

	// for Lua arguments
	ArgTable::SetDefaultArguments(argc - arg_size + 1, argv + (arg_size - 1));
	// run script
	{
		Script	sc;

		if(sc.Run(__sLuaFileName))
			iRet	= 0;

		VL::ReleaseAll();
	}
	return iRet;
}
