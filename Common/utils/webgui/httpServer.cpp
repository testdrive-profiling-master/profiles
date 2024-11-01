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
// Title : WebGUI project
// Rev.  : 11/1/2024 Fri (clonextop@gmail.com)
//================================================================================
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "httpServer.h"

static bool __bDebug = false;
// clang-format off
#define DLOGI(...) 		{if(__bDebug) LOGI(__VA_ARGS__);}
#define DLOGE(...) 		{if(__bDebug) LOGE(__VA_ARGS__);}
// clang-format on

httpData::httpData(void)
{
	m_pData		 = NULL;
	m_dwByteSize = 0;
}

httpData::~httpData(void)
{
	SAFE_DELETE_ARRAY(m_pData);
	m_dwByteSize = 0;
}

void httpData::Set(const void *pData, uint64_t off, size_t size)
{
	size_t request_size = off + size;

	if (request_size > m_dwByteSize) {
		BYTE *pNew = new BYTE[request_size];

		if (m_pData) {
			memcpy(pNew, m_pData, m_dwByteSize);
			delete[] m_pData;
		}

		m_dwByteSize = request_size;
		m_pData		 = pNew;
	}

	memcpy(m_pData + off, pData, size);
}

string httpData::c_string(void)
{
	string s;

	if (m_dwByteSize) {
		s.resize(m_dwByteSize);
		memcpy((BYTE *)s.data(), m_pData, m_dwByteSize);
	}

	return s;
}

httpConnection::httpConnection(HTTP_METHOD method, struct MHD_Connection *pConnection)
{
	m_Method		 = method;
	m_pPostProcessor = NULL;
	m_pData			 = NULL;
	m_sDataType		 = NULL;
	m_iDataByteSize	 = 0;

	if (method == HTTP_METHOD_POST) {
		m_pPostProcessor = MHD_create_post_processor(
			pConnection, 65536,
			(MHD_PostDataIterator)([](void *coninfo_cls, enum MHD_ValueKind kind, const char *key, const char *filename,
									  const char *content_type, const char *transfer_encoding, const char *data, uint64_t off,
									  size_t size) -> MHD_Result {
				DLOGI("Iterate_post : kind(%d) key(%s) filename(%s) content_type(%s) size(%d)", (int)kind, key, filename, content_type, size);

				if (kind == MHD_POSTDATA_KIND && key) {
					((httpConnection *)coninfo_cls)->PostData()[key].Set(data, off, size);
				}

				return MHD_YES;
			}),
			(void *)this);
	}
}

httpConnection::~httpConnection(void)
{
	if (m_pPostProcessor) {
		MHD_destroy_post_processor(m_pPostProcessor);
		m_pPostProcessor = NULL;
	}

	SetData(NULL);
}

httpServer::httpServer(bool bDebug, const char *sAllowedDomain)
{
	__bDebug = bDebug;
	if (sAllowedDomain && *sAllowedDomain)
		m_sAllowedDomain = sAllowedDomain;

	m_pDaemon = NULL;
}

httpServer::~httpServer(void)
{
	Release();
}

bool httpServer::Initialize(uint16_t iPort, bool bInternalOnly, const char *sHttpsKey, const char *sHttpsCert, const char *sRootCa)
{
	Release();
	unsigned int MHD_flags = MHD_USE_AUTO_INTERNAL_THREAD;
	typedef struct {
		enum MHD_OPTION id;
		const void	   *pdata;
	} MHD_OPT;
	MHD_OPT opt[5];
	int		opt_index  = 0;
	auto	set_option = [&](enum MHD_OPTION id, const void *pdata) {
		   opt[opt_index].id	= id;
		   opt[opt_index].pdata = pdata;
		   opt_index++;
	};
	memset(opt, 0, sizeof(opt));

	// #1,2 secured connection
	bool bSecure = (sHttpsKey != NULL || sHttpsCert != NULL || sRootCa != NULL);
	if (bSecure) {
		if (sHttpsKey)
			set_option(MHD_OPTION_HTTPS_MEM_KEY, sHttpsKey);
		if (sHttpsCert)
			set_option(MHD_OPTION_HTTPS_MEM_CERT, sHttpsCert);
		if (sRootCa)
			set_option(MHD_OPTION_HTTPS_MEM_TRUST, sRootCa);
		MHD_flags |= MHD_USE_SSL;
	}
	struct sockaddr_in loopback_addr;
	if (bInternalOnly) {
		// internal loopback connection only
		memset(&loopback_addr, 0, sizeof(loopback_addr));
		loopback_addr.sin_family	  = AF_INET;
		loopback_addr.sin_port		  = htons(iPort);
		loopback_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

		set_option(MHD_OPTION_SOCK_ADDR, (struct sockaddr *)&loopback_addr);
	}

#define OPTION_LIST(n) opt[n].id, opt[n].pdata
	m_pDaemon = MHD_start_daemon(
		MHD_flags, iPort, // type & port
		(MHD_AcceptPolicyCallback)([](void *cls, const struct sockaddr *addr, socklen_t addrlen) -> MHD_Result {
			return (enum MHD_Result)((httpServer *)cls)->Callback_Accept(addr, addrlen);
		}),
		this, // accept callback
		(MHD_AccessHandlerCallback)([](void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version,
									   const char *upload_data, size_t *upload_data_size, void **req_cls) -> MHD_Result {
			return (enum MHD_Result)((httpServer *)cls)
				->Callback_Access(connection, url, method, version, upload_data, upload_data_size, req_cls);
		}),
		this, // answer callback
		MHD_OPTION_NOTIFY_COMPLETED,
		(MHD_RequestCompletedCallback)([](void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
			httpConnection *pConnection = (httpConnection *)*con_cls;
			if (cls)
				((httpServer *)cls)->Callback_Complete(pConnection, toe);

			if (pConnection) {
				delete pConnection;
				*con_cls = NULL;
			}
		}),
		this,			// completion
		OPTION_LIST(0), // #0
		OPTION_LIST(1), // #1
		OPTION_LIST(2), // #2
		OPTION_LIST(3), // #3
		OPTION_LIST(4), // #4
		MHD_OPTION_END);

	return IsInitialized();
}

