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
// Title : System remote
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __TD_IPC_H__
#define __TD_IPC_H__
#include "common.h"
#include<boost/asio.hpp>

using namespace		std;
using				boost::asio::ip::tcp;
#define	MAX_TRANS_SIZE				4096
#define	DEFAULT_PORT_NUMBER			1212

typedef struct {
	DWORD	dwMagicCode;
	DWORD	dwByteSize;
} xHEADER_PACKET;


class xNetService {
protected:
	static boost::asio::io_service		m_Service;
};

class xSession {
public:
	xSession(tcp::socket& sock);
    void start(void);
	inline tcp::socket& socket(){return m_Socket;}

private:
	tcp::socket&	m_Socket;
    BYTE 			m_Buffer[MAX_TRANS_SIZE];
};

class xServer : public xNetService {
public:
	xServer(short port = DEFAULT_PORT_NUMBER);
private:
	tcp::acceptor				m_Acceptor;
};

class xClient : public xNetService {
public:
	xClient(const string& host, const string& port);
	~xClient(void);

private:
	tcp::socket					m_Socket;
};

#endif//__TD_IPC_H__
