%%% ko
# Lua 표현

Lua 스크립트는 매우 가벼우면서 속도가 빠르고 단순하여, 비-프로그래머를 위해 주로 사용되는 스크립트 언어입니다. \
'워크래프트 WoW', '앵그리버드' 등 각종 게임에서 비 프로그래머인 게임 디자이너가 사용한 예시가 있습니다.
docgen 또한 Lua로 작성되어 있으며, MarkDown+HTML 표현법 이외에 Lua 문법을 사용하여 문서를 보다 세밀하게 구성할 수 있습니다.
Lua 기능엔 @<link:https://testdrive-profiling-master.github.io/download/codegen_userguide.pdf;codegen>에서 \
추가된 기능을 모두 사용할 수 있고, 추가적으로 아래의 문법들이 사용될 수 있습니다.

* 외부 Lua 관련 링크
** @<link:https://librewiki.net/wiki/%EC%8B%9C%EB%A6%AC%EC%A6%88:%EC%89%BD%EA%B2%8C_%EB%B0%B0%EC%9A%B0%EB%8A%94_%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%9E%85%EB%AC%B8/Lua;쉽게 배우는 프로그래밍 입문/Lua>
** docgen 변수들
** AddRevision() 함수
** AddTerm() 함수
** AddParagraph() 함수

여기서는 docgen에 추가된 기능만을 설명하고 있으며, Lua 에 대한 학습 없이 최소 template 구현만으로 문서를 생성할 수 있습니다.
%%% en
# Lua expression

Lua scripts are very lightweight, fast, and simple, making them the preferred script language for non-programmers. \
There are examples of non-programmer game designers using it in various games such as 'Warcraft WoW' and 'Angry Birds'.
docgen is also written in Lua, and documents can be structured in more detail using Lua syntax in addition to the MarkDown+HTML expression.
All functions added in @<link:https://testdrive-profiling-master.github.io/download/codegen_userguide.pdf;codegen> can be used for Lua functions, and the syntax below can be used additionally.

* External Lua-related links
** @<link:https://en.wikipedia.org/wiki/Lua_(programming_language)/Lua wiki>
** docgen variables
** AddRevision() function
** AddTerm() function
** AddParagraph() function

Here, only the features added to docgen are explained, and documents can be created with minimal template implementation without learning Lua.
%%%





;;;
%%% ko
## docgen 변수들

Lua에서 사용 가능한 변수 목록은 아래와 같습니다.

@<tbl:#media/docgen_variables.xlsx;ko;docgen 변수 목록>
%%% en
## docgen variables

Here is a list of variables available in Lua:

@<tbl:media/docgen_variables.xlsx;en;docgen variables list>
%%%


 
%%% ko
### Property 객체
```lua
docgen.property["Document_Name"]			-- 문서 이름
docgen.property["IP_Version"]				-- IP 버전 (예:"1.00")
docgen.property["Main_Title"]				-- 문서 첫장의 메인 타이틀 이름
docgen.property["Sub_Title"]				-- 문서 첫장의 서브 타이틀 이름 (생략 가능)
docgen.property["IP_Name_First_Page"]		-- 문서 첫장의 이름
docgen.property["IP_Name_Header"]			-- 해더 및 파일 상의 이름
docgen.property["Ownership"]				-- 오너쉽 명
docgen.property["Document_Name_Header"]		-- 해더 이름 (...)
docgen.property["Water_Mark"]				-- 워터마킹 문구 (생략 가능)
```
:::Note
Lua 문법에서 "--" 로 시작하는 문장은 '주석' 표기를 의미합니다. \
위 속성 이름은 문서에 따라 다를 수 있으며, 위에 나열된 이름들은 기본 TestDrive 문서에 사용된 한 예시이며, \
"Water_Mark"를 제외하고 고정된 이름이 아닙니다.

 
워드에는 메뉴 "파일/정보"에 @<bookmark:WORD 속성 탭>와 같이 '속성' 탭이 존재합니다.


@<img:media/advanced_properties.jpg;0.8;WORD 속성 탭>

이 메뉴에서 사용자가 원하는 Field 속성을 추가하거나, 기존 Property를 변경할 수 있으며, \
선언된 Property는 문서 전체의 Field 정보에 반영됩니다.

이걸 lua 코드 내에서는 아래와 같은 방법으로 템플릿 문서에 이미 존재하는 필드 값을 변경 가능합니다.
또는 '@<bookmark:속성(Property) 참조>'에서 처럼 markdown 문장 내에서 직접적인 참조가 가능합니다.

