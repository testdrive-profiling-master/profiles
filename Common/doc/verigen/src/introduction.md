# 소개

@<i>"성능은 편리함을 이기지 못한다."@</i>
 
verilog 디자인을 설계할 때 가장 문제가 되는 부분 중 하나는 \
모듈간 wire / port 들의 연결에 상당한 시간히 소요되다는 점입니다. \
이 부분을 자동으로 해결하고, 빠르게 모듈간 네트워크를 변경할 수 있도록 하는 툴이 필요힙니다.

:::NoteHeading
신규 기능이 필요하거나 개선사항이나 버그를 발견하시면, 정형기(@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>)에게 건의 부탁드립니다.

## 주요 기능

verigen 은..

## 문서 자동화 생성 과정
verigen 을 통한 프로젝트 작성은 아래와 같은 단계로 진행됩니다.

* >1. 모듈간의 hierarchy 연결 
* >2. A

 
## 실행 방법

docgen 을 실행하기 위해서는 아래와 같이 실행합니다.

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

;;;
## 사용상 제한 및 라이센스 허가와 선행 과정

### 라이센스 허가
verigen 에 구현된 소스는 BSD 라이센스를 준수하며, verilog 생성에 사용된 사용자의 개별 스크립트나 verilog 등의 2차 저작물은 온전히 사용자의 소유입니다.
