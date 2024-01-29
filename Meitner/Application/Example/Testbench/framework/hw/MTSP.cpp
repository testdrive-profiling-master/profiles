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
// Title : Testbench
// Rev.  : 1/29/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "MTSP.h"
#include <io.h>
#include <assert.h>

MTSP::MTSP(DDK* pDDK) : DUT(pDDK)
{
	m_pHDMI		= new HDMI_Controller(m_pDDK, 0x10000, 250.f);	// 0x10000 hdmi base address, @200Mhz
}

MTSP::~MTSP(void)
{
	SAFE_DELETE(m_pHDMI);
}

int MTSP::GetCommandItemCount(void)
{
	MTSP_STATUS	status;
	status.all			= RegRead(0);
	BYTE	size		= status.cmd_fifo_windex - status.cmd_fifo_rindex;
	return size;
}

int MTSP::GetCommandFiFoSize(void)
{
	MTSP_STATUS	status;
	status.all			= RegRead(0);
	return (1 << status.cmd_fifo_depth);
}

void MTSP::WaitCommandFifoEmpty(void)
{
	while(GetCommandItemCount());
}

void MTSP::SetMemoryDescription(DWORD* pDesc)
{
	for(int i = 0; i < 4; i++)
		RegWrite(i << 2, pDesc[i]);
}

DDKMemory* MTSP::Compile(const char* sFileName)
{
	DDKMemory*	pInst	= NULL;
	char		sTargetFileName[1024];
	{
		// compile
		char cmd[4096];
		sprintf(sTargetFileName, "%s.testdrive.bin", sFileName);
		remove(sTargetFileName);	// remove previous compiled file
		sprintf(cmd, "%%TESTDRIVE_DIR%%\\bin\\gpasm -silent -i .\\GPASM %s %s", sFileName, sTargetFileName);
		system(cmd);	// do compile

		if(access(sTargetFileName, 0)) {	// not compiled.
			printf("*E: GPASM Compilation is failed. : %s\n", sFileName);
			{
				// show log
				char	sCmd[1024];
				sprintf(sCmd, "cat %s.testdrive.log", sFileName);
				system(sCmd);
			}
			return NULL;
		}
	}
	{
		FILE* fp = fopen(sTargetFileName, "rb");

		if(fp) {
			{
				// check magic code
				DWORD	dwMagicCode;
				fread(&dwMagicCode, sizeof(DWORD), 1, fp);

				if(dwMagicCode != 0x41505047) {	// check magic code = "GPPA"
					printf("*E: Invalid MTSP's GPASM magic code.\n");
					goto UNEXPECTED_ERROR;
				}
			}
			{
				// read binary
				DWORD	dwByteSize;
				fread(&dwByteSize, sizeof(DWORD), 1, fp);
				pInst	= CreateDDKMemory(dwByteSize, 32);	// 256bit aligned memory
				fread(pInst->Virtual(), dwByteSize, 1, fp);
				pInst->Flush();	// write all to FPGA memory
			}
		UNEXPECTED_ERROR:
			fclose(fp);
		}
	}
	return pInst;
}

void MTSP::RunProgram(DWORD dwPC, BOOL bNew, BOOL bWaitForDone)
{
	assert((dwPC & 3) == 0);	// PC must be aligned with 32bit
	RegWrite(4 << 2, dwPC | bNew | (bWaitForDone << 1));
}

BOOL MTSP::WriteGMB(DDKMemory* pMem, DWORD dwIndex, DWORD dwSize)
{
	MTSP_MEM_COMMAND	cmd;
	memset(&cmd, 0, sizeof(MTSP_MEM_COMMAND));
	cmd.op		= (0 << 3) | (0x0);	// read, GLB to external
	cmd.index	= dwIndex;
	cmd.p_base	= pMem->Physical();
	cmd.size	= pMem->ByteSize() / 32;	// 256 bit alignment

	for(int i = 0; i < 4; i++)
		RegWrite(i << 2, cmd.m[i]);

	return FALSE;
}

BOOL MTSP::ReadGMB(DDKMemory* pMem, DWORD dwIndex, DWORD dwSize)
{
	return FALSE;
}
