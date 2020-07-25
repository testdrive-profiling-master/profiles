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
// Title : Common DPI
// Rev.  : 11/5/2019 Tue (clonextop@gmail.com)
//================================================================================
#ifndef __ROUND_QUEUE_H__
#define __ROUND_QUEUE_H__
#include "STDInterface.h"

template<typename T, int queue_size>
class RoundQueue
{
	T*		m_pQ;
	T		m_Pop;
	DWORD	m_dwTotalCount;
	DWORD	m_dwHead;
	DWORD	m_dwTail;
	DWORD	m_dwRestCount;
public:
	RoundQueue(void){
		assert(queue_size>0);
		m_pQ			= new T[queue_size];
		m_dwTotalCount	=
		m_dwRestCount	= queue_size;
		m_dwHead		=
		m_dwTail		= 0;
	}

	~RoundQueue(void){
		if(m_pQ){
			delete [] m_pQ;
			m_pQ	= NULL;
		}
	}

	T* Push(void){
		if(IsFull()) return NULL;
		T*	pData = &m_pQ[m_dwHead];
		m_dwRestCount--;
		m_dwHead = (m_dwHead + 1) % m_dwTotalCount;
		return pData;
	}

	T* Pop(void){
		if(IsEmpty()) return NULL;
		memcpy(&m_Pop, &m_pQ[m_dwTail], sizeof(T));
		m_dwRestCount++;
		m_dwTail = (m_dwTail + 1) % m_dwTotalCount;
		return &m_Pop;
	}

	inline BOOL IsEmpty(void)	{return m_dwRestCount == m_dwTotalCount;}
	inline BOOL IsFull(void)	{return !m_dwRestCount;}
};

#endif//__ROUND_QUEUE_H__
