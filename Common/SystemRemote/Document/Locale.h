//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : System remote
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

#define	PROFILE_NAME	"System Remote"

LOCALE_DEFINE(3, LANG_KOREAN, LANG_JAPANESE, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("시스템 리모트"),
	_T("システムのリモート"),
	_T("System Remote"))
LOCALE_TEXT(USAGE,
	_T("'시스템 리모트'를 사용하려면 속성 설정 후 재시작해야 합니다."),
	_T("「システムのリモート」を使用するには、プロパティの設定後、再起動する必要があります。"),
	_T("To use the 'System Remote', must be restarted after the property is set."))
LOCALE_TEXT(RESTART_IS_REQUIRED,
	_T("설정을 적용하려면, 재시작이 필요합니다."),
	_T("設定を適用するには、再起動が必要になります。"),
	_T("To apply the settings, restart is required."))
LOCALE_TEXT(FONT,
	_T("돋움체"),
	_T("arial"),
	_T("arial"))
LOCALE_TEXT(SERVER,
	_T("서버"),
	_T("サーバー"),
	_T("Server"))
LOCALE_TEXT(CLIENT,
	_T("클라이언트"),
	_T("クライアント"),
	_T("Client"))
LOCALE_TEXT(SERVER_PORT,
	_T("포트 번호"),
	_T("ポート番号"),
	_T("Port number"))
LOCALE_TEXT(SERVER_IP,
	_T("서버 IP 주소"),
	_T("サーバーIPアドレス"),
	_T("Server IP Address"))
LOCALE_TEXT(DESC_SERVER_ENABLE,
	_T("서버를 활성화 합니다."),
	_T("サーバを起動します。"),
	_T("Enable the server."))
LOCALE_TEXT(DESC_SERVER_PORT,
	_T("포트 번호를 지정합니다."),
	_T("ポート番号を指定します。"),
	_T("Specifies the port number."))
LOCALE_TEXT(DESC_SERVER_IP,
	_T("서버 IP 주소를 지정합니다.\n예)192.168.0.10"),
	_T("サーバーのIPアドレスを指定します。\n例)192.168.0.10"),
	_T("Specifies the IP address of the server.\nex.)192.168.0.10"))
LOCALE_TEXT(INITIALIZE_SYSTEM_REMOTE,
	_T("시스템 리모트를 초기화 합니다."),
	_T("システムのリモートを初期化します。"),
	_T("Initialize 'System Remote'."))
LOCALE_TEXT(CONNECTED_TO_SERVER_IS_SUCCEEDED,
	_T("서버(%s:%s)에 연결되었습니다."),
	_T("サーバー(%s:%s)に接続しました。"),
	_T("The connection to the server(%s:%s) was succeeded."))
LOCALE_TEXT(CONNECTED_TO_SERVER_IS_FAILED,
	_T("서버(%s:%s)에 연결이 실패했습니다."),
	_T("サーバー(%s:%s)への接続が失敗しました。"),
	_T("The connection to the server(%s:%s) has failed."))
LOCALE_END
