%%% ko
# 소개

@<img:media/Introduction.vsdx[Introduction];1.0>

docgen은 템플릿 WORD(.docx) 파일을 참조하여, Markdown + HTML + Lua 스크립트를 통해 새로운 WORD(.docx)와 PDF/HTML/RTF등의 다양한 문서를 생성하는 도구입니다.

:::NoteHeading
만약 소개 과정을 생략하고 바로 사용하고자 한다면, 반드시 '@<bookmark:@선행 과정>'을 참고하여 진행해 주시기 바랍니다. \
신규 기능이 필요하거나 개선사항이나 버그를 발견하시면, 정형기(@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>)에게 건의 부탁드리며, 이 문서 또한 docgen으로 작성 및 생성 되었음을 알려드립니다.
%%% en
# Introduction

@<img:media/Introduction.vsdx[Introduction];1.0>

docgen is a tool that refers to a template WORD (.docx) file and creates a new WORD (.docx) and various documents such as PDF/HTML/RTF through Markdown + HTML + Lua script.

:::NoteHeading
If you want to skip the introduction process and use it right away, please be sure to refer to '@<bookmark:@Prerequisites>'. \
If you need new features or find improvements or bugs, please make suggestions to Hyunng-Ki Jeong(@<link:mailto:clonextop@gmail.com;clonextop@gmail.com>). Please note that this document was also written and created with docgen.
%%%




 
%%% ko
## 주요 기능

docgen 은 Template 워드(.docx) 파일을 참조하여 새로운 WORD 문서(.docx)와 PDF 문서(.pdf)를 자동 생성 해주는 툴입니다.

기본적으로 CodeGen을 통해 범용적인 @<link:https://ko.wikipedia.org/wiki/%EB%A3%A8%EC%95%84_(%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4);Lua> 프로그래밍 환경과 markdown에 호환되는 문법을 통해 text 형태로 문서를 작성할 수 있습니다. 주요 기능을 나열하면 아래와 같습니다.

* Lua 스크립트를 통해 다양한 문자 변조 기능 지원
* WORD 및 ODT, PDF, HTML 파일등 다양한 문서 출력 포멧 지원
* 자동화된 절/캡션(Caption)/상호참조 생성 기능
* 그림, 표, 스타일 서식, 코드 블럭, 수학 수식 표현 기능
* 워터마크(water mark) 삽입 기능
* 워드 템플릿 문서 기반으로 사용자가 손쉽게 다양한 스타일 변경 구축과 일관된 서식 적용

%%% en
## Main functions

docgen is a tool that automatically creates new WORD documents (.docx) and PDF documents (.pdf) by referring to template word (.docx) files.

Basically, CodeGen allows you to write documents in text format through a grammar compatible with the universal @<link:https://en.wikipedia.org/wiki/Lua_(programming_language);Lua> programming environment and markdown. The main features are listed below.

* Supports various character modulation functions through Lua script
* Supports various document output formats such as WORD, ODT, PDF, and HTML files.
* Automated clause/caption/cross-reference creation function
* Picture, table, style formatting, code block, and mathematical equation expression functions
* Watermark insertion function
* Users can easily change various styles and apply consistent formatting based on Word template documents
%%%
 



%%% ko
## 문서 자동화 생성 과정
문서 자동화 생성 과정은 아래와 같은 단계로 진행됩니다.

* >1. 템플릿 문서(template.docx) 열기
* >2. 사용자 소스(.md 또는 .lua) 문서로 부터 내용 반영
* >3. 워드(.docx), PDF(.pdf), OpenDocument(.odt), HTML(.html), XML(.xml), RichEdit(.rtf), Text(.txt) 문서 생성
%%% en
## Document automated creation process
The document automatedn creation process proceeds in the following steps.

* >1. Open the template document (template.docx)
* >2. Apply content from user source (.md or .lua) document
* >3. Create Word(.docx), PDF(.pdf), OpenDocument(.odt), HTML(.html), XML(.xml), RichEdit(.rtf), Text(.txt) document
%%%




 
%%% ko
## 실행 방법

docgen 을 실행하기 위해서는 아래와 같이 실행합니다.
%%% en
## How to run

To run docgen, run as follows.
%%%

