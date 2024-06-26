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
// Title : Common DPI
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __DPI_COMMON_H__
#define __DPI_COMMON_H__
#include <string>
#include "SimHDL_common.h"
#include "SelfDestory.h"
#include "ChainList.h"

class SystemLog
{
public:
	typedef enum {
		LOG_ID_INFO,
		LOG_ID_WARNING,
		LOG_ID_ERROR
	} LOG_ID;

	SystemLog(const char *sTitle = NULL);
	~SystemLog(void);

	void SetTitle(const char *fmt, ...);

#define Info(...)	 Log(SystemLog::LOG_ID_INFO, __VA_ARGS__)
#define Warning(...) Log(SystemLog::LOG_ID_WARNING, __VA_ARGS__)
#define Error(...)	 Log(SystemLog::LOG_ID_ERROR, __VA_ARGS__)

	void Log(LOG_ID id, const char *fmt, ...);

private:
	std::string m_sTitle;
};

class MemoryWriteFilter : public SelfDestory
{
public:
	MemoryWriteFilter(void);
	virtual ~MemoryWriteFilter(void);

	static void DoFilter(uint64_t uAddress, uint32_t dwByteSize);

protected:
	virtual void OnDoFilter(uint64_t uAddress, uint32_t dwByteSize) = 0;

private:
	ChainList<MemoryWriteFilter> m_pChain;
};

#define LOGI Log.Info
#define LOGE Log.Error
#define LOGW Log.Warning

#endif //__DPI_COMMON_H__
