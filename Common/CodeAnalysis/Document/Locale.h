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
// Title : Code Analysis
// Rev.  : 4/30/2024 Tue (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"
/* clang-format off */
#define	PROFILE_NAME	"Code Analysis"

LOCALE_DEFINE(4, LANG_KOREAN, LANG_JAPANESE, LANG_SPANISH, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("코드 분석"),
	_T("コード分析"),
	_T("Análisis de código"),
	_T("Code Analysis"))
LOCALE_TEXT(FONT,
	_T("돋움체"),
	_T("arial"),
	_T("arial"),
	_T("arial"))
LOCALE_TEXT(ANALYSIS_TARGET,
	_T("대상 : "),
	_T("ターゲット : "),
	_T("Objetivo : "),
	_T("Target : "))
LOCALE_TEXT(NO_ERROR_FOUND,
	_T("\t오류 없음.\n"),
	_T("\tエラーはありません。\n"),
	_T("\tNo hay error.\n"),
	_T("\tNo error.\n"))
LOCALE_TEXT(STATIC_CODE_ANALYSIS,
	_T("정적 코드 분석중...\n\n"),
	_T("静的コード分析中...\n\n"),
	_T("análisis de código estático...\n\n"),
	_T("Static code analysis...\n\n"))
LOCALE_TEXT(STATIC_CODE_ANALYSIS_PLEASE_WAIT,
	_T("정적 코드 분석중입니다. 잠시 기다려주시기 바랍니다..."),
	_T("静的コード分析中です。しばらくお待ち非んですよ..."),
	_T("analysising código estático . Por favor espera..."),
	_T("Static code analysising. Please wait..."))
LOCALE_TEXT(CODE_ANALYSIS_COMPLETE,
	_T("\n코드 분석 완료.\n"),
	_T("\nコード分析完了。\n"),
	_T("\nanálisis de código completa.\n"),
	_T("\nCode analysis complete.\n"))
LOCALE_TEXT(STATIC_CODE_ANALYSIS_COMPLETE,
	_T("\n정적 코드 분석 완료."),
	_T("\n静的コード分析完了。"),
	_T("\nEstática completa de análisis de código."),
	_T("\nStatic code analysis complete."))
LOCALE_TEXT(CODE_ANALYSIS_PASSED,
	_T("\t코드 분석 테스트를 통과했습니다.\n"),
	_T("\tコード分析テストに合格しました。\n"),
	_T("\tSe pasó la prueba de análisis de código.\n"),
	_T("\tCode analysis test is passed.\n"))
LOCALE_TEXT(CODE_ANALYSIS_FAILED,
	_T("\t잠재적 오류(%d회)와 경고(%d회)가 발견되었습니다.\n\t일시적으로 프로젝트에 큰 영향을 주지 않을 수 있으나,\n\t소스 무결성을 위해 반드시 해결할 것을 권고합니다.\n"),
	_T("\t潜在的なエラー（%d回）と警告（%d回）が発見されました。\n\t一時的にプロジェクトに大きな影響を与えないことができるが、\n\tソースの整合性のために必ず解決することをお勧めします。\n"),
	_T("\t%d error(es) potenciales y %d se encuentran las advertencia(s).\n\Temporalmente no es tanto afecta a su,\n\tpero recomendamos que revise el código fuente para asegurarse.\n"),
	_T("\t%d potential error(s) and %d warning(s) is/are found.\n\tTemporarily it's not much affect to your project,\n\tbut we recommend that you review the source code for sure.\n"))
LOCALE_TEXT(CODE_ANALYSIS_INFO,
	_T("이 문서는 매주 월요일 1회 자동으로 실행되며, 수동으로 프로파일 뷰 메뉴에서 실행할 수 있습니다.\n"),
	_T("この記事は、毎週月曜日に1回、自動的に実行され、手動でプロファイル表示]メニューから実行できます。\n"),
	_T("Este documento se ejecute automáticamente una vez que todos los lunes , y de forma manual , puede ejecutar en el menú una vista de perfil.\n"),
	_T("This document is automatically run once every Monday, and manually, you can run at the a profile view menu.\n"))
LOCALE_TEXT(LOG_SURPPRESS,
	_T("로그 생략"),
	_T("ログ省略"),
	_T("la supresión de registro"),
	_T("Log suppressing"))
LOCALE_TEXT(DESC_LOG_SURPPRESS,
	_T("필요한 로그만 보이도록 나머지 로그를 생략합니다."),
	_T("必要なログだけを表示する残りのログを省略します。"),
	_T("Visibles sólo los registros importantes y suprimir el resto de los registros."),
	_T("Visible only important logs and suppress the rest of the logs."))
LOCALE_TEXT(STATIC_ANALYSIS_SURPPRESS,
	_T("정적 분석 에러 무시"),
	_T("静的解析エラー無視"),
	_T("Ignorar errores de análisis estático"),
	_T("Ignore static analysis errors"))
LOCALE_TEXT(DESC_STATIC_ANALYSIS_SURPPRESS,
	_T("무시할 에러를 나열합니다.\n예) objectIndex,preprocessorErrorDirective"),
	_T("無視エラーが表示されます。\n例) objectIndex,preprocessorErrorDirective"),
	_T("Lista de errores a ignorar.\nex) objectIndex,preprocessorErrorDirective"),
	_T("List errors to ignore.\nex) objectIndex,preprocessorErrorDirective"))
/* clang-format on */
LOCALE_END
