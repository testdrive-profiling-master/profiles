//================================================================================
// Copyright (c) 2013 ~ 2026. HyungKi Jeong(clonextop@gmail.com)
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
// Title : __PROJECT__ application
// Rev.  : 10/26/2026 Mon (clonextop@gmail.com)
//================================================================================
#ifndef __COMMON_H__
#define __COMMON_H__
#include ".__PROJECT___version.inl"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/PciIo.h>
#include <Protocol/SimpleFileSystem.h>

extern EFI_SYSTEM_TABLE *g_SysTable;

// for debug output
#define LOG(...)						AsciiPrint(__VA_ARGS__)
#define LOGI(...)						AsciiPrint("*I: " __VA_ARGS__)
#define LOGW(...)						AsciiPrint("*W: " __VA_ARGS__)
#define LOGE(...)						AsciiPrint("*E: " __VA_ARGS__)

#define Sleep(ms)						gBS->Stall(ms * 1000)
#define ConsoleClearScreen()			g_SysTable->ConOut->ClearScreen(g_SysTable->ConOut)
#define ConsoleEnableCursor(bEnable)	g_SysTable->ConOut->EnableCursor(g_SysTable->ConOut, bEnable)
#define ConsoleSetAttribute(attr)		g_SysTable->ConOut->SetAttribute(g_SysTable->ConOut, attr)
#define ConsoleCursorPosition(col, row) g_SysTable->ConOut->SetCursorPosition(g_SysTable->ConOut, col, row)

typedef struct {
	const char *sTitle;
} MENU_ITEM;

int	 DoMenu(const char **pMenuList, int iMenuCount, const char *sHeader, const char *sFooter);
#define DO_MENU(memnu_list, sHeader, sFooter) DoMenu(memnu_list, ARRAY_SIZE(memnu_list), sHeader, sFooter)

void Pause(const char *sMsg);
#define PAUSE Pause(NULL);

#endif //__COMMON_H__
