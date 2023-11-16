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
// Title : Xilinx synthesis
// Rev.  : 11/16/2023 Thu (clonextop@gmail.com)
//================================================================================
﻿//================================================================================
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
// Title : Xilinx synthesis
// Rev.  : 4/25/2023 Tue (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

#define	PROFILE_NAME	"Xilinx Synthesis"

LOCALE_DEFINE(4, LANG_KOREAN, LANG_JAPANESE, LANG_SPANISH, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
			_T("자일링스 합성"),
			_T("ザイリンクス合成"),
			_T("Xilinx Síntesis"),
			_T("Xilinx Synthesis"))
LOCALE_TEXT(XILINX_INSTALL_PATH,
			_T("자일링스 설치 경로"),
			_T("ザイリンクスのインストールパス"),
			_T("ruta instalado Xilinx"),
			_T("Xilinx installed path"))
LOCALE_TEXT(DESC_XILINX_INSTALL_PATH,
			_T("자일링스(Vivado) 설치 경로를 지정합니다.\n\n예) C:\\Xilinx\\Vivado\\20XX.X\\"),
			_T("ザイリンクス(Vivado)インストールパスを指定します。\n\n例) C:\\Xilinx\\Vivado\\20XX.X\\"),
			_T("Especifica la ruta instalada Xilinx(Vivado).\n\nEj.) C:\\Xilinx\\Vivado\\20XX.X\\"),
			_T("Specifies the Xilinx(Vivado) installed path.\n\nEx.) C:\\Xilinx\\Vivado\\20XX.X\\"))
LOCALE_TEXT(XILINX_FAMILY,
			_T("페밀리"),
			_T("ファミリー"),
			_T("Familia"),
			_T("Family"))
LOCALE_TEXT(DESC_XILINX_FAMILY,
			_T("자일링스 페밀리를 지정합니다."),
			_T("ザイリンクスファミリーを指定します。"),
			_T("Especifica la familia de la Xilinx."),
			_T("Specifies the Xilinx's family."))
LOCALE_TEXT(XILINX_DEVICE,
			_T("디바이스"),
			_T("デバイス"),
			_T("Dispositivo"),
			_T("Device"))
LOCALE_TEXT(DESC_XILINX_DEVICE,
			_T("자일링스 디바이스를 지정합니다.\n존재하지 않을 경우, 직접 입력합니다."),
			_T("ザイリンクスデバイスを指定します。\n存在しない場合、直接入力します。"),
			_T("Especifica el dispositivo de Xilinx.\nSi no está presente, introducirlo usted mismo."),
			_T("Specifies the Xilinx's device.\nIf not present, enter it yourself."))
LOCALE_TEXT(XILINX_PACKAGE,
			_T("패키지"),
			_T("パッケージ"),
			_T("Paquete"),
			_T("Package"))
LOCALE_TEXT(DESC_XILINX_PACKAGE,
			_T("자일링스 디바이스의 패키지를 지정합니다.\n존재하지 않을 경우, 직접 입력합니다."),
			_T("ザイリンクスデバイスのパッケージを指定します。\n存在しない場合、直接入力します。"),
			_T("Especifica el dispositivo del paquete de Xilinx.\nSi no está presente, introducirlo usted mismo."),
			_T("Specifies the Xilinx's device package.\nIf not present, enter it yourself."))
LOCALE_TEXT(XILINX_SPEED_GRADE,
			_T("스피드 등급"),
			_T("スピード・グレード"),
			_T("Grado de velocidad"),
			_T("Speed grade"))
LOCALE_TEXT(DESC_XILINX_SPEED_GRADE,
			_T("자일링스 디바이스의 스피드 등급을 지정합니다.\n존재하지 않을 경우, 직접 입력합니다."),
			_T("ザイリンクスデバイスのスピードグレードを指定します。\n存在しない場合、直接入力します。"),
			_T("Especifica grado de velocidad del dispositivo de Xilinx.\nSi no está presente, introducirlo usted mismo."),
			_T("Specifies the Xilinx's device speed grade.\nIf not present, enter it yourself."))
LOCALE_TEXT(TARGET_SPEED,
			_T("목표 속도(MHz)"),
			_T("目標速度(MHz)"),
			_T("Velocidad objetivo(MHz)"),
			_T("Target speed(MHz)"))
