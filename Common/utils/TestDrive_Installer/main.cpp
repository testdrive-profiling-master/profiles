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
// Title : TestDrive installer Maker
// Rev.  : 2/26/2023 Sun (clonextop@gmail.com)
//================================================================================
#include "Common.h"
#include <ncurses/ncurses.h>
#include "indicators/indicators.hpp"
#include <git2.h>

using namespace indicators;

typedef struct {
	const char*			sRepo;
	const char*			sPath;
	const char*			sDesc;
} REPO;

REPO	__DefaultRepoList[] = {
	{"http://github.com/testdrive-profiling-master/release.git", "TestDrive", "TestDrive Profiling Master release"},
	{"https://github.com/testdrive-profiling-master/profiles.git", "Profiles", "TestDrive Profiling Master profiles"},
	{NULL, NULL}
};

string			__sInstallPath;
list<REPO>		g_RepoList;

typedef struct {
	int data;
} progress_data;

int git_certificate_check_cb(git_cert* cert, int valid, const char* host, void* payload)
{
	return 1;	// bypass checking SSL certification
}

int fetch_progress(const git_indexer_progress* stats, void* payload)
{
	progress_data* pd = (progress_data*)payload;
	return 0;
}

ProgressBar*	__pProgress	= NULL;
void checkout_progress(const char* path, size_t cur, size_t tot, void* payload)
{
	progress_data* pd = (progress_data*)payload;
	cstring	sProgress;

	if(cur == tot) {
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
	if(!__pProgress) {
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
			option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
		};
		__pProgress->set_progress(0);
	}
}

void finish_progress(void)
{
	if(__pProgress) {
		delete __pProgress;
		__pProgress	= NULL;
		show_console_cursor(true);
	}
}

bool IsDirectoryExists(const char* path)
{
	struct stat info;

	if(stat(path, &info) != 0)
		return false;

	if(info.st_mode & S_IFDIR)
		return true;

	return false;
}

progress_data d = {0};
git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;

bool DoInstall(void)
{
	bool	bRet	= true;
	git_repository* repo;
	const git_config_entry* entry = NULL;
	int error;
	git_libgit2_init();
	clone_opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
	clone_opts.checkout_opts.progress_cb = checkout_progress;
	clone_opts.checkout_opts.progress_payload = &d;
	clone_opts.fetch_opts.callbacks.transfer_progress = fetch_progress;
	clone_opts.fetch_opts.callbacks.certificate_check = git_certificate_check_cb;
	clone_opts.fetch_opts.callbacks.payload = &d;
	int iIndex	= 0;

	for(auto& i : g_RepoList) {
		iIndex++;
		printf("\n\n[%d/%d] *** Install %s...\n", iIndex, g_RepoList.size(), i.sDesc);

		if(IsDirectoryExists(i.sPath)) {
			printf("*E: '%s' is already existed.\n", i.sPath);
			bRet	= false;
			break;
		}

		start_progress();
		error = git_clone(&repo, i.sRepo, i.sPath, &clone_opts);
		finish_progress();

		if(error < 0) {
			const git_error* e = git_error_last();
			printf("\n*E: [%d/%d] %s\n", error, e->klass, e->message);
			bRet	= false;
			break;
		}
	}

	git_repository_free(repo);
	git_libgit2_shutdown();

	system("start explorer .");
	Sleep(3000);
	if(bRet) {
		system("start TestDrive/TestDrive.exe");
	}

	return bRet;
}

int main(int argc, const char* argv[])
{
	bool		bError	= false;
	ArgTable	main_arg_table("TestDrive Installer");
	main_arg_table.AddOptionFile("install_path", NULL, NULL, NULL, "install_path", "set install path");

	if(!main_arg_table.DoParse(argc, argv))
		return 1;

	__sInstallPath	= main_arg_table.GetOptionFile("install_path");;
	printf("Installing Target : %s\n", __sInstallPath.c_str());
	DIR* dir = opendir(__sInstallPath.c_str());

	for(int i = 0; __DefaultRepoList[i].sRepo != NULL; i++) {
		g_RepoList.push_back(__DefaultRepoList[i]);
	}

	if(dir) {
		closedir(dir);
		chdir(__sInstallPath.c_str());

		if(!DoInstall()) {
			printf("*E: Installation is failed...\n");
			bError	= true;
		} else {
			printf("Done!\n");
		}
	} else {
		printf("*E: Directory is not exist : %s\n", __sInstallPath.c_str());
		bError	= true;
	}

	if(bError) {
		printf("\n* Press any key to exit.\n");
		getchar();
	}
	return bError ? 1 : 0;
}
