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
// Title : System manager
// Rev.  : 5/4/2021 Tue (clonextop@gmail.com)
//================================================================================
#include "LocaleString.h"

LOCALE_DEFINE(4, LANG_KOREAN, LANG_JAPANESE, LANG_SPANISH, LANG_ENGLISH)

LOCALE_TEXT(DOCUMENT_TITLE,
			_T("시스템"),
			_T("システム"),
			_T("Sistema"),
			_T("System"))
LOCALE_TEXT(SUB_SYSTEM,
			_T("하위 시스템"),
			_T("サブシステム"),
			_T("Subsistema"),
			_T("Subsystem"))
LOCALE_TEXT(DESC_SUB_SYSTEM,
			_T("하위 시스템을 지정합니다."),
			_T("サブシステムを指定します。"),
			_T("Especificar el subsistema."),
			_T("Specify the subsystem."))
LOCALE_TEXT(BUILD_AUTOMATION,
			_T("빌드 자동화"),
			_T("ビルドの自動化"),
			_T("Construir automatización"),
			_T("Build automation"))
LOCALE_TEXT(DESC_BUILD_AUTOMATION,
			_T("빌드 자동화를 활성화 합니다.\n엄격한 를빌드 수행하려면 'Strict build'를 활성화합니다.\n소스 저장시 「Control」키를 1초 이상 누르면, 빌드가 취소됩니다."),
			_T("ビルドの自動化を有効にします。\n厳格なビルドを実行するには、'Strict build'を有効にします。\nソースの保存時に「Control」キーを1秒以上押すと、ビルドがキャンセルされます。"),
			_T("Habilitar la automatización de compilación.\nPara realizar una compilación estricta, seleccione 'Compilación estricta'.\nSi presiona la tecla 「Control」 durante más de 1 segundo al guardar su fuente, la compilación se cancelará."),
			_T("Enable build automation.\nTo perform strict build, select 'Strict build'.\nIf press 「Control」 key for more than 1 second during saving your source, the build will be canceled."))
LOCALE_TEXT(COMPILER,
			_T("컴파일러"),
			_T("コンパイラ"),
			_T("Compilador"),
			_T("Compiler"))
LOCALE_TEXT(DESC_COMPILER,
			_T("메인 컴파일러를 선택합니다."),
			_T("メインコンパイラを選択します。"),
			_T("Elija un compilador principal."),
			_T("Choose a main compiler."))
LOCALE_TEXT(SIMULATOR,
			_T("시뮬레이터"),
			_T("シミュレーター"),
			_T("Simulador"),
			_T("Simulator"))
LOCALE_TEXT(DESC_SIMULATOR,
			_T("메인 시뮬레이터를 선택합니다.\nverilator, iverilog(미완성)"),
			_T("メインシミュレータを選択します。\nverilator, iverilog(未完成)"),
			_T("Seleccione el simulador principal.\nverilator, iverilog(incompleto)"),
			_T("Choose a main compiler.\nverilator, iverilog(incomplete)"))
LOCALE_TEXT(DEBUG_INFO,
			_T("디버그 정보"),
			_T("デバッグ情報"),
			_T("Información de depuración."),
			_T("Debug info."))
LOCALE_TEXT(DESC_USE_DEBUG,
			_T("디버깅 정보를 활성화합니다."),
			_T("デバッグ情報を有効にします。"),
			_T("Habilitar la información de depuración."),
			_T("Enable debugging information."))
LOCALE_TEXT(AUTO_UPDATE,
			_T("자동 업데이트"),
			_T("自動更新"),
			_T("Actualización automática"),
			_T("Auto Update"))
LOCALE_TEXT(DESC_AUTO_UPDATE,
			_T("자동 업데이트를 활성화 합니다. (MinGW)"),
			_T("自動更新を有効にします。(MinGW)"),
			_T("Habilite las actualizaciones automáticas. (MinGW)"),
			_T("Enable automatic updates. (MinGW)"))
LOCALE_TEXT(CODING_A_STYLE,
			_T("코딩 A-스타일"),
			_T("コーディングA-スタイル"),
			_T("Codificación A-style"),
			_T("Coding A-style"))
