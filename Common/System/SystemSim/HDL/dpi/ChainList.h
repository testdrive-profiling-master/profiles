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
#ifndef __CHAIN_LIST_H__
#define __CHAIN_LIST_H__

template<typename T>
class ChainList
{
	static ChainList<T>*	m_pHead;
	ChainList<T>*			m_pNext;
	T*						m_pItem;

public:
	ChainList(T* pItem){
		m_pItem	= pItem;
		m_pNext	= m_pHead;
		m_pHead	= this;
	}
	virtual ~ChainList(void){
		if(m_pHead == this){
			m_pHead	= m_pNext;
		}else{
			ChainList<T>*	pNode	= m_pHead;
			for(ChainList<T>* pNode = m_pHead; pNode; pNode = pNode->Next()){
				if(pNode->Next() == this){
					pNode->m_pNext = m_pNext;
					break;
				}
			}
		}
	}
	
	static ChainList<T>* Head(void)	{return m_pHead;}
	inline ChainList<T>* Next(void)	{return m_pNext;}
	inline T* Item(void)			{return m_pItem;}
};

template<typename T>
ChainList<T>* ChainList<T>::m_pHead	= NULL;

#endif//__CHAIN_LIST_H__