예) 이 문서의 Field 지정 예시
%%% en
### Property object
```lua
docgen.property["Document_Name"]			-- Document name
docgen.property["IP_Version"]				-- IP version (ex."1.00")
docgen.property["Main_Title"]				-- Main title name on the first page of the document
docgen.property["Sub_Title"]				-- Sub title name on the first page of the document (Can be skipped)
docgen.property["IP_Name_First_Page"]		-- Name of the first page of the document
docgen.property["IP_Name_Header"]			-- Header and name on file
docgen.property["Ownership"]				-- Ownership name
docgen.property["Document_Name_Header"]		-- header name (...)
docgen.property["Water_Mark"]				-- watermarking text (Can be skipped)
```
:::Note
Sentences starting with "--" is 'comment' notation in Lua. \
The above property names may vary across documents, \
the names listed above are examples used in the default TestDrive document and are not fixed names, except for "Water_Mark".

 
In Word, there is a 'Property' tab in the menu "File/Information" as like @<bookmark:Property tab in WORD>.

@<img:media/advanced_properties_en.jpg;0.8;Property tab in WORD>

In this menu, you can add the <b>Field</b> property you want or change the existing <b>Property</b>, and \
the declared <b>property</b> is reflected in the <b>Field</b> information throughout the document.

Within the lua code, you can change the field value that already exists in the template document in the following way.
Alternatively, direct reference is possible within markdown sentences, such as '@<bookmark:Property reference>'.

ex) Example of field designation in this document
%%%
```lua
docgen.property["Document_Name"]			= "UserGuide"
docgen.property["IP_Version"]				= "1.00"
docgen.property["Main_Title"]				= "Document Generator"
docgen.property["IP_Name_First_Page"]		= "User Guide"
docgen.property["IP_Name_Header"]			= "document_generator"
docgen.property["Ownership"]				= "TestDrive"
docgen.property["Document_Name_Header"]		= "userguide"
docgen.property["Water_Mark"]				= "TESTDRIVE CONFIDENTIAL"
```





;;;
%%% ko
## AddRevision() 함수
@<size:30>@<b>함수 원형 : AddRevisoion(@<color:FF0000>버전@</color>, @<color:FF0000>연도@</color>, @<color:FF0000>월@</color>, @<color:FF0000>일@</color>, @<color:FF0000>설명@</color>)@</b>@</size>

문서의 버전 정보를 관리하는 함수입니다. 아래 예시와 같이 버전 순서로 나열할 수 있으며, \
@<color:FF0000>설명@</color>은 '@<b>@<bookmark:@Paragraph 표현>@</b>'을 사용할 수 있습니다.
AddRevision() 함수를 한번도 사용하지 않을 경우는 "Document Revision History" 절은 자동으로 제거됩니다.

예)
```lua
AddRevision("1.0",	2022, 1,  14,	"Initial Draft")
AddRevision("1.1",	2022, 2,  15,	"Second release")
AddRevision("1.2",	2022, 3,  16,	"Third release")
```

결과)
@<img:#media/revision_history.png;0.8;AddRevision() 함수 사용 예시>
%%% en
## AddRevision() function
@<size:30>@<b>Function prototype : AddRevisoion(@<color:FF0000>version@</color>, @<color:FF0000>year@</color>, @<color:FF0000>month@</color>, @<color:FF0000>day@</color>, @<color:FF0000>description@</color>)@</b>@</size>

This is a function that manages the version information of a document. They can be listed in version order as shown in the example below, \
and @<color:FF0000>description@</color> can use '@<b>@<bookmark:@Paragraph expression>@</b>'.
If you never use the AddRevision() function, the "Document Revision History" section is automatically removed.

ex)
```lua
AddRevision("1.0",	2022, 1,  14,	"Initial Draft")
AddRevision("1.1",	2022, 2,  15,	"Second release")
AddRevision("1.2",	2022, 3,  16,	"Third release")
```

Result)
@<img:#media/revision_history.png;0.8;AddRevision() function example>
%%%





;;;
%%% ko
## AddTerm() 함수
@<size:30>@<b>함수 원형 : AddTerm(@<color:FF0000>단어@</color>, @<color:FF0000>설명@</color>)@</b>@</size>

단어에 대한 설명을 삽입하는 함수입니다. 아래 예시와 같이 사용될 수 있으며, @<color:FF0000>설명@</color>은 '@<b>@<bookmark:@Paragraph 표현>@</b>'을 사용할 수 있습니다.
AddTerm() 함수를 한번도 사용하지 않을 경우는 "List of Terms" 절은 자동으로 제거됩니다.

```lua
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://ko.wikipedia.org/wiki/루아_(프로그래밍_언어);Wiki>, @<link:http://www.lua.org/;Homepage>)")
```
결과)
@<img:#media/list_of_terms.png;0.8;AddTerm() 함수 사용 예시>
%%% en
## AddTerm() function
@<size:30>@<b>Function prototype : AddTerm(@<color:FF0000>word@</color>, @<color:FF0000>description@</color>)@</b>@</size>

This function inserts a description of a word. It can be used as in the example below, and @<color:FF0000>Description@</color> can use '@<b>@<bookmark:@Paragraph expression>@</b>'.
If you never use the AddTerm() function, the "List of Terms" clause is automatically removed.

