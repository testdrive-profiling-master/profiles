//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common DPI
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __DPI_FiFo_H__
#define __DPI_FiFo_H__
#include "dpi_common.h"
#include <assert.h>

template <typename T, const int fifo_size, const int minimum_pushable_size = 1>
class FiFo {
public:
	FiFo(void) {
		Reset();
		m_pArchive	= new T[fifo_size];
	}

	virtual ~FiFo(void) {
		assert(fifo_size > 0);
		assert(minimum_pushable_size < fifo_size);
		SAFE_DELETE_ARRAY(m_pArchive);
	}

	void Reset(void) {
		m_iUsed		= 0;
		m_iPush		= 0;
		m_iPop		= 0;
	}

	inline bool IsFull(void) {
		return (m_iUsed >= (fifo_size - minimum_pushable_size));
	}

	inline bool IsEmpty(void) {
		return (!m_iUsed);
	}

	bool Push(const T* pData) {
		if(IsFull()) return FALSE;

		memcpy(&m_pArchive[m_iPush], pData, sizeof(T));
		m_iPush = (m_iPush + 1) % fifo_size;
		m_iUsed++;
		return true;
	}

	bool Pop(T* pData) {
		if(IsEmpty()) return FALSE;

		memcpy(pData, &m_pArchive[m_iPop], sizeof(T));
		m_iPop = (m_iPop + 1) % fifo_size;
		m_iUsed--;
		return true;
	}

private:
	int		m_iUsed;
	int		m_iPush, m_iPop;
	T*		m_pArchive;
};

#endif//__DPI_FiFo_H__
