# 소개

만약 소개 과정을 생략하고 바로 사용하고자 한다면, 반드시 '@<bookmark:@선행 과정>'을 참고하여 진행해 주시기 바랍니다.
신규 기능이 필요하거나 개선사항이나 버그를 발견하시면, 정형기(@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>)에게 건의 부탁드리며, 이 문서 또한 docgen으로 작성 및 생성 되었음을 알려드립니다.

## 주요 기능

docgen 은 Template 워드(.docx) 파일을 참조하여 새로운 WORD 문서(.docx)와 PDF 문서(.pdf)를 자동 생성 해주는 툴입니다.

기본적으로 CodeGen을 통해 범용적인 @<link:https://ko.wikipedia.org/wiki/%EB%A3%A8%EC%95%84_(%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4);Lua> 프로그래밍 환경과 markdown에 호환되는 문법을 통해 text 형태로 문서를 작성할 수 있습니다. 주요 기능을 나열하면 아래와 같습니다.

* Lua 스크립트를 통해 다양한 문자 변조 기능 지원
* WORD 및 PDF 파일 생성
* 자동화된 절/캡션(Caption)/상호참조 생성 기능
* 그림, 표, 스타일 서식, 코드 인용, 수학 수식 표현 기능
* 워터마크(water mark) 삽입 기능
* 워드 템플릿 문서 기반으로 사용자가 손쉽게 다양한 스타일 변경 구축과 일관된 서식 적용

 
## 문서 자동화 생성 과정
문서 자동화 생성 과정은 아래와 같은 단계로 진행됩니다.

* >1. 템플릿 문서(template.docx) 열기
* >2. 사용자 lua 코드로 부터 아래 내용 추가 하기
* >3. .docx 문서 생성
* >4. .docx 문서 필드 갱신 및 .pdf 생성

 
## 실행 방법

docgen 을 실행하기 위해서는 아래와 같이 실행합니다.

```txt
> docgen

Document Generator for TestDrive Profiling Master. v1.3
Usage: docgen  [--help] [-t template] [-l language] input_file [output_file]

      --help                display this help and exit
  -t template               Document template name/file.
                            *** Installed docgen template list ***
                            testdrive      : ** system default template **
  -l language               Document language code string.
                            ('docgen_language' variable in Lua)
  input_file                input Lua file
  output_file               output Microsoft Word(.docx) file
```

실행 명령 : @<fixed>docgen @<color:FF0000>INPUT_LUA_FILE@</color>  @<color:FF0000>OUTPUT_DOCX_FILE@</color>@</fixed>

@<color:FF0000>INPUT_LUA_FILE@</color>에 해당하는 Lua 스크립트를 작성하여 실행하게 되며, @<color:FF0000>OUTPUT_DOCX_FILE@</color>을 지정하지 않을 경우 주어진 property를 참조하여 자동으로 알맞게 생성합니다. '-t' 옵션은 기본 바탕이 될 템플릿 문서를 지정하게 되는데, 템플릿 문서를 지정하지 않을 경우 기본 docgen_template.docx로 지정하도록 되어 있으나 이를 참조/변경하여 다양한 문서 형태를 만들 수 있습니다.
'-l' 옵션은 임의의 언어코드를 지정합니다. Lua 에서는 'docgen_language' 변수로 확인할 수 있고, 문장에서는 @<b>'@<bookmark:@언어코드 변경>'@</b> 표현식을 사용하여, 원하는 언어 코드를 선택할 수 있습니다.

;;;
## 사용상 제한 및 라이센스 허가와 선행 과정

### 제한
WORD의 VBA가 사용되므로 WORD가 설치된 윈도우즈 PC 환경이 필요합니다. 또한 @<bookmark:그림 삽입>에 Visio 파일(vsd/vsdx)을 바로 지정하기 위해서는 Visio 설치가 별도로 필요합니다. 리눅스 상에서도 생성은 가능하나, 문서 전체 필드 업데이트(WORD에서 직접 열어 수동으로 업데이트는 가능)/워터마킹/pdf변환 등의 기능이 수행되지 않습니다.

### 라이센스 허가
docgen 에 구현된 소스는 BSD 라이센스를 준수하며, 문서 생성에 사용된 사용자의 개별 스크립트나 이미지등의 2차 저작물은 온전히 사용자의 소유입니다.
 
### 선행 과정
Microsoft Word 의 설정 자동화 제한으로 수식 표현에 대하여 MS 표준 수식 표현인 Unicode로 초기 고정되어 있습니다. 이를 "LaTeX"로 표현하고자 한다면, @<bookmark:LaTeX setting>와 같이 최초 한번 리본 메뉴의 '수식/변환'(먼저 '삽입/수식'으로 수식이 생성한 후 선택되어야 아래 메뉴가 보임.) 탭에서 '@<b>@<u>/유니코드@</u>@</b>' 대신 '@<b>@<u>{} LaTeX@</u>@</b>'를 선택하는 과정이 필요합니다.

@<img:#media/latex_setting.png;0.3;LaTeX setting>

:::NoteHeading
Microsoft 에서는 현재 이 부분에서 자동 변경 방법을 제공하지 않고 있으며, 왜 변경 기능을 막아놨는지 모르겠음. 단지 LaTeX 표준화를 견제하기 위한 상업적 이유로 추측하고 있습니다.