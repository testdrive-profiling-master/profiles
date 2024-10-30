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
// Title : Common profiles
// Rev.  : 10/30/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __WEB_GUI_H__
#define __WEB_GUI_H__
#include "webview.h"
#include "httpServer.h"
#include <json/json.h>

typedef enum {
	WEBGUI_SIZE_NORMAL,
	WEBGUI_SIZE_MIN,
	WEBGUI_SIZE_MAX,
	WEBGUI_SIZE_FIXED
} WEBGUI_SIZE;

typedef enum {
	WEBGUI_BTN_MINIMIZE,
	WEBGUI_BTN_MAXIMIZE,
	WEBGUI_BTN_CLOSE
} WEBGUI_BTN;

typedef enum {
	WEBGUI_MODE_STANALONE,
	WEBGUI_MODE_SERVER,
	WEBGUI_MODE_CLIENT
} WEBGUI_MODE;

class WebGUI : private webview::webview, private httpServer
{
public:
#ifdef _WIN32
	typedef HWND WIN_HANDLE;
#elif defined(__unix__)
	typedef GtkWidget *WIN_HANDLE;
#elif defined(__APPLE__)
	typedef id WIN_HANDLE;
#endif
	typedef Json::Value ARGS;

private:
	WIN_HANDLE	m_hHwnd;
	cstring		m_sRootPath;
	uint16_t	m_iPort;
	bool		m_bEnableClose;
	WEBGUI_MODE m_Mode;
	struct {
		bool bEanbled;
#ifdef _WIN32
		LONG_PTR style, style_ex;
		RECT	 rc;
#endif
	} m_FullScreen;

public:
	WebGUI(void);
	virtual ~WebGUI(void);

	using WebGUI_binding_t = std::function<void(const ARGS &, cstring &)>;

	// common
	bool Initialize(
		WEBGUI_MODE mode = WEBGUI_MODE_STANALONE, uint16_t iPort = 0, const char *sHttpsKey = NULL, const char *sHttpsCert = NULL,
		const char *sRootCa = NULL);
	// server side function
	void SetRootPath(const char *sRootPath);
	// client side function
	void	   SetTitle(const char *sTitle);
	void	   SetSize(int iWidth, int iHeight, WEBGUI_SIZE hint = WEBGUI_SIZE_NORMAL);
	void	   EnableWinButton(WEBGUI_BTN btn, bool bEnable = true);
	void	   SetFullScreen(bool bEnable = true);
	bool	   Navigate(const char *sURL);
	bool	   SetHtml(const char *sHtml);
	bool	   Run(const char *sURL = NULL);
	bool	   CallJScript(const char *sFormat, ...);
	bool	   Bind(const char *name, WebGUI_binding_t fn);
	bool	   Unbind(const char *sName);
	void	   Teminate(void);
	WIN_HANDLE WindowHandle(void);

protected:
	virtual bool OnInitialize(void);
	virtual bool OnClose(void);
	virtual bool OnGet(const char *sURL, httpConnection *pCon);
	virtual bool OnPost(const char *sURL, httpConnection *pCon);

private:
	webview::noresult run_impl() override;
};

#endif //__WEB_GUI_H__
