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
// Title : Testbench
// Rev.  : 10/15/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "GeSystemMessage.h"

CGeSystemMessage::CGeSystemMessage(void)
{
	int i;

	for(i = 0; i < SYSTEM_MESSAGE_SIZE; i++) {
		m_sMsg[i] = NULL;
		m_dwTime[i] = 0;
	}
}

CGeSystemMessage::~CGeSystemMessage(void)
{
	ClearAll();
}

void CGeSystemMessage::UpdateTime(DWORD milli_sec)
{
	int i, Index;
	BOOL bEmpty = FALSE;

	for(i = 0; i < SYSTEM_MESSAGE_SIZE; i++) {
		if(m_dwTime[i] < milli_sec) {
			m_dwTime[i] = 0;
			SAFE_DELETE_ARRAY(m_sMsg[i]);

			if(!bEmpty) Index = i;

			bEmpty = TRUE;
		} else {
			m_dwTime[i] -= milli_sec;

			if(bEmpty) {
				m_dwTime[Index] = m_dwTime[i];
				m_sMsg[Index] = m_sMsg[i];
				m_dwColor[Index] = m_dwColor[i];
				m_dwTime[i] = 0;
				m_sMsg[i] = NULL;
				Index++;
			}
		}
	}
}

void CGeSystemMessage::ClearAll(void)
{
	int i;

	for(i = 0; i < SYSTEM_MESSAGE_SIZE; i++) SAFE_DELETE_ARRAY(m_sMsg[i]);
}

void CGeSystemMessage::SetMessage(const char* msg, DWORD color, DWORD showtime_milli_sec)
{
	int i;

	if(!msg) return;

	for(i = 0; i < SYSTEM_MESSAGE_SIZE; i++) if(m_sMsg[i] == NULL) break;

	if(i == SYSTEM_MESSAGE_SIZE) {
		SAFE_DELETE_ARRAY(m_sMsg[0]);

		for(i = 0; i < SYSTEM_MESSAGE_SIZE - 1; i++) {
			m_sMsg[i] = m_sMsg[i + 1];
			m_dwTime[i] = m_dwTime[i + 1];
			m_dwColor[i] = m_dwColor[i + 1];
		}
	}

	m_sMsg[i] = new char[strlen(msg) + 1];
	m_dwTime[i] = showtime_milli_sec;
	m_dwColor[i] = color;
	strcpy(m_sMsg[i], msg);
}

char* CGeSystemMessage::GetMessage(DWORD index)
{
	if(index >= SYSTEM_MESSAGE_SIZE) return NULL;

	return m_sMsg[index];
}

DWORD CGeSystemMessage::GetColor(DWORD index)
{
	DWORD color = m_dwColor[index];

	if(m_dwTime[index] < 256) color |= (m_dwTime[index] << 24);
	else color |= ((DWORD)0xFF << 24);

	return color;
}

DWORD CGeSystemMessage::GetSize(void)
{
	DWORD i;

	for(i = 0; i < SYSTEM_MESSAGE_SIZE; i++) if(!m_sMsg[i]) break;

	return i;
}