LOCALE_TEXT(DESC_TARGET_SPEED,
			_T("합상의 목표 속도(MHz)를 지정합니다.\n(이값은 FPGA가 달성할 수있는 최대 속도보다 높게 설정하는 것이 좋습니다.)"),
			_T("合計上の目標速度(MHz)を指定します。\n(この値は、FPGAが達成できる最大速度よりも高く設定することをお勧めします。)"),
			_T("Especificar la velocidad objetivo (MHz) en la síntesis.\n(Se recomienda que este valor se establezca por encima de la velocidad máxima que puede alcanzar la FPGA.)"),
			_T("Specify the target speed(MHz) on the synthesis.\n(It is recommended that this value be set higher than the maximum speed that the FPGA can achieve.)"))
LOCALE_TEXT(LIST_CLOCKS,
			_T("클럭 신호"),
			_T("クロック信号"),
			_T("Las señales de reloj"),
			_T("Clock signals"))
LOCALE_TEXT(DESC_LIST_CLOCKS,
			_T("목표속도를 지정할 클럭 신호를 나열합니다."),
			_T("目標速度を指定するクロック信号を一覧表示します。"),
			_T("Especificar las listas de las señales de reloj para su velocidad objetivo."),
			_T("Specify the lists of the clock signals for your target speed."))
LOCALE_TEXT(LIST_RESETS,
			_T("리셋 신호"),
			_T("リセット信号"),
			_T("señales de restablecimiento"),
			_T("Reset signals"))
LOCALE_TEXT(DESC_LIST_RESETS,
			_T("리셋 신호를 지정합니다.\n지정한 리셋신호는 TIG(Timing Ignore) 속성을 가집니다."),
			_T("リセット信号を指定します。\n指定されたリセット信号は、 TIG(Timing Ignore)属性があります。"),
			_T("Especificar las señales de restablecimiento.\nSeñales de restablecimiento especificados tienen una TIG(Timing Ignorar) propiedad."),
			_T("Specify the reset signals.\nSpecified reset signals have a TIG(Timing Ignore) property."))
LOCALE_TEXT(MAX_PATHS,
			_T("최대 경로 수"),
			_T("パスの最大数"),
			_T("sEl numero maximo de rutas"),
			_T("Maximum number of paths"))
LOCALE_TEXT(DESC_MAX_PATHS,
			_T("타이밍 리포트에서 나열할 최대 경로 수를 지정합니다.(1~100)"),
			_T("タイミングレポートで一覧表示するパスの最大数を指定します。(1~100)"),
			_T("En él se especifica el número máximo de rutas que se enumeran en el informe de temporización.(1~100)"),
			_T("It specifies the maximum number of paths to be listed in the timing report.(1~100)"))
LOCALE_TEXT(REFRESH_TABLE,
			_T("테이블 갱신"),
			_T("テーブル更新"),
			_T("Tabla de actualización"),
			_T("Refresh table"))
LOCALE_TEXT(PROJECT_PATH,
			_T("프로젝트 경로"),
			_T("プロジェクトのパス"),
			_T("ruta del proyecto"),
			_T("Project path"))
LOCALE_TEXT(DESC_PROJECT_PATH,
			_T("프로젝트 경로를 지정합니다."),
			_T("プロジェクトのパスを指定します。"),
			_T("Especificar la ruta del proyecto."),
			_T("Specify the project's path."))
LOCALE_TEXT(SOURCE_LIST,
			_T("기본 소스 목록"),
			_T("基本的なソースのリスト"),
			_T("Lista de fuente por defecto"),
			_T("Default source list"))
LOCALE_TEXT(DESC_SOURCE_LIST,
			_T("기본 소스 목록 파일 경로를 지정합니다."),
			_T("基本的なソースのリストファイルのパスを指定します。"),
			_T("Especifique la ruta de la lista de fuentes por defecto."),
			_T("Specify the path of the default source list."))
LOCALE_TEXT(CAN_NOT_FIND_XILINX_PATH,
			_T("자일링스 경로를 찾을 수 없습니다. 설치된 자이링스 경로를 확인하고, 다시 경로를 지정하시기 바랍니다."),
			_T("ザイリンクスのパスを見つけることができません。インストールされたザイリンクスのパスを確認し、再度パスを指定してください。"),
			_T("No se puede encontrar la ruta de Xilinx. Compruebe la ruta de Xilinx instalado, y especifique la ruta de nuevo."),
			_T("Can not found the Xilinx path. Check the installed Xilinx path, and specify the path again."))
