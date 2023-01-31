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
// Rev.  : 1/31/2023 Tue (clonextop@gmail.com)
//================================================================================
#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <io.h>

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

void checkout_progress(const char* path, size_t cur, size_t tot, void* payload)
{
	progress_data* pd = (progress_data*)payload;
	printf("PROGRESS: %d/%d\n", cur, tot);
}

progress_data d = {0};
git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;

int main(int argc, const char* argv[])
{
	printf("TestDrive Automatic Installer.\n");
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
	error = git_clone(&repo, "http://github.com/testdrive-profiling-master/release.git", "TestDrive", &clone_opts);

	if(error < 0) {
		const git_error* e = git_error_last();
		printf("Error %d/%d: %s\n", error, e->klass, e->message);
		exit(error);
	}

	/*{	// test
		char path[1024];
		getcwd(path, 1024);
		printf("patjh : %s\n", path);
		getchar();
	}*/
	git_repository_free(repo);
	git_libgit2_shutdown();
	return 0;
}
