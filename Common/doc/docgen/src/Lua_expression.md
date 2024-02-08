%%% ko
# Lua 표현

Lua 스크립트는 매우 가벼고 속도가 빠르며 단순하여, 비-프로그래머를 위해 주로 사용되는 언어입니다. '워크래프트 WoW', '앵그리버드' 등 각종 게임에서 비 프로그래머인 게임 디자이너가 사용한 예시가 있습니다.
docgen 역시 '@<bookmark:@실행 방법>'에서 나열한 것과 같이 docgen.lua 파일을 실행하여 동작합니다.
세부적으로는 범용적인 Lua 의 기본 기능과 codegen에 추가된 기능을 사용하여 docgen을 구현하고, 다시 이 구현을 통해 문서 자동화가 이루어 집니다.
표현은 범용적인 Lua 문법과 CodeGen의 추가 문법 그리고 아래에서 나열되는 기능들이 추가됩니다.

* 외부 Lua 관련 링크
** @<link:https://librewiki.net/wiki/%EC%8B%9C%EB%A6%AC%EC%A6%88:%EC%89%BD%EA%B2%8C_%EB%B0%B0%EC%9A%B0%EB%8A%94_%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%9E%85%EB%AC%B8/Lua;쉽게 배우는 프로그래밍 입문/Lua>
** property[] 속성 객체
** AddRevision() 함수
** AddTerm() 함수
** AddParagraph() 함수

여기서는 docgen 에 추가된 기능만을 설명하고 있으며, Lua 에 대한 학습 없이 최소 template 구현만으로 문서를 생성할 수 있습니다.
%%% en
# Lua expression

Lua scripts are very lightweight, fast, and simple, making them the preferred language for non-programmers. There are examples of non-programmer game designers using it in various games such as 'Warcraft WoW' and 'Angry Birds'.
docgen also operates by executing the docgen.lua file as listed in '@<bookmark:@How to run>'.
In detail, docgen is implemented using general-purpose Lua basic functions and functions added to codegen, and document automation is again achieved through this implementation.
The expression is a general-purpose Lua syntax, CodeGen's additional syntax, and the functions listed below are added.

* External Lua-related links
** @<link:https://en.wikipedia.org/wiki/Lua_(programming_language)/Lua wiki>
** property[] property object
** AddRevision() function
** AddTerm() function
** AddParagraph() function

Here, only the features added to docgen are explained, and documents can be created with minimal template implementation without learning Lua.
%%%





;;;
%%% ko
## 속성 객체
```lua
property["Document_Name"]			-- 문서 이름
property["IP_Version"]				-- IP 버전 (예:"1.00")
property["Main_Title"]				-- 문서 첫장의 메인 타이틀 이름
property["Sub_Title"]				-- 문서 첫장의 서브 타이틀 이름 (생략 가능)
property["IP_Name_First_Page"]		-- 문서 첫장의 이름
property["IP_Name_Header"]			-- 해더 및 파일 상의 이름 (예 : "doc_guide")
property["Ownership"]				-- 오너쉽 명
property["Document_Name_Header"]	-- 해더 이름 (...)
property["Water_Mark"]				-- 워터마킹 문구 (사용하지 않을 경우 비워 둔다.)
```

워드에는 메뉴 "파일/정보" 에 @<bookmark:WORD 속성 탭>와 같이 '속성' 탭이 존재합니다.

@<img:#media/word_property.png;0.4;WORD 속성 탭>

이 메뉴에서 사용자가 원하는 Field 속성을 추가하거나, 기존 Property를 변경할 수 있습니다. 선언된 Property 는 문서 전체의 Field 정보에 반영됩니다.
이걸 lua 코드 내에서는 아래와 같은 방법으로 템플릿 문서에 이미 존재하는 필드 값을 변경 가능합니다.
또는 @<bookmark:속성(Property) 참조>에서 처럼 markdown 문장 내에서 직접적인 참조가 가능합니다.

예) 이 문서의 Field 지정 예시 ("--" 로 시작하는 문장은 Lua에서 '주석' 표기를 의미합니다.)
%%% en
## Property object
```lua
property["Document_Name"]			-- Document name
property["IP_Version"]				-- IP version (예:"1.00")
property["Main_Title"]				-- Main title name on the first page of the document
property["Sub_Title"]				-- Sub title name on the first page of the document (Can be skipped)
property["IP_Name_First_Page"]		-- Name of the first page of the document
property["IP_Name_Header"]			-- Header and name on file (예 : "doc_guide")
property["Ownership"]				-- Ownership name
property["Document_Name_Header"]	-- header name (...)
property["Water_Mark"]				-- watermarking text (Leave blank if not used.)
```

In Word, there is a 'Property' tab in the menu "File/Information" as like @<bookmark:Property tab in WORD>.

@<img:#media/word_property_en.png;0.4;Property tab in WORD>

In this menu, you can add the <b>Field</b> property you want or change the existing <b>Property</b>. The declared <b>property</b> is reflected in the <b>Field</b> information throughout the document.
Within the lua code, you can change the field value that already exists in the template document in the following way.
Alternatively, direct reference is possible within markdown sentences, such as @<bookmark:Property reference>.

ex) Example of field designation in this document (Sentences starting with "--" refer to 'comment' notation in Lua.)
%%%
```lua
property["Document_Name"]			= "UserGuide"
property["IP_Version"]				= "1.00"
property["Main_Title"]				= "Document Generator"
property["IP_Name_First_Page"]		= "User Guide"
property["IP_Name_Header"]			= "document_generator"
property["Ownership"]				= "TestDrive"
property["Document_Name_Header"]	= "userguide"
property["Water_Mark"]				= "TESTDRIVE CONFIDENTIAL"
```





