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
// Title : Common profiles
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __CIRCULAR_LINK_H__
#define __CIRCULAR_LINK_H__

template <typename T, int idx>
class CircularLink
{
public:
	CircularLink(void){
		pPrev	= NULL;
		pNext	= NULL;
	}

	virtual ~CircularLink(void){
		UnLink();
	}

	void SetObject(T object){
		Object	= object;
	}

	void Link(CircularLink<T, idx>* pPrevLink = NULL){
		UnLink();
		if(pPrevLink){
			pPrev				= pPrevLink;
			pNext				= pPrevLink->pNext;
			pPrevLink->pNext	= this;
			if(pNext)
				pNext->pPrev	= this;
		}else{
			if(pHead) pHead->pPrev	= this;
			pNext			= pHead;
			pHead			= this;
		}
	}

	void UnLink(void){
		if(pHead == this)
			pHead			= pNext;
		if(pPrev)
			pPrev->pNext	= pNext;
		if(pNext)
			pNext->pPrev	= pPrev;
		
		pPrev			= NULL;
		pNext			= NULL;
	}

	static inline CircularLink<T, idx>* Head(void)	{return pHead;}
	inline CircularLink<T, idx>* Prev(void)	{return pPrev;}
	inline CircularLink<T, idx>* Next(void)	{return pNext;}
	inline T Item(void)						{return Object;}

private:
	static CircularLink<T, idx>*	pHead;
	CircularLink<T, idx>*			pPrev;
	CircularLink<T, idx>*			pNext;
	T								Object;
};

template <typename T, int idx>
CircularLink<T, idx>*	CircularLink<T, idx>::pHead		= NULL;

#endif//__CIRCULAR_LINK_H__
