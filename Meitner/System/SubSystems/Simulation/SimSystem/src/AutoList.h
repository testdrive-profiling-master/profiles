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
// Title : Simulation sub-system
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __AUTO_LIST_H__
#define __AUTO_LIST_H__
#include "STDInterface.h"

template<typename T>
class AutoList
{
public:
	AutoList(void){
		m_pNext	= m_pHead;
		m_pHead	= (T*)this;
	}
	virtual ~AutoList(void){
		if(m_pHead == (T*)this) {
			m_pHead	= m_pNext;
		} else {
			T*	pNode	= m_pHead;

			while(pNode) {
				if(pNode->m_pNext == (T*)this) {
					pNode->m_pNext	= m_pNext;
					break;
				}

				pNode	= pNode->m_pNext;
			}
		}
	}
	static void ReleaseAll(void){
		while(Head()) delete Head();
	}
	inline void Release(void)	{delete this;}
	inline static T* Head(void)	{return m_pHead;}
	inline T* Next(void)			{return m_pNext;}

private:
	static T*		m_pHead;
	T*				m_pNext;
};

template<typename T>
T* AutoList<T>::m_pHead	= NULL;

#endif//__AUTO_LIST_H__
