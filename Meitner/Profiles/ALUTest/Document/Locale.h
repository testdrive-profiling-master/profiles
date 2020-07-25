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
// Title : Private dynamic document
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

LOCALE_DEFINE(3, LANG_KOREAN, LANG_JAPANESE, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("연산기 테스트"),
	_T("ALUテスト"),
	_T("ALU Test"))
LOCALE_TEXT(TOP_DESIGN,
	_T("탑 디자인"),
	_T("テスト対象"),
	_T("Test target"))
LOCALE_TEXT(DESC_TOP_DESIGN,
	_T("시뮬레이션 할 탑 디자인을 지정합니다."),
	_T("シミュレートするトップのデザインを指定します。"),
	_T("Specifies the simulation top design."))
LOCALE_TEXT(SAMPLE_SIZE,
	_T("샘플링 크기"),
	_T("サンプルサイズ"),
	_T("Sample size"))
LOCALE_TEXT(DESC_SAMPLE_SIZE,
	_T("샘플링 카운트 횟수를 설정합니다.\n0을 지정하면 전수 검사를 시행합니다."),
	_T("サンプリングカウント回数を設定します。0を指定すると、全数検査を実施します。"),
	_T("Specifies the sampling count size. If you specify to 0, the total inspection will be performed."))
LOCALE_TEXT(SAMPLE_START,
	_T("샘플 시작"),
	_T("サンプル開始"),
	_T("Sample start"))
LOCALE_TEXT(DESC_SAMPLE_START,
	_T("샘플 시작 값을 지정합니다."),
	_T("サンプルの開始値を指定します。"),
	_T("Specifies the sampling start value."))
LOCALE_TEXT(SAMPLE_END,
	_T("샘플 끝"),
	_T("サンプル終了"),
	_T("Sample end"))
LOCALE_TEXT(DESC_SAMPLE_END,
	_T("샘플링 끝 값을 지정합니다."),
	_T("サンプル終了値を指定します。"),
	_T("Specifies the sampling end value."))
LOCALE_TEXT(BTN_BUILD,
	_T("연산기 빌드"),
	_T("ALU構築"),
	_T("ALU build"))
LOCALE_TEXT(BTN_RUN,
	_T("실행"),
	_T("実行"),
	_T("Execute"))
LOCALE_TEXT(BTN_STOP,
	_T("중지"),
	_T("停止"),
	_T("Stop"))
LOCALE_TEXT(READY,
	_T("준비 완료..."),
	_T("準備完了..."),
	_T("Ready..."))
LOCALE_TEXT(BUILDING_ALU_HW,
	_T("-- ALU H/W 빌드 중... : %s"),
	_T("-- ALU H/W ビルド中... : %s"),
	_T("-- Building ALU H/W design... : %s"))
LOCALE_TEXT(SIMULATION_MODULE_IS_NOT_FOUND,
	_T("시뮬레이션 모듈을 찾을 수 없습니다. : %s"),
	_T("シミュレーションモジュールが見つかりません。: %s"),
	_T("Simulation module is not found. : %s"))
LOCALE_TEXT(IS_NOT_SIMULATION_MODULE,
	_T("시뮬레이션 모듈이 아닙니다. : %s"),
	_T("シミュレーションモジュールはありません。: %s"),
	_T("Not a simulation module. : %s"))
LOCALE_TEXT(SIMULATION_MODULE_NEED_TO_BUILD,
	_T("ALU 시뮬레이션 모듈의 빌드가 필요합니다."),
	_T("ALUシミュレーションモジュールをビルドする必要があります。"),
	_T("ALU simulation module needs to build."))
LOCALE_TEXT(SIMULATION_MODULE_LOADING_IS_SUCCESSFUL,
	_T("ALU 시뮬레이션 모듈을 성공적으로 로딩했습니다."),
	_T("ALUシミュレーションモジュールのロードは成功です。"),
	_T("ALU simulation module loading is successful."))
LOCALE_TEXT(CANT_INITAILIZE_SIMULATION_MODULE,
	_T("ALU 시뮬레이션 모듈을 초기화할 수 없습니다."),
	_T("ALUシミュレーションモジュールを初期化することができません。"),
	_T("The ALU simulation module can not be initialized."))
LOCALE_TEXT(HW_BUILD_IS_FAILED,
	_T("하드웨어 빌드가 실패했습니다."),
	_T("ハードウェアビルドが失敗しました。"),
	_T("Hardware build is failed."))
LOCALE_END
