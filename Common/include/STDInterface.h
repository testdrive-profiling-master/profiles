//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Common profiles
// Rev.  : 11/1/2024 Fri (clonextop@gmail.com)
//================================================================================
#ifndef __STD_INTERFACE_H__
#define __STD_INTERFACE_H__
#include <cstdint>
#include <limits.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __GNUC__
#	include <unistd.h>
#endif

#ifdef WIN32
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <windows.h>
#else
#	ifndef PVOID
typedef void *PVOID;
#	endif
#	if !defined(INT64)
#		define INT64 long long
#	endif
#	if !defined(UINT64)
#		define UINT64 unsigned INT64
#	endif
#	if !defined(LONGLONG)
#		define LONGLONG long long
#	endif
#	if !defined(LONG)
#		define LONG long
#	endif
#	if !defined(DWORD)
#		define DWORD unsigned int
#	endif
#	if !defined(WORD)
#		define WORD unsigned short
#	endif
#	if !defined(BYTE)
#		define BYTE unsigned char
#	endif
#	if !defined(BOOL)
#		define BOOL  DWORD
#		define TRUE  1
#		define FALSE 0
#	endif
#	if !defined(NULL)
#		define NULL 0
#	endif

#	if defined(KERNEL_64BIT)
typedef INT64 KPTR;
#	else
typedef DWORD KPTR;
#	endif
#endif

// Safe memory deletion
#define SAFE_DELETE(x)                                                                                                                          \
	if (x) {                                                                                                                                    \
		delete x;                                                                                                                               \
		(x) = NULL;                                                                                                                             \
	}
#define SAFE_DELETE_ARRAY(x)                                                                                                                    \
	if (x) {                                                                                                                                    \
		delete[] x;                                                                                                                             \
		(x) = NULL;                                                                                                                             \
	}
#define SAFE_RELEASE(x)                                                                                                                         \
	if (x) {                                                                                                                                    \
		(x)->Release();                                                                                                                         \
		(x) = NULL;                                                                                                                             \
	}

#ifndef DEBUG_ENABLE
#	ifdef USE_DEBUG
#		define DEBUG_ENABLE true
#	else
#		define DEBUG_ENABLE false
#	endif
#endif

#endif //__STD_INTERFACE_H__