```txt
> docgen

Document Generator for TestDrive Profiling Master. v1.8
Usage: docgen  [--help] [-t template] [-i template_desc] [-f format] [-l language] [-r|--run=lua_code] input_file [output_file]

      --help                display this help and exit
  -t template               Document template name/file.
                            *** Installed docgen template list ***
                            testdrive      : TestDrive Profiling Master
                            (default : testdrive)
  -i template_desc          install new docgen template.
                            ex) docgen -t name -i "description" template.docx
  -f format                 Extra output format.
                            - supported output format
                            html : Standard HTML format
                            odt  : OpenDocument Text format
                            pdf  : Portable Document Format
                            rtf  : Rich Text Format
                            txt  : Plain text format (unicode)
                            xml  : Extensible Markup Language format
                            (ex: -f pdf,html)
  -l language               Document language code string.
                            'docgen.language' variable in Lua
                            (default : 'en')
  -r, --run=lua_code        Run Lua snippet code
  input_file                input Lua or .md(markdown) file
  output_file               output Microsoft Word(.docx) file
```
 
%%% ko
실행 명령 : @<fixed>docgen @<color:FF0000>INPUT_LUA_FILE@</color>  @<color:FF0000>OUTPUT_DOCX_FILE@</color>@</fixed>

@<color:FF0000>INPUT_LUA_FILE@</color>에 해당하는 Lua 스크립트 또는 Markdown 문서를 입력 소스 파일로 지정하며, @<color:FF0000>OUTPUT_DOCX_FILE@</color>을 지정하지 않을 경우 주어진 property를 참조하여 자동으로 알맞게 생성합니다. \
'-t' 옵션은 기본 바탕이 될 템플릿 문서를 지정하게 되는데, 템플릿 문서를 지정하지 않을 경우 기본 docgen_template.docx로 지정하도록 되어 있으나 이를 참조/변경하여 다양한 문서 형태를 만들 수 있습니다.
'-i' 옵션은 새로운 템플릿 문서를 추가할 수 있습니다. 이 때, 'input_file' 이 새로운 템플릿 문서 원본이며, 'template' 가 지정할 타겟 템플릿 이름입니다.
'-f' 옵션은 .docx 포멧 이외에 다른 파일로 저장할 수 있도록 지정합니다. '-f pdf,html' 과 같이 복합적으로 출력을 지정할 수 있습니다.
'-l' 옵션은 임의의 언어코드를 지정합니다. Lua 에서는 'docgen.language' 변수로 확인할 수 있고, 문장에서는 @<b>'@<bookmark:@문서 활성화>'@</b> 표현식을 사용하여, 원하는 언어 코드를 선택할 수 있습니다.
'-r' 옵션은 임의의 선행 Lua 코드를 지정합니다. 이 코드는 문서 구조를 변경을 위한 조건을 넣을 수 있습니다.

:::NoteHeading
기본 설정은 'en'(english) 이며, 이 'language' 설정을 사용하지 않는 문서에는 영향을 주지 않습니다.

%%% en
command line : @<fixed>docgen @<color:FF0000>INPUT_LUA_FILE@</color>  @<color:FF0000>OUTPUT_DOCX_FILE@</color>@</fixed>

In @<color:FF0000>INPUT_LUA_FILE@</color>, set a Lua script or Markdown text file as the input source file. If @<color:FF0000>OUTPUT_DOCX_FILE@</color> is not specified, it is automatically created appropriately by referring to the given property. \
The '-t' option specifies the template document that will be the base. If the template document is not specified, the default docgen_template.docx is specified, but various document types can be created by referencing/changing it.
The '-i' option allows adding a new template document. At this time, 'input_file' is the new template document source, and 'template' is the target template name to be specified.
The '-f' option specifies saving as a file other than .docx format. You can specify output in combination, such as '-f pdf,html'.
The '-l' option specifies an arbitrary language code. In Lua, you can check it with the 'docgen.language' variable, and in sentences, you can use the expression @<b>'@<bookmark:@Document activation>'@</b> to select the desired language code.
The '-r' option specifies arbitrary preceding Lua code. This code can contain conditions for changing the document structure.

:::NoteHeading
The default setting is 'en' (english) and will have no effect on documents that do not use this 'language' setting.
%%%
;;;


 
%%% ko
### 프로젝트 생성 예

아래와 같이 명령어를 입력하여, 간소화된 프로젝트를 생성하고 빌드 할 수 있습니다.

%%% en
### Project creation example

아래와 같이 명령어를 입력하여, 간소화된 프로젝트를 생성하고 빌드 할 수 있습니다.
%%%

```txt
> create_project docgen_simplified example
*I: Create DocGen simplified project : 'example'
Run 'build.bat' to build document.

> cd example

> ls
build.bat  main.md  media

> build.bat
1. Introduction
   Main functions
2. first Contents
   media contents
*I: Link all bookmarks.
*I: Build document : Header_Name_userguide_rev1.0.docx
*I: Fields calculation & Saving to PDF output : Header_Name_userguide_rev1.0.pdf

```



 
%%% ko
## 사용상 제한 및 라이센스 허가와 선행 과정
%%% en
## Restrictions on use, licensing and prerequisites
%%%




