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
// Title : ToolAutoDownload project
// Rev.  : 5/27/2024 Mon (clonextop@gmail.com)
//================================================================================
#include "ArgTable.h"
#include "cstring.h"
#include <memory.h>
#include <string.h>

int GetFileSize(char *sPath)
{
	int	  rst = 0;

	FILE *f = fopen(sPath, "r");
	if (f != NULL) {
		fseek(f, 0, SEEK_END);
		rst = ftell(f);
		fclose(f);
	}

	return rst;
}

bool GetDownloadPathEclipse(cstring &sPath)
{
	bool  bFound = false;
	FILE *fp	 = fopen("download.html", "rt");

	if (fp) {
		char *sLine = new char[1024 * 1024];
		int	  iStep = 0;
		while (fgets(sLine, 1024 * 1024, fp) && !bFound) {
			switch (iStep) {
			case 0:
				if (!strstr(sLine, "C/C++")) {
					iStep++;
				}
				break;
			case 1:
				if (!strstr(sLine, "cpp"))
					break;
				if (!strstr(sLine, "win32-x86_64.zip"))
					break;
				{
					char *sTok = strstr(sLine, ".zip");
					sTok[4]	   = NULL;
					sTok	   = strstr(sLine, "www.eclipse.org");
					if (sTok) {
						sPath = sTok;
					} else {
						if ((sTok = strstr(sLine, "href=\"")) != NULL) {
							sTok += 6;

							if ((sTok = strstr(sLine, "file=")) != NULL) {
								sTok += 5;
								sPath.Format("mirror.kakao.com/eclipse/%s", sTok);
							}
						}
					}

					bFound = true;
				}
				break;
			}
		}
		delete[] sLine;
		fclose(fp);
		system("del /Q download.html");
	}

	return bFound;
}

bool GetDownloadPathNotepad(cstring &sPath)
{
	bool  bFound = false;
	FILE *fp;

	char *sLine = new char[1024 * 64];
	char *sTok, *sEnd;

	if ((fp = fopen("download.html", "rt")) != NULL) {
		// find download page
		while (fgets(sLine, 1024 * 64, fp)) {
			sTok = strstr(sLine, "href=\"");
			if (!sTok)
				continue;
			sTok += 6;
			if (strstr(sTok, "/downloads/") != sTok)
				continue;
			{
				sEnd = strstr(sTok, "\"");
				if (sEnd) {
					*sEnd = NULL;
					{
						// get download page
						char sCmd[1024 * 4];
						sprintf(sCmd, "wget --no-check-certificate https://notepad-plus-plus.org%s -O download_2.html",
								sTok);
						system(sCmd);
						bFound = true;
					}
					break;
				}
				bFound = true;
			}
		}
		fclose(fp);
		system("del /Q download.html");
	}

	if (bFound) {
		bFound = false;
		if ((fp = fopen("download_2.html", "rt")) != NULL) {
			// find download url
			while (fgets(sLine, 1024 * 64, fp)) {
				sTok = strstr(sLine, "href=\"");
				if (!sTok)
					continue;
				sTok += 6;
				sEnd = strstr(sTok, ".zip\"");
				if (sEnd) {
					sEnd[4] = NULL;
					if (strstr(sTok, "http") == sTok) {
						sPath = sTok;
					} else {
						sPath.Format("https://notepad-plus-plus.org%s", sTok);
					}
					bFound = true;
					break;
				}
			}
			fclose(fp);
			system("del /Q download_2.html");
		}
	}

	delete[] sLine;
	return bFound;
}

bool GetDownloadHtmlJRE(cstring &sPath)
{
	bool  bFound = false;
	FILE *fp	 = fopen("download.html", "rt");

	if (fp) {
		char sLine[1024 * 64];
		while (fgets(sLine, 1024 * 64, fp)) {
			char *sURL = strstr(sLine, "-x64.exe</a>");
			if (!sURL)
				continue;
			sURL[8] = NULL;
			{
				char *sTok = sLine;
				while (sURL = strstr(sTok, "<a href")) {
					sTok = sURL + 1;
				}
				sURL = strstr(sTok, ">");
				if (!sURL)
					continue;
				strcpy(sLine, sURL + 1);
			}
			{
				sPath.Format("https://enos.itcollege.ee/~jpoial/allalaadimised/jdk8/%s", sLine);
				bFound = true;
				printf("URL found : %s\n", sPath.c_str());
				break;
			}
		}
		fclose(fp);
		system("del /Q download.html");
	} else
		false;

	return bFound;
}

bool GetDownloadPathJRE(char *sPath)
{
	bool  bFound = false;
	FILE *fp	 = fopen("download.html", "rt");

	if (fp) {
		char sLine[1024 * 64];
		while (fgets(sLine, 1024 * 64, fp)) {
			char *sURL = strstr(sLine, "<a title=\"");
			if (!sURL)
				continue;
			strcpy(sLine, sURL + 10);
			sURL = strstr(sLine, "\"");
			if (!sURL)
				continue;
			*sURL = NULL;
			{ // check title
				strlwr(sLine);
				if (!strstr(sLine, "download"))
					continue;
				if (!strstr(sLine, "java"))
					continue;
				if (!strstr(sLine, "windows"))
					continue;
				if (!strstr(sLine, "64"))
					continue;
			}
			strcpy(sLine, sURL + 1);
			sURL = strstr(sLine, "\"");
			if (!sURL)
				continue;
			strcpy(sLine, sURL + 1);
			sURL = strstr(sLine, "\"");
			if (!sURL)
				continue;
			*sURL = NULL;
			{
				strcpy(sPath, sLine);
				bFound = true;
				printf("URL found : %s\n", sPath);
				break;
			}
		}
		fclose(fp);
		system("del /Q download.html");
	}
	return bFound;
}

