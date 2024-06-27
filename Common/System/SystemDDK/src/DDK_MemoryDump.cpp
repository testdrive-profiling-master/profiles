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
// Title : Common profiles
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "DDK_Context.h"
#include <zlib.h>

#define DUMP_MEMORY_MAGIC_CODE 'TDMP'

typedef struct {
	uint32_t dwMagicCode; // must be DUMP_MEMORY_MAGIC_CODE
	int		 dwSize;	  // header size
} MEMORY_DUMP_HEADER;

typedef struct {
	FILE			  *fp;
	MEMORY_DUMP_HEADER header;
} MEMORY_DUMP;

typedef struct {
	uint64_t ulPhyAddress;
	uint32_t dwByteSize;
	uint32_t dwCompressedSize;
} MEMORY_DUMP_DESC;

static const char *__sDefaultDumpMemoryFileName = "memory.dump";

static void		   __EnumMemory(DDKMemory *pMemory, MEMORY_DUMP *pDump)
{
	MEMORY_DUMP_DESC desc;
	desc.ulPhyAddress	  = pMemory->Physical();
	desc.dwByteSize		  = pMemory->ByteSize();
	desc.dwCompressedSize = 0;
	pMemory->Flush(false); // read from system
	{
		unsigned long int zipSize = desc.dwByteSize + 16;
		unsigned char	 *pSrc	  = (unsigned char *)pMemory->Virtual();
		BYTE			 *pData	  = new BYTE[zipSize];
		compress((unsigned char *)pData, &zipSize, pSrc, pMemory->ByteSize());
		desc.dwCompressedSize = zipSize;
		fwrite(&desc, sizeof(desc), 1, pDump->fp);
		fwrite(pData, desc.dwCompressedSize, 1, pDump->fp);
		delete[] pData;
		pDump->header.dwSize++;
	}
}

bool DDKContext::MakeMemoryDump(const char *sFileName)
{
	MEMORY_DUMP dump;
	memset(&dump, 0, sizeof(MEMORY_DUMP));
	dump.header.dwMagicCode = DUMP_MEMORY_MAGIC_CODE;

	if (!sFileName)
		sFileName = __sDefaultDumpMemoryFileName;

	if ((dump.fp = fopen(sFileName, "wb"))) {
		fwrite(&dump.header, sizeof(dump.header), 1, dump.fp);
		EnumerateDDKMemory((ENUMERATE_DDK_MEMORY_FUNCTION)__EnumMemory, (void *)&dump);
		fseek(dump.fp, 0, SEEK_SET);
		fwrite(&dump.header, sizeof(dump.header), 1, dump.fp);
		fclose(dump.fp);
		return true;
	}

	return false;
}

bool DDKContext::LoadMemoryDump(const char *sFileName)
{
	MEMORY_DUMP dump;
	memset(&dump, 0, sizeof(MEMORY_DUMP));

	if (!sFileName)
		sFileName = __sDefaultDumpMemoryFileName;

	if ((dump.fp = fopen(sFileName, "rb"))) {
		fread(&dump.header, sizeof(dump.header), 1, dump.fp);

		// check magic code
		if (dump.header.dwMagicCode != DUMP_MEMORY_MAGIC_CODE) {
			fclose(dump.fp);
			return false;
		}

		// store to memory
		for (int i = 0; i < dump.header.dwSize; i++) {
			MEMORY_DUMP_DESC desc;
			fread(&desc, sizeof(desc), 1, dump.fp);
			IMemory *pMem = CreateMemory(desc.dwByteSize, 1, desc.ulPhyAddress);

			if (!pMem) {
				fclose(dump.fp);
				printf("*Can't create memory space : 0x%08llX(%d bytes)\n", desc.ulPhyAddress, desc.dwByteSize);
				return false;
			}

			if (!desc.dwCompressedSize) {
				fread(pMem->Virtual(), desc.dwByteSize, 1, dump.fp);
			} else {
				unsigned long int zipSize = desc.dwByteSize;
				BYTE			 *pData	  = new BYTE[desc.dwCompressedSize];
				fread(pData, desc.dwCompressedSize, 1, dump.fp);
				uncompress((unsigned char *)pMem->Virtual(), &zipSize, pData, desc.dwCompressedSize);
				delete[] pData;
			}

			pMem->Flush(true);
			SAFE_RELEASE(pMem);
		}

		fclose(dump.fp);
		return true;
	}

	return false;
}
