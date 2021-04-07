//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common profiles
// Rev.  : 4/7/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __TESTDRIVE_LICENSE_MANAGER_H__
#define __TESTDRIVE_LICENSE_MANAGER_H__
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <memory.h>
#include <assert.h>

using namespace std;

#ifdef WIN32
#include <windows.h>
#else
typedef unsigned char			BYTE;
typedef unsigned short			WORD;
typedef unsigned int			DWORD;
#endif

namespace testdrive_license_manager {
	typedef enum {
		LICENSE_PATH_TAG,
		LICENSE_PATH_KEY,
	} LICENSE_PATH;

	string GetLicensePath(LICENSE_PATH id);
	bool GetHash(DWORD* pHash, const DWORD* pPublicKey, const char* sPrivateString);	// 8 DWORD hash, 4 DWORD public key

	class LicenseCheck {
	public:
		LicenseCheck(const DWORD* pPublicKey);
		~LicenseCheck(void);
	};
};

#endif// __TESTDRIVE_LICENSE_MANAGER_H__
