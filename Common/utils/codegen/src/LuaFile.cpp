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
// Rev.  : 7/31/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "LuaFile.h"

#define LUA_FILE_MAGIC_CODE 0xFF41554C

typedef struct {
	DWORD dwMagicCode;
	DWORD dwCompressedByteSize;
	DWORD initial_hash[8];
	DWORD matching_hash[8];
} LUA_FILE_HEADER;

string LuaFile::m_sKeyCode;

class SEED128_Lua : public SEED128_Crypto
{
public:
	SEED128_Lua(SHA256_Hash *pInitHash, string sSeedKey)
	{
		SHA256_Hash mod_hash;
		DWORD		dwKey[4];
		mod_hash.Initialize(pInitHash->Hash());
		mod_hash.Push((const BYTE *)sSeedKey.c_str(), sSeedKey.size());

		// seed key
		for (int i = 0; i < 4; i++) dwKey[i] = mod_hash.Hash()[i] ^ mod_hash.Hash()[i + 4];

		SetupKey(dwKey);
	}
	virtual ~SEED128_Lua(void) {}
};

LuaFile::LuaFile(void)
{
	m_pBuff			= NULL;
	m_dwSize		= 0;
	m_bWasEncrypted = false;
}

LuaFile::~LuaFile(void)
{
	Close();
}

bool LuaFile::Load(const char *sFileName)
{
	bool bRet = false;
	Close();
	FILE *fp = fopen(sFileName, "rb");

	if (fp) {
		fseek(fp, 0, SEEK_END);
		m_dwSize = ftell(fp);

		if (m_dwSize) {
			BYTE *pBuff = new BYTE[m_dwSize];
			fseek(fp, 0, SEEK_SET);
			fread(pBuff, 1, m_dwSize, fp);
			LUA_FILE_HEADER *pHeader = (LUA_FILE_HEADER *)pBuff;

			if (m_dwSize > sizeof(LUA_FILE_HEADER) && pHeader->dwMagicCode == LUA_FILE_MAGIC_CODE) {
				SHA256_Hash hash(pHeader->initial_hash);
				SEED128_Lua seed(&hash, m_sKeyCode);
				BYTE	   *encrypted_pBuff = pBuff + sizeof(LUA_FILE_HEADER);
				size_t		encrypt_dwSize	= m_dwSize - sizeof(LUA_FILE_HEADER);
				seed.Decrypt(encrypted_pBuff, encrypt_dwSize);
				hash.Push(encrypted_pBuff, encrypt_dwSize);
				m_bWasEncrypted = true;

				if (hash.Compare(pHeader->matching_hash)) {
					size_t	dwCompressedSize   = ((DWORD *)encrypted_pBuff)[0];
					size_t	dwDecompressedSize = ((DWORD *)encrypted_pBuff)[1];
					ZipFile zf;

					if (zf.Decompress(encrypted_pBuff + 8, dwCompressedSize, dwDecompressedSize)) {
						m_dwSize = zf.DecompressedSize();
						m_pBuff	 = new BYTE[m_dwSize];
						memcpy(m_pBuff, zf.Buffer(), m_dwSize);
						bRet = true;
					} else {
						LOGE("Encrypted Lua file '%s' is corrupted.", sFileName);
						Close();
					}
				} else {
					LOGE("Need a correct KeyCode.");
					Close();
				}

				delete[] pBuff;
			} else {
				m_pBuff = pBuff;
				bRet	= true;
			}
		} else { // empty but must be ok.
			bRet = true;
		}

		fclose(fp);
	}

	return bRet;
}

bool LuaFile::Save(const char *sFileName, bool bEncrypt)
{
	if (m_pBuff) {
		FILE *fp = fopen(sFileName, "wb");

		if (!fp)
			return false;

		if (bEncrypt) { // do encrypt
			ZipFile zf;

			if (zf.Compress(m_pBuff, m_dwSize)) {
				LUA_FILE_HEADER header;
				SHA256_Hash		hash;
				hash.Randomize();
				SEED128_Lua seed(&hash, m_sKeyCode);
				header.dwMagicCode = LUA_FILE_MAGIC_CODE;
				memcpy(header.initial_hash, hash.Hash(), sizeof(DWORD) * 8);
				header.dwCompressedByteSize = zf.CompressedSize();
				{
					// encrypt buffer
					size_t encrypt_size	 = ((size_t)header.dwCompressedByteSize + 8 + 15) & (~(size_t)15); // align to 16
					BYTE  *pEncyptedBuff = new BYTE[encrypt_size];
					memset(&pEncyptedBuff[encrypt_size - 16], 0xED, 16);		 // dummy tail reset
					((DWORD *)pEncyptedBuff)[0] = zf.CompressedSize();			 // compressed size
					((DWORD *)pEncyptedBuff)[1] = zf.DecompressedSize();		 // decompressed size
					memcpy(pEncyptedBuff + 8, zf.Buffer(), zf.CompressedSize()); // dump original
					hash.Push(pEncyptedBuff, encrypt_size);						 // get matching hash
					memcpy(header.matching_hash, hash.Hash(), sizeof(DWORD) * 8);
					seed.Encrypt(pEncyptedBuff, encrypt_size); // do encrypt
					// write down
					fwrite(&header, 1, sizeof(LUA_FILE_HEADER), fp);
					fwrite(pEncyptedBuff, 1, encrypt_size, fp);
				}
			}
		} else {
			// decrypted
			fwrite(m_pBuff, 1, m_dwSize, fp);
		}

		fclose(fp);
		return true;
	}

	return false;
}

void LuaFile::Close(void)
{
	SAFE_DELETE_ARRAY(m_pBuff);
	m_dwSize		= 0;
	m_bWasEncrypted = false;
}

void LuaFile::SetKeyCode(const char *sKeyCode)
{
	if (sKeyCode)
		m_sKeyCode = sKeyCode;
}

const char *LuaFile::GetKeyCode(void)
{
	return m_sKeyCode.c_str();
}
