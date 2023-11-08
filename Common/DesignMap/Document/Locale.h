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
// Title : System map
// Rev.  : 11/8/2023 Wed (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

#define	PROFILE_NAME	"Design Map"

LOCALE_DEFINE(4, LANG_KOREAN, LANG_JAPANESE, LANG_SPANISH, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
			_T("디자인 맵"),
			_T("デザインマップ"),
			_T("Mapa de diseño"),
			_T("Design Map"))
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
LOCALE_TEXT(CANT_FIND_MODULE_SOURCE,
			_T("모듈('<b>%s</b>') 소스 파일을 찾을 수 없습니다.<br><button type='button' class='btn btn-light btn-sm' onclick='CreateModuleFile(\\\"%s\\\");'><i class='bi bi-magic'></i> 새로 파일을 생성</button> 하시겠습니까?"),
			_T("モジュール('%s') ソース ファイル が見つかりません。<br><button type='button' class='btn btn-light btn-sm' onclick='CreateModuleFile(\\\"%s\\\");'><i class='bi bi-magic'></i> 新しいファイルを作成</button> しますか？"),
			_T("No se encuentra el archivo fuente del módulo('%s').<br>Le gustaría <button type='button' class='btn btn-light btn-sm' onclick='CreateModuleFile(\\\"%s\\\");'><i class='bi bi-magic'></i> crear un nuevo archivo</button>?"),
			_T("Module('%s') source file is not found.<br>Would you like to <button type='button' class='btn btn-light btn-sm' onclick='CreateModuleFile(\\\"%s\\\");'><i class='bi bi-magic'></i> create a new file</button>?"))
LOCALE_TEXT(MODULE_SOURCE_FILE_CREATED,
			_T("모듈('<b>%s</b>') 소스 파일이 생성되었습니다."),
			_T("モジュール ('<b>%s</b>') ソース ファイルが作成されました。"),
			_T("Se ha creado el archivo fuente del módulo ('<b>%s</b>')."),
			_T("Module ('<b>%s</b>') source file has been created."))
LOCALE_END
