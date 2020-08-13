//================================================================================
// Copyright (c) 2013 ~ 2020. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Scenario test
// Rev.  : 8/13/2020 Thu (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

LOCALE_DEFINE(4, LANG_KOREAN, LANG_JAPANESE, LANG_SPANISH, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("시나리오 테스트"),
	_T("シナリオテスト"),
	_T("Prueba de escenario"),
	_T("Scenario Test"))
LOCALE_TEXT(TEST_ALL,
	_T("전체 테스트"),
	_T("全体テスト"),
	_T("Prueba de Todo"),
	_T("Test All"))
LOCALE_TEXT(RESCAN_ALL,
	_T("전체 갱신"),
	_T("全更新"),
	_T("Volver a analizar Todos"),
	_T("Rescan All"))
LOCALE_TEXT(STOP,
	_T("중지"),
	_T("停止"),
	_T("Detener"),
	_T("Stop"))
LOCALE_TEXT(OPEN_FOLDER,
	_T("폴더 열기"),
	_T("フォルダを開く"),
	_T("Carpeta abierta"),
	_T("Open folder"))
LOCALE_TEXT(TEXT_EDITOR,
	_T("문서 편집기"),
	_T("テキストエディタ"),
	_T("Text editor"),
	_T("Text editor"))
LOCALE_TEXT(TEXT_EDITOR_DESC,
	_T("문서편집기를 선택합니다."),
	_T("テキスト·エディタを選択します。"),
	_T("Seleccione el editor de texto."),
	_T("Select the text editor."))
LOCALE_TEXT(SCORE_RESULT,
	_T("스코어 결과"),
	_T("スコア結果"),
	_T("Puntajes resultados"),
	_T("Score result"))
LOCALE_TEXT(SCORE_TOTAL,
	_T("전체 (%d)"),
	_T("合計 (%d)"),
	_T("Total (%d)"),
	_T("Total (%d)"))
LOCALE_TEXT(SCORE_PASSED,
	_T("%d (%.1f%%) 성공"),
	_T("%d (%.1f%%) 成功"),
	_T("%d (%.1f%%) Pasado"),
	_T("%d (%.1f%%) Passed"))
LOCALE_TEXT(SCORE_FAILD,
	_T("%d (%.1f%%) 실패"),
	_T("%d (%.1f%%) 失敗"),
	_T("%d (%.1f%%) Ha fallado"),
	_T("%d (%.1f%%) Failed"))
LOCALE_TEXT(SCORE_CRASHED,
	_T("%d (%.1f%%) 크래쉬"),
	_T("%d (%.1f%%) クラッシュ"),
	_T("%d (%.1f%%) Estrellado"),
	_T("%d (%.1f%%) Crashed"))
LOCALE_TEXT(SCORE_NOT_TESTED,
	_T("%d (%.1f%%) 테스트 필요"),
	_T("%d (%.1f%%) テスト必要"),
	_T("%d (%.1f%%) No probado"),
	_T("%d (%.1f%%) Not tested"))
LOCALE_TEXT(TEST_STATUS_NOT_TESTED,
	_T("<font color='#808080'>[테스트 필요]</font>"),
	_T("<font color='#808080'>[テスト必要]</font>"),
	_T("<font color='#808080'>[No probado]</font>"),
	_T("<font color='#808080'>[Not tested]</font>"))
LOCALE_TEXT(TEST_STATUS_RUN_PASSED,
	_T("<font color='#0000FF'>[성공]</font>"),
	_T("<font color='#0000FF'>[成功]</font>"),
	_T("<font color='#0000FF'>[Pasado]</font>"),
	_T("<font color='#0000FF'>[Passed]</font>"))
LOCALE_TEXT(TEST_STATUS_RUN_FAILED,
	_T("<font color='#ff0000'>[실패]</font>"),
	_T("<font color='#ff0000'>[失敗]</font>"),
	_T("<font color='#ff0000'>[Ha fallado]</font>"),
	_T("<font color='#ff0000'>[Failed]</font>"))
LOCALE_TEXT(TEST_STATUS_RUN_CRACHED,
	_T("<font color='#C80080'><b>[크래쉬]</b></font>"),
	_T("<font color='#C80080'><b>[クラッシュ]</b></font>"),
	_T("<font color='#C80080'><b>[Estrellado]</b></font>"),
	_T("<font color='#C80080'><b>[Crashed]</b></font>"))
LOCALE_TEXT(TEST_STATUS_COMPILE_FAILED,
	_T("<font color='#ff0000'>[컴파일 실패]</font>"),
	_T("<font color='#ff0000'>[コンパイル失敗]</font>"),
	_T("<font color='#ff0000'>[Compilación se falla]</font>"),
	_T("<font color='#ff0000'>[Compilation is failed]</font>"))
LOCALE_TEXT(TEST_STATUS_LINK_FAILED,
	_T("<font color='#ff0000'>[링크 실패]</font>"),
	_T("<font color='#ff0000'>[リンキング失敗]</font>"),
	_T("<font color='#ff0000'>[La unión se logró]</font>"),
	_T("<font color='#ff0000'>[Linking is failed]</font>"))
LOCALE_TEXT(TEST_STATUS_FILE_NOT_FOUND,
	_T("<font color='#ff7f00'><b>[파일 없음]</b></font>"),
	_T("<font color='#ff7f00'><b>[ファイルはありません]</b></font>"),
	_T("<font color='#ff7f00'><b>[Archivo no encontrado]</b></font>"),
	_T("<font color='#ff7f00'><b>[File not found]</b></font>"))
