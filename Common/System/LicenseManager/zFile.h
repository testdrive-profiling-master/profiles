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
// Title : License Manager
// Rev.  : 12/15/2021 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __Z_FILE_H__
#define __Z_FILE_H__
#include "TestDrive_LM.h"

class zFile {
public:
	zFile(void);
	virtual ~zFile(void);

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

#endif//__Z_FILE_H__
