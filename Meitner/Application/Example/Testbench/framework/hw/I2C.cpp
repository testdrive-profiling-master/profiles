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
#include "I2C.h"
#include <assert.h>

static void _I2C_Wait(double delay_time = 0.00001) 	// 100kHz
{
	static BOOL		bInitialize = FALSE;
	static LONGLONG llQPFTicksPerSec  = 0;
	static LONGLONG	PrevTime;
	LONGLONG		CurTime;

	if(!bInitialize) {
		QueryPerformanceFrequency((LARGE_INTEGER*)&llQPFTicksPerSec);
		bInitialize	= TRUE;
		QueryPerformanceCounter((LARGE_INTEGER*)&PrevTime);
	}

	do {
		QueryPerformanceCounter((LARGE_INTEGER*)&CurTime);
	} while(((double)(CurTime - PrevTime) / llQPFTicksPerSec) < delay_time);

	PrevTime	= CurTime;
}

I2C::I2C(II2C* pI2C)
{
	assert(pI2C != NULL);
	m_pI2C		= pI2C;
	m_bEnable	= FALSE;
}

I2C::~I2C(void)
{
}

void I2C::Reset(void)
{
	if(!m_bEnable) return;

	Set(1, 1, 0);
	_I2C_Wait(0.02);
	Set(1, 1, 1);
	_I2C_Wait(0.02);
}

void I2C::Enable(BOOL bEnable)
{
	m_bEnable	= bEnable;
}

void I2C::Set(int SCL, int SDA, int nRST)
{
	if(!m_pI2C || !m_bEnable) return;

	m_pI2C->I2C_Set(SCL, SDA, nRST);
	_I2C_Wait();
}

int I2C::Get(void)
{
	if(m_pI2C && m_bEnable) {
		return m_pI2C->I2C_Get();
	}

	return 0;
}

void I2C::Start(void)
{
	// start
	Set(1, 1);
	Set(1, 0);
	Set(0, 0);
}

void I2C::Stop(void)
{
	// stop
	Set(0, 0);
	Set(1, 0);
	Set(1, 1);
}

void I2C::RepeatedStart(void)
{
	// repeated start
	Set(0, 0);
	Set(0, 1);
	Set(1, 1);
	Set(1, 0);
	Set(0, 0);
}

BOOL I2C::SendByte(BYTE data)
{
	BOOL bRet = FALSE;

	for(int t = 0; t < 8; t++) {
		int bit = (data & 0x80) ? 1 : 0;
		Set(0, bit);
		Set(1, bit);
		Set(0, bit);
		data <<= 1;
	}

	// ack
	Set(0, 1);
	Set(1, 1);
	bRet	= (Get() == 0);	// ack(0), nack(1)
	Set(0, 1);
	return bRet;
}

BOOL I2C::GetByte(BYTE& data, BOOL bLast)
{
	for(int t = 0; t < 8; t++) {
		data <<= 1;
		Set(0, 1);
		Set(1, 1);
		data |= Get();
		Set(0, 1);
	}

	// ack
	Set(0, bLast);
	Set(1, bLast);
	Set(0, bLast);
	return TRUE;
}

BOOL I2C::Write16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount)
{
	// start
	Start();
	{
		// send device
		//           dev_id       write
		if(!SendByte((DevAddr & 0xFE) | (0))) goto ERROR_OCCURED;
	}
	{
		// send address (16 bit(2byte))
		if(!SendByte((BYTE)(InterAddr >> 8))) goto ERROR_OCCURED;

		if(!SendByte((BYTE)InterAddr)) goto ERROR_OCCURED;
	}
	// repeated start
	RepeatedStart();
	{
		// send device
		//           dev_id       write
		if(!SendByte((DevAddr & 0xFE) | (0))) goto ERROR_OCCURED;
	}
	{
		// send data (16 bit pair)
		for(DWORD i = 0; i < dwCount; i++) {
			if(!SendByte((BYTE)(pData[i] >> 8))) goto ERROR_OCCURED;

			if(!SendByte((BYTE)pData[i])) goto ERROR_OCCURED;
		}
	}
	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2C::Read16(BYTE DevAddr, WORD InterAddr, WORD* pData, DWORD dwCount)
{
	// start
	Start();
	{
		// send device
		//           dev_id       write
		if(!SendByte((DevAddr & 0xFE) | (0))) goto ERROR_OCCURED;
	}
	{
		// send address (16 bit(2byte))
		if(!SendByte((BYTE)(InterAddr >> 8))) goto ERROR_OCCURED;

		if(!SendByte((BYTE)InterAddr)) goto ERROR_OCCURED;
	}
	// repeated start
	RepeatedStart();
	{
		// send device
		//           dev_id       read
		if(!SendByte((DevAddr) | (1))) goto ERROR_OCCURED;
	}
	{
		// read data (16 bit pair)
		for(DWORD i = 0; i < dwCount; i++) {
			BYTE byte_data;

			if(!GetByte(byte_data)) goto ERROR_OCCURED;

			pData[i] = ((WORD)byte_data) << 8;

			if(!GetByte(byte_data, i == (dwCount - 1))) goto ERROR_OCCURED;

			pData[i] |= byte_data;
		}
	}
	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2C::Write8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount)
{
	// start
	Start();
	{
		// send device
		//           dev_id       write
		if(!SendByte((DevAddr & 0xFE) | (0))) goto ERROR_OCCURED;
	}
	{
		// send address (8 bit(1byte))
		if(!SendByte(InterAddr)) goto ERROR_OCCURED;
	}
	{
		// send data (8 bit)
		for(DWORD i = 0; i < dwCount; i++) {
			if(!SendByte((BYTE)pData[i])) goto ERROR_OCCURED;
		}
	}
	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2C::Read8(BYTE DevAddr, BYTE InterAddr, BYTE* pData, DWORD dwCount)
{
	// start
	Start();
	{
		// send device
		//           dev_id       write
		if(!SendByte((DevAddr & 0xFE) | (0))) goto ERROR_OCCURED;
	}
	{
		// send address (8 bit(1byte))
		if(!SendByte(InterAddr)) goto ERROR_OCCURED;
	}
	// repeated start
	RepeatedStart();
	{
		// send device
		//           dev_id       read
		if(!SendByte((DevAddr & 0xFE) | (1))) goto ERROR_OCCURED;
	}
	{
		// read data (8 bit)
		for(DWORD i = 0; i < dwCount; i++) {
			BYTE byte_data;

			if(!GetByte(byte_data, i == (dwCount - 1))) goto ERROR_OCCURED;

			pData[i] = byte_data;
		}
	}
	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2C::Write8Direct(BYTE DevAddr, BYTE Data)
{
	// start
	Start();
	{
		// send device
		//           dev_id       write
		if(!SendByte((DevAddr & 0xFE) | (0))) goto ERROR_OCCURED;
	}
	{
		// send data (8 bit)
		if(!SendByte(Data)) goto ERROR_OCCURED;
	}
	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}

BOOL I2C::Read8Direct(BYTE DevAddr, BYTE* pData)
{
	// start
	Start();
	{
		// send device
		//           dev_id       read
		if(!SendByte((DevAddr & 0xFE) | (1))) goto ERROR_OCCURED;
	}
	{
		// read data (8 bit)
		BYTE byte_data;

		if(!GetByte(byte_data, TRUE)) goto ERROR_OCCURED;

		*pData = byte_data;
	}
	// stop
	Stop();
	return TRUE;
ERROR_OCCURED:
	Stop();
	return FALSE;
}
