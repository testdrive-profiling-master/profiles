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
// Rev.  : 4/5/2023 Wed (clonextop@gmail.com)
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

static void __EnableStdinEcho(bool enable = true)
{
#ifdef WIN32
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);

	if(!enable)
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode);
#else
	struct termios tty;
	tcgetattr(STDIN_FILENO, &tty);

	if(!enable)
		tty.c_lflag &= ~ECHO;
	else
		tty.c_lflag |= ECHO;

	(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

static int __credentials_cb(git_cred** out, const char* url, const char* username_from_url, unsigned int allowed_types, void* payload)
{
	minGit*	pGit	= (minGit*)payload;

	if(pGit->sUser.empty()) {
		show_console_cursor(true);
		printf("     > Credential login.\n");
		printf("       * Enter ID       : ");
		getline(cin, pGit->sUser);
		printf("       * Enter password : ");
		__EnableStdinEcho(false);
		getline(cin, pGit->sPassword);
		__EnableStdinEcho(true);
		show_console_cursor(false);
	}

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

bool Pull(const char* sPath)
{
	bool			bRet	= false;
	git_repository* repo	= NULL;
	git_remote*		remote	= NULL;
	git_reference*	local_master		= nullptr;
	git_reference*	origin_master		= nullptr;
	git_index*		index				= nullptr;
	git_commit*		their_commit		= nullptr;
	git_commit*		our_commit			= nullptr;
	git_oid			new_tree_id;
	git_tree*		new_tree			= nullptr;
	git_signature*	me					= nullptr;
	git_oid			commit_id;
	int				error;
	git_repository_index(&index, repo);

	// open git
	if(git_repository_open(&repo, sPath) < 0) goto EXIT;

	// get remote
	if(git_remote_lookup(&remote, repo, "origin") < 0) goto EXIT;

	// git fetch
	{
		git_fetch_options	fetch_opts		= GIT_FETCH_OPTIONS_INIT;
		fetch_opts.callbacks.credentials	= __credentials_cb;

		//fetch_opts.prune = GIT_FETCH_PRUNE;
		if(git_remote_fetch(remote, NULL, &fetch_opts, NULL) < 0) goto EXIT;
	}
	// merge local from remote
	{
		git_merge_options merge_opt			= GIT_MERGE_OPTIONS_INIT;
		git_checkout_options checkout_opt	= GIT_CHECKOUT_OPTIONS_INIT;
		const git_annotated_commit* their_head[10];

		if(git_branch_lookup(&local_master, repo, "master", GIT_BRANCH_LOCAL) < 0) goto EXIT;

		if(git_branch_lookup(&origin_master, repo, "origin/master", GIT_BRANCH_REMOTE) < 0) goto EXIT;

		// set local to head
		if(git_repository_set_head(repo, git_reference_name(local_master)) < 0) goto EXIT;

		// and merge
		if(git_annotated_commit_from_ref((git_annotated_commit**)their_head, repo, origin_master) < 0) goto EXIT;

		if(git_merge(repo, their_head, 1, &merge_opt, &checkout_opt)) goto EXIT;
	}
	// resolve conflicts

	if(git_index_has_conflicts(index)) {
		git_index_conflict_iterator* conflict_iterator	= nullptr;
		const git_index_entry* ancestor_out				= nullptr;
		const git_index_entry* our_out					= nullptr;
		const git_index_entry* their_out				= nullptr;
		git_index_conflict_iterator_new(&conflict_iterator, index);

		while(git_index_conflict_next(&ancestor_out, &our_out, &their_out, conflict_iterator) != GIT_ITEROVER) {
			if(ancestor_out) std::cout << "ancestor: " << ancestor_out->path << std::endl;

			if(our_out) std::cout << "our: " << our_out->path << std::endl;

			if(their_out) std::cout << "their: " << their_out->path << std::endl;
		}

		// git checkout --theirs <file>
		{
			git_checkout_options	opt	= GIT_CHECKOUT_OPTIONS_INIT;
			opt.checkout_strategy		|= GIT_CHECKOUT_USE_THEIRS;
			git_checkout_index(repo, index, &opt);
			git_index_conflict_iterator_free(conflict_iterator);
		}
	}

	// add & commit
	git_commit_lookup(&their_commit, repo, git_reference_target(origin_master));
	git_commit_lookup(&our_commit, repo, git_reference_target(local_master));
	git_index_update_all(index, nullptr, nullptr, nullptr);
	git_index_write(index);
	git_index_write_tree(&new_tree_id, index);
	git_tree_lookup(&new_tree, repo, &new_tree_id);
	git_signature_now(&me, "name", "email@address");
	git_commit_create_v(&commit_id, repo, git_reference_name(local_master), me, me, "UTF-8", "pull commit", new_tree, 2, our_commit, their_commit);
	bRet	= true;
EXIT:

	if(!bRet) {
		const git_error*	e	= giterr_last();
		LOGE("GITERROR[%d] %s", e->klass, e->message);
	}

	if(repo) {
		// cleanup
		git_repository_state_cleanup(repo);
		git_repository_free(repo);
		repo	= NULL;
	}

	return bRet;
}
