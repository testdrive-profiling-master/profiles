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
// Title : ImageIdentify project
// Rev.  : 1/11/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "UtilFramework.h"
#include <filesystem>

using namespace std;

int main(int argc, const char* argv[])
{
	// temporary : imagestick is unstable now..
	if(argc == 2) {
		cstring sProfilePath;
		cstring s;
		sProfilePath.GetEnvironment("TESTDRIVE_PROFILE");
		s.Format("MAGICK_HOME=%sCommon\\bin\\ImageMagick", sProfilePath.c_str());
		s.Replace("/", "\\");
		putenv(s);
		s.Format("MAGICK_CONFIGURE_PATH=%sCommon\\bin\\ImageMagick", sProfilePath.c_str());
		s.Replace("/", "\\");
		putenv(s);
		s.Format("MAGICK_CODER_MODULE_PATH=%sCommon\\bin\\ImageMagick\\modules\\coders", sProfilePath.c_str());
		s.Replace("/", "\\");
		putenv(s);
		s.Format("MAGICK_CODER_FILTER_PATH=%sCommon\\bin\\ImageMagick\\modules\\filters", sProfilePath.c_str());
		s.Replace("/", "\\");
		putenv(s);
		cstring sFilename = filesystem::absolute(filesystem::path(argv[1])).string().c_str();
		cstring sMagick;
		char sCurPath[4096];
		GetCurrentDirectory(4096, sCurPath);
		{
			cstring sMagickPath;
			sMagickPath.Format("%sCommon\\bin\\ImageMagick", sProfilePath.c_str());
			sMagickPath.Replace("/", "\\");
			SetCurrentDirectory(sMagickPath);
			cstring sCmd;

			if(sFilename.CompareBack(".wmf"))
				sCmd.Format("magick identify \"%s\"", sFilename.c_str());
			else
				sCmd.Format("magick identify -size x \"%s\"", sFilename.c_str());

			system(sCmd);
		}
		SetCurrentDirectory(sCurPath);
	}

	return 0;
}