;;;
%%% ko
## AddRevision() 함수
@<size:30>@<b>함수 원형 : AddRevisoion(@<color:FF0000>버전@</color>, @<color:FF0000>연도@</color>, @<color:FF0000>월@</color>, @<color:FF0000>일@</color>, @<color:FF0000>설명@</color>)@</b>@</size>

문서의 버전 정보를 관리하는 함수입니다. 아래 예시와 같이 버전 순서로 나열할 수 있으며, \
@<color:FF0000>설명@</color>은 "@<b>@<bookmark:@Paragraph 표현>@</b>"을 사용할 수 있습니다.
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
and @<color:FF0000>description@</color> can use "@<b>@<bookmark:@Paragraph expression>@</b>".
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




 
%%% ko
## AddTerm() 함수
@<size:30>@<b>함수 원형 : AddTerm(@<color:FF0000>단어@</color>, @<color:FF0000>설명@</color>)@</b>@</size>

단어에 대한 설명을 삽입하는 함수입니다. 아래 예시와 같이 사용될 수 있으며, @<color:FF0000>설명@</color>은 "@<b>@<bookmark:@Paragraph 표현>@</b>"을 사용할 수 있습니다.
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

This function inserts a description of a word. It can be used as in the example below, and @<color:FF0000>Description@</color> can use "@<b>@<bookmark:@Paragraph expression>@</b>".
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
@<size:30>@<b>함수 원형 : AddParagraph(@<color:FF0000>단어@</color>, @<color:FF0000>문장@</color>)@</b>@</size>

실제 문서 내용을 채울 수 있으며, @<color:FF0000>문장@</color>은 "@<b>@<bookmark:@Paragraph 표현>@</b>"을 사용할 수 있습니다.
@<color:FF0000>문장@</color>의 내용을 직접 문자열로 기술할 수 있지만, @<color:FF0000>문장@</color>을 "@<b>[[@<color:FF0000>파일명@</color>]]@</b>"으로 기술하면 외부 text 파일로 참조하게 되며, 직접 문장 표기는 POSIX 이스케이프 문자 표기법에 영향을 받으므로 소프트 프로그래밍에 익숙하지 않은 사용자는 "예 #2)"와 같이 외부 text 파일로 기술하는 것을 권장합니다.


예 #1) 직접 구현
```lua
AddParagraph("#제목\
문단 내용 1\
문단 내용 2")
```
 
예 #2) 외부 markdown 표현 text 파일 구현
```lua
AddParagraph("[[some.txt]]")
```

 
예 #3) 파일 읽기 함수를 할용하여 구현
```lua
do
	local	txt_contents	= String(nil)
	txt_contents:ReadFile("some.txt", false)
	AddParagraph(txt_contents.s)
end
```

 
예 #4) 'IsInsert' 변수를 확인하여, 여러 파일 읽은 뒤,
       "%ABC%" 표현을 "good" 으로 모두 변경 후 문장 삽입하는 Lua 응용
```lua
if IsInsert == true then	-- directive 확인
	local	txt_contents	= String(nil)
	
	txt_contents:Append(ReadText("some_1.txt"))	-- some_1.txt 추가
	txt_contents:Append(ReadText("some_2.txt"))	-- some_2.txt 추가
	txt_contents:Append(ReadText("some_3.txt"))	-- some_3.txt 추가
	txt_contents:Replace("%ABC%", "good", true)	-- "%ABC%"를 "good"로 모두 변경
	
	AddParagraph(txt_contents.s)	-- 문서 적용
end
```
%%% en
## AddParagraph() function
@<size:30>@<b>Function prototype : AddParagraph(@<color:FF0000>word@</color>, @<color:FF0000>sentence@</color>)@</b>@</size>

You can fill in the actual document content, and @<color:FF0000>sentence@</color> can use "@<b>@<bookmark:@Paragraph expression>@</b>".

The contents of @<color:FF0000>sentence@</color> can be directly described as a string, but the @<color:FF0000>sentence@</color> can be converted to "@<b>[[@<color:FF0000>file_name@</color>]]@</b>" refers to an external text file, \
and direct sentence notation is affected by the POSIX escape character notation, so users unfamiliar with soft programming can use "Example #2) It is recommended to describe it in an external text file like "


ex #1) Direct implementation
```lua
AddParagraph("#Title\
Paragraph content 1\
Paragraph content 2")
```
 
ex #2) External markdown expression text file implementation
```lua
AddParagraph("[[some.txt]]")
```

 
ex #3) Implemented using the file read function
```lua
do
	local	txt_contents	= String(nil)
	txt_contents:ReadFile("some.txt", false)
	AddParagraph(txt_contents.s)
end
```

 
ex #4) Lua application that checks the 'IsInsert' variable, reads several files,
       changes all "%ABC%" expressions to "good", and inserts sentences.
	   
```lua
if IsInsert == true then	-- directive check
	local	txt_contents	= String(nil)
	
	txt_contents:Append(ReadText("some_1.txt"))	-- some_1.txt added
	txt_contents:Append(ReadText("some_2.txt"))	-- some_2.txt added
	txt_contents:Append(ReadText("some_3.txt"))	-- some_3.txt added
	txt_contents:Replace("%ABC%", "good", true)	-- change all "%ABC%" to "good"
	
	AddParagraph(txt_contents.s)	-- Apply to document
end
```
%%%
