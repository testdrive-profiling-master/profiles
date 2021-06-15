//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TestDrive SCP
// Rev.  : 6/15/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "tscp_interface.h"

class TestDriveSCP {
public:
	TestDriveSCP(void){
		m_pMem	= NULL;
		m_pTSCP	= NULL;
	}
	~TestDriveSCP(void){Release();}
	void Release(void){
		SAFE_RELEASE(m_pMem);
		m_pTSCP	= NULL;
	}
	bool Initialize(void){
		Release();
		m_pMem	= TestDriver_GetMemory("testdrive_scp");
		if(m_pMem) {
			m_pTSCP	= (TSCP*)(m_pMem->GetConfig()->UserConfig);
			if(!*m_pTSCP->sID || !*m_pTSCP->sIP || !*m_pTSCP->sPW) {
				LOGE("Need more information on TestDrive project.");
				Release();
			}
		} else {
			LOGE("No TSCP's shared memory. Run the TestDrive project first!");
		}
		return m_pMem != NULL;
	}
	inline TSCP* Data(void)	{return m_pTSCP;}
	ITestDriverMemory*		m_pMem;
	TSCP*					m_pTSCP;
};

int main(int argc, const char* argv[])
{
	TestDriveSCP		tscp;
	if(argc == 1) {
		printf("TestDrive Proxy Secure Copy client\n" \
				"Usage: tscp [options] local_source [local_source..] host_target\n" \
				"       tscp [options] host_source local_target\n" \
				"Options:\n" \
				"  -pgpfp    print PGP key fingerprints and exit\n" \
				"  -p        preserve file attributes\n" \
				"  -q        quiet, don't show statistics\n" \
				"  -r        copy directories recursively\n" \
				"  -v        show verbose messages\n" \
				"  -load sessname  Load settings from saved session\n" \
				"  -P port   connect to specified port\n" \
				"  -l user   connect with specified username\n" \
				"  -pw passw login with specified password\n" \
				"  -1 -2     force use of particular SSH protocol version\n" \
				"  -4 -6     force use of IPv4 or IPv6\n" \
				"  -C        enable compression\n" \
				"  -i key    private key file for user authentication\n" \
				"  -noagent  disable use of Pageant\n" \
				"  -agent    enable use of Pageant\n" \
				"  -hostkey aa:bb:cc:...\n" \
				"            manually specify a host key (may be repeated)\n" \
				"  -batch    disable all interactive prompts\n" \
				"  -no-sanitise-stderr  don't strip control chars from standard error\n" \
				"  -proxycmd command\n" \
				"            use 'command' as local proxy\n"
				);
		return 0;
	}

	if(tscp.Initialize()) {
		bool	bServerPath	= false;
		cstring	sCommand("pscp -scp -unsafe");
		sCommand.AppendFormat(" -pw %s", tscp.Data()->sPW);
		for(int i=1;i<argc;i++) {
			sCommand	+= " ";
			if(*argv[i] == '/' || *argv[i] == '~') {
				if(bServerPath) {
					LOGE("Multiple server paths specified.");
					return 1;
				}
				sCommand.AppendFormat("%s@%s:", tscp.Data()->sID, tscp.Data()->sIP);
				bServerPath	= true;
			}
			sCommand	+= argv[i];
		}
		system(sCommand);
		tscp.Release();
	}
	return 0;
}
