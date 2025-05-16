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
// Title : WebGUI project
// Rev.  : 5/16/2025 Fri (clonextop@gmail.com)
//================================================================================
#include "WebGUI.h"
#include <time.h>
#include <stdarg.h>

using namespace webview;

#define WEBGUI_MIN_PORT 40000
#define WEBGUI_MAX_PORT 50000

WebGUI::WebGUI(bool bDebug) : httpServer(bDebug), browser_engine(bDebug, nullptr), m_FullScreen({0})
{
	srand((unsigned int)time(NULL)); // initialize for use rand()
	m_hHwnd		   = NULL;
	m_sRootPath	   = "./";
	m_iPort		   = 0;
	m_Mode		   = WEBGUI_MODE_STANALONE;
	m_bEnableClose = true;
	m_pIcon		   = NULL;
}
WebGUI::~WebGUI(void)
{
#ifdef WIN32
	if (m_pIcon) {
		DeleteObject((HICON)m_pIcon);
		m_pIcon = NULL;
	}
#endif
}

bool WebGUI::Initialize(WEBGUI_MODE mode, uint16_t iPort, const char *sHttpsKey, const char *sHttpsCert, const char *sRootCa)
{
	if (m_hHwnd)
		return false;
	m_iPort			   = 0;
	m_Mode			   = mode;
	bool bInternalOnly = (mode == WEBGUI_MODE_STANALONE);
	if (mode != WEBGUI_MODE_CLIENT) {
		if (iPort) {
			if (!httpServer::Initialize(iPort, bInternalOnly, sHttpsKey, sHttpsCert, sRootCa)) {
				return false;
			}
		} else {
			// find free port.
			for (int i = WEBGUI_MIN_PORT; i <= WEBGUI_MAX_PORT; i++) {
				iPort = WEBGUI_MIN_PORT + (((uint32_t)rand()) % (WEBGUI_MAX_PORT - WEBGUI_MIN_PORT + 1));
				if (httpServer::Initialize(iPort, bInternalOnly, sHttpsKey, sHttpsCert, sRootCa)) {
					if (bInternalOnly)
						m_iPort = iPort;
					break;
				}
			}
			if (!m_iPort)
				return false;
		}
	} else {
		m_iPort = iPort ? iPort : 80;
	}

	m_hHwnd = WindowHandle();
	return m_hHwnd ? OnInitialize() : false;
}

void WebGUI::SetRootPath(const char *sRootPath)
{
	m_sRootPath = sRootPath;
	m_sRootPath.Replace("\\", "/", true);
	if (!m_sRootPath.CompareBack("/"))
		m_sRootPath += "/";
}