LOCALE_TEXT(TEST_STATUS_SYSTEM_IS_REQUIRED,
	_T("<font color='#ff7f00'><b>[시스템 필요]</b></font>"),
	_T("<font color='#ff7f00'><b>[システム必要]</b></font>"),
	_T("<font color='#ff7f00'><b>[El sistema es requerido]</b></font>"),
	_T("<font color='#ff7f00'><b>[System is required]</b></font>"))
LOCALE_TEXT(TEST_STATUS_SCORE,
	_T("<font color='#%06X'>[%.3lf%%]</font>"),
	_T("<font color='#%06X'>[%.3lf%%]</font>"),
	_T("<font color='#%06X'>[%.3lf%%]</font>"),
	_T("<font color='#%06X'>[%.3lf%%]</font>"))
LOCALE_TEXT(TEST_IN_PROGRESS,
	_T("테스트 진행중..."),
	_T("テスト進行中。。。"),
	_T("Prueba en progreso..."),
	_T("test in progress..."))
LOCALE_TEXT(TODAY,
	_T("오늘"),
	_T("今日"),
	_T("Hoy"),
	_T("Today"))
LOCALE_TEXT(YESTERDAY,
	_T("어제"),
	_T("昨日"),
	_T("Ayer"),
	_T("Yesterday"))
LOCALE_TEXT(DAYS_AGO,
	_T("%d일 전"),
	_T("%d日前"),
	_T("Hace %d días"),
	_T("%d days ago"))
LOCALE_TEXT(A_MONTH_AGO,
	_T("한달 전"),
	_T("一ヶ月前"),
	_T("Hace un mes"),
	_T("A month ago"))
LOCALE_TEXT(MONTHS_AGO,
	_T("%d개월 전"),
	_T("%dヶ月前"),
	_T("hace %d meses"),
	_T("%d months ago"))
LOCALE_TEXT(A_YEAR_AGO,
	_T("일년전"),
	_T("一年前"),
	_T("Hace un año"),
	_T("A year ago"))
LOCALE_TEXT(YEARS_AGO,
	_T("%d년전"),
	_T("%d年前"),
	_T("hace %d años"),
	_T("%d years ago"))
LOCALE_TEXT(LIST_REFRESH_IS_DONE,
	_T("테스트 리스트 갱신 완료."),
	_T("テストリスト更新完了。"),
	_T("actualización de la lista de examen se."),
	_T("Test list update is done."))
LOCALE_TEXT(TEST_IS_RUNNING,
	_T("테스트가 진행중입니다."),
	_T("テストが進行中です。"),
	_T("La prueba está en curso."),
	_T("Testing is already in progress."))
LOCALE_TEXT(ALREADY_TEST_IS_RUNNING,
	_T("이미 테스트가 진행중입니다. 완료 후 다시 시도하십시오."),
	_T("すでにテストが進行中です。 完了後、再試行してください。"),
	_T("Las pruebas ya están en progreso. Vuelva a intentarlo después de completarlo."),
	_T("already the test is in progress. Please retry after completion."))
LOCALE_TEXT(INFO_FORCE_TO_QUIT_PROGRAM,
	_T("프로그램이 장시간 종료되지 않는다면, <a href='quit:quit'>강제 종료</a>를 선택하세요."),
	_T("プログラムが長時間終了していなければ、<a href='quit:quit'>強制終了</a>を選択します。"),
	_T("Si el programa no termina por un tiempo prolongado, elija <a href='quit:quit'>Forzar para salir.</a>"),
	_T("If the program does not end for a long time, choose <a href='quit:quit'>Force to Quit</a>."))
LOCALE_TEXT(FORCE_TO_QUIT_PROGRAM,
	_T("프로그램을 강제로 종료합니다."),
	_T("プログラムを強制的に終了します。"),
	_T("Por la fuerza termina el programa."),
	_T("Forcibly terminates the program.."))
LOCALE_TEXT(ADD_TEST_LIST,
	_T("테스트 진행 목록 추가 : %s"),
	_T("テストの進行リストの追加 : %s"),
	_T("Añadir secuencia de prueba : %s"),
	_T("Add test sequence : %s"))
LOCALE_TEXT(TEST_IS_OVER,
	_T("시나리오 테스트가 끝났습니다."),
	_T("シナリオのテストが終わりました。"),
	_T("prueba de escenario ha terminado."),
	_T("Scenario test is over."))
LOCALE_TEXT(TEST_PROGRAM_IS_NOT_FOUND,
	_T("시나리오 테스트 프로그램(\"%s\")을 찾을 수 없습니다. 테스트가 취소 됩니다."),
	_T("シナリオのテストプログラム(\"%s\")を見つけることができませんでした。 テストがキャンセルされます。"),
	_T("No se pudo encontrar el programa(\"%s\") de prueba para el escenario. La prueba será cancelada."),
	_T("Scenario Test program file(\"%s\") could not be found. The test is canceled."))
LOCALE_TEXT(NAME_FILTER,
	_T("이름 필터"),
	_T("名前フィルター"),
	_T("Filtro de nombre"),
	_T("Name filter"))
LOCALE_TEXT(DESC_NAME_FILTER,
	_T("이름 필터를 지정합니다."),
	_T("名前フィルターを指定します。"),
	_T("Especifique un filtro de nombre."),
	_T("Specify a name filter."))
LOCALE_END