```lua
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://en.wikipedia.org/wiki/Lua_(programming_language);Wiki>, @<link:http://www.lua.org/;Homepage>)")
```
Result)
@<img:#media/list_of_terms.png;0.8;AddTerm() function example>
%%%





;;;
%%% ko
## AddParagraph() 함수
@<size:30>@<b>함수 원형 : AddParagraph(@<color:FF0000>문장@</color>, [@<color:FF0000>소스명@</color>])@</b>@</size>

실제 문서 내용을 채울 수 있으며, @<color:FF0000>문장@</color>은 '@<b>@<bookmark:@Paragraph 표현>@</b>'을 사용할 수 있습니다.
@<color:FF0000>문장@</color>의 내용을 직접 문자열로 기술할 수 있지만, \
@<color:FF0000>문장@</color>을 "@<b>[[@<color:FF0000>파일명@</color>]]@</b>"으로 기술하면 외부 text 파일로 참조하게 되며, \
직접 문장 표기는 POSIX 이스케이프 문자 표기법에 영향을 받으므로 소프트 프로그래밍에 익숙하지 않은 사용자는 "예 #2)"와 같이 외부 text 파일로 기술하는 것을 권장합니다.
@<color:FF0000>소스명@</color>은 문장의 이름을 부여합니다. 이는 실시간 상태 추적을 보일 때, 소스 출처 확인용도로 사용됩니다. \
소스명이 지정되지 않고 문장에 파일로 지정되면, 소스명은 파일 이름을 자동으로 지정됩니다.


예 #1) 직접 구현
```lua
AddParagraph("#제목\
문단 내용 1\
문단 내용 2", "본문1")
```
 
예 #2) 외부 markdown 표현 text 파일 구현
```lua
AddParagraph("[[some.md]]")
```

 
예 #3) 파일 읽기 함수를 활용하여 구현
```lua
do
	local	txt_contents	= String(nil)
	txt_contents:ReadFile("some.md", false)
	AddParagraph(txt_contents.s)
end
```

 
예 #4) 'IsInsert' 변수를 확인하여, 여러 파일 읽은 뒤,
       "%ABC%" 표현을 "good"으로 모두 변경 후 문장 삽입하는 Lua 응용
```lua
if IsInsert == true then	-- directive 확인
	local	txt_contents	= String(nil)
	
	txt_contents:Append(ReadText("some_1.md"))	-- some_1.md 추가
	txt_contents:Append(ReadText("some_2.md"))	-- some_2.md 추가
	txt_contents:Append(ReadText("some_3.md"))	-- some_3.md 추가
	txt_contents:Replace("%ABC%", "good", true)	-- "%ABC%"를 "good"로 모두 변경
	
	AddParagraph(txt_contents.s)	-- 문서 적용
end
```
%%% en
## AddParagraph() function
@<size:30>@<b>Function prototype : AddParagraph(@<color:FF0000>sentence@</color>, [@<color:FF0000>source_name@</color>])@</b>@</size>

You can fill in the actual document content, and @<color:FF0000>sentence@</color> can use '@<b>@<bookmark:@Paragraph expression>@</b>'.

The contents of @<color:FF0000>sentence@</color> can be directly described as a string, \
but the @<color:FF0000>sentence@</color> can be converted to "@<b>[[@<color:FF0000>file_name@</color>]]@</b>" refers to an external text file, \
and direct sentence notation is affected by the POSIX escape character notation, so users unfamiliar with soft programming can use "Example #2)" It is recommended to describe it in an external text file like.
@<color:FF0000>source_name@</color> gives the sentence a name. This is used to identify the source when showing real-time status tracking. \
If the source name is not specified and a file is specified in the sentence, the source name is automatically assigned to the file name.


ex #1) Direct implementation
```lua
AddParagraph("#Title\
Paragraph content 1\
Paragraph content 2", "contents_1")
```
 
ex #2) External markdown expression text file implementation
```lua
AddParagraph("[[some.md]]")
```

 
ex #3) Implemented using the file read function
```lua
do
	local	txt_contents	= String(nil)
	txt_contents:ReadFile("some.md", false)
	AddParagraph(txt_contents.s)
end
```

 
ex #4) Lua application that checks the 'IsInsert' variable, reads several files,
       changes all "%ABC%" expressions to "good", and inserts sentences.
	   
```lua
if IsInsert == true then	-- directive check
	local	txt_contents	= String(nil)
	
	txt_contents:Append(ReadText("some_1.md"))	-- some_1.md added
	txt_contents:Append(ReadText("some_2.md"))	-- some_2.md added
	txt_contents:Append(ReadText("some_3.md"))	-- some_3.md added
	txt_contents:Replace("%ABC%", "good", true)	-- change all "%ABC%" to "good"
	
	AddParagraph(txt_contents.s)	-- Apply to document
end
```
%%%
