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
// Title : License Manager
// Rev.  : 4/7/2021 Wed (clonextop@gmail.com)
//================================================================================
#include "LicenseManager.h"
#include "SEED128_Crypto.h"
#include "SHA256_Hash.h"
#include "zFile.h"
#include <list>
#include <stdarg.h>
#include <time.h>

using namespace std;
using namespace testdrive_license_manager;

typedef enum {
	OP_SET,
	OP_HASH,
	OP_CHECK,
	OP_ENCRYPT,
	OP_DECRYPT,
	OP_NONE		= -1
} OP;

list<string>	__sArgList;

static bool __PatchArgument(int iArgSize, ...)
{
	va_list		ap;

	if(iArgSize != __sArgList.size()) {
		printf("Argument size is not matched.\n");
		return false;
	}

	va_start(ap, iArgSize);

	for(auto i = __sArgList.begin(); i != __sArgList.end(); i++) {
		(*va_arg(ap, string*))	= (*i).c_str();
	}

	va_end(ap);
	return true;
}

static bool __IsAffordableKey(string& s)
{
	for(int i = 0; i < s.size(); i++) {
		char ch = s[i];

		if(!(ch >= 'a' && ch <= 'f') && !(ch >= 'A' && ch <= 'F') && !isdigit(ch))
			return false;
	}

	return true;
}

static void RetrievePublicKey(const char* sKey, DWORD* pPublicKey)
{
	// get public key
	string	s(sKey);
	char	sKeyString[1024];
	strcpy(sKeyString, sKey);

	if(s.size() != 32 || !__IsAffordableKey(s)) {
		SHA256_Hash		hash;
		// put hash
		hash.Push((const BYTE*)s.c_str(), s.size());

		// make 128 bit public code from hash code
		for(int i = 0; i < 4; i++) {
			pPublicKey[i]	= hash.Hash()[i] ^ hash.Hash()[i + 4];
		}
	} else {
		// copy 128 bit public code
		for(int i = 0; i < 4; i++) {
			sscanf(sKeyString + (8 * i), "%08X", &pPublicKey[i]);
		}
	}
}

static bool __EncrpytDecryptFile(bool bEncrypt, const DWORD* pPublicKey, const char* sInputFileName, const char* sOutputFileName)
{
	bool			bOk	= false;
	SEED128_Crypto	cr(pPublicKey);
	SHA256_Hash		hash;

	if(!strcmp(sInputFileName, sOutputFileName)) {
		printf("*E: Input & output file name is identical.\n");
		return false;
	}

	static const DWORD		magic_code	= 0xECDEF1AC;
	zFile			zf;

	if(bEncrypt) {
		//// encryption
		// compression
		if(!zf.Compress(sInputFileName)) {
			printf("*E: Can't open file.");
			exit(1);
		}

		// create new file
		FILE*			fo	= fopen(sOutputFileName, "wb");

		if(!fo) {
			printf("*E: Can't create file : %s\n", sOutputFileName);
			exit(1);
		}

		{
			BYTE	FrameData[64];						// 64 bit frame
			BYTE*	pBuffer		= zf.Buffer();
			size_t	iByteSize	= zf.CompressedSize();
			hash.Randomize();							// randomize initial hash
			fwrite(&magic_code, 4, 1, fo);				// magic code
			fwrite(&zf.DecompressedSize(), 8, 1, fo);	// original size
			fwrite(&zf.CompressedSize(), 8, 1, fo);		// compression size
			fwrite(hash.Hash(), 4, 8, fo);				// initial hash
			fwrite(hash.Hash(), 4, 8, fo);				// dummy last hash codes

			while(iByteSize > 0) {
				int	iReadByteSize	= (iByteSize >= 64) ? 64 : iByteSize;
				memcpy(FrameData, pBuffer, iReadByteSize);

				if(iReadByteSize != 64) {	// fill rest
					for(int i = iReadByteSize; i < 64; i++) {
						FrameData[i]	= rand() & 0xFF;
					}
				}

				hash.PushFrame((const BYTE*)FrameData);
				cr.Encrypt(FrameData, 64);
				fwrite(FrameData, 1, 64, fo);
				pBuffer		+= 64;
				iByteSize	-= iReadByteSize;
			}

			fseek(fo, 4 * 13, SEEK_SET);					// seek final hash
			fwrite(hash.Hash(), 4, 8, fo);				// finalize update hash codes
		}

		fclose(fo);
		bOk	= true;
	} else {
		//// decryption
		FILE*			fi	= fopen(sInputFileName, "rb");

		if(!fi) {
			printf("*E: Can't read file : %s\n", sInputFileName);
			exit(1);
		}

		DWORD dwMagicCode;
		fread(&dwMagicCode, 4, 1, fi);

		if(dwMagicCode != magic_code) {
			printf("*E: It's not a encrypted file.\n");
		} else {
			DWORD	FinalHash[8];
			size_t	iCompressedSize, iDecompressedSize;
			DWORD	dwReadByteSize;
			fread(&iDecompressedSize, 8, 1, fi);		// get file size
			fread(&iCompressedSize, 8, 1, fi);			// get compressed size
			fread((void*)hash.Hash(), 4, 8, fi);		// get initial hash
			fread(FinalHash, 4, 8, fi);					// get final hash
			{
				// read compressed data
				BYTE*	pCompData	= new BYTE[iCompressedSize + 64];
				BYTE*	pFrameData	= pCompData;

				while((dwReadByteSize = fread(pFrameData, 1, 64, fi)) != 0) {
					cr.Decrypt(pFrameData, 64);
					hash.PushFrame(pFrameData);
					pFrameData	+= 64;
				}

				// decompression
				zf.Decompress(pCompData, iCompressedSize, iDecompressedSize);
				delete [] pCompData;
			}

			if(!hash.Compare(FinalHash)) {
				printf("*E: Invalid key code.\n");
			} else {
				FILE*			fo	= fopen(sOutputFileName, "wb");

				if(!fo) {
					printf("*E: Can't create file : %s\n", sOutputFileName);
					exit(1);
				}

				fwrite(zf.Buffer(), 1, zf.DecompressedSize(), fo);
				fclose(fo);
				printf("*I: Done!\n");
				bOk	= true;
			}
		}

		fclose(fi);
	}

	return bOk;
}

