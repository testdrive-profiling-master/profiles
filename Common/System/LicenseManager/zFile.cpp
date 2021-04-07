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
// Title : License Register
// Rev.  : 4/6/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "zFile.h"
#include <zlib.h>

zFile::zFile(void){
	m_iCompressedSize		= 0;
	m_iDecompressedSize		= 0;
	m_pData					= NULL;
}

zFile::~zFile(void){
	Release();
}

void zFile::Release(void){
	if(m_pData) {
		delete [] m_pData;
		m_pData	= NULL;
	}
	m_iCompressedSize		= 0;
	m_iDecompressedSize		= 0;
}

bool zFile::Compress(const char* sFileName)
{
	bool	bOk	= false;
	Release();
	FILE* fp	= fopen(sFileName, "rb");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		m_iDecompressedSize	= ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if(m_iDecompressedSize && m_iDecompressedSize < (1024*1024*1024*2)) {
			uLongf iDestByteSize	= m_iDecompressedSize * 2 + 16;
			BYTE*	pFileData		= new BYTE[m_iDecompressedSize];
			m_pData					= new BYTE[iDestByteSize];
			fread(pFileData, 1, m_iDecompressedSize, fp);
			compress2(m_pData, &iDestByteSize, pFileData, m_iDecompressedSize, Z_BEST_COMPRESSION);
			m_iCompressedSize		= iDestByteSize;
			delete [] pFileData;
			bOk	= true;

		}
		fclose(fp);
	}

	return bOk;
}

bool zFile::Decompress(BYTE* pCompressedBuffer, size_t iCompressedSize, size_t iDecompressedSize){
	Release();
	unsigned long int	iDecompressedByteSize	= iDecompressedSize;
	m_pData				= new BYTE[iDecompressedSize];
	m_iCompressedSize	= iCompressedSize;
	uncompress((unsigned char*)m_pData, &iDecompressedByteSize, (unsigned char*)pCompressedBuffer, iCompressedSize);
	m_iDecompressedSize	= iDecompressedSize;
	if(iDecompressedByteSize != iDecompressedSize)
		return false;
	return true;
}