bool GetDownloadPathMinGW(cstring &sPath)
{
	bool  bFound = false;
	FILE *fp	 = fopen("download.html", "rt");
	if (fp) {
		char *sLine = new char[1024 * 1024];
		while (fgets(sLine, 1024 * 1024, fp)) {
			char *sTok = strstr(sLine, "/msys2-installer/tree/");
			if (!sTok)
				continue;
			strcpy(sLine, sTok + 22);
			sTok = strstr(sLine, "\"");
			if (!sTok)
				continue;
			*sTok = NULL;
			sPath.Format("https://github.com/msys2/msys2-installer/releases/%s", sLine);
			{
				char sShort[1024];
				strcpy(sShort, sLine);
				while ((sTok = strstr(sShort, "-")) != 0) {
					strcpy(sTok, sTok + 1);
				}

				sPath.Format(
					"https://github.com/msys2/msys2-installer/releases/download/%s/msys2-base-x86_64-%s.tar.xz", sLine,
					sShort);
			}

			bFound = true;
			printf("URL found : %s\n", sPath.c_str());
			break;
		}
		fclose(fp);
		system("del /Q download.html");
	}

	return bFound;
}

int main(int argc, const char *argv[])
{
	ArgTable arg_table("Tool auto downloader for TestDrive Profiling Master (" __DATE__ ")");
	arg_table.AddOptionString("tool", NULL, NULL, NULL, "tool_name", "Downloading tool target name.");
	arg_table.AddRemark(NULL, "-- List of downloadable tools --");
	arg_table.AddRemark(NULL, "eclipse    : Eclipse CDT");
	arg_table.AddRemark(NULL, "notepad++  : NotePad++");
	arg_table.AddRemark(NULL, "mingw      : MinGWx64 + MSys2");
	arg_table.AddRemark(NULL, "jre        : Java Runtime Environment");

	if (!arg_table.DoParse(argc, argv))
		return 0;

	cstring sToolName(arg_table.GetOptionString("tool"));
	cstring sPath, sCommand;

	if (sToolName == "eclipse") {
		system("curl https://www.eclipse.org/downloads/packages/ -o download.html");

		if (GetDownloadPathEclipse(sPath)) {
			sCommand.Format("curl -L \"https://%s&r=1\" -o eclipse.zip", sPath.c_str());

			printf("Downloading Eclipse for x64\n:'https://%s'\n\n", sPath.c_str());
			system(sCommand);
		} else {
			printf("*E : Failed to access to Eclipse page...\n");
		}
	} else if (sToolName == "notepad++") {
		system("wget --no-check-certificate https://notepad-plus-plus.org -O download.html");
		if (GetDownloadPathNotepad(sPath)) {
			printf("path : %s\n", sPath.c_str());
			sCommand.Format("wget --no-check-certificate \"%s\" -O notepad.zip", sPath.c_str());
			printf("Downloading notepad++... : %s\n\n", sPath.c_str());
			system(sCommand);
		} else {
			printf("*E : Failed to access to notepad++ page...\n");
		}
	} else if (!strcmp(argv[1], "jre")) {
		// system("wget --no-cookies --no-check-certificate --header \"Cookie: oraclelicense =
		// accept-securebackup-cookie\" \"https://www.java.com/download/\" -O download.html");
		system("wget --no-cookies --no-check-certificate \"https://enos.itcollege.ee/~jpoial/allalaadimised/jdk8/\" -O "
			   "download.html");
		if (GetDownloadHtmlJRE(sPath)) {

			sCommand.Format("wget --no-cookies --no-check-certificate \"%s\" -O jre.exe", sPath.c_str());
			system(sCommand);
		}
	} else if (!strcmp(argv[1], "mingw")) {
		sCommand.Format(
			"wget --no-check-certificate \"https://github.com/msys2/msys2-installer/releases\" -O download.html");
		system(sCommand.c_str());
		if (GetDownloadPathMinGW(sPath)) {
			sCommand.Format("wget --no-check-certificate \"%s\" -O msys2.tar.xz", sPath.c_str());
			system(sCommand);

			if (!GetFileSize("msys2.tar.xz")) {
				sCommand.Format("wget --no-check-certificate "
								"\"https://github.com/msys2/msys2-installer/releases/download/nightly-x86_64/"
								"msys2-base-x86_64-latest.tar.xz\" -O msys2.tar.xz",
								sPath.c_str());
				system(sCommand);
			}
		}
	} else {
		printf("*E: Invalid tool name : %s\n", sToolName.c_str());
		return 1;
	}

	return 0;
}