int main(int argc, const char* argv[])
{
	OP	op		= OP_NONE;
	srand(time(NULL));

	for(int i = 1; i < argc; i++) {
		const char* sArg	= argv[i];

		if(*sArg == '-') {
			if(strlen(sArg) != 2) {
				goto INVALID_OPTION;
			}

			switch(sArg[1]) {
			case 'd':
				goto INVALID_OPTION;
				break;

			default:
			INVALID_OPTION:
				printf("*E: Invalid option '%s'\n", sArg);
				return 1;
			}
		} else {
			if(op == OP_NONE) {
				if(!strcmp(sArg, "set"))
					op	= OP_SET;
				else if(!strcmp(sArg, "hash"))
					op	= OP_HASH;
				else if(!strcmp(sArg, "check"))
					op	= OP_CHECK;
				else if(!strcmp(sArg, "encrypt"))
					op	= OP_ENCRYPT;
				else if(!strcmp(sArg, "decrypt"))
					op	= OP_DECRYPT;
				else goto INVALID_OPTION;
			} else {
				__sArgList.push_back(sArg);
			}
		}
	}

	if(op == OP_NONE) {
	SHOW_USAGE:
		printf(
			"- TestDrive Profiling Master License Manager. (SEED 128bit + SHA256)\n\n" \
			"Usage : %s [options] OP ...\n" \
			"        [options]\n" \
			"            -p public_key  : specify public key.\n" \
			"        OP\n" \
			"            set license_key\n" \
			"            hash public_key private_string\n" \
			"            check public_key\n" \
			"            encrypt key_code input_files ...\n" \
			"            decrypt key_code input_files ...\n" \
			, argv[0]);
		return 1;
	}

	switch(op) {
	case OP_SET: {
		string	sLicenseKey;

		if(!__PatchArgument(1, &sLicenseKey))
			break;

		if(sLicenseKey.size() != 64 || !__IsAffordableKey(sLicenseKey)) {
			printf("*E: Not a license key : %s.\n", sLicenseKey.c_str());
			return 1;
		}

		FILE* fp = fopen(GetLicensePath(LICENSE_PATH_KEY).c_str(), "a+");

		if(fp) {
			fprintf(fp, "%s\n", sLicenseKey.c_str());
			fclose(fp);
			printf("*I: Done.\n");
		} else {
			printf("*E: Can not create license file.\n");
		}
	}
	break;

	case OP_HASH: {
		string	sPublicKey, sPrivateString;

		if(!__PatchArgument(2, &sPublicKey, &sPrivateString))
			break;

		DWORD	dwPublicKey[4];
		DWORD	dwHash[8];
		RetrievePublicKey(sPublicKey.c_str(), dwPublicKey);	// get public key
		GetHash(dwHash, dwPublicKey, sPrivateString.c_str());
		printf("\nSHA256 Hash value : ");

		for(int i = 0; i < 8; i++) {
			printf("%08X", dwHash[i]);
		}

		printf("\n");
	}
	break;

	case OP_CHECK: {
		string	sPublicKey;

		if(!__PatchArgument(1, &sPublicKey))
			break;

		DWORD	dwPublicKey[4];
		RetrievePublicKey(sPublicKey.c_str(), dwPublicKey);	// get public key
		LicenseCheck	l(dwPublicKey);
		printf("*I: License is ok!\n");
	}
	break;

	case OP_ENCRYPT:
	case OP_DECRYPT: {
		string	sPublicKey, sInputFile, sOutputFile;

		if(!__sArgList.size()) {
			printf("*E: No input files.\n");
			exit(1);
		}

		{
			for(auto i = __sArgList.begin(); i != __sArgList.end(); i++) {
				string sInputFile	= *i;
				string sOutputFile	= *i;

				if(op == OP_ENCRYPT) {
					sOutputFile		+= ".encrypted";
					printf("* Encrypting... '%s'\n", sInputFile.c_str());
				} else {
					int iPos	= sOutputFile.rfind(".encrypted");

					if(iPos <= 0 || iPos != (sOutputFile.size() - 10)) {
						sOutputFile		+= ".decrypted";
					} else {
						sOutputFile.erase(iPos, -1);
					}
					printf("* Decrypting... '%s'\n", sOutputFile.c_str());
				}

				__EncrpytDecryptFile((op == OP_ENCRYPT), NULL, sInputFile.c_str(), sOutputFile.c_str());
			}
		}
	}
	break;
	}

	return 0;
}
