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
// Title : Testbench
// Rev.  : 1/30/2023 Mon (clonextop@gmail.com)
//================================================================================
#include "Testbench.h"
#include "hw/DUT.h"

class Testbench : public TestbenchFramework {
	DUT*			m_pDUT;		// Processor (Design Under Testing)
	DDKMemory*		m_pBuff;

	virtual bool OnInitialize(void) {
		m_pDUT	= NULL;
		m_pBuff	= NULL;

		// H/W system equality check
		if(!CheckSimulation("Processor axi wrapper"))
			return false;

		m_pDUT	= new DUT(m_pDDK);
		m_pDUT->SetClock(300.f);	// set processor clock to 300MHz (High speed.) for performance
		m_pBuff		= CreateDDKMemory(1024 * 8, 32);	// 8KB, 256bit alignment
		MemoryLog(m_pBuff, "Test buffer memory");
		return true;
	}

	virtual void OnRelease(void) {
		if(m_pDUT) m_pDUT->SetClock(50.f);	// set processor clock to 50MHz (Low speed.) for IDLE status

		SAFE_RELEASE(m_pBuff);
		SAFE_DELETE(m_pDUT);
	}

	virtual bool OnTestBench(void) {
		//-----------------------------------------------------
		// slave R/W test
		//-----------------------------------------------------
		printf("\nAsynchronous slave FIFO R/W test...\n");
		printf("\tDUT_CLOCK_GEN STATUS : 0x%X\n", m_pDDK->RegRead(DUT_CLOCKGEN_BASE));
		printf("\tWrite slave fifo 5 times...\n");

		for(int i = 0; i < 5; i++) {
			DWORD	dwData	= 0xABCD0000 | i;
			printf("\tWrite : 0x%X\n", dwData);
			m_pDDK->RegWrite(DUT_BASE | (3 << 2), dwData);
		}

		printf("\tDUT_CLOCK_GEN STATUS : 0x%X\n", m_pDDK->RegRead(DUT_CLOCKGEN_BASE));
		printf("\tRead slave fifo 5 times...\n");

		for(int i = 0; i < 5; i++)
			printf("\tRead : 0x%X\n", m_pDDK->RegRead(DUT_BASE));

		//-----------------------------------------------------
		// master R/W test
		//-----------------------------------------------------
		//// ***** write test *****
		printf("\nAsynchronous master write test...\n");
		// clear memory
		printf("\tClear memory...\n");
		memset(m_pBuff->Virtual(), 0xCC, m_pBuff->ByteSize());
		// write to system memory
		m_pBuff->Flush();
		// write
		m_pDDK->RegWrite(DUT_BASE | (0 << 2), m_pBuff->Physical());		// set target memory address
		m_pDDK->RegWrite(DUT_BASE | (1 << 2), 8);		// set transfer size = 8
		m_pDDK->RegWrite(DUT_BASE | (2 << 2), 1);		// do write

		// wait for master bus write done
		// s/w is too fast in the h/w simulation mode, so we will wait a while.
		for(int i = 0; i < 20; i++) m_pDDK->RegRead(DUT_BASE);

		// read from system memory
		m_pBuff->Flush(FALSE);
		{
			DWORD* pData	= (DWORD*)m_pBuff->Virtual();

			for(int i = 0; i < 20; i++)
				printf("\tRead : [%d] %08X %08X %08X %08X %08X %08X %08X %08X\n",
					   i,
					   pData[i * 8],
					   pData[i * 8 + 1],
					   pData[i * 8 + 2],
					   pData[i * 8 + 3],
					   pData[i * 8 + 4],
					   pData[i * 8 + 5],
					   pData[i * 8 + 6],
					   pData[i * 8 + 7]);
		}
		//// ***** read test *****
		printf("\nAsynchronous master read test...\n");
		{
			printf("\tInitiailze memory : 0x%X\n", m_pBuff->Physical());
			DWORD* pData	= (DWORD*)m_pBuff->Virtual();

			for(int i = 0; i < 16; i++) {
				pData[i * 8]		= 0xABC00000 | (i * 8 + 0);
				pData[i * 8 + 1]	= 0xABC10000 | (i * 8 + 1);
				pData[i * 8 + 2]	= 0xABC20000 | (i * 8 + 2);
				pData[i * 8 + 3]	= 0xABC30000 | (i * 8 + 3);
				pData[i * 8 + 4]	= 0xABC40000 | (i * 8 + 4);
				pData[i * 8 + 5]	= 0xABC50000 | (i * 8 + 5);
				pData[i * 8 + 6]	= 0xABC60000 | (i * 8 + 6);
				pData[i * 8 + 7]	= 0xABC70000 | (i * 8 + 7);
			}

			// write to system memory
			m_pBuff->Flush(TRUE);
		}
		m_pDDK->RegWrite(DUT_BASE | (0 << 2), m_pBuff->Physical());		// set target memory address
		m_pDDK->RegWrite(DUT_BASE | (1 << 2), 8);		// set transfer size = 8
		m_pDDK->RegWrite(DUT_BASE | (2 << 2), 0);		// do read

		// wait for master bus read done
		// s/w is too fast in the h/w simulation mode, so we will wait a while.
		for(int i = 0; i < 20; i++) m_pDDK->RegRead(DUT_BASE);

		// check out~~
		printf("\tCheck out processor's valid read operation from simulation's waveform...\n");
		//// ***** interrupt test *****
		printf("\nInterrupt test...\n");
		m_pDDK->RegWrite((DUT_BASE | (4 << 2)), 1);
		m_pDDK->WaitInterruptDone();
		printf("\tInterrupt Counted = 0x%X\n", (m_pDDK->RegRead((DUT_CLOCKGEN_BASE | (4 << 2))) >> 18) & 0x3FF);	// clear & get interrupt counter
		printf("process is done!\n");
		return true;
	}
};

int main(int argc, char** argv)
{
	Testbench	tb;

	if(tb.Initialize()) {
		if(!tb.DoTestbench())
			printf("Testbench is failed.\n");
	} else {
		printf("Initialization is failed.\n");
	}

	tb.Release();
}
