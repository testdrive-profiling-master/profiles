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
#ifndef __COMPTR_H__
#define __COMPTR_H__
#include <assert.h>
#include <objbase.h>

template<class OBJ>
class ComPtr {
public:
	ComPtr(void): m_pObj(NULL) {}
	ComPtr(const ComPtr<OBJ> &ptr): m_pObj(ptr.get()) {
		if ( m_pObj != NULL ) m_pObj->AddRef();
	}
	~ComPtr(void) { Release(); }
	ComPtr<OBJ> &operator=(const ComPtr<OBJ> &other) {
		Release();
		if((m_pObj = other.get()) != NULL) m_pObj->AddRef();
	  return *this;
	};
	OBJ** operator&(void) {
		Release();
		return &m_pObj;
	}
	HRESULT coCreateInstance( REFCLSID clsid, REFIID iid) {
		Release();
		return CoCreateInstance( clsid, NULL, CLSCTX_INPROC, iid, (void **)&m_pObj );
	}
	OBJ* get(void) const { return m_pObj; }
	BOOL IsValid(void) const {return (m_pObj != NULL);}
	OBJ& operator*(void) {
		assert( m_pObj != NULL );
		return *m_pObj;
	}
	OBJ* operator->(void) {
		assert( m_pObj != NULL );
		return m_pObj;
	}
	void Release(void) { if ( m_pObj != NULL ) { m_pObj->Release(); m_pObj = NULL; } }

protected:
	OBJ*	m_pObj;
};

#endif//__COMPTR_H__