LOCALE_TEXT(NO_XILINX_ENVIRONMENT,
			_T("자일링스 툴 환경이 존재하지 않습니다. 정상적으로 설치되었는지 확인 바랍니다."),
			_T("ザイリンクスツール環境は存在しません。正しくインストールされているか確認してください。"),
			_T("El entorno de herramientas Xilinx no existe. Compruebe si está instalado correctamente."),
			_T("Xilinx tools environment does not exist. Please check if it is installed properly."))
LOCALE_TEXT(FOUND_XILINX_PATH,
			_T("설치된 Vivado 경로 : %s"),
			_T("インストールされてVivadoパス : %s"),
			_T("Ruta de Vivado instalada : %s"),
			_T("Installed Vivado Path : %s"))
LOCALE_TEXT(INSTALLED_VIVADO_VERSION,
			_T("설치된 Vivado 버전 : %s"),
			_T("インストールされてvivadoバージョン : %s"),
			_T("Versión de Vivado instalada : %s"),
			_T("Installed Vivado version : %s"))
LOCALE_TEXT(SYNTHESIS_TARGET,
			_T("합성 대상"),
			_T("合成ターゲット"),
			_T("síntesis de destino"),
			_T("Synthesis target"))
LOCALE_TEXT(OPERATION_SPEED,
			_T("동작속도"),
			_T("タイミング"),
			_T("Sincronización"),
			_T("Timing"))
LOCALE_TEXT(CLOCK_IO_DELAY,
			_T("Setup/Hold 시간"),
			_T("Setup/Holdタイム"),
			_T("Hora de preparación / espera"),
			_T("Setup/Hold time"))
LOCALE_TEXT(COMBINATIONAL_DELAY,
			_T("조합 딜레이"),
			_T("コンビネショナルディレイ"),
			_T("Retardo combinatoria"),
			_T("Combinational delay"))
LOCALE_TEXT(TOTAL,
			_T("전체"),
			_T("合計"),
			_T("Total"),
			_T("Total"))
LOCALE_TEXT(OPEN_FOLDER,
			_T("폴더 열기"),
			_T("フォルダを開く"),
			_T("Carpeta abierta"),
			_T("Open folder"))
LOCALE_TEXT(EDIT,
			_T("편집"),
			_T("編集"),
			_T("compilación"),
			_T("Edit"))
LOCALE_TEXT(REPORT,
			_T("보고서"),
			_T("レポート"),
			_T("informe"),
			_T("Report"))
LOCALE_TEXT(SET_DEFAULT_PROJECT_PATH,
			_T("프로젝트의 기본 경로를 지정합니다. (경로 : %s)"),
			_T("プロジェクトの既定のパスを指定します。 (パス : %s)"),
			_T("Especifica la ruta por defecto del proyecto. (Camino : %s)"),
			_T("Specifies the default path of the project. (Path : %s)"))
LOCALE_TEXT(CURRENT_SYNTHESIS_TARGET,
			_T("* 합성 대상 : %s/%s"),
			_T("* 合成対象 : %s/%s"),
			_T("* Síntesis de destino : %s/%s"),
			_T("* Synthesis target : %s/%s"))
LOCALE_TEXT(SYNTHESIS_IS_RUNNING,
			_T("합성이 진행중입니다."),
			_T("合成が完了しました。"),
			_T("La síntesis está en progreso."),
			_T("Synthesis is in progress."))
LOCALE_TEXT(SYNTHESIS_IS_DONE,
			_T("합성이 완료되었습니다."),
			_T("合成が進行中です。"),
			_T("La síntesis se completó."),
			_T("Synthesis is completed."))
LOCALE_TEXT(SYNTHESIS_IS_FAILED,
			_T("합성이 실패했습니다."),
			_T("合成が失敗しました。"),
			_T("La síntesis se falló."),
			_T("Synthesis is failed."))
LOCALE_TEXT(SOURCE_LIST_IS_NEEDED,
			_T("소스 리스트 파일이 필요합니다. (%s)"),
			_T("ソースリストファイルが必要です。 (%s)"),
			_T("El archivo de lista de origen requiere. (%s)"),
			_T("The source list file required. (%s)"))
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
			_T("月前"),
			_T("hace un mes"),
			_T("A month ago"))
LOCALE_TEXT(MONTHS_AGO,
			_T("%d개월 전"),
			_T("%dヶ月前"),
			_T("Hace %d meses"),
			_T("%d months ago"))
LOCALE_TEXT(A_YEAR_AGO,
			_T("일년 전"),
			_T("一年前"),
			_T("hace un año"),
			_T("One year ago"))
LOCALE_TEXT(YEARS_AGO,
			_T("%d년 전"),
			_T("%d年前"),
			_T("Hace %d años"),
			_T("%d years ago"))