void WebGUI::SetIcon(const char *sFileName)
{
#ifdef _WIN32
	if (m_hHwnd) {
		if (m_pIcon) {
			DeleteObject((HICON)m_pIcon);
		}
		m_pIcon = (void *)(HICON)LoadImage(NULL, sFileName, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
		SendMessage(m_hHwnd, WM_SETICON, ICON_BIG, (LPARAM)m_pIcon);
		SendMessage(m_hHwnd, WM_SETICON, ICON_SMALL, (LPARAM)m_pIcon);
	}
#endif
}

void WebGUI::SetTitle(const char *sTitle)
{
	if (sTitle) {
		set_title(sTitle);
	}
}

void WebGUI::SetSize(int iWidth, int iHeight, WEBGUI_SIZE hint)
{
	if (iWidth > 0 && iHeight > 0)
		set_size(iWidth, iHeight, (webview_hint_t)hint);

#ifdef _WIN32
	if (hint == WEBGUI_SIZE_MIN || hint == WEBGUI_SIZE_FIXED) {
		RECT rc;
		GetWindowRect(m_hHwnd, &rc);
		int cur_width  = rc.right - rc.left;
		int cur_height = rc.bottom - rc.top;
		if (cur_width < iWidth || cur_height < iHeight) {
			set_size(iWidth, iHeight, WEBVIEW_HINT_NONE);
		}
	}
#endif
}

bool WebGUI::OnInitialize(void)
{
	return true;
}

bool WebGUI::OnClose(void)
{
	return m_bEnableClose;
}

bool WebGUI::OnGet(const char *sURL, httpConnection *pCon)
{
	// default web pages.
	FILE *fp = fopen(m_sRootPath + sURL, "rb+"); // must be a file (for linux)
	if (!fp) {
		static const char *__search_link[] = {"/index.html", ".html", ".js", NULL};
		for (int i = 0; __search_link[i]; i++) {
			if ((fp = fopen(m_sRootPath + sURL + __search_link[i], "rb+")) != NULL)
				break;
		}
		// error case
		if (!fp)
			fp = fopen(m_sRootPath + "404_error.html", "rb+");
	}

	if (fp) {
		fseek(fp, 0, SEEK_END);
		size_t iByteSize = ftell(fp);

		if (iByteSize > 0) {
			BYTE *pData = new BYTE[iByteSize];
			fseek(fp, 0, SEEK_SET);
			fread(pData, iByteSize, 1, fp);
			pCon->SetData(pData, iByteSize);
		}

		fclose(fp);
		return true;
	}

	return false;
}

bool WebGUI::OnPost(const char *sURL, httpConnection *pCon)
{
	// nulled return
	return false;
}

void WebGUI::EnableWinButton(WEBGUI_BTN btn, bool bEnable)
{
#ifdef _WIN32
	if (m_hHwnd) {
		HMENU menu = GetSystemMenu(m_hHwnd, FALSE);
		switch (btn) {
		case WEBGUI_BTN_MINIMIZE:
			EnableMenuItem(menu, SC_MINIMIZE, MF_BYCOMMAND | (bEnable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
			break;
		case WEBGUI_BTN_MAXIMIZE:
			EnableMenuItem(menu, SC_MAXIMIZE, MF_BYCOMMAND | (bEnable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
			break;
		case WEBGUI_BTN_CLOSE:
			EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND | (bEnable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
			m_bEnableClose = bEnable;
			break;
		}
	}
#endif
}

void WebGUI::SetFullScreen(bool bEnable)
{
	if (m_FullScreen.bEanbled == bEnable && m_hHwnd)
		return;
	m_FullScreen.bEanbled = bEnable;

	if (bEnable) { // set full screen
#ifdef _WIN32
		m_FullScreen.style_ex = GetWindowLongPtr(m_hHwnd, GWL_EXSTYLE);
		m_FullScreen.style	  = GetWindowLongPtr(m_hHwnd, GWL_STYLE);
		GetWindowRect(m_hHwnd, &m_FullScreen.rc);
		SetWindowLongPtr(m_hHwnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
		SetWindowLongPtr(m_hHwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		ShowWindow(m_hHwnd, SW_MAXIMIZE);
#endif
	} else { // restore normal screen
#ifdef _WIN32
		SetWindowLongPtr(m_hHwnd, GWL_EXSTYLE, m_FullScreen.style_ex);
		SetWindowLongPtr(m_hHwnd, GWL_STYLE, m_FullScreen.style);
		ShowWindow(m_hHwnd, SW_NORMAL);

		SetWindowPos(
			m_hHwnd, NULL, m_FullScreen.rc.left, m_FullScreen.rc.top, m_FullScreen.rc.right - m_FullScreen.rc.left,
			m_FullScreen.rc.bottom - m_FullScreen.rc.top, SWP_SHOWWINDOW);
#endif
	}
}

noresult WebGUI::run_impl()
{
#ifdef _WIN32
	static WNDPROC proc_fn;
	WNDPROC		   new_proc = +[](HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) -> LRESULT {
		   WebGUI *pImp = (WebGUI *)((win32_edge_engine *)GetWindowLongPtrW(hwnd, GWLP_USERDATA));
		   switch (msg) {
		   case WM_CLOSE: {
			   if (!pImp->OnClose()) {
				   return 0;
			   }
		   } break;
		   }
		   return proc_fn(hwnd, msg, wp, lp);
	};
	proc_fn = (WNDPROC)GetWindowLongPtrW(m_hHwnd, GWLP_WNDPROC);
	SetWindowLongPtr(m_hHwnd, GWLP_WNDPROC, (LONG_PTR)new_proc);

	return win32_edge_engine::run_impl();
#endif
}

bool WebGUI::Navigate(const char *sURL)
{
	if (sURL) {
		try {
			navigate(sURL);
		} catch (const webview::exception &e) {
			LOGE("%s\n", (const char *)e.what());
			return false;
		}
		return true;
	}
	return false;
}

bool WebGUI::SetHtml(const char *sHtml)
{
	if (!sHtml)
		return false;
	try {
		set_html(sHtml);
	} catch (const webview::exception &e) {
		LOGE("%s\n", (const char *)e.what());
		return false;
	}
	return true;
}

bool WebGUI::Run(const char *sURL)
{
	cstring sUrl(sURL);
	sUrl = sURL ? sURL : "http://127.0.0.1";
	if (m_iPort && (m_iPort != 80))
		sUrl.AppendFormat(":%d", m_iPort);
	try {
		navigate(sUrl.c_str());
		run();
	} catch (const webview::exception &e) {
		LOGE("%s\n", (const char *)e.what());
		return false;
	}

	return true;
}

bool WebGUI::CallJScript(const char *sFormat, ...)
{
	bool bRet = true;
	if (sFormat) {
		int		iLen = 0;
		va_list vaArgs;
		va_start(vaArgs, sFormat);
		{
			// get size only
			va_list vaCopy;
			va_copy(vaCopy, vaArgs);
			iLen = std::vsnprintf(NULL, 0, sFormat, vaCopy);
			va_end(vaCopy);
		}
		{
			char			 *pBuff = new char[iLen + 1];
			std::vector<char> zc(iLen + 1);
			std::vsnprintf(pBuff, iLen + 1, sFormat, vaArgs);
			va_end(vaArgs);

			try {
				eval(pBuff);
			} catch (const webview::exception &e) {
				LOGE("%s\n", (const char *)e.what());
				bRet = false;
			}

			delete[] pBuff;
		}
	}

	return bRet;
}

bool WebGUI::Bind(const char *name, WebGUI_binding_t fn, void *pPrivate)
{
	if (name && *name) {
		auto wrapper = [this, fn](const std::string &id, const std::string &req, void *pPrivate) {
			ARGS args;
			{
				Json::Reader reader;
				reader.parse(req, args);
			}

			cstring result;
			fn(args, result, pPrivate);
			resolve(id, 0, result.c_string());
		};
		try {
			bind(name, wrapper, pPrivate);
		} catch (const webview::exception &e) {
			LOGE("%s\n", (const char *)e.what());
			return false;
		}
	} else
		return false;
	return true;
}

bool WebGUI::Unbind(const char *sName)
{
	if (sName) {
		string s(sName);
		try {
			unbind(s);
		} catch (const webview::exception &e) {
			LOGE("%s\n", (const char *)e.what());
			return false;
		}
	}
	return true;
}

void WebGUI::Terminate(void)
{
	terminate();
}

WebGUI::WIN_HANDLE WebGUI::WindowHandle(void)
{
	try {
		return (WIN_HANDLE)window().value();
	} catch (const webview::exception &e) {
		LOGE("%s\n", (const char *)e.what());
	}
	return NULL;
}