LOCALE_TEXT(DESC_CODING_A_STYLE,
			_T("A-Style을 사용하여, C/C++ 코딩 스타일을 체크하고 보정합니다."),
			_T("A-Styleを使用して、C/C++ コーディングスタイルをチェックして補正します。"),
			_T("Usando A-Style, verifique y corrija el estilo de codificación para C/C ++."),
			_T("Using A-Style, check and correct the coding style for C/C++."))
LOCALE_TEXT(CODING_I_STYLE,
			_T("코딩 I-스타일"),
			_T("コーディングI-スタイル"),
			_T("Codificación I-style"),
			_T("Coding I-style"))
LOCALE_TEXT(DESC_CODING_I_STYLE,
			_T("i-Style을 사용하여, Verilog/VHDL 코딩 스타일을 체크하고 보정합니다."),
			_T("i-Styleを使用して、Verilog/VHDL コーディングスタイルをチェックして補正します。"),
			_T("Con i-Style, verifique y corrija el estilo de codificación para Verilog/VHDL."),
			_T("Using i-Style, check and correct the coding style for Verilog/VHDL."))
LOCALE_TEXT(STRICT_BUILD,
			_T("엄격한 빌드"),
			_T("厳しいビルド"),
			_T("Construcción estricta"),
			_T("Strict build"))
LOCALE_TEXT(DESC_STRICT_BUILD,
			_T("컴파시 문법 경고를 오류로 처리합니다."),
			_T("コンパイル時に文法の警告をエラーとして処理します。"),
			_T("Trate las advertencias como errores al compilar."),
			_T("Treat warnings as errors when compiling."))
LOCALE_TEXT(AUTHOR,
			_T("개발자 서명"),
			_T("開発者の署名"),
			_T("Signo desarrollador"),
			_T("Developer sign"))
LOCALE_TEXT(DESC_AUTHOR,
			_T("소스에 기록될 서명을 지정합니다."),
			_T("ソースに書き込まれる署名を指定します。"),
			_T("Especifique el autor que se escribe en la fuente."),
			_T("Specify the author that is written to the source."))
LOCALE_TEXT(AUTHOR_IS_EMPTY,
			_T("개발자 서명이 비어 있습니다. 속성에서 서명을 기입하세요."),
			_T("開発者の署名がありません。属性で署名を記入してください。"),
			_T("La firma del desarrollador está vacía. Por favor complete el letrero en la propiedad."),
			_T("Developer's signature is empty. Please fill out the sign on the property."))
LOCALE_TEXT(SIM_TOP,
			_T("시뮬레이션 탑 디자인"),
			_T("シミュレーショントップ·デザイン"),
			_T("Diseño superior de simulación"),
			_T("Simulation top design"))
LOCALE_TEXT(DESC_SIM_TOP,
			_T("시뮬레이션 탑 디자인 파일을 지정합니다."),
			_T("シミュレーショントップのデザインファイルを指定します。"),
			_T("Especifica el archivo de diseño superior de simulación."),
			_T("Specifies the simulation top design file."))
LOCALE_TEXT(SIM_DEFINITION,
			_T("시뮬레이션 정의"),
			_T("シミュレーションの定義"),
			_T("Definición de simulación"),
			_T("Simulation definition"))
LOCALE_TEXT(DESC_SIM_DEFINITION,
			_T("시뮬레이션 정의 또는 Verilator의 옵션을 지정합니다.\n여러 정의를 사용할 때에는 ' '(space) 또는 ';' 문자를 사용합니다.\n\nex.) USE_ALU=0;SW_FP"),
			_T("シミュレーションの定義またはVerilatorのオプションを指定します。\n複数の定義を使用する場合は' '(space)または ';'文字を使用します。\n\nex.) USE_ALU=0;SW_FP"),
			_T("Especifica la definición de simulación o la opción de Verilator.\nSi desea usar más de una definición, use ' ' (espacio) o ';' personaje.\n\nej.) USE_ALU=0;SW_FP"),
			_T("Specifies the simulation definition or Verilator's option.\nIf you want to use more than one definition, use ' '(space) or ';' character.\n\nex.) USE_ALU=0;SW_FP"))
