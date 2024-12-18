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
// Title : Test
// Rev.  : 11/8/2024 Fri (clonextop@gmail.com)
//================================================================================
#include "WebGUI.h"

class WebGUI_imp : public WebGUI
{
public:
	WebGUI_imp(void)
	{
		Bind("PressBtn", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"PressBtn(%d, '%s')\");", args[0].asInt(), args[1].asCString());
		});
		Bind("RadioBtn", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"RadioBtn(%s)\");", args[0].asCString());
		});
		Bind("CheckBtn", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"CheckBtn(%s)\");", args[0].asBool() ? "true" : "false");
		});
		Bind("SetText", [&](const ARGS &args, cstring &result, void *pPrivate) {
			CallJScript("$('#cpp_output').text(\"%s\");", args[0].asCString());
		});
		Bind("OnLoad", [&](const ARGS &args, cstring &result, void *pPrivate) {
#ifndef USE_DEBUG // prevent context menu
			CallJScript("document.body.setAttribute('oncontextmenu', 'return false;');");
#endif
		});
	}
	virtual ~WebGUI_imp(void) {}
};

int main(int argc, const char *argv[])
{
	WebGUI_imp gui;
	if (gui.Initialize()) {
		gui.SetRootPath("ui/");
		gui.SetTitle("GUI window");
		gui.SetSize(640, 480, WEBGUI_SIZE_MIN);
		gui.Run();
	}

	return 0;
}