%%% ko
### 제한
WORD의 VBA가 사용되므로 WORD가 설치된 윈도우즈 PC 환경이 필요합니다. \
또한 @<bookmark:그림 삽입>에 Visio 파일(vsd/vsdx)을 바로 지정하기 위해서는 Visio 설치가 별도로 필요하며, Excel 차트를 이용하기 위해서 Excel 설치가 추가적으로 필요합니다. \
리눅스 상에서도 생성은 가능하나, 문서 전체 필드 업데이트(WORD에서 직접 열어 수동으로 업데이트는 가능)/워터마킹/pdf변환 등의 기능이 수행되지 않습니다.
%%% en
### Limitation
Since WORD's VBA is used, a Windows PC environment with WORD installed is required. \
Additionally, in order to directly specify a Visio file (vsd/vsdx) in @<bookmark:Picture insertion>, \
Visio installation is required separately. \
And to use Excel charts, additional installation of Excel is required. \
Creation is possible on Linux, but functions such as updating entire document fields (can be updated manually by opening directly in WORD)/watermarking/PDF conversion are not performed.
%%%




%%% ko
### 라이센스 허가
docgen 에 구현된 소스는 BSD 라이센스를 준수하며, 문서 생성에 사용된 사용자의 개별 스크립트나 이미지등의 2차 저작물은 온전히 사용자의 소유입니다.
%%% en
### Licenses
The source code implemented in docgen complies with the BSD license, and secondary works such as the user's individual scripts or images used to create the document are entirely owned by the user.
%%%





%%% ko 
### 선행 과정
Microsoft Word 의 설정 자동화 제한으로 수식 표현에 대하여, 초기에 MS 표준 수식 표현인 <b>유니코드</b>로 고정되어 있습니다. \
이를 "LaTeX"로 표현하고자 한다면, @<bookmark:LaTeX 설정>와 같이 최초 한번 리본 메뉴의 '수식/변환'(먼저 '삽입/수식'으로 수식이 생성한 후 선택되어야 아래 메뉴가 보임.) 탭에서 '@<b>@<u>/유니코드@</u>@</b>' 대신 '@<b>@<u>{} LaTeX@</u>@</b>'를 선택하는 과정이 필요합니다.

@<img:#media/latex_setting.png;0.3;LaTeX 설정>
%%% en
### Prerequisites
Due to limitations in Microsoft Word's setting automation, the math equation expression is initially set to <b>Unicode</b>, the MS standard math equation expression. \
If you want to express this in "LaTeX", tab 'Equation/Conversions' on the ribbon menu (the menu below must be selected after creating the math equation with 'Insert/Equation' first) as in @<bookmark:LaTeX setting>. Select '@<b>@<u>{} LaTeX@</u>@</b>' instead of '@<b>@<u>/Unicode@</u>@</b>' A process is required.

@<img:#media/latex_setting_en.png;0.3;LaTeX setting>
%%%

%%% ko
:::NoteHeading
Microsoft 에서는 현재 이 부분에서 자동 변경 방법을 제공하지 않고 있으며, 왜 변경 기능을 막아놨는지 모르겠음.
%%% en
:::NoteHeading
Microsoft currently does not provide an automatic change method in this area, and I do not know why the change function is blocked.
%%%




 
%%% ko
## 알려진 문제들

### Excel table의 지역화 문제

엑셀의 숫자 데이터의 표현에 대해서, 원래는 기본 표현형식 날짜의 'number format'이 국가 설정에 따라 다릅니다.

예) 2024년 3월 8일의 경우
```txt
미국 : number format("M/D/YYYY")
       -> '3/8/2024'
한국 : number format("YYYY-M-D")
       -> 2024-3-8
```
 
위와 같이 국가설정에 따라 연도/월/일 표시 순서가 다르지만, 여기서는 미국 표현형식만을 사용합니다. \
만약 다른 형식을 사용하고자 한다면, excel에서 기본 형식이 아닌, 직접  사용자 정의 'number format'을 입력해야 합니다.

%%% en
## Knwon issues

### Localization issue in Excel table

Regarding the expression of numeric data in Excel, the 'number format' of the default date format originally differs depending on the country setting.

예) For March 8, 2024
```txt
United state : number format("M/D/YYYY")
               -> '3/8/2024'
Korea        : number format("YYYY-M-D")
               -> 2024-3-8
```
 
As shown above, the year/month/day display order varies depending on the country setting, but only the US format is used here. \
If you want to use a different format, you must enter custom 'number format' directly in Excel, not the default format.

%%%