LOCALE_TEXT(WAVE_OUTPUT,
			_T("웨이브 출력"),
			_T("波形出力"),
			_T("Wave output"),
			_T("Wave output"))
LOCALE_TEXT(DESC_WAVE_OUTPUT,
			_T("시뮬레이션 웨이브 출력을 지정합니다.\n'None' : 출력 없음.\n'File output' : FST 포멧의 파일로 저장.\n'Interactive' : GTKWave에 실시간 출력.\n이 설정은 하위 시스템이 'Simulation'으로 설정되어야 합니다."),
			_T("シミュレーション波形出力を指定します。\n'None' : 出力はありません。\n'File output' : FSTフォーマットのファイルとして保存します。\n'Interactive' : GTKWaveにリアルタイムで出力します。\nこの設定は、サブシステムが'Simulation'に設定する必要があります。"),
			_T("Especifique la forma de onda de simulación.\n'None' : Ninguna salida.\n'File output' : Guarde el archivo en formato FST.\n'Interactive' : Modo interactivo GTKWave.\nSe debe establecer este subsistema de configuración 'Simulación'."),
			_T("Specify the simulation waveform.\n'None' : No output.\n'File output' : Save the file to FST format.\n'Interactive' : GTKWave interactive mode.\nThis setting subsystem 'Simulation' must be set."))
LOCALE_TEXT(SIMULATION_MULTITHREADING,
			_T("시뮬레이션 멀티스레딩"),
			_T("シミュレーションマルチスレッド"),
			_T("Simulación multihilo"),
			_T("Simulation multithreading"))
LOCALE_TEXT(DESC_SIMULATION_MULTITHREADING,
			_T("시뮬레이션 멀티스레딩 레벨을 지정합니다.\nDisable : 사용안함.\nEnable(HDL) : HDL만 적용.\nEnable(HDL+DPI) : HDL과 DPI에 모두 적용.(DPI code 구현은 thread safe 여부를 확인해야 합니다. 또한 HDL 상의「UNOPTFLAT」경고를 모두 제거해야만 정상적인 멀티스레딩 가속이 됩니다.)"),
			_T("シミュレーションのマルチスレッドレベルを指定します。\nDisable : を無効にする.\nEnable(HDL) : HDLのみ適用.\nEnable(HDL+DPI) : HDLとDPIの両方に適用.(DPI code実装はthread safeかどうかを確認する必要があります。また、HDL上「UNOPTFLAT」の警告をすべて削除しなければなら正常なマルチスレッド加速になります。)"),
			_T("Especifica el nivel de simulación de subprocesos múltiples.\nDisable : No multihilo.\nEnable(HDL) : Aplicar solo a HDL.\nEnable(HDL+DPI) : Aplicar a HDL y DPI tanto.(El usuario debe considerar la seguridad de los hilos en los códigos DPI. Además, si tiene que eliminar todas las advertencias de 「UNOPTFLAT」 en el HDL, será una aceleración de subprocesos normal.)"),
			_T("Specifies the simulation multithreading level.\nDisable : No multithreading.\nEnable(HDL) : Apply to HDL only.\nEnable(HDL+DPI) : Apply to HDL and DPI both.(User must consider about thread safe on DPI codes. Also, if you have to delete all the warnings of「UNOPTFLAT」on the HDL, it will be normal multi-thread acceleration.)"))
LOCALE_TEXT(WAVE_OUTPUT_FILENAME,
			_T("출력 파일명;fst;fst 파일들 (*.fst)|*.fst|모든 파일들 (*.*)|*.*||"),
			_T("出力ファイル名;fst;fst ファイル (*.fst)|*.fst|すべてのファイル (*.*)|*.*||"),
			_T("Nombre del archivo de salida;fst;fst archivos (*.fst)|*.fst|Todos los archivos (*.*)|*.*||"),
			_T("Output filename;fst;fst files (*.fst)|*.fst|All files (*.*)|*.*||"))
