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
// Title : TestDrive codegen project
// Rev.  : 3/10/2023 Fri (clonextop@gmail.com)
//================================================================================
#include "minGit.h"
#include <git2.h>
#include "indicators/indicators.hpp"

using namespace indicators;

static int __certificate_check_cb(git_cert* cert, int valid, const char* host, void* payload)
{
	return 1;	// bypass checking SSL certification
}

static int __fetch_progress(const git_indexer_progress* stats, void* payload)
{
	return 0;
}

static void __checkout_progress(const char* path, size_t cur, size_t tot, void* payload)
{
	ProgressBar*	pProgress	= (ProgressBar*)payload;
	cstring			sProgress;

	if(cur == tot) {
		sProgress.Format("Clone complete : %d/%d", cur, tot);
		pProgress->set_option(option::PostfixText{sProgress.c_str()});
		pProgress->mark_as_completed();
	} else {
		sProgress.Format("Download %3d%% : %d/%d", (cur * 100) / tot, cur, tot);
		pProgress->set_option(option::PostfixText{sProgress.c_str()});
		pProgress->set_progress((cur * 100.f) / tot);
	}
}

static int __credentials_cb(git_cred** out, const char* url, const char* username_from_url, unsigned int allowed_types, void* payload)
{
	minGit*	pGit	= (minGit*)payload;
	return git_cred_userpass_plaintext_new(out, pGit->sUser.c_str(), pGit->sPassword.c_str());
}

static bool __IsDirectoryExists(const char* path)
{
	struct stat info;

	if(stat(path, &info) != 0)
		return false;

	if(info.st_mode & S_IFDIR)
		return true;

	return false;
}

static int __iRefCount	= 0;

minGit::minGit(void)
{
	if(!__iRefCount) git_libgit2_init();

	__iRefCount++;
}

minGit::~minGit(void)
{
	__iRefCount--;

	if(!__iRefCount) git_libgit2_shutdown();
}

bool minGit::Clone(const char* sGitURL, const char* sPath)
{
	bool			bRet	= false;
	git_repository* repo	= NULL;

	if(!sGitURL || !sPath) return false;

	ProgressBar*	pProgress = new ProgressBar{
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
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	clone_opts.checkout_opts.checkout_strategy			= GIT_CHECKOUT_SAFE;
	clone_opts.checkout_opts.progress_cb				= __checkout_progress;
	clone_opts.checkout_opts.progress_payload			= pProgress;
	clone_opts.fetch_opts.callbacks.transfer_progress	= __fetch_progress;
	clone_opts.fetch_opts.callbacks.certificate_check	= __certificate_check_cb;
	clone_opts.fetch_opts.callbacks.credentials			= __credentials_cb;
	clone_opts.fetch_opts.callbacks.payload				= this;

	if(__IsDirectoryExists(sPath)) {
		LOGE("Git clone('%s') target path('%s') is already existed.", sGitURL, sPath);
	} else {
		printf("* Git clone : %s\n", sGitURL);
		printf("      to    : %s\n", sPath);
		int	error = git_clone(&repo, sGitURL, sPath, &clone_opts);

		if(error < 0) {
			const git_error* e = git_error_last();
			LOGE("[%d/%d] %s", error, e->klass, e->message);
		} else {
			bRet	= true;
		}
	}

	if(repo) {
		git_repository_free(repo);
		repo	= NULL;
	}

	SAFE_DELETE(pProgress);
	return bRet;
}
