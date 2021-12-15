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
// Title : Common profiles
// Rev.  : 12/15/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __TESTDRIVE_LICENSE_MANAGER_H__
#define __TESTDRIVE_LICENSE_MANAGER_H__
#include "STDInterface.h"
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

namespace testdrive_license_manager {
class LicenseCheck {
public:
	LicenseCheck(const DWORD* pPublicKey);
	~LicenseCheck(void);
};
};

class ZipFile {
public:
	ZipFile(void);
	virtual ~ZipFile(void);

	void Release(void);

	bool Compress(const char* sFileName);
	bool Compress(BYTE* pBuffer, size_t dwByteSize);
	bool Decompress(BYTE* pCompressedBuffer, size_t iCompressedSize, size_t iDecompressedSize);

	inline size_t& DecompressedSize(void)	{
		return m_dwDecompressedSize;
	}
	inline size_t& CompressedSize(void)		{
		return m_dwCompressedSize;
	}
	inline BYTE* Buffer(void)				{
		return m_pData;
	}

private:
	BYTE*		m_pData;
	size_t		m_dwCompressedSize;
	size_t		m_dwDecompressedSize;
};

class SEED128_Crypto {
public:
	SEED128_Crypto(const DWORD* pKey = NULL);
	virtual ~SEED128_Crypto(void);

	void SetupKey(const DWORD* pKey);				// 4 DWORD input
	void Encrypt(BYTE* pData, DWORD dwByteSize);	// size alignment is 16 bytes
	void Decrypt(BYTE* pData, DWORD dwByteSize);	// size alignment is 16 bytes
	void EncryptFrame(DWORD* pData);
	void DecryptFrame(DWORD* pData);

private:
	DWORD		m_dwRoundKey[32];
};

class SHA256_Hash {	// fastest custom SHA256 hash
public:
	SHA256_Hash(const DWORD* pInitHash = NULL);
	~SHA256_Hash(void);

	void Initialize(const DWORD* pInitHash = NULL);
	void Push(const BYTE* pData, DWORD dwByteSize);
	void PushFrame(const BYTE* pData);
	const DWORD* Hash(void)	{
		return m_Hash;
	}
	void Randomize(void);
	bool Compare(const DWORD* pCmpHash);

private:
	DWORD					m_Hash[8];
};

#endif// __TESTDRIVE_LICENSE_MANAGER_H__
