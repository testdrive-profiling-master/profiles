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
// Title : Virtual display
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

LOCALE_DEFINE(3, LANG_KOREAN, LANG_JAPANESE, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
	_T("디스플레이"),
	_T("ディスプレー"),
	_T("Display"))
LOCALE_TEXT(FONT,
	_T("돋움체"),
	_T("arial"),
	_T("arial"))
LOCALE_TEXT(SCREEN_COORDINATE,
	_T("화면 좌표계"),
	_T("スクリーン座標系"),
	_T("Screen coordinate"))
LOCALE_TEXT(DESC_SCREEN_COORDINATE,
	_T("사용할 화면 좌표계를 지정합니다.\n'Screen' : 왼쪽 상위가 (0,0) 입니다.\n'OpenGL' : 왼쪽 하위가 (0,0) 입니다."),
	_T("スクリーン座標系を指定します。\n'Screen' : 左のトップが（0,0）です。\n'OpenGL' : 左サブは（0,0）です。"),
	_T("Specify the screen coordinates.\n'Screen' : Top left is (0,0).\n'OpenGL' : The lower left (0,0)."))
LOCALE_TEXT(FRONT_BUFFER,
	_T("프론트 버퍼"),
	_T("フロントバッファ"),
	_T("Front buffer"))
LOCALE_TEXT(DESC_FRONT_BUFFER,
	_T("Front buffer를 설정합니다.\n이 값을 True로 설정할 경우, Front buffer가 보이게 됩니다. False로 설정할 경우 Back buffer가 실시간으로 그려지는 것을 보이게 됩니다."),
	_T("フロントバッファを設定します。\nこの値をTrueに設定すると、Front bufferが見えるようになります。 Falseに設定すると、Back bufferがリアルタイムで描かれることが見えるようになります。"),
	_T("Set Front buffer.\nIf you set this value to True, Front buffer is shown. If set to False Back buffer being drawn in real-time is shown."))
LOCALE_TEXT(MOVIE_RECORD,
	_T("동영상 저장"),
	_T("動画保存"),
	_T("Record movie"))
LOCALE_TEXT(DESC_MOVIE_RECORD,
	_T("Front buffer를 동영상으로 저장합니다.\n저장을 시작하려면 이 값을 'True' 로 변경하십시오. 저장을 끝낼 때에는 이 값을 다시 FALSE로 변경하시기 바랍니다.\n이 기능은 '프론트 버퍼'가 True 일 때만 사용 가능합니다."),
	_T("Front bufferを動画として保存します。\n保存を開始するには、この値を'True'に変更します。 保存を終了するときには、この値を再度FALSEに変更してください。\nフロントバッファは'True'の場合、この機能は利用可能です。"),
	_T("Front buffer will be recoord in a movie.\nTo start saving change this value to 'True'. When you exit the recoord, change the value to FALSE again.\nThis feature is available when 'front buffer' is 'True'."))
LOCALE_TEXT(MOVIE_FPS,
	_T("동영상 FPS"),
	_T("動画 FPS"),
	_T("Movie FPS"))
LOCALE_TEXT(DESC_MOVIE_FPS,
	_T("저장할 동영상의 초당 프레임 수를 지정합니다."),
	_T("動画のFPSを指定します。"),
	_T("Specify the FPS of the movie."))
LOCALE_TEXT(MOVIE_FILE,
	_T("동영상 파일"),
	_T("動画ファイル"),
	_T("Movie file"))
LOCALE_TEXT(DESC_MOVIE_FILE,
	_T("저장할 동영상 파일 경로를 지정합니다.\n프로젝트의 실행 위치가 기준이 됩니다."),
	_T("保存する動画ファイルのパスを指定します。\nプロジェクトの実行位置が基準になります。"),
	_T("Specify the path to save the video file.\nThis position is based on the execution of projects."))
LOCALE_TEXT(MOVIE_STOP,
	_T("재생중 정지"),
	_T("再生中停止"),
	_T("Stop on playback"))
LOCALE_TEXT(DESC_MOVIE_STOP,
	_T("프레임이 변경될 때 일시 중지 시킵니다.\n계속하려면 이 값을 False로 변경하거나 디스플레이 아래 '▶' 버튼을 클릭하십시오.\n이 기능은 '프론트 버퍼'가 True 일 때만 사용 가능합니다."),
	_T("フレームが変更されたときに一時停止します。\n続行するには、この値をFalseに変更するか、画面下の'▶'ボタンを押してください。\nこの機能は、'フロントバッファ'が'True'に設定されている場合のみ使用可能です。"),
	_T("When you change the frame pauses.\nTo continue, change the value to False, or press '▶' button the at the bottom of the display.\nThis feature is available only when 'front buffer' is set to 'True'."))
LOCALE_TEXT(PREFERRED_SCREEN_MODE,
	_T("선호 스크린 모드"),
	_T("優先スクリーンモード"),
	_T("Preferred screen mode"))
LOCALE_TEXT(DESC_PREFERRED_SCREEN_MODE,
	_T("선호하는 스크린 모드를 지정합니다."),
	_T("優先スクリーンモードを指定します。"),
	_T("Specify your preferred screen mode."))
LOCALE_TEXT(PREFERRED_SCREEN_SIZE,
	_T("선호 스크린 크기"),
	_T("優先画面サイズ"),
	_T("Preferred screen size"))
LOCALE_TEXT(DESC_PREFERRED_SCREEN_SIZE,
	_T("선호하는 화면 크기를 지정합니다."),
	_T("優先画面サイズを指定します。"),
	_T("Specify your preferred screen size."))
LOCALE_TEXT(PRESS_BUTTON_TO_CONTINUE,
	_T("<<= 프론트 버퍼가 갱신되었습니다. 계속 진행하려면 이 버튼을 누르십시오."),
	_T("<<= フロントバッファが更新されました。 続行するには、このボタンを押してください。"),
	_T("<<= Front buffer has been updated. Press this button to continue."))
LOCALE_TEXT(CHOOSE_FRONTBUFFER_FIRST,
	_T("먼저 프론트 버퍼를 선택 하십시오."),
	_T("まず、フロントバッファを選択してください。"),
	_T("First, choose the front buffer."))
LOCALE_TEXT(ACTIVATE_MOVIE_RECOORD,
	_T("동영상 저장 활성화."),
	_T("動画保存の有効化。"),
	_T("ACtivate video recoords."))
LOCALE_TEXT(COMPLETE_MOVIE_RECOORD,
	_T("동영상 저장이 완료되었습니다."),
	_T("動画保存が完了しました。"),
	_T("Movie recording has been completed."))
LOCALE_TEXT(CANCEL_MOVIE_RECOORD,
	_T("한번도 랜더링된 Front buffer 가 없으므로 저장이 취소되었습니다."),
	_T("Front bufferが表示されていないため、保存はキャンセルされました。"),
	_T("Front buffer has not been rendered, saving is canceled."))
LOCALE_TEXT(CURRENT_DISPLAY_RESOLUTION,
	_T("현재 디스플레이 해상도 : %dx%d"),
	_T("現在のディスプレイ解像度 : %dx%d"),
	_T("Current display resolution : %dx%d"))
LOCALE_TEXT(BOX,
	_T("■"),
	_T("■"),
	_T("■"))
LOCALE_TEXT(PLAY,
	_T("▶"),
	_T("▶"),
	_T("▶"))
LOCALE_END
