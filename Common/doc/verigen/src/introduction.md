# 소개

@<i>"성능이란 건 편리함을 이기지 못한다."@</i>

대단위 프로젝트의 verilog 디자인을 설계할 때, 가장 문제가 되는 부분 중 하나는 \
모듈간 control path 구성에 상당한 시간과 노력이 소요되다는 점입니다. \
또한 많은 시간과 노력을 들여 한번 완성된 디자인의 control path 일부를 수정하거나, \
대대적인 변경이 필요한 경우는 더더욱 신중을 기해야만 합니다. \
그렇지 않으면 새로운 오류를 유발시키거나 디자인을 첨부터 다시 만드는 것과 동일한 노력이 필요하게 될 것입니다.

이에 최소한의 디자인 설계만으로 control path 를 생성해 주는 verigen 툴을 구상했습니다. \
이 툴은 control path 를 프로그래밍 방식으로 최소한의 노력을 통해 쉽고 빠르게 구축할 수 있게 하며, \
짜여진 control path 를 한 눈에 design hierarchy 로 확인 가능케 하는 기능을 가지고 있습니다. \
또한 이로써 보다 빠른 설계 변경과 다른 팀원과의 디자인 공유가 가능할 수 있습니다.


:::NoteHeading
신규 제안할 기능이 있거나 개선사항이나 버그를 발견하시면, 문의(@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>) 부탁드립니다.
 

## 주요 기능

verigen 은 TestDrive Profiling Master 의 codegen 을 이용하여 제작되었습니다. \
이 툴은 lua 로 작성된 코드를 실행하여, verilog 디자인을 구축하며, codegen 기능 전부를 포함하고 있으며, \
아래 파일들을 생성합니다.

* verilog 디자인(.sv, .f) 자동 생성
* constraint(.xdc) 자동 생성
* hierarchy diagram(.svg), HTML highlited source code(.html) 자동 생성
 

## verilog 자동화 생성 과정
verigen 을 통한 프로젝트 작성은 아래와 같은 단계로 진행됩니다.

* >1. Lua 스크립트 작성
** > 1). 모듈 생성
** > 2). 모듈 연결
** > 3). 모듈에 parameter 선언 및 연결
** > 4). 모듈에 interface 선언 및 연결
* >2. verigen 실행하여 코드 생성
 
## 실행 방법

verigen을 실행하기 위해서는 아래와 같이 실행합니다.

```txt
> verigen

Verilog Generator for TestDrive Profiling Master. v1.00
Usage: verigen  [--help] input_file [output_path]

      --help                display this help and exit
  input_file                input Lua file
  output_path               output path
                            default : ./output
```

:::NoteHeading
실행 명령 : @<fixed>verigen @<color:FF0000>INPUT_LUA_FILE@</color>  @<color:FF0000>OUTPUT_PATH@</color>@</fixed>
 
@<color:FF0000>INPUT_LUA_FILE@</color>에 해당하는 Lua 스크립트를 작성하여 실행하게 되며, \
@<color:FF0000>OUTPUT_PATH@</color>을 지정하지 않을 경우 기본 "./output" 폴더에 결과를 생성합니다.
 

## 라이센스 허가
verigen에 구현된 소스는 BSD 라이센스를 준수하며, verilog 생성에 사용된 사용자의 개별 스크립트나 verilog 등의 2차 저작물은 온전히 사용자의 소유입니다.
