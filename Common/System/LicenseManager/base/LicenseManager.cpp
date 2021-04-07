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
#include "LicenseManager.h"
#include "SEED128_Crypto.h"
#include "SHA256_Hash.h"
#ifndef WIN32
#include <pwd.h>        // getpwuid
#endif

#define	LICENSE_FLAG_FILE		".testdrive.lic.flag"
#define	LICENSE_KEY_FILE		".testdrive.lic"

const DWORD	__DefaultPublicKey[4]		= {0x254302EA, 0xDEAD3EDA, 0x2D115413, 0xA00D48};

namespace testdrive_license_manager {
static string __timeToString(struct tm* t)
{
	char s[1024];
	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d",
			t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			t->tm_hour, t->tm_min, t->tm_sec
		   );
	return s;
}

string GetLicensePath(LICENSE_PATH id)
{
	string	sPath;
#ifdef WIN32
	{
		const char* sTD_Path	= getenv("TESTDRIVE_DIR");

		if(sTD_Path) {
			sPath				= sTD_Path;
		} else {
			printf("*E: 'TestDrive Profiling Master' environment is not found.\n");
			exit(1);
		}
	}
#else
	{
		struct passwd* pwuid	= getpwuid(getuid());
		sPath	= pwuid->pw_dir;
		sPath	+= "/";
	}
#endif

	switch(id) {
	case LICENSE_PATH_TAG:
		sPath	+= LICENSE_FLAG_FILE;
		break;

	case LICENSE_PATH_KEY:
		sPath	+= LICENSE_KEY_FILE;
		break;
	}

	return sPath;
}

static string GetLicenseTag(void)
{
	string	sPath = GetLicensePath(LICENSE_PATH_TAG), sLicense;

	//---------------------------------------------------
	// from license tag
	if(access(sPath.c_str(), 0)) {	// if not exist, make new tag
		remove(GetLicensePath(LICENSE_PATH_KEY).c_str());	// delete previous license key file
		FILE* fp  = fopen(sPath.c_str(), "wt");

		if(fp) {
			fprintf(fp,
					"Do not edit or copy this file!!!\n" \
					"Otherwise, all licenses codes will be discarded.\n"
				   );
			fclose(fp);
		} else {
			goto LICENSE_FILE_ACCESS_NOT_GRANTED;
		}
	}

	struct stat buf;

	if(stat(sPath.c_str(), &buf) != 0) {
	LICENSE_FILE_ACCESS_NOT_GRANTED:
		// no file
		printf("*E: License file access is not granted.\n");
		exit(1);
	} else {
		sLicense	+= __timeToString(localtime(&buf.st_ctime)).c_str();
		sLicense	+= ":";
		sLicense	+= __timeToString(localtime(&buf.st_mtime)).c_str();
	}

	//---------------------------------------------------
	// computer name
	{
		char	sName[256];
#ifdef WIN32
		DWORD	dwNameSize	= 256;
		GetComputerName(sName, &dwNameSize);
#else
		gethostname(sName, 256);
#endif
		sLicense	+= ":";
		sLicense	+= sName;
	}
	//---------------------------------------------------
	// user name
	{
		char	sName[256];
#ifdef WIN32
		DWORD	dwNameSize	= 256;
		GetUserName(sName, &dwNameSize);
#else
		{
			struct passwd* pwuid	= getpwuid(getuid());
			strcpy(sName, pwuid->pw_name);
		}
#endif
		sLicense	+= ":";
		sLicense	+= sName;
	}
	return sLicense;
}

static bool CheckLicenseKey(const DWORD* pHash)
{
	bool	bOk		= false;
	DWORD	dwKey;
	FILE* fp = fopen(GetLicensePath(LICENSE_PATH_KEY).c_str(), "rt");

	if(fp) {
		char sKey[1024];

		while(fgets(sKey, 1024, fp)) {
			for(int i = 0; i < 8; i++) {
				sscanf(sKey + (i * 8), "%08X", &dwKey);

				if(dwKey != pHash[i])
					break;
				else if(i == 7)
					bOk	= true;
			}
		}

		fclose(fp);
	}

	return bOk;
}

bool GetHash(DWORD* pHash, const DWORD* pPublicKey, const char* sPrivateString)
{
	if(!sPrivateString || !pPublicKey || !pHash) return false;

	{
		DWORD	dwByteSize	= strlen(sPrivateString) / 2;
		BYTE*	pData		= new BYTE[dwByteSize];

		for(int i = 0; i < dwByteSize; i++) {
			DWORD	dwData;
			sscanf(sPrivateString + (i * 2), "%02X", &dwData);
			pData[i]	= dwData;
		}

		{
			// do decryption
			SEED128_Crypto cr(pPublicKey);
			cr.Decrypt(pData, dwByteSize);
		}

		{
			// get SHA256 hash
			SHA256_Hash	h;
			h.Push(pData, dwByteSize);

			for(int i = 0; i < 8; i++) {
				pHash[i]	= h.Hash()[i];
			}
		}

		delete [] pData;
	}
	return true;
}

bool	g_bCustomized	= false;
bool	g_bChecked		= false;

LicenseCheck::LicenseCheck(const DWORD* pPublicKey)
{
	string	sLicense	= GetLicenseTag();

	if(!pPublicKey) pPublicKey = __DefaultPublicKey;

	{
		DWORD	dwByteSize	= (sLicense.length() + 3) & (~3);
		BYTE*	pBuff		= new BYTE[dwByteSize];
		DWORD	dwKey[8];
		SHA256_Hash		hash;
		memset(pBuff, 0xDE, dwByteSize);
		strcpy((char*)pBuff, sLicense.c_str());
		hash.Push(pBuff, dwByteSize);

		if(!CheckLicenseKey(hash.Hash())) {
			printf(
				"*E: No license key. Contact your license manager with following your private key code.\n"
			);
			{
				SEED128_Crypto cr(pPublicKey);
				cr.Encrypt(pBuff, dwByteSize);
				printf("\n    - ");

				for(DWORD i = 0; i < dwByteSize; i++) {
					printf("%02X", pBuff[i]);
				}

				printf("\n");
			}
			if(!g_bCustomized){
				exit(1);
			}
		} else {
			g_bChecked	= true;
		}

		delete [] pBuff;
	}
}

LicenseCheck::~LicenseCheck(void)
{
}
};
