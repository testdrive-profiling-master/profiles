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
// Rev.  : 11/9/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __VIRTUAL_DISPLAY_H__
#define __VIRTUAL_DISPLAY_H__
#include "dpi_lib.h"

class VirtualDisplay : public MemoryWriteFilter {
public:
	VirtualDisplay(void);
	virtual ~VirtualDisplay(void);

	void Initialize(bool bReverse = FALSE);
	void SetBaseAddress(DWORD dwBaseAddress, bool bFront = FALSE);
	void SetFormat(int iWidth, int iHeight, DWORD dwByteStride = 0, DISPLAY_FORMAT Format = DISPLAY_FORMAT_ARGB_8888);
	void Update(bool bFront = FALSE);

protected:
	virtual void OnDoFilter(DWORD dwAddress, DWORD dwByteSize);

private:
	virtual void UpdateMemoryRange(void);

	DisplayConfig*		m_pConfig;
	struct {
		DWORD			dwBase;
		DWORD			dwEnd;
	} m_MemoryRange;
};

DPI_FUNCTION void VirtualDisplayInitialize(int bReverse = FALSE);
DPI_FUNCTION void VirtualDisplayBaseAddress(unsigned int dwBaseAddress, int bFront = FALSE);
DPI_FUNCTION void VirtualDisplayFormat(int iWidth, int iHeight, unsigned int dwByteStride, unsigned int Format);
DPI_FUNCTION void VirtualDisplayUpdate(int bFront = FALSE);

#endif//__VIRTUAL_DISPLAY_H__
