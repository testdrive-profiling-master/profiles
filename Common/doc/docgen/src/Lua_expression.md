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


;;;
## property[] 속성 객체
```lua
property["Document_Name"]			-- 문서 이름
property["IP_Version"]				-- IP 버전 (예:"1.00")
property["Main_Title"]				-- 문서 첫장의 타이틀 이름
property["IP_Name_First_Page"]		-- 문서 첫장의 이름
property["IP_Name_Header"]			-- 해더 및 파일 상의 이름 (예 : "doc_guide")
property["Ownership"]				-- 오너쉽 명
property["Document_Name_Header"]	-- 해더 이름 (...)
property["Water_Mark"]				-- 워터마킹 문구 (사용하지 않을 경우 비워 둔다.)
```

워드에는 메뉴 "파일/정보" 에 @<bookmark:WORD 속성 탭>와 같이 '속성' 탭이 존재합니다.

@<img:#media/word_property.png;0.4;WORD 속성 탭>

이 메뉴에서 사용자가 원하는 Field 속성을 추가할 수 있으며, 이 값이 변경되면 문서 전체의 Field 정보를 같이 변경할 수 있습니다.
이걸 lua 코드 내에서 아래와 같은 방법으로 템플릿 문서에 이미 존재하는 필드 값을 변경 할 수 있습니다.

예) 이 문서의 Field 지정 예시 ("--" 로 시작하는 문장은 Lua에서 '주석' 표기를 의미합니다.)
```lua
property["Document_Name"]			= "UserGuide"
property["IP_Version"]				= "1.00"
property["Main_Title"]				= "Document Generator"
property["IP_Name_First_Page"]		= "User Guide"
property["IP_Name_Header"]			= "document_generator"
property["Ownership"]				= "TestDrive"
property["Document_Name_Header"]	= "userguide"
-- 워터마크를 쓰지 않으려면 이부분을 주석 처리 합니다.
property["Water_Mark"]				= "TESTDRIVE CONFIDENTIAL"
```


;;;
## AddRevision() 함수
@<size:30>@<b>함수 원형 : AddRevisoion(@<color:FF0000>버전@</color>, @<color:FF0000>연도@</color>, @<color:FF0000>월@</color>, @<color:FF0000>일@</color>, @<color:FF0000>설명@</color>)@</b>@</size>

문서의 버전 정보를 관리하는 함수입니다. 아래 예시와 같이 버전 순서로 나열할 수 있으며, @<color:FF0000>설명@</color>은 "@<b>@<bookmark:@Paragraph 표현>@</b>"을 사용할 수 있습니다.
AddRevision() 함수를 한번도 사용하지 않을 경우는 "Document Revision History" 절은 자동으로 제거됩니다.

예)
```lua
AddRevision("1.0",	2022, 1,  14,	"Initial Draft")
AddRevision("1.1",	2022, 2,  15,	"Second release")
AddRevision("1.2",	2022, 3,  16,	"Third release")
```

결과)
@<img:#media/revision_history.png;0.8;AddRevision() 함수 사용 예시>
 
## AddTerm() 함수
@<size:30>@<b>함수 원형 : AddTerm(@<color:FF0000>단어@</color>, @<color:FF0000>설명@</color>)@</b>@</size>

단어에 대한 설명을 삽입하는 함수입니다. 아래 예시와 같이 사용될 수 있으며, @<color:FF0000>설명@</color>은 "@<b>@<bookmark:@Paragraph 표현>@</b>"을 사용할 수 있습니다.
AddTerm() 함수를 한번도 사용하지 않을 경우는 "List of Terms" 절은 자동으로 제거됩니다.

```lua
AddTerm("TestDrive", "TestDrive Profiling Master (@<link:https://testdrive-profiling-master.github.io/>)")
AddTerm("Lua", "Lua script language (@<link:https://ko.wikipedia.org/wiki/%EB%A3%A8%EC%95%84_(%ED%94%84%EB%A1%9C%EA%B7%B8%EB%9E%98%EB%B0%8D_%EC%96%B8%EC%96%B4);Wiki>, @<link:http://www.lua.org/;Homepage>)")
```
결과)
@<img:#media/list_of_terms.png;0.8;AddTerm() 함수 사용 예시>


;;;
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
