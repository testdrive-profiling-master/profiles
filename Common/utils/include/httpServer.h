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
#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__
#include "UtilFramework.h"
#include <microhttpd.h>

typedef enum {
	HTTP_METHOD_NONE,
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
} HTTP_METHOD;

class httpData
{
	BYTE  *m_pData;
	size_t m_dwByteSize;

public:
	httpData(void);
	~httpData(void);

	void		 Set(const void *pData, uint64_t off, size_t size);
	string		 c_string(void);
	inline BYTE *Data(void)
	{
		return m_pData;
	}
	inline size_t Size(void)
	{
		return m_dwByteSize;
	}
};

class httpConnection
{
public:
	httpConnection(HTTP_METHOD method, struct MHD_Connection *pConnection);
	~httpConnection(void);

	bool			   ConfirmPostData(const char *sData, size_t *pSize);
	void			   SetData(void *pData, size_t iByteSize = 0, const char *sDataType = NULL, bool bCopy = false);

	inline HTTP_METHOD Method(void)
	{
		return m_Method;
	}

	inline void *Data(void)
	{
		return m_pData;
	}

	inline size_t DataSize(void)
	{
		return m_iDataByteSize;
	}

	inline const char *DataType(void)
	{
		return m_sDataType;
	}

	inline map<string, httpData> &PostData(void)
	{
		return m_PostData;
	}

private:
	HTTP_METHOD				  m_Method;
	struct MHD_PostProcessor *m_pPostProcessor;
	cstring					  m_sData;
	void					 *m_pData;
	const char				 *m_sDataType;
	size_t					  m_iDataByteSize;
	map<string, httpData>	  m_PostData;
};

class httpServer
{
public:
	httpServer(bool bDebug = false, const char *sAllowedDomain = "*");
	virtual ~httpServer(void);

	bool Initialize(
		uint16_t	iPort		  = 8080,  // socket port number
		bool		bInternalOnly = false, // internal use only, prevent connection from outside
		const char *sHttpsKey = NULL, const char *sHttpsCert = NULL, const char *sRootCa = NULL // for secured http
	);
	void Release(void);
	bool IsInitialized(void);
	bool Send(struct MHD_Connection *connection, httpConnection *pCon);
	bool SendFail(struct MHD_Connection *connection);

protected:
	virtual bool OnGet(const char *sURL, httpConnection *pCon);
	virtual bool OnPost(const char *sURL, httpConnection *pCon);

private:
	bool Callback_Accept(const struct sockaddr *addr, socklen_t addrlen);
	bool Callback_Access(
		struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data,
		size_t *upload_data_size, void **con_cls);
	void Callback_Complete(httpConnection *pCon, enum MHD_RequestTerminationCode toe);

private:
	cstring			   m_sAllowedDomain;
	struct MHD_Daemon *m_pDaemon;
};

#endif //__HTTP_SERVER_H__
