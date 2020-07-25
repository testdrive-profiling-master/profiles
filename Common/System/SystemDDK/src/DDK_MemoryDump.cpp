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
// Title : Common profiles
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "DDK_Context.h"
#include <zlib.h>

#define DUMP_MEMORY_MAGIC_CODE		0x504D5544

typedef struct {
	DWORD	dwMagicCode;	// "DUMP" (0x504D5544)
	int		iSize;
} MEMORY_DUMP_HEADER;

typedef struct {
	FILE*	fp;
	DDK*	pDDK;
	MEMORY_DUMP_HEADER	header;
} MEMORY_DUMP;

typedef struct {
	DWORD	dwPhyAddress;
	DWORD	dwByteSize;
	DWORD	dwCompressedSize;
} MEMORY_DUMP_DESC;

static const char* __sDefaultDumpMemoryFileName	= "memory.dump";

static void __EnumMemory(DDKMemory* pMemory, MEMORY_DUMP* pDump)
{
	MEMORY_DUMP_DESC desc;
	desc.dwPhyAddress		= pMemory->Physical();
	desc.dwByteSize			= pMemory->ByteSize();
	desc.dwCompressedSize	= 0;
	{
		unsigned long int	zipSize = desc.dwByteSize + 16;
		unsigned char* pSrc	= (unsigned char*)pDump->pDDK->GetMemoryPointer(desc.dwPhyAddress, desc.dwByteSize);
		BYTE* pData			= new BYTE[zipSize];
		compress((unsigned char*)pData, &zipSize, pSrc, pMemory->ByteSize());
		desc.dwCompressedSize	= zipSize;
		fwrite(&desc, sizeof(desc), 1, pDump->fp);
		fwrite(pData, desc.dwCompressedSize, 1, pDump->fp);
		delete [] pData;
		pDump->header.iSize++;
	}
}

BOOL DDKContext::MakeMemoryDump(const char* sFileName)
{
	MEMORY_DUMP	dump;
	memset(&dump, 0, sizeof(MEMORY_DUMP));
	dump.pDDK				= this;
	dump.header.dwMagicCode	= DUMP_MEMORY_MAGIC_CODE;

	if(!sFileName) sFileName = __sDefaultDumpMemoryFileName;

	if((dump.fp	= fopen(sFileName, "wb"))) {
		fwrite(&dump.header, sizeof(dump.header), 1 , dump.fp);
		EnumerateDDKMemory((ENUMERATE_DDK_MEMORY_FUNCTION)__EnumMemory, (void*)&dump);
		fseek(dump.fp, 0, SEEK_SET);
		fwrite(&dump.header, sizeof(dump.header), 1 , dump.fp);
		fclose(dump.fp);
		return TRUE;
	}

	return FALSE;
}

BOOL DDKContext::LoadMemoryDump(const char* sFileName)
{
	MEMORY_DUMP	dump;
	memset(&dump, 0, sizeof(MEMORY_DUMP));
	dump.pDDK		= this;

	if(!sFileName) sFileName = __sDefaultDumpMemoryFileName;

	if((dump.fp	= fopen(sFileName, "rb"))) {
		fread(&dump.header, sizeof(dump.header), 1, dump.fp);

		// check magic code
		if(dump.header.dwMagicCode != DUMP_MEMORY_MAGIC_CODE) {
			fclose(dump.fp);
			return FALSE;
		}

		printf("iSize = %d\n", dump.header.iSize);

		// store to memory
		for(int i = 0; i < dump.header.iSize; i++) {
			MEMORY_DUMP_DESC	desc;
			fread(&desc, sizeof(desc), 1, dump.fp);
			void* pMem = GetMemoryPointer(desc.dwPhyAddress, desc.dwByteSize);

			if(!pMem) {
				fclose(dump.fp);
				printf("*Can't find memory space : 0x%08(%d bytes)\n", desc.dwPhyAddress, desc.dwByteSize);
				return FALSE;
			}

			if(!desc.dwCompressedSize) {
				fread(pMem, desc.dwByteSize, 1, dump.fp);
			} else {
				unsigned long int	zipSize = desc.dwByteSize;
				BYTE* pData = new BYTE[desc.dwCompressedSize];
				fread(pData, desc.dwCompressedSize, 1, dump.fp);
				uncompress((unsigned char*)pMem, &zipSize, pData, desc.dwCompressedSize);
				delete [] pData;
			}
		}

		fclose(dump.fp);
		return TRUE;
	}

	if(dump.fp)
		fclose(dump.fp);

	return FALSE;
}
