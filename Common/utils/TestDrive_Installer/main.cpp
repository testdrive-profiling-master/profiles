//================================================================================
// Copyright (c) 2013 ~ 2025. HyungKi Jeong(clonextop@gmail.com)
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
// Title : TestDrive installer Maker
// Rev.  : 9/16/2025 Tue (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <ncurses/ncurses.h>
#include "indicators/indicators.hpp"
#include <filesystem>
#include <git2.h>

using namespace indicators;

typedef struct {
	cstring sRepo;
	cstring sPath;
	cstring sDesc;
} REPO;

REPO __DefaultRepoList[] = {
	{"http://github.com/testdrive-profiling-master/release.git", "TestDrive", "TestDrive Profiling Master release"},
	{"https://github.com/testdrive-profiling-master/profiles.git", "Profiles", "TestDrive Profiling Master profiles"},
};

cstring	   __sCurrentPath;
string	   __sInstallPath;
list<REPO> g_RepoList;

typedef struct {
	int data;
} progress_data;

void EnableStdinEcho(bool enable = true)
{
#ifdef WIN32
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD  mode;
	GetConsoleMode(hStdin, &mode);

	if (!enable)
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode);
#else
	struct termios tty;
	tcgetattr(STDIN_FILENO, &tty);

	if (!enable)
		tty.c_lflag &= ~ECHO;
	else
		tty.c_lflag |= ECHO;

	(void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

int git_credentials_cb(git_cred **out, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload)
{
	string sID, sPW;

	// clear current line
	for (int i = 0; i < 120; i++) printf(" ");

	for (int i = 0; i < 120; i++) printf("\b");

	show_console_cursor(true);
	printf("     > Credential login.\n");
	printf("       * Enter ID       : ");
	getline(cin, sID);
	printf("       * Enter password : ");
	EnableStdinEcho(false);
	getline(cin, sPW);
	EnableStdinEcho(true);
	show_console_cursor(false);
	return git_cred_userpass_plaintext_new(out, sID.c_str(), sPW.c_str());
}

int git_certificate_check_cb(git_cert *cert, int valid, const char *host, void *payload)
{
	return 1; // bypass checking SSL certification
}

int fetch_progress(const git_indexer_progress *stats, void *payload)
{
	return 0;
}

ProgressBar *__pProgress = NULL;
void		 checkout_progress(const char *path, size_t cur, size_t tot, void *payload)
{
	cstring sProgress;

	if (cur == tot) {
		sProgress.Format("Clone complete : %d/%d", cur, tot);
		__pProgress->set_option(option::PostfixText{sProgress.c_str()});
		__pProgress->mark_as_completed();
	} else {
		sProgress.Format("Download %3d%% : %d/%d", (cur * 100) / tot, cur, tot);
		__pProgress->set_option(option::PostfixText{sProgress.c_str()});
		__pProgress->set_progress((cur * 100.f) / tot);
	}
}

void start_progress(void)
{
	if (!__pProgress) {
		show_console_cursor(false);
		__pProgress = new ProgressBar{
			option::BarWidth{80},
			option::Start{"["},
			option::End{"]"},
			option::Fill{"="},
			option::Lead{">"},
			option::Remainder{" "},
			option::ShowRemainingTime{true},
			option::PostfixText{"Gathering information..."},
			option::ForegroundColor{Color::white},
			option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
		__pProgress->set_progress(0);
	}
}

void finish_progress(void)
{
	if (__pProgress) {
		delete __pProgress;
		__pProgress = NULL;
		show_console_cursor(true);
	}
}

bool IsDirectoryExists(const char *path)
{
	struct stat info;

	if (stat(path, &info) != 0)
		return false;

	if (info.st_mode & S_IFDIR)
		return true;

	return false;
}

progress_data	  d			 = {0};
git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;

bool			  DoInstall(void)
{
	bool			bRet = true;
	git_repository *repo;
	int				error;
	git_libgit2_init();
	clone_opts.checkout_opts.checkout_strategy		  = GIT_CHECKOUT_SAFE;
	clone_opts.checkout_opts.progress_cb			  = checkout_progress;
	clone_opts.checkout_opts.progress_payload		  = &d;
	clone_opts.fetch_opts.callbacks.transfer_progress = fetch_progress;
	clone_opts.fetch_opts.callbacks.certificate_check = git_certificate_check_cb;
	clone_opts.fetch_opts.callbacks.credentials		  = git_credentials_cb;
	clone_opts.fetch_opts.callbacks.payload			  = &d;
	int iIndex										  = 0;

	for (auto &i : g_RepoList) {
		iIndex++;
		printf("\n\n[%d/%d] *** Install %s...\n", iIndex, g_RepoList.size(), i.sDesc.c_str());
		printf("     > GIT from   : %s\n", i.sRepo.c_str());
		printf("     > Install to : %s\n", i.sPath.c_str());

		if (IsDirectoryExists(i.sPath.c_str())) {
			printf("*W: Git clone('%s') is already existed.\n", i.sPath.c_str());
		} else {
			start_progress();
			error = git_clone(&repo, i.sRepo.c_str(), i.sPath.c_str(), &clone_opts);
			finish_progress();

			if (error < 0) {
				const git_error *e = git_error_last();
				printf("\n*E: [%d/%d] %s\n", error, e->klass, e->message);
				bRet = false;
				break;
			}
		}
	}

	git_repository_free(repo);
	git_libgit2_shutdown();
	system("start explorer .");
	Sleep(3000);

	if (bRet) {
		system("start TestDrive/TestDrive.exe");
	}

	return bRet;
}

string GetCustomRepoInfo(int iIndex, const char *sKey)
{
	char	sData[4096];
	cstring sApp;
	sApp.Format("REPO_%d", iIndex);
	GetPrivateProfileString(sApp, sKey, "", sData, 4096, __sCurrentPath + "/testdrive_installer.ini");
	return sData;
}

int main(int argc, const char *argv[])
{
	bool	 bError = false;
	ArgTable main_arg_table("TestDrive Installer");
	main_arg_table.AddOptionFile("install_path", NULL, NULL, NULL, "install_path", "set install path");

	if (!main_arg_table.DoParse(argc, argv))
		return 1;

	__sCurrentPath = filesystem::current_path().string();
	__sInstallPath = main_arg_table.GetOptionFile("install_path");
	;
	printf("Installing Target : %s\n", __sInstallPath.c_str());
	DIR *dir = opendir(__sInstallPath.c_str());

	// set default repo. path
	for (int i = 0; i < ARRAY_SIZE(__DefaultRepoList); i++) {
		g_RepoList.push_back(__DefaultRepoList[i]);
	}

	// additional repo. path
	for (int i = 0; GetCustomRepoInfo(i, "URL").size(); i++) {
		REPO repo;
		repo.sDesc = GetCustomRepoInfo(i, "DESC");
		repo.sRepo = GetCustomRepoInfo(i, "URL");
		repo.sPath = GetCustomRepoInfo(i, "PATH");
		g_RepoList.push_back(repo);
	}

	if (dir) {
		closedir(dir);
		chdir(__sInstallPath.c_str());

		if (!DoInstall()) {
			printf("*E: Installation is failed...\n");
			bError = true;
		} else {
			printf("Done!\n");
		}
	} else {
		printf("*E: Directory is not exist : %s\n", __sInstallPath.c_str());
		bError = true;
	}

	if (bError) {
		printf("\n* Press any key to exit.\n");
		getchar();
	}

	return bError ? 1 : 0;
}
