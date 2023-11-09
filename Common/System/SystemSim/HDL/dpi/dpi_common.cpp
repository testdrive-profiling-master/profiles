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
// Title : Common DPI
// Rev.  : 12/30/2021 Thu (clonextop@gmail.com)
//================================================================================
#include "dpi_lib.h"
#include <stdarg.h>

SystemLog::SystemLog(const char* sTitle)
{
	SetTitle(sTitle);
}

SystemLog::~SystemLog(void)
{
}

static const char __cTag[] = {
	'I',
	'W',
	'E'
};

void SystemLog::SetTitle(const char* fmt, ...)
{
	if(!fmt) return;

	{
		char sTitle[1024];
		va_list		ap;
		va_start(ap, fmt);
		vsprintf(sTitle, fmt, ap);
		va_end(ap);
		m_sTitle	= sTitle;
	}
}

void SystemLog::Log(LOG_ID id, const char* fmt, ...)
{
	printf("*%c: %s[", __cTag[id], m_sTitle.c_str());
	{
		// present simulation time.
		UINT64	llTime	= SimulationTime();
		{
			static const char*	_time_unit[] = {"ps", "ns", "us", "ms", "s", "m", "h"};
			DWORD		dwTimeBuff[7];
			int			iMaxTimeScale	= 0;

			for(; iMaxTimeScale < 7; iMaxTimeScale++) {
				switch(iMaxTimeScale) {
				case 4:	// 's' second
				case 5: // 'm' minute
					dwTimeBuff[iMaxTimeScale]	= llTime % 60;
					llTime /= 60;
					break;

				case 6:	// 'h' hour
					dwTimeBuff[iMaxTimeScale]	= llTime % 24;
					llTime /= 24;
					break;

				default:	// ps, ns, us, ms
					dwTimeBuff[iMaxTimeScale]	= llTime % 1000;
					llTime /= 1000;
					break;
				}

				if(!llTime)	break;
			}

			for(int i = iMaxTimeScale; i >= 0; i--) {
				bool bFirst	= (i == iMaxTimeScale);

				if(dwTimeBuff[i] || bFirst) {
					if(!bFirst) printf(" ");

					printf((i == iMaxTimeScale) || (i > 3) ? "%d" : "%03d", dwTimeBuff[i]);
					printf(_time_unit[i]);
				}
			}
		}
	}
	printf("] - ");
	{
		va_list		ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
	printf("\n");
	fflush(stdout);

	if(id == LOG_ID_ERROR) {
		SimulationQuit(true);
	}
}

MemoryWriteFilter::MemoryWriteFilter(void) : m_pChain(this)
{
}

MemoryWriteFilter::~MemoryWriteFilter(void)
{
}

void MemoryWriteFilter::DoFilter(DWORD dwAddress, DWORD dwByteSize)
{
	for(ChainList<MemoryWriteFilter>* pNode = ChainList<MemoryWriteFilter>::Head(); pNode; pNode = pNode->Next()) {
		pNode->Item()->OnDoFilter(dwAddress, dwByteSize);
	}
}