LOCALE_TEXT(MARGIN_RATE,
			_T("마진율"),
			_T("マージン率"),
			_T("Tasa de margen"),
			_T("Margin rate"))
LOCALE_TEXT(ESTIMATED,
			_T("로 추정"),
			_T("に推定"),
			_T(" Estimado"),
			_T(" Estimated"))
LOCALE_TEXT(SYNTHESIS_IS_BUSY,
			_T("이미 합성이 진행 중입니다. 잠시 기다려 주세요..."),
			_T("すでに合成が進行中です。しばらくお待ちください..."),
			_T("Ya síntesis sobre los progresos. espera un minuto..."),
			_T("Already synthesis on progress. wait a minutes..."))
LOCALE_TEXT(MODULE_NAME_IS_NOT_FOUND,
			_T("모듈 이름을 찾을 수 없습니다."),
			_T("モジュールの名前を検索することができません。"),
			_T("El nombre del módulo no se pudo encontrar."),
			_T("The module name could not be found."))
LOCALE_TEXT(SOURCE_IS_NOT_FOUND,
			_T("소스에서 기술된 파일을 찾을 수 없습니다. : %s"),
			_T("ソースに記載されているファイルが見つかりません。 : %s"),
			_T("No se pudo encontrar el archivo descrito en la fuente. : %s"),
			_T("The file described in the source could not be found. : %s"))
LOCALE_TEXT(CURRENT_TOP_MODULE,
			_T("TOP 모듈 이름 : %s"),
			_T("TOPモジュール名 : %s"),
			_T("Nombre del módulo TOP : %s"),
			_T("TOP module name : %s"))
LOCALE_TEXT(ADD_SYNTHESIS_LIST,
			_T("합성 목록을 추가했습니다."),
			_T("合成のリストを追加しました。"),
			_T("lista de síntesis añadido."),
			_T("Added list of synthesis."))
LOCALE_TEXT(ALREADY_EXIST_IN_SYNTHESIS_LIST,
			_T("이미 합성 목록에 존재합니다."),
			_T("すでに合成リストに存在します。"),
			_T("Ya existe en la lista compuesta."),
			_T("It already exists in the composite list."))
LOCALE_TEXT(SET_MARK,
			_T("현재 결과를 비교 대상으로 지정합니다."),
			_T("現在の結果を比較対象として指定します。"),
			_T("Establecer los resultados actuales para comparar objetivo."),
			_T("Set current results to compare target."))
LOCALE_TEXT(TARGET_SPEED_MUST_GREATER_THAN_ZERO,
			_T("목표 속도는 0 이상이어야 합니다."),
			_T("目標速度は0以上である。"),
			_T("Velocidad del blanco debe estar por encima de cero."),
			_T("Target speed should be above zero."))
LOCALE_TEXT(MUST_FILL_ALL_PROPERTIES,
			_T("모든 속성을 지정하여야 합니다."),
			_T("すべての属性を指定しなければなりませ。"),
			_T("Debe especificar todos los atributos."),
			_T("You must specify all attributes."))
LOCALE_TEXT(PATH_CLOCK_SETUP,
			_T("입력 Setup 패스"),
			_T("入力Setupパス"),
			_T("Ruta de configuración de entrada"),
			_T("Input setup path"))
LOCALE_TEXT(PATH_CLOCK_HOLD,
			_T("출력 Hold 패스"),
			_T("出力Holdパス"),
			_T("Ruta de retención de salida"),
			_T("Output hold path"))
LOCALE_TEXT(PATH_CLOCK_PULSE,
			_T("최대 클럭 펄스 간격"),
			_T("最大クロックパルス間隔"),
			_T("Intervalo de pulso de reloj máxima"),
			_T("Maximum clock pulse interval"))
LOCALE_TEXT(PATH_COMBINATIONAL,
			_T("최대 Combinational 로직 딜레이"),
			_T("最大Combinationalロジックディレイ"),
			_T("ㄲetardo máximo lógica combinatoria"),
			_T("Maximum combinational logic delay"))
LOCALE_TEXT(PATH_FILTER,
			_T("경로 필터"),
			_T("パスフィルター"),
			_T("Filtro de ruta"),
			_T("Path filter"))
LOCALE_TEXT(DESC_PATH_FILTER,
			_T("경로 필터를 지정합니다."),
			_T("パスフィルターを指定します。"),
			_T("Especifique un filtro de ruta."),
			_T("Specify a path filter."))
LOCALE_END
