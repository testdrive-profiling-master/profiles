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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __I2C_H__
#define __I2C_H__
#include "STDInterface.h"

interface II2C{
public:
	STDMETHOD_(void, I2C_Set)(int SCL, int SDA, int nRST) PURE;		// set scl/sda/rst_n
	STDMETHOD_(int, I2C_Get)(void) PURE;								// get sda
};

class I2C{
public:
	I2C(II2C* pI2C);
	virtual ~I2C(void);

	void Reset(void);
	void Enable(BOOL bEnable = TRUE);
	inline BOOL IsEnable(void)		{return m_bEnable;}

	BOOL Write16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount = 1);
	BOOL Read16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount = 1);
	BOOL Write8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount = 1);
	BOOL Read8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount = 1);
	BOOL Write8Direct(BYTE DevAddr, BYTE Data);
	BOOL Read8Direct(BYTE DevAddr, BYTE* pData);

	inline BOOL Set16(BYTE DevAddr, WORD InterAddr, WORD Data)		{return Write16(DevAddr, InterAddr, &Data);}
	inline BOOL Set8(BYTE DevAddr, BYTE InterAddr, BYTE Data)		{return Write8(DevAddr, InterAddr, &Data);}
	inline BYTE Get8(BYTE DevAddr, BYTE InterAddr)					{BYTE data;Read8(DevAddr, InterAddr, &data);return data;}

protected:
	void Set(int SCL, int SDA, int nRST = 1);
	int Get(void);

private:
	void Start(void);
	void RepeatedStart(void);
	void Stop(void);
	BOOL SendByte(BYTE data);
	BOOL GetByte(BYTE& data, BOOL bLast = FALSE);

	II2C*		m_pI2C;
	BOOL		m_bEnable;
};

#endif//__I2C_H__
