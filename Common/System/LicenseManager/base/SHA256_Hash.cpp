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
#include "SHA256_Hash.h"

#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

SHA256_Hash::SHA256_Hash(const DWORD* pInitHash)
{
	static const DWORD __InitialHash[8] = {
		0x6a09e667,
		0xbb67ae85,
		0x3c6ef372,
		0xa54ff53a,
		0x510e527f,
		0x9b05688c,
		0x1f83d9ab,
		0x5be0cd19
	};

	if(!pInitHash) pInitHash = __InitialHash;

	memcpy(m_Hash, pInitHash, sizeof(m_Hash));
	m_DataLen	= 0;
}

SHA256_Hash::~SHA256_Hash(void)
{
}

void SHA256_Hash::Randomize(void)
{
	WORD*	pHash	= (WORD*)m_Hash;

	for(int i = 0; i < 8 * 2; i++) {
	RETRY_RANDOMIZE:
		pHash[i]	= rand();

		if(!pHash[i]) goto RETRY_RANDOMIZE;
	}
}

bool SHA256_Hash::Compare(const DWORD* pCmpHash)
{
	for(int i = 0; i < 8; i++) {
		if(pCmpHash[i] != m_Hash[i])
			return false;
	}

	return true;
}

void SHA256_Hash::Push(const BYTE* pData, DWORD dwByteSize)
{
	DWORD i;

	while(dwByteSize >= 64) {
		dwByteSize		-= 64;
		PushFrame(pData);
		pData		+= 64;
	}

	if(dwByteSize) {
		BYTE temp[64];
		memcpy(temp, pData, dwByteSize);
		memset(temp + dwByteSize, 0xDE, 64 - dwByteSize);
		PushFrame(temp);
	}

	m_DataLen	+= dwByteSize;
}

void SHA256_Hash::PushFrame(const BYTE* pData)
{
	static const DWORD k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	DWORD a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for(i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (pData[j] << 24) | (pData[j + 1] << 16) | (pData[j + 2] << 8) | (pData[j + 3]);

	for(; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = m_Hash[0];
	b = m_Hash[1];
	c = m_Hash[2];
	d = m_Hash[3];
	e = m_Hash[4];
	f = m_Hash[5];
	g = m_Hash[6];
	h = m_Hash[7];

	for(i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	m_Hash[0] += a;
	m_Hash[1] += b;
	m_Hash[2] += c;
	m_Hash[3] += d;
	m_Hash[4] += e;
	m_Hash[5] += f;
	m_Hash[6] += g;
	m_Hash[7] += h;
}