LOCALE_TEXT(DESC_OUTPUT_FILENAME,
			_T("시뮬레이션 웨이브 파형 결과 파일명(.fst)을 지정합니다.\n이 설정은 하위 시스템이 'Simulation'으로 설정되어야 하고 '웨이브 출력'이 'File output'로 설정 되어야 합니다.\n파일명은 탑 디자인의 경로로부터 재구성됩니다.\n예) top_design = Test/top.v\n    sim.fst => Test/top.sim.fst"),
			_T("シミュレーション波形の波形の結果ファイル名（.fst）を指定します。\nこの設定は、サブシステムが'Simulation'に設定し、'波形出力'が 'File output'で設定する必要があります。\nファイル名は、トップデザインのパスから再構成されます。\n例) top_design = Test/top.v\n    sim.fst => Test/top.sim.fst"),
			_T("Especifique los resultados de la simulación del archivo de forma de onda (.fst).\nPara habilitar esta configuración, establezca 'Subsistema' en 'Simulation' y debe configurarse 'Salida de onda' en 'File output'.\nEl nombre del archivo se reconstruye a partir de la ruta del diseño superior.\nEj.) top_design = Test/top.v\n     sim.fst => Test/top.sim.fst"),
			_T("Specify simulation results of waveform file(.fst).\nTo enable this setting, set 'Subsystem' to 'Simulation' and should be set 'Wave Output' to 'File output'.\nFile name is reconstructed from the path of the top design.\nEx.) top_design = Test/top.v\n     sim.fst => Test/top.sim.fst"))
LOCALE_TEXT(DO_FILENAME,
			_T("DO 파일명;do;do 파일들 (*.do)|*.do|모든 파일들 (*.*)|*.*||"),
			_T("DOファイル名;do;do ファイル (*.do)|*.do|すべてのファイル (*.*)|*.*||"),
			_T("DO nombre del archivo;do;do archivos (*.do)|*.do|Todos los archivos (*.*)|*.*||"),
			_T("DO filename;do;do files (*.do)|*.do|All files (*.*)|*.*||"))
LOCALE_TEXT(DESC_DO_FILENAME,
			_T("시뮬레이션 DO 파일을 지정합니다.\n이 설정은 하위 시스템이 'Simulation'으로 설정되어야 하고 '웨이브 출력'이 'Interactive'로 설정 되어야 합니다.\n파일명은 탑 디자인의 경로로부터 재구성됩니다.\n예) top_design = Test/top.v\n    sim.do => Test/top.sim.do"),
			_T("シミュレーション'DO'ファイルを指定します。\nこの設定は、サブシステムが'Simulation'に設定し、'波形出力'が 'Interactive'で設定する必要があります。\nファイル名は、トップデザインのパスから再構成されます。\n例) top_design = Test/top.v\n    sim.do => Test/top.sim.do"),
			_T("Establecer forma de onda de simulación DO archivo.\nPara habilitar esta configuración, configure 'Subsistema' en 'Simulation' y se debe configurar en modo 'Interactive'.\nEl nombre del archivo se reconstruye a partir de la ruta del diseño superior.\nEj.) top_design = Test/top.v\n     sim.do => Test/top.sim.do"),
			_T("Set simulation waveform DO file.\nTo enable this setting, set 'Subsystem' to 'Simulation' and should be set to 'Interactive' mode.\nFile name is reconstructed from the path of the top design.\nEx.) top_design = Test/top.v\n     sim.do => Test/top.sim.do"))
LOCALE_TEXT(SYSTEM_IS_LOCKED,
			_T("락 상태일 때에는 시스템 속성을 변경할 수 없습니다."),
			_T("ロック状態のときにシステムのプロパティを変更することができません。"),
			_T("Cuando el estado de bloqueo, no puede cambiar los atributos del sistema."),
			_T("When the lock state, you can not change the system attributes."))
LOCALE_TEXT(START_BUILD,
			_T("◈ %s 빌드 시작..."),
			_T("◈ %s ビルドを開始する..."),
			_T("◈ %s Iniciar compilación..."),
			_T("◈ %s Start build..."))
