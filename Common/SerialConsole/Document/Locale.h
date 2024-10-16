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
// Title : Serial console
// Rev.  : 10/16/2024 Wed (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

#define PROFILE_NAME "Serial console"
/* clang-format off */
LOCALE_DEFINE(3, LANG_KOREAN, LANG_JAPANESE, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("시리얼 콘솔"),
	_T("シリアルコンソール"),
	_T("Serial console"))
LOCALE_TEXT(COM_PORT,
	_T("COM 포트"),
	_T("COMポート"),
	_T("COM. Port"))
LOCALE_TEXT(DESC_COM_PORT,
	_T("COM 포트를 지정합니다."),
	_T("COMポートを指定します。"),
	_T("Specify the COM port."))
LOCALE_TEXT(BAUDRATE,
	_T("전송 속도"),
	_T("転送速度"),
	_T("Baud rate"))
LOCALE_TEXT(DESC_BAUDRATE,
	_T("전송 속도를 지정합니다."),
	_T("転送速度を指定します。"),
	_T("Specify the baud rate."))
LOCALE_TEXT(DATA_BITS,
	_T("데이터 비트"),
	_T("データビット"),
	_T("Data bits"))
LOCALE_TEXT(DESC_DATA_BITS,
	_T("데이터 비트 수를 지정합니다."),
	_T("データビットを指定します。"),
	_T("Specify the data bits."))
LOCALE_TEXT(PARITY,
	_T("패리티 비트"),
	_T("パリティビット"),
	_T("Parity bits"))
LOCALE_TEXT(DESC_PARITY,
	_T("패리티 비트를 지정합니다."),
	_T("パリティビットを指定します。"),
	_T("Specify the parity bits."))
LOCALE_TEXT(STOP_BITS,
	_T("스탑 비트"),
	_T("ストップビット"),
	_T("Stop bits"))
LOCALE_TEXT(DESC_STOP_BITS,
	_T("스탑 비트를 지정합니다."),
	_T("ストップビットを指定します。"),
	_T("Specify the stop bits."))
LOCALE_TEXT(FLOW_CONTROL,
	_T("흐름 제어"),
	_T("フロー制御"),
	_T("Flow control"))
LOCALE_TEXT(DESC_FLOW_CONTROL,
	_T("흐름 제어를 지정합니다."),
	_T("フロー制御を指定します。"),
	_T("Specify the flow control."))
LOCALE_TEXT(CONNECTABLE,
	_T("(연결가능)"),
	_T("(接続可能)"),
	_T("(Connectable)"))
LOCALE_TEXT(UNCONNECTABLE,
	_T("(연결불가)"),
	_T("(接続不可)"),
	_T("(Unconnectable)"))
LOCALE_TEXT(CONNECTION_OCCUPIED,
	_T("(사용중)"),
	_T("(使用中)"),
	_T("(Occupied)"))
LOCALE_TEXT(AUTODETECT,
	_T("(자동검색)"),
	_T("(自動検出)"),
	_T("(Auto detect)"))
LOCALE_TEXT(NO_AVAILABLE_PORT,
	_T("사용 가능한 시리얼 포트를 찾을 수 없습니다."),
	_T("使用可能なシリアルポートを見つけることができませんでした。"),
	_T("No available serial port."))
LOCALE_TEXT(PORT_IS_NOT_AVAILABLE,
	_T("지정된 시리얼 포트(%s)는 존재하지 않습니다. 「자동검색」을 설정하거나 다른 포트를 지정하십시오."),
	_T("指定されたシリアルポート（%s）は存在しません。「自動検索」を設定するか、別のポートを指定してください。"),
	_T("The specified serial port (%s) does not exist. Set 'Auto detect' or specify a different port."))
LOCALE_TEXT(PORT_IS_ALREADY_IN_USE,
	_T("지정된 시리얼 포트(%s)는 이미 사용중입니다. 「자동검색」을 설정하거나 다른 포트를 지정하십시오."),
	_T("指定されたシリアルポート(%s)はすでに使用中です。「自動検索」を設定するか、別のポートを指定してください。"),
	_T("The specified serial port(%s) is already in use. Set 'Auto detect' or specify a different port."))
/* clang-format on */
LOCALE_END