void httpServer::Release(void)
{
	if (m_pDaemon) {
		MHD_stop_daemon(m_pDaemon);
		m_pDaemon = NULL;
	}
}

bool httpServer::IsInitialized(void)
{
	return (m_pDaemon != NULL);
}

bool httpServer::Send(struct MHD_Connection *connection, httpConnection *pCon)
{
	bool ret = false;

	if (pCon->Data()) {
		struct MHD_Response *response = MHD_create_response_from_buffer(pCon->DataSize(), pCon->Data(), MHD_RESPMEM_PERSISTENT);

		if (response) {
			if (m_sAllowedDomain.size())
				MHD_add_response_header(response, "Access-Control-Allow-Origin", m_sAllowedDomain.c_str());
			// MHD_add_response_header(response, "Access-Control-Allow-Methods", "POST,GET,OPTIONS,DELETE");

			if (pCon->DataType()) {
				// 	qMHD_add_response_header(response, MHD_HTTP_HEADER_ACCEPT_RANGES, "bytes");
				MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_TYPE, pCon->DataType());
			}

			ret = (bool)MHD_queue_response(connection, MHD_HTTP_OK, response);
			MHD_destroy_response(response);
		} else {
			LOGE("Send failed.");
		}
	}

	return ret;
}

bool httpServer::SendFail(struct MHD_Connection *connection)
{
	bool				 ret  = false;
	char				*resp = "";

	struct MHD_Response *response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_MUST_FREE);

	if (response) {
		if (m_sAllowedDomain.size())
			MHD_add_response_header(response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");

		ret = (bool)MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
		MHD_destroy_response(response);
	} else {
		LOGE("SendFail failed.");
	}

	return ret;
}

bool httpServer::Callback_Accept(const struct sockaddr *addr, socklen_t addrlen)
{
	return true;
}

bool httpServer::Callback_Access(
	struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data,
	size_t *upload_data_size, void **req_cls)
{
	HTTP_METHOD method_id;
	if (!strcmp(method, "GET"))
		method_id = HTTP_METHOD_GET;
	else if (!strcmp(method, "POST"))
		method_id = HTTP_METHOD_POST;
	else if (!strcmp(method, "PUT"))
		method_id = HTTP_METHOD_PUT;
	else if (!strcmp(method, "DELETE"))
		method_id = HTTP_METHOD_DELETE;
	else
		return false;

	// grant a request.
	if (!*req_cls) {
		httpConnection *pConnection = new httpConnection(method_id, connection);

		if (!pConnection)
			return false;

		*req_cls = (void *)pConnection;
		return true;
	}

	httpConnection *pConnection = (httpConnection *)*req_cls;
	DLOGI("Connection(0x%p:0x%p)[%s] : %s", connection, pConnection, method, url);

	switch (method_id) {
	case HTTP_METHOD_GET: {
		if (OnGet(url, pConnection) && pConnection->DataSize()) {
			return Send(connection, pConnection);
		} else {
			return SendFail(connection);
		}
	} break;
	case HTTP_METHOD_POST: {
		if (*upload_data_size) {
			// confirm post data size
			if (pConnection->ConfirmPostData(upload_data, upload_data_size))
				return true;
		} else {
			// got post data
			if (OnPost(url, pConnection) && pConnection->DataSize()) {
				return Send(connection, pConnection);
			} else {
				pConnection->SetData(NULL);
				return SendFail(connection);
			}
		}
	} break;
	}

	return false;
}

bool httpServer::OnGet(const char *sURL, httpConnection *pCon)
{
	return false;
}

bool httpServer::OnPost(const char *sURL, httpConnection *pCon)
{
	return false;
}

void httpServer::Callback_Complete(httpConnection *pCon, enum MHD_RequestTerminationCode toe) {}

bool httpConnection::ConfirmPostData(const char *sData, size_t *pSize)
{
	if (!*pSize)
		return true;

	if (m_pPostProcessor) {
		MHD_post_process(m_pPostProcessor, sData, *pSize);
		*pSize = 0;
		return true;
	} else {
		SetData((void *)sData, *pSize, NULL, true);
		*pSize = 0;
		return true;
	}

	return false;
}

void httpConnection::SetData(void *pData, size_t iByteSize, const char *sDataType, bool bCopy)
{
	if (!pData && m_pData) {
		delete[] m_pData;
		m_pData			= NULL;
		m_iDataByteSize = 0;
		m_sDataType		= NULL;
	}

	if (pData && iByteSize) {
		if (bCopy) {
			if (m_pData) {
				// append
				BYTE *pNew = new BYTE[iByteSize + m_iDataByteSize];
				memcpy(pNew, m_pData, m_iDataByteSize);
				memcpy(pNew + m_iDataByteSize, pData, iByteSize);
				m_iDataByteSize += iByteSize;
			} else {
				// set
				m_pData = new BYTE[iByteSize];
				memcpy(m_pData, pData, iByteSize);
				m_iDataByteSize = iByteSize;
			}
		} else {
			// replace
			if (m_pData)
				delete[] m_pData;

			m_pData			= pData;
			m_iDataByteSize = iByteSize;
		}

		m_sDataType = sDataType;
	}
}
