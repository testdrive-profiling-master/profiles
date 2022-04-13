//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TestDrive link
// Rev.  : 4/13/2022 Wed (clonextop@gmail.com)
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
		printf("TestDrive Proxy command-line connection utility\n" \
			   "Usage: tlink [options] [command]\n" \
			   "Options:\n" \
			   "  -pgpfp    print PGP key fingerprints and exit\n" \
			   "  -v        show verbose messages\n" \
			   "  -load sessname  Load settings from saved session\n" \
			   "  -P port   connect to specified port\n" \
			   "  -l user   connect with specified username\n" \
			   "  -proxycmd command\n" \
			   "            use 'command' as local proxy\n" \
			   "The following options only apply to SSH connections:\n" \
			   "  -D [listen-IP:]listen-port\n" \
			   "            Dynamic SOCKS-based port forwarding\n" \
			   "  -L [listen-IP:]listen-port:host:port\n" \
			   "            Forward local port to remote address\n" \
			   "  -R [listen-IP:]listen-port:host:port\n" \
			   "            Forward remote port to local address\n" \
			   "  -A -a     enable / disable agent forwarding\n" \
			   "  -t -T     enable / disable pty allocation\n" \
			   "  -1 -2     force use of particular protocol version\n" \
			   "  -4 -6     force use of IPv4 or IPv6\n" \
			   "  -C        enable compression\n" \
			   "  -i key    private key file for user authentication\n" \
			   "  -noagent  disable use of Pageant\n" \
			   "  -agent    enable use of Pageant\n" \
			   "  -noshare  disable use of connection sharing\n" \
			   "  -share    enable use of connection sharing\n" \
			   "  -hostkey aa:bb:cc:...\n" \
			   "            manually specify a host key (may be repeated)\n" \
			   "  -sanitise-stderr, -sanitise-stdout, -no-sanitise-stderr, -no-sanitise-stdout\n" \
			   "            do/don't strip control chars from standard output/error\n" \
			   "  -no-antispoof   omit anti-spoofing prompt after authentication\n" \
			   "  -m file   read remote command(s) from file\n" \
			   "  -s        remote command is an SSH subsystem (SSH-2 only)\n" \
			   "  -N        don't start a shell/command (SSH-2 only)\n" \
			   "  -nc host:port\n" \
			   "            open tunnel in place of session (SSH-2 only)\n" \
			   "  -sshlog file\n" \
			   "  -sshrawlog file\n" \
			   "            log protocol details to a file\n" \
			   "  -shareexists\n" \
			   "            test whether a connection-sharing upstream exists\n"
			  );
		return 0;
	}

	if(ssh.Initialize()) {
		vector<const char*>		arg_list;
		cstring					sServer;
		sServer.Format("%s@%s", ssh.Data()->sID, ssh.Data()->sIP);
		arg_list.push_back("plink");
		arg_list.push_back("-ssh");
		arg_list.push_back("-X");
		arg_list.push_back("-batch");
		arg_list.push_back("-pw");
		arg_list.push_back(ssh.Data()->sPW);
		arg_list.push_back(sServer.c_str());

		for(int i = 1; i < argc; i++) {
			arg_list.push_back(argv[i]);
		}

		{
			cstring	sCommand;

			for(auto& i : arg_list) sCommand.AppendFormat(" %s", i);

			system(sCommand);
		}

		fflush(stdout);
		ssh.Release();
	}

	return 0;
}
