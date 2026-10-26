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
#include "common.h"
EFI_SYSTEM_TABLE *g_SysTable = NULL;

#define ATTR_NORMAL	  (EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK)
#define ATTR_SELECTED (EFI_BLACK | EFI_BACKGROUND_LIGHTGRAY)

// menu select
int DoMenu(const char **pMenuList, int iMenuCount, const char *sHeader, const char *sFooter)
{
	BOOLEAN		  bRunning	 = TRUE;
	int			  iStartRow	 = 0;
	int			  iSelection = 0;
	int			  i;
	UINTN		  iWidth, iHeight;
	EFI_STATUS	  Status;
	EFI_INPUT_KEY Key;
	// PrintAsciiNString

	ConsoleEnableCursor(FALSE);
	ConsoleClearScreen();

	// total width/height
	g_SysTable->ConOut->QueryMode(g_SysTable->ConOut, g_SysTable->ConOut->Mode->Mode, &iWidth, &iHeight);

	// show header
	ConsoleSetAttribute(ATTR_NORMAL);
	if (sHeader) {
		LOG(sHeader);
		for (i = 0; sHeader[i]; i++) {
			if (sHeader[i] == '\n')
				iStartRow++;
		}
		if (sHeader[AsciiStrLen(sHeader) - 1] != '\n') {
			LOG("\n");
			iStartRow++;
		}
	}
	// show footer
	if (sFooter) {
		ConsoleCursorPosition(0, iStartRow + iMenuCount + 2);
		LOG(sFooter);
	}
	// show selection box
	{
		ConsoleCursorPosition(0, iStartRow);
		Print(L"┌");
		for (i = 2; i < iWidth; i++) Print(L"─");
		Print(L"┐");

		iStartRow++;
		for (i = 0; i < iMenuCount; i++) {
			ConsoleCursorPosition(0, iStartRow + i);
			Print(L"│");
			ConsoleCursorPosition(iWidth - 1, iStartRow + i);
			Print(L"│");
		}

		ConsoleCursorPosition(0, iStartRow + iMenuCount);
		Print(L"└");
		for (i = 2; i < iWidth; i++) Print(L"─");
		Print(L"┘");
	}

	while (bRunning) {
		// show menu list
		for (i = 0; i < iMenuCount; i++) {
			ConsoleCursorPosition(1, iStartRow + i);
			ConsoleSetAttribute((i == iSelection) ? ATTR_SELECTED : ATTR_NORMAL);
			LOG("%2d. %a", i + 1, pMenuList[i]);
			for (int iRest = iWidth - AsciiStrLen(pMenuList[i]) - 6; iRest > 0; iRest--) LOG(" ");
		}
		ConsoleSetAttribute(ATTR_NORMAL);

		// wait for key
		while (TRUE) {
			Status = g_SysTable->ConIn->ReadKeyStroke(g_SysTable->ConIn, &Key);
			if (!EFI_ERROR(Status)) {
				if (Key.ScanCode == SCAN_UP) {
					if (iSelection > 0) {
						iSelection--;
					} else {
						iSelection = iMenuCount - 1;
					}
					break;
				} else if (Key.ScanCode == SCAN_DOWN) {
					if (iSelection < iMenuCount - 1) {
						iSelection++;
					} else {
						iSelection = 0;
					}
					break;
				} else if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
					bRunning = FALSE;
					break;
				}
			}
			Sleep(50);
		}
	}

	ConsoleSetAttribute(ATTR_NORMAL);
	ConsoleEnableCursor(TRUE);
	ConsoleClearScreen();

	return iSelection;
}

void Pause(const char *sMsg)
{
	const char	 *__default_msg = "\nPress any key to return to main menu...";
	EFI_STATUS	  Status;
	EFI_INPUT_KEY Key;
	LOG("\n");
	if (!sMsg)
		sMsg = __default_msg;
	LOG(sMsg);
	do {
		Status = g_SysTable->ConIn->ReadKeyStroke(g_SysTable->ConIn, &Key);
	} while (EFI_ERROR(Status));
}
