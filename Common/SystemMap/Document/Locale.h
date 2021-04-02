//================================================================================
// Copyright (c) 2013 ~ 2021. HyungKi Jeong(clonextop@gmail.com)
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
// Title : System map
// Rev.  : 4/2/2021 Fri (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

#define	PROFILE_NAME	"System Map"

LOCALE_DEFINE(4, LANG_KOREAN, LANG_JAPANESE, LANG_SPANISH, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("시스템 맵"),
	_T("システムマップ"),
	_T("Mapa del sistema"),
	_T("System Map"))
LOCALE_TEXT(TEXT_EDITOR,
	_T("문서 편집기"),
	_T("テキストエディタ"),
	_T("Editor de texto"),
	_T("Text editor"))
LOCALE_TEXT(TEXT_EDITOR_DESC,
	_T("문서편집기를 선택합니다."),
	_T("テキスト·エディタを選択します。"),
	_T("Seleccione el editor de texto."),
	_T("Select the text editor."))
LOCALE_TEXT(CANT_FIND_FILE,
	_T("파일을 찾을 수 없습니다."),
	_T("ファイルが見つかりません。"),
	_T("No se encuentra el archivo."),
	_T("File is not found."))
LOCALE_TEXT(CANT_FIND_TAG_STRING,
	_T("태그 문자열을 찾을 수 없습니다. : %s"),
	_T("タグ文字列を見つけることができません。 : %s"),
	_T("No se puede encontrar la cadena de etiquetas. : %s"),
	_T("Can not find the tag string. : %s"))
LOCALE_TEXT(VISIO_INSTALLATION_IS_NEEDED,
	_T("'시스템 맵' 도큐먼트를 구동하기 위해서는, 마이크로소프트의 Visio 뷰어 64bit(무료버전)의 설치가 필요합니다."),
	_T("「システムマップ」ドキュメントを駆動するためには、マイクロソフトのVisioビューア64bit（無料版）のインストールが必要です。"),
	_T("Para ejecutar el documento 'Mapa del sistema', se necesita la instalación del visor de Microsoft Visio de 64 bits (versión gratuita)."),
	_T("To run the 'System Map' document, Microsoft Visio viewer 64bit (free version) installation is needed."))

LOCALE_END