LOCALE_TEXT(START_FAILD,
			_T("◈ %s 빌드 실패!"),
			_T("◈ %s ビルドに失敗!"),
			_T("◈ %s ¡La construcción ha fallado!"),
			_T("◈ %s Build is failed!"))
LOCALE_TEXT(START_SUCCEED,
			_T("◈ %s 빌드 성공."),
			_T("◈ %s ビルドに成功."),
			_T("◈ %s Construir es tener éxito."),
			_T("◈ %s Build is succeed."))
LOCALE_TEXT(TRACE_START_TIME,
			_T("트레이스 시작 시간"),
			_T("トレースの開始時刻"),
			_T("Tiempo de inicio de seguimiento"),
			_T("Trace start time"))
LOCALE_TEXT(DESC_TRACE_START_TIME,
			_T("트레이스 시작 시간(㎲)을 지정합니다."),
			_T("トレースの開始時刻(㎲)を指定します。"),
			_T("Especifica el tiempo de inicio (㎲) de la traza."),
			_T("Specifies the start time(㎲) of the trace."))
LOCALE_TEXT(BUILD_CANCELED_BY_USER,
			_T("「Control」 키가 1초 이상 눌러져 있어, 소스빌드가 생략됩니다."),
			_T("「Control」 キーが1秒以上押されており、ソースビルドが省略されます。"),
			_T("La tecla 「Control」 se presiona durante 1 segundo o más, por lo que se omite la compilación de origen."),
			_T("「Control」 key is pressed for 1 second or longer, so source build is omitted."))
LOCALE_TEXT(UNRECOGNIZED_CODE,
			_T("인식할 수 없는 코드입니다. : '%s' ('%s', %d 번째 줄)"),
			_T("認識できないコードです。 : '%s' ('%s', %d 行目)"),
			_T("Código no reconocido : '%s' en ('%s', Línea %d)"),
			_T("Unrecognized code. : '%s' in ('%s', Line %d)"))
LOCALE_TEXT(CONTENTS_IS_EMPTY,
			_T("내용이 비워져 있습니다. : '%s' ('%s', %d 번째 줄)"),
			_T("内容が空欄ます。 : '%s' ('%s', %d 行目)"),
			_T("El contenido esta vacio. : '%s' en ('%s', Línea %d)"),
			_T("The content is empty. : '%s' in ('%s', Line %d)"))
LOCALE_TEXT(TOPDESIGN_FIRST,
			_T("탑 디자인이 먼저 선언되어야 합니다. : '%s' ('%s', %d 번째 줄)"),
			_T("トップデザインが最初に宣言する必要があります。 : '%s' ('%s', %d 行目)"),
			_T("El diseño superior debe declararse primero. : '%s' en ('%s', Línea %d)"),
			_T("The top design should be declared first. : '%s' in ('%s', Line %d)"))
LOCALE_TEXT(DO_ESSENTIAL_BUILD,
			_T("필수 시스템 빌드 절차를 확인 중입니다..."),
			_T("必要なシステムを構築を確認中です。"),
			_T("Comprobación de la secuencia de compilación del sistema esencial..."),
			_T("Checking essential system build sequence..."))
LOCALE_TEXT(DONE,
			_T("완료!"),
			_T("完了!"),
			_T("¡Hecho!"),
			_T("Done!"))
LOCALE_TEXT(INFO_BUILD_ALL,
			_T("전체 빌드는 '프로파일 뷰'에서 'System / Build / Build All' 을 선택하여 언제든 실행 가능합니다."),
			_T("フルビルドは「プロファイルビュー」から「System / Build / Build All」を選択して、いつでも実行可能です。"),
			_T("La compilación completa se puede ejecutar en cualquier momento seleccionando 'System / Build / Build All' en la 'Vista de perfil'."),
			_T("The entire build can be run at any time by selecting 'System / Build / Build All' in the 'Profile View'."))
LOCALE_TEXT(MONTHLY_UPDATE,
			_T("리눅스 시스템 업데이트를 시도합니다."),
			_T("Linuxシステムの更新をしようとします。"),
			_T("Intentando actualizar su sistema Linux."),
			_T("Trying to update your Linux system."))
LOCALE_END
