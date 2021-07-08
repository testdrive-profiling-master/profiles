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
// Rev.  : 6/22/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "testdrive_ssh_interface.h"

class TestDriveSSH {
public:
	TestDriveSSH(void) {
		m_pMem	= NULL;
		m_SSH	= NULL;
	}
	~TestDriveSSH(void) {
		Release();
	}
	void Release(void) {
		SAFE_RELEASE(m_pMem);
		m_SSH	= NULL;
	}
	bool Initialize(void) {
		Release();
		m_pMem	= TestDriver_GetMemory(TESTDRIVE_SSH_MEMORY_NAME);

		if(m_pMem) {
			m_SSH	= (TestDrive_SSH*)(m_pMem->GetConfig()->UserConfig);

			if(!*m_SSH->sID || !*m_SSH->sIP || !*m_SSH->sPW) {
				LOGE("Need more information on TestDrive project.");
				Release();
			}
		} else {
			LOGE("No TSCP's shared memory. Run the TestDrive project first!");
		}

		return m_pMem != NULL;
	}
	inline TestDrive_SSH* Data(void)	{
		return m_SSH;
	}
	ITestDriverMemory*		m_pMem;
	TestDrive_SSH*			m_SSH;
};

int main(int argc, const char* argv[])
{
	TestDriveSSH		ssh;

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

	if(ssh.Initialize()) {
		string					sCmd("pscp");
		bool					bServerPath	= false;

		sCmd	+= " -scp -unsafe -pw ";
		sCmd	+= ssh.Data()->sPW;

		for(int i = 1; i < argc; i++) {
			cstring		sArg;

			if(*argv[i] == '/' || *argv[i] == '~') {
				if(bServerPath) {
					LOGE("Multiple server paths specified.");
					return 1;
				}

				sArg.AppendFormat("%s@%s:", ssh.Data()->sID, ssh.Data()->sIP);
				bServerPath	= true;
			}

			sArg	+= argv[i];
			sCmd	+= " ";
			sCmd	+= sArg.c_str();
		}

		system(sCmd.c_str());
		ssh.Release();
	}

	return 0;
}
