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
// Title : Simulation sub-system
// Rev.  : 10/18/2021 Mon (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include "SimCircuit.h"

static int __ngspice_recieve_char(char* str, int id, void* p)
{
	LOGI("recieved %s", str);
	return 0;
}

static int __ngspice_recieve_stat(char* status, int id, void* p)
{
	LOGI("status: %s", status);
	return 0;
}

static int __ngspice_exit(int status, bool unload, bool exit, int id, void* p)
{
	LOGI("exit: %d", status);
	return 0;
}

static int __ngspice_recieve_data(vecvaluesall* data, int numstructs, int id, void* p)
{
	LOGI("data recieved: %f", data->vecsa[0]->creal);
	return 0;
}

static int __ngspice_recieve_init_data(vecinfoall* data, int id, void* p)
{
	LOGI("init data recieved from: %d", id);
	return 0;
}

static int __ngspice_running(bool running, int id, void* p)
{
	if(running) {
		LOGI("ng is running\n");
	} else {
		LOGI("ng is not running\n");
	}

	return 0;
}

SimCircuit::SimCircuit(void)
{
	ngSpice_Init(&__ngspice_recieve_char, &__ngspice_recieve_stat, &__ngspice_exit, &__ngspice_recieve_data, &__ngspice_recieve_init_data, &__ngspice_running, (void*)NULL);
}

SimCircuit::~SimCircuit(void)
{
}

bool SimCircuit::Initialize(char** sCircuit)
{
	ngSpice_Init(&__ngspice_recieve_char, &__ngspice_recieve_stat, &__ngspice_exit, &__ngspice_recieve_data, &__ngspice_recieve_init_data, &__ngspice_running, (void*)NULL);
	/*char** sCircuit = (char**)malloc(sizeof(char*) * 3);
	circarray[0] = strdup("some codes...");
	circarray[1] = strdup(".end");
	circarray[2] = NULL;*/
	ngSpice_Circ(sCircuit);
	ngSpice_Command("run");
	//ngSpice_SetBkpt(double time);
	return true;
}
