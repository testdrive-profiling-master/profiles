%%% ko
# Paragraph 표현

Paragraph 표현은 Lua 함수 AddRevision, AddTerm, AddParagraph 의 인자 @<color:FF0000>설명@</color> 및 @<color:FF0000>문장@</color>을 기술하는 법을 의미합니다. \
표기 방법은 markdown 과 몇몇 html 표기법을 혼합하여 취하고 있으며, 표현의 다향성을 위해 추가적으로 자체 문법도 가지고 있습니다. \
또한 각 수식은 문단, 표등 문장이 쓰이는 어디서든 동등하게 사용할 수 있습니다.

:::NoteHeading
Paragraph 표현에 대한 건의 사항이 개선 요청은 언제든지 저에게 연락해 주세요. (@<bookmark:@소개> 참조.)
%%% en
# Paragraph expression

Paragraph expression refers to how to describe the arguments @<color:FF0000>description@</color> and @<color:FF0000>sentence@</color> of the Lua functions of AddRevision, AddTerm, and AddParagraph. \
The notation method is a mixture of markdown and some html notation, and it additionally has its own grammar for versatility in expression. \
Additionally, each formula can be used equally wherever sentences are used, such as paragraphs and tables.

:::NoteHeading
If you have any suggestions for paragraph expressions or requests for improvement, please contact me at any time. (Refer to @<bookmark:@Introduction>.)
%%%





 
%%% ko
## 제목 표현

@<size:30>@<b>표현식 : # @<color:FF0000>대제목@</color>@</b>@</size>
@<size:30>@<b>        ## @<color:FF0000>부제목1@</color>@</b>@</size>
@<size:30>@<b>        ### @<color:FF0000>부제목2@</color>@</b>@</size>
@<size:30>@<b>        #### @<color:FF0000>부제목3@</color>@</b>@</size>
@<size:30>@<b>        ##### @<color:FF0000>부제목4@</color>@</b>@</size>
@<size:30>@<b>        ###### @<color:FF0000>부제목5@</color>@</b>@</size>

문서 제목 표기는 @<color:FF0000>대제목@</color>과 @<color:FF0000>부제목n@</color>까지 '#' 문자를 라인 선두에 두어 제목을 표시합니다. \
이는 markdown 과 같습니다. \
'#' 문자가 라인 선두에 없으면 제목으로 인식하지 않으며, 워드의 템플릿 문서의 "텍스트 스타일" 에 영향을 받습니다. \
최대 6단 까지 사용 가능.
%%% en
## Title expression

@<size:30>@<b>Expression : # @<color:FF0000>Main title@</color>@</b>@</size>
@<size:30>@<b>            ## @<color:FF0000>Sub title1@</color>@</b>@</size>
@<size:30>@<b>            ### @<color:FF0000>Sub title2@</color>@</b>@</size>
@<size:30>@<b>            #### @<color:FF0000>Sub title3@</color>@</b>@</size>
@<size:30>@<b>            ##### @<color:FF0000>Sub title4@</color>@</b>@</size>
@<size:30>@<b>            ###### @<color:FF0000>Sub title5@</color>@</b>@</size>

The document title is displayed by placing the '#' character at the beginning of the line, from @<color:FF0000>Main title@</color> to @<color:FF0000>Sub title@</color>. \
This is the same as markdown. \
If the '#' character is not at the beginning of the line, it is not recognized as a title and is affected by the "text style" of the WORD's template document. \
Up to 6 levels can be used.
%%%





 
%%% ko
## 목록화

@<size:30>@<b>표현식 : * @<color:FF0000>요소1@</color>@</b>@</size>
@<size:30>@<b>        ** @<color:FF0000>요소2@</color>@</b>@</size>
@<size:30>@<b>        *** @<color:FF0000>요소3@</color>@</b>@</size>
@<size:30>@<b>        **** @<color:FF0000>요소4@</color>@</b>@</size>
@<size:30>@<b>        ***** @<color:FF0000>요소5@</color>@</b>@</size>
@<size:30>@<b>        ****** @<color:FF0000>요소6@</color>@</b>@</size>

목록화는 markdown 과 비슷하지만 더 다양한 표현기능을 위해 약간의 차이가 있습니다. '*' 문자가 줄 선두에 존재할 경우 목록화로 표시되며, '*' 개수만큼 들여쓰기가 결정됩니다.
또한 번호나 특수한 표현을 위해 나열된 '*' 문자 다음에 '>' 를 사용하여, 기존 표식을 없애고 다른 형태의 표현식을 쓸 수 있습니다. 최대 6단 까지 사용 가능.

예)
```txt
* 첫번째 요소
** 두번째 요소
*** 세번째 요소
**** 네번째 #1 요소
**** 네번째 #2 요소
**** >네번째 #2 요소 확장
**** @<b>@<color:FF0000>목록@</color>(目錄, List)@</b>은 사람의 이름이나 물품의 이름, 책 제목, 목차, 점검 해야할 항목 따위를 일정한 기준과 순서로 적어 알아보기 쉽도록 만든 것을 이르는 말이다.
* >1). 첫번째 번호 요소
** >1-1). 두번째 번호 요소
* >2). 첫번째 번호 요소
** >2-1). 첫번째 번호 요소
*** >표식 없는 요소 연장 단락
```
결과)
* 첫번째 요소
** 두번째 요소
*** 세번째 요소
**** 네번째 #1 요소
**** 네번째 #2 요소
**** >네번째 #2 요소 확장
**** @<b>@<color:FF0000>목록@</color>(目錄, List)@</b>은 사람의 이름이나 물품의 이름, 책 제목, 목차, 점검 해야할 항목 따위를 일정한 기준과 순서로 적어 알아보기 쉽도록 만든 것을 이르는 말이다.
* >1). 첫번째 번호 요소
** >1-1). 두번째 번호 요소
* >2). 첫번째 번호 요소
** >2-1). 첫번째 번호 요소
*** >표식 없는 요소 연장 단락
%%% en
## Cataloging

@<size:30>@<b>Expression : * @<color:FF0000>Element1@</color>@</b>@</size>
@<size:30>@<b>             ** @<color:FF0000>Element2@</color>@</b>@</size>
@<size:30>@<b>             *** @<color:FF0000>Element3@</color>@</b>@</size>
@<size:30>@<b>             **** @<color:FF0000>Element4@</color>@</b>@</size>
@<size:30>@<b>             ***** @<color:FF0000>Element5@</color>@</b>@</size>
@<size:30>@<b>             ****** @<color:FF0000>Element6@</color>@</b>@</size>

Cataloging is similar to markdown, but there are some differences for more expressive functions. If the '*' character exists at the beginning of a line, it is displayed as a list, and the indentation is determined by the number of '*'s.
You can also use '>' after the '*' character listed for a number or special expression to remove the existing marker and use a different form of expression. Up to 6 levels can be used.

ex)
```txt
* first element
** second element
*** third element
**** fourth #1 element
**** fourth #2 element
**** >fourth #2 element extension
**** @<b>@<color:FF0000>Cataloging@</color>(目錄, List)@</b> refers to writing down names of people, names of products, book titles, tables of contents, and items to be checked in a certain standard and order to make them easier to read.
* >1). first number element
** >1-1). second number element
* >2). first number element
** >2-1). second number element
*** >Unmarked Element Extended element
```
Result)
* first element
** second element
*** third element
**** fourth #1 element
**** fourth #2 element
**** >fourth #2 element extension
**** @<b>@<color:FF0000>Cataloging@</color>(目錄, List)@</b> refers to writing down names of people, names of products, book titles, tables of contents, and items to be checked in a certain standard and order to make them easier to read.
* >1). first number element
** >1-1). second number element
* >2). first number element
** >2-1). second number element
*** >Unmarked Element Extended element
%%%





%%% ko 
## 문자 수식

문자 수식은 글자의 색상, 굵기, 이텔릭, 밑줄, 크기, 위첨자/아래첨자 등을 지정할 수 있으며, 적용 범위는 한 라인 내에서만 제한되며, 여러 수식 표기법이 중복 표현 될 수 있습니다.
%%% en
## Text expression

Text expressions can specify the color, thickness, italics, underline, size, superscript/subscript, etc. of the text. The scope of application is limited to one line, and multiple formula notations may be expressed overlapping.
%%%





 
%%% ko
### 굵은 글자
@<size:30>@<b>표현식 : @@<b>@<color:FF0000>표현식@</color>@@</b>@</b>@</size>

굵은 글씨는 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'b' 태그로 둘러쌓아 표현합니다.

예)
```html
@<b>굵은 글씨입니다.@</b> 굵은 글씨가 아닙니다.
```
결과 : @<b>굵은 글씨입니다.@</b> 굵은 글씨가 아닙니다.
%%% en
### Bold text
@<size:30>@<b>Expression : @@<b>@<color:FF0000>expression@</color>@@</b>@</b>@</size>

Bold text is expressed by surrounding @<color:FF0000>Expression@</color> with a 'b' tag, similar to HTML.

ex)
```html
@<b>It's in bold.@</b> It's not in bold.
```
Result : @<b>It's in bold.@</b> It's not in bold.
%%%




 
%%% ko
### 이텔릭체
@<size:30>@<b>표현식 : @@<i>@<color:FF0000>표현식@</color>@@</i>@</b>@</size>

이텔릭체 문장은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'i' 태그로 둘러쌓아 표현합니다.

예)
```html
@<i>이텔릭체@</i> Non 이텔릭체
```
결과 : @<i>이텔릭체@</i> Non 이텔릭체
%%% en
### Italic text
@<size:30>@<b>Expression : @@<i>@<color:FF0000>expression@</color>@@</i>@</b>@</size>

Italic text is expressed by surrounding the @<color:FF0000>expression@</color> with an 'i' tag, similar to HTML.

ex)
```html
@<i>italic@</i> Non italic
```
Result : @<i>italic@</i> Non italic
%%%




 
%%% ko
### 밑줄
@<size:30>@<b>표현식 : @@<u>@<color:FF0000>표현식@</color>@@</u>@</b>@</size>

밑줄 글씨 표현은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'u' 태그로 둘러쌓아 표현합니다.

예)
```html
@<u>밑줄 글자@</u>
```
결과 : @<u>밑줄 글자@</u>
%%% en
### Underline
@<size:30>@<b>Expression : @@<u>@<color:FF0000>expression@</color>@@</u>@</b>@</size>

The underlined text is expressed by surrounding @<color:FF0000>expression@</color> with a 'u' tag, similar to HTML.

ex)
```html
@<u>underlined text@</u>
```
Result : @<u>underlined text@</u>
%%%




 
%%% ko
### 취소선
@<size:30>@<b>표현식 : @@<s>@<color:FF0000>표현식@</color>@@</s>@</b>@</size>

취소선 표현은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 's' 태그로 둘러쌓아 표현합니다.

예)
```html
@<s>취소선 글자@</s>
```
결과 : @<s>취소선 글자@</s>
%%% en
### Strikethrough
@<size:30>@<b>Expression : @@<s>@<color:FF0000>expression@</color>@@</s>@</b>@</size>

The strikethrough text is expressed by surrounding @<color:FF0000>expression@</color> with 's' tags, similar to HTML.

ex)
```html
@<s>Strikethrough text@</s>
```
Result : @<s>Strikethrough text@</s>
%%%




 
%%% ko
### 위첨자
@<size:30>@<b>표현식 : @@<sup>@<color:FF0000>표현식@</color>@@</sup>@</b>@</size>

위첨자 표현은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'sup' 태그로 둘러쌓아 표현합니다.

예)
```html
글씨의@<sup>위첨자@</sup>
```
결과 : 글씨의@<sup>위첨자@</sup>
%%% en
### Superscript
@<size:30>@<b>Expression : @@<sup>@<color:FF0000>expression@</color>@@</sup>@</b>@</size>

Superscript text is expressed by surrounding @<color:FF0000>expression@</color> with 'sup' tags, similar to HTML.

ex)
```html
Text@<sup>Superscript@</sup>
```
Result : Text@<sup>Superscript@</sup>
%%%




 
%%% ko
### 아래첨자
@<size:30>@<b>표현식 : @@<sub>@<color:FF0000>표현식@</color>@@</sub>@</b>@</size>

아래첨자 표현은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'sub' 태그로 둘러쌓아 표현합니다.

예)
```html
글씨의@<sub>아래첨자@</sub>
```
결과 : 글씨의@<sub>아래첨자@</sub>
%%% en
### Subscript
@<size:30>@<b>Expression : @@<sub>@<color:FF0000>expression@</color>@@</sub>@</b>@</size>

Subscript text is expressed by surrounding @<color:FF0000>expression@</color> with 'sub' tags, similar to HTML.

ex)
```html
Text@<sub>Subscript@</sub>
```
Result : Text@<sub>Subscript@</sub>
%%%




 
%%% ko
### 글자 색상
@<size:30>@<b>표현식 : @@<color:@<color:FF0000>색상값@</color>>@<color:FF0000>표현식@</color>@@</color>@</b>@</size>

글자 색상 변경을 위해서는 표현은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'color' 태그로 둘러쌓아 표현하며, 색상 지정은 @<color:FF0000>색상값@</color>에 24bit RGB 16진수로 표현합니다.

예)
```html
@<color:FF0000>붉은 글씨@</color> 표현
```
결과 : @<color:FF0000>붉은 글씨@</color> 표현
%%% en
### Text color
@<size:30>@<b>Expression : @@<color:@<color:FF0000>color_value@</color>>@<color:FF0000>expression@</color>@@</color>@</b>@</size>

To change the font color, the expression is @<color:FF0000>expression@</color> surrounded by 'color' tags similar to HTML, and the color is specified by @<color:FF0000>color_value@</color> Expressed in 24bit RGB hexadecimal.

ex)
```html
@<color:FF0000>Red text@</color> expression
```
Result : @<color:FF0000>Red text@</color> expression
%%%




 
%%% ko
### 글자 크기
@<size:30>@<b>표현식 : @@<size:@<color:FF0000>크기값@</color>>@<color:FF0000>표현식@</color>@@</size>@</b>@</size>

글자 크기 변경을 위해서는 표현은 @<color:FF0000>표현식@</color> 을 HTML 방식과 유사하게 'size' 태그로 둘러쌓아 표현하며, 색상 지정은 @<color:FF0000>크기값@</color>에 point 단위로 지정합니다.

예)
```html
@<size:30>큰 글씨@</size> @<size:10>작은 글씨@</size>
```
결과 : @<size:30>큰 글씨@</size> @<size:10>작은 글씨@</size>
%%% en
### Text size
@<size:30>@<b>Expression : @@<size:@<color:FF0000>font_size@</color>>@<color:FF0000>expression@</color>@@</size>@</b>@</size>

To change the font size, the expression is @<color:FF0000>expression@</color> surrounded by 'size' tags similar to HTML, and the color is specified by @<color:FF0000>font_size@</color> Specify it in point units.

ex)
```html
@<size:30>Big text@</size> @<size:10>small text@</size>
```
Result : @<size:30>Big text@</size> @<size:10>small text@</size>
%%%




 
%%% ko
### 문단 스타일
@<size:30>@<b>표현식 : :::@<color:FF0000>스타일_이름@</color>@</b>@</size>

다음 줄의 문단 스타일을 '@<color:FF0000>스타일_이름@</color>'로 변경합니다. 한 줄에 대해서만 스타일이 변경되며, 다음 라인에 대해서는 원래의 스타일 서식으로 다시 돌아갑니다.
스타일 서식은 초기 템플릿 문서에 지정된 스타일이 참조되어 구현되며, WORD 상에 '텍스트 스타일'로 검색하여 변경 및 추가할 수 있습니다.

예) 노트 서식을 적용
```html
:::NoteHeading
민간인과 군인 사망자를 모두 합하여 약 6,000만~7,000만 명에 달하는 사람들이 제2차 세계 대전으로 인해 사망했다. 이 전쟁의 여파로 서구권에서는 그동안 사회 주류였던 집단주의 사상이 쇠퇴하고 개인주의 사상이 대두되어 오늘날까지 이어지게 된다.
```
결과)
:::NoteHeading
민간인과 군인 사망자를 모두 합하여 약 6,000만~7,000만 명에 달하는 사람들이 제2차 세계 대전으로 인해 사망했다. 이 전쟁의 여파로 서구권에서는 그동안 사회 주류였던 집단주의 사상이 쇠퇴하고 개인주의 사상이 대두되어 오늘날까지 이어지게 된다.
%%% en
### Paragraph style
@<size:30>@<b>Expression : :::@<color:FF0000>style_name@</color>@</b>@</size>

Change the paragraph style of the next line to '@<color:FF0000>style_name@</color>'. The style changes for only one line, and reverts to the original style formatting for the next line.
Style formatting is implemented by referencing the style specified in the initial template document, and can be changed or added by searching for 'text style' in WORD.

ex) Apply note style
```html
:::NoteHeading
Including both civilian and military deaths, an estimated 60 to 70 million people died as a result of World War II. In the aftermath of this war, collectivistic ideology, which had been the mainstream of society, declined in the Western world and individualistic ideology emerged, which continues to this day.
```
Result)
:::NoteHeading
Including both civilian and military deaths, an estimated 60 to 70 million people died as a result of World War II. In the aftermath of this war, collectivistic ideology, which had been the mainstream of society, declined in the Western world and individualistic ideology emerged, which continues to this day.
%%%




 
%%% ko
## 상호 참조
@<size:30>@<b>표현식 : @@<bookmark:@<color:FF0000>대상@</color>>@</b>@</size>

제목, 그림, 표등의 상호 참조를 구현합니다.
@<color:FF0000>대상@</color>에 그림, 표의 캡션 내용이나 제목의 내용을 적으면 됩니다.
%%% en
## Cross-reference
@<size:30>@<b>Expression : @@<bookmark:@<color:FF0000>target@</color>>@</b>@</size>

Implement cross-referencing of titles, pictures, tables, etc.
Just write the caption or title of the figure or table in @<color:FF0000>target@</color>.
%%%




 
%%% ko
### 그림 또는 표의 상호 참조

예)
```html
@<bookmark:LaTeX setting>
```
결과 : @<bookmark:LaTeX 설정>
%%% en
### Cross-reference in picture or table

ex)
```html
@<bookmark:LaTeX setting>
```
Result : @<bookmark:LaTeX setting>
%%%






%%% ko 
### 제목의 상호 참조

제목의 경우 아래와 같이 Prefix 유무에 따라 상호 참조의 표현은 크게 4가지로 구현 가능합니다.
* >1. 일반 문장 참조 (별도의 prefix 없이 표현)
** >: @<b>@<color:FF0000>대상@</color>@</b>
* >2. 페이지 번호 참조
** >: @<b>@<color:0000FF>&@</color>@<color:FF0000>대상@</color>@</b>
* >3. 장/절 번호 참조
** >: @<b>@<color:0000FF>#@</color>@<color:FF0000>대상@</color>@</b>
* >4. 장/절 번호 및 문장 전체 참조
** >: @<b>@<color:0000FF>@@@</color>@<color:FF0000>대상@</color>@</b>

아래 예제는 실제 사용 예시를 보이며, 문장 클릭시 현 문서의 링크로 이동 됨을 확인 할 수 있습니다.
%%% en
### Cross-reference in title

In the case of titles, cross-references can be expressed in four ways depending on the presence or absence of a prefix, as shown below.
* >1. Reference to general sentences (expressed without a separate prefix)
** >: @<b>@<color:FF0000>target@</color>@</b>
* >2. Page number reference
** >: @<b>@<color:0000FF>&@</color>@<color:FF0000>target@</color>@</b>
* >3. Chapter/section number reference
** >: @<b>@<color:0000FF>#@</color>@<color:FF0000>target@</color>@</b>
* >4. Chapter/section number and full sentence reference
** >: @<b>@<color:0000FF>@@@</color>@<color:FF0000>target@</color>@</b>

The example below shows an actual usage example, and you can see that clicking on the sentence moves to the link to the current document.
%%%



%%% ko
예) 일반 문장 참조
```html
@<bookmark:사용상 제한 및 라이센스 허가와 선행 과정>
```
결과 : @<bookmark:사용상 제한 및 라이센스 허가와 선행 과정>

예) 페이지 번호 참조
```html
@<bookmark:&사용상 제한 및 라이센스 허가와 선행 과정>
```
결과 : @<bookmark:&사용상 제한 및 라이센스 허가와 선행 과정>

예) 장/절 번호 참조
```html
@<bookmark:#사용상 제한 및 라이센스 허가와 선행 과정>
```
결과 : @<bookmark:#사용상 제한 및 라이센스 허가와 선행 과정>


예) 장/절 번호 및 문장 전체 참조
```html
@<bookmark:@사용상 제한 및 라이센스 허가와 선행 과정>
```

결과 : @<bookmark:@사용상 제한 및 라이센스 허가와 선행 과정>
%%% en
ex) Reference to general sentences
```html
@<bookmark:Restrictions on use, licensing and prerequisites>
```
Result : @<bookmark:Restrictions on use, licensing and prerequisites>

ex) Page number reference
```html
@<bookmark:&Restrictions on use, licensing and prerequisites>
```
Result : @<bookmark:&Restrictions on use, licensing and prerequisites>

ex) Chapter/section number reference
```html
@<bookmark:#Restrictions on use, licensing and prerequisites>
```
Result : @<bookmark:#Restrictions on use, licensing and prerequisites>


ex) Chapter/section number and full sentence reference
```html
@<bookmark:@Restrictions on use, licensing and prerequisites>
```

Result : @<bookmark:@Restrictions on use, licensing and prerequisites>
%%%




 
%%% ko
## 속성(Property) 참조
@<size:30>@<b>표현식 : @@<property:@<color:FF0000>속성이름@</color>>@</b>@</size>
'@<bookmark:Lua 표현>' 중 '@<bookmark:속성 객체>'로 다양한 이름의 속성을 만들 수 있으며, 이를 본문에서 사용하는 방법을 제공한다.
예)
```html
@<property:Main_Title>, @<property:Ownership>
```
결과 : @<property:Main_Title>, @<property:Ownership>
%%% en
## Property reference
@<size:30>@<b>Expression : @@<property:@<color:FF0000>property_name@</color>>@</b>@</size>
You can create properties with various names using '@<bookmark:Property object>' among '@<bookmark:Lua expression>', and methods for using them in the text are provided.
예)
```html
@<property:Main_Title>, @<property:Ownership>
```
Result : @<property:Main_Title>, @<property:Ownership>
%%%




 
%%% ko
## 하이퍼 링크(Hyperlink) 표현
@<size:30>@<b>표현식 : @@<link:@<color:FF0000>URL_대상@</color>;@<color:FF0000>표시_문구@</color>>@</b>@</size>

하이퍼 링크는 위와 같이 표시하며, @<color:FF0000>표시_문구@</color>는 생략 가능합니다.

예) "https://testdrive-profiling-master.github.io/" 링크 표현
```html
@<link:https://testdrive-profiling-master.github.io/;TestDrive Profiling Master>
```
결과 : @<link:https://testdrive-profiling-master.github.io/;TestDrive Profiling Master>
%%% en
## Hyperlink
@<size:30>@<b>Expression : @@<link:@<color:FF0000>URL_target@</color>;@<color:FF0000>display_text@</color>>@</b>@</size>

Hyperlinks are displayed as above, and @<color:FF0000>display_text@</color> can be skipped.

ex) "https://testdrive-profiling-master.github.io/" hyperlink expression
```html
@<link:https://testdrive-profiling-master.github.io/;TestDrive Profiling Master>
```
Result : @<link:https://testdrive-profiling-master.github.io/;TestDrive Profiling Master>
%%%





 
%%% ko
## 수학 수식 표현
@<size:30>@<b>표현식 : \$\$@<color:FF0000>수학수식@</color>\$\$@</b>@</size>

수학 수식의 경우 markdown 과 같게 수학 수식에 "\$$" 로 둘러싸서 표시합니다.
단! WORD 의 수학 수식은 기본이 "@<color:FF0000>@<b>유니코드@</b>@</color>" 표현식으로 초기 지정되어 있습니다. \
때문에 "@<color:0000FF>@<b>LaTeX@</b>@</color>" 수식을 활용하고자 한다면, "@<u>@<b>@<bookmark:@선행 과정>@</b>@</u>"을 참고하여 설정을 변경해야 함을 유의합니다.
수학 수식을 단일로 표현할 경우, 가운데 정렬로 표기 되지만, 문장 중간에 표현할 경우 문장과 어울리도록 표현이 자동 변경되며, \
아래의 예제는 LaTeX 표현으로 구현된 내용이므로 만약 결과 표기가 잘못 되어 있다면, 위 "@<bookmark:선행 과정>"을 시도하지 않은 결과입니다.
%%% en
## Math equation expression
@<size:30>@<b>Expression : \$\$@<color:FF0000>math_formula@</color>\$\$@</b>@</size>

In the case of mathematical formulas, the mathematical equation is displayed by surrounding it with "\$$", as in markdown.
But, WORD's mathematical equation is initially set to the expression "@<color:FF0000>@<b>Unicode@</b>@</color>". \
Therefore, if you want to use the equation "@<color:0000FF>@<b>LaTeX@</b>@</color>", please note that you must change the settings by referring to "@<u>@<b>@<bookmark:@Prerequisites>@</b>@</u>".
When a mathematical equation is expressed as a single expression, it is displayed centered, but when expressed in the middle of a sentence, the expression is automatically changed to match the sentence, and \
the example below is implemented in LaTeX expression, so if the result is notated incorrectly, it is the result of not attempting the "@<bookmark:Prerequisites>" above.
%%%


%%% ko
예) 수학 수식 표현 2가지
```docbook
$$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$
문장 안의 표현 식은 이렇게 $$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$ 변합니다.
```

결과 : 
$$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$
문장 안의 표현 식은 이렇게 $$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$ 변합니다.

WORD의 수학 수식은 @<bookmark:WORD 수학 수식 LaTeX 변환>처럼 같이 수식을 먼저 만든 후, 메뉴에서 '1차원 형식' 버튼을 눌러 변경 가능합니다. 이 문자열을 양끝단에 "\$$" 문자를 덧붙여 수학 수식으로 작성할 수 있습니다.
@<img:#media/math_latex.png;0.9;WORD 수학 수식 LaTeX 변환>
%%% en
ex) 2 types of mathematical equation expressions
```docbook
$$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$
The expression in the sentence changes to $$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$.
```

Result : 
$$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$
The expression in the sentence changes to $$f\left(x\right)=a_0+\sum_{n=1}^{\infty}\left(a_n\cos{\frac{n\pi x}{L}}+b_n\sin{\frac{n\pi x}{L}}\right)$$.

WORD's mathematical equation can be changed by first creating the equation as shown in @<bookmark:WORD mathematical equation LaTeX conversion> and then pressing the 'Linear format' button in the menu. \
You can write this string as a mathematical equation by adding the characters "\$$" to both ends.
@<img:#media/math_latex_en.png;0.9;WORD mathematical equation LaTeX conversion>
%%%




 
%%% ko
## 코드 인용

@<size:30>@<b>표현식 : ```@<color:FF0000>코드_형식@</color>@</b>@</size>
@<size:30>@<b>        @<color:FF0000>코드_내용@</color>@</b>@</size>
@<size:30>@<b>        ```@</b>@</size>

@<color:FF0000>코드_형식@</color> 의 경우 아래와 같이 @<fixed>"code_highlighter -l"@</fixed> 명령어 입력을 통해 아래와 같이 지원하는 코드 형식을 확인 할 수 있습니다.
%%% en
## Code citation

@<size:30>@<b>Expression : ```@<color:FF0000>code_format@</color>@</b>@</size>
@<size:30>@<b>             @<color:FF0000>code_contents@</color>@</b>@</size>
@<size:30>@<b>             ```@</b>@</size>

In the case of @<color:FF0000>code_format@</color>, you can check the supported code formats by entering the @<fixed>"code_highlighter -l"@</fixed> command as shown below.
%%%

```bash
> code_highlighter -l
*** Available current input code format(C:/Project/Profiles/Common/bin/source-highlight-lang/lang.map) list

        C, F77, F90, H, ac, ada, adb, am, applescript, asm
        autoconf, awk, bash, bat, batch, bib, bison, c, caml, cbl
        cc, changelog, clipper, cls, cobol, coffee, coffeescript, conf, cpp, cs
        csh, csharp, css, ctp, cxx, d, desktop, diff, dmd, docbook
        dtx, el, eps, erl, erlang, errors, f, f77, f90, feature
        fixed-fortran, flex, fortran, free-fortran, glsl, go, groovy, h, haskell, haxe
        hh, hpp, hs, htm, html, hx, hxx, in, ini, ipxe
        islisp, java, javalog, javascript, js, json, kcfg, kdevelop, kidl, ksh
        l, lang, langdef, latex, ldap, ldif, lex, lgt, lhs, lilypond
        lisp, ll, log, logtalk, lsm, lua, ly, m4, makefile, manifest
        mf, ml, mli, moc, opa, outlang, oz, pas, pascal, patch
        pc, perl, php, php3, php4, php5, pkgconfig, pl, pm, po
        postscript, pot, prg, prolog, properties, proto, protobuf, ps, py, python
        r, rb, rc, rs, ruby, s, scala, scheme, scm, scpt
        sh, shell, sig, sl, slang, slsh, sml, spec, sql, sty
        style, syslog, systemverilog, tcl, tcsh, tex, texi, texinfo, tk, tml
        txt, ui, upc, v, vala, vbs, vbscript, verilog, vim, xhtml
        xml, xorg, y, yacc, yaml, yml, yy, zsh
```

%%% ko
아래는 verilog 코드를 인용하는 예를 나타낸다. 인용시 "```" 문자열을 사용하려면, "@```" 로 표기하여 인용할 수 있으며, 코드에 라인 번호를 삽입하려면 '#' 를 붙여 "@<color:0000FF>#@</color>@<color:FF0000>코드_형식@</color>"으로 표기할 수 있습니다.

예) 라인 번호가 있는 verilog 코드 인용
%%% en
Below is an example of quoting verilog code. To use the string "```" when quoting, you can quote it by writing "@```". To insert a line number in the code, you can add '#' and quote it like this: "@<color:0000FF>#@</color It can be expressed as ">@<color:FF0000>code_format@</color>".

ex) Verilog code citation with line numbers
%%%

```txt
@```#verilog
`ifndef __TESTDRIVE_SRAM_SINGLE_V__
`define __TESTDRIVE_SRAM_SINGLE_V__
`timescale 1ns/1ns

module SRAM_Single #(
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 4
) (
	// System
	input							CLK,			// clock
	// SRAM interface
	input							nCE,			// Chip enable (active low)
	input							nWE,			// write enable (active low)
	input	[(ADDR_WIDTH-1):0]		ADDR,			// address
	input	[(DATA_WIDTH-1):0]		DIN,			// input data
	output	reg [(DATA_WIDTH-1):0]	DOUT			// output data
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0];

// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	if(!nCE) begin
		if(!nWE)
			mem[ADDR]	<= DIN;
		DOUT	<= mem[ADDR];
	end
end

endmodule

`endif//__TESTDRIVE_SRAM_SINGLE_V__
@```
```

%%% ko
결과)
%%% en
Result)
%%%

```#verilog
`ifndef __TESTDRIVE_SRAM_SINGLE_V__
`define __TESTDRIVE_SRAM_SINGLE_V__
`timescale 1ns/1ns

module SRAM_Single #(
	parameter	ADDR_WIDTH			= 4,
	parameter	DATA_WIDTH			= 4
) (
	// System
	input							CLK,			// clock
	// SRAM interface
	input							nCE,			// Chip enable (active low)
	input							nWE,			// write enable (active low)
	input	[(ADDR_WIDTH-1):0]		ADDR,			// address
	input	[(DATA_WIDTH-1):0]		DIN,			// input data
	output	reg [(DATA_WIDTH-1):0]	DOUT			// output data
);
// synopsys template

// definition & assignment ---------------------------------------------------
reg		[(DATA_WIDTH-1):0]		mem[(2**ADDR_WIDTH)-1:0];

// implementation ------------------------------------------------------------
always@(posedge CLK) begin
	if(!nCE) begin
		if(!nWE)
			mem[ADDR]	<= DIN;
		DOUT	<= mem[ADDR];
	end
end

endmodule

`endif//__TESTDRIVE_SRAM_SINGLE_V__
```




 
%%% ko
## 페이지 나눔

@<size:30>@<b>표현식 : ;;;@</b>@</size>
라인 선두에 ';' 문자를 최소 3개 이상 입력하면, 다음 페이지로 넘깁니다.
%%% en
## Seperating page

@<size:30>@<b>Expression : ;;;@</b>@</size>
If you enter at least three ';' characters at the beginning of a line, it will advance to the next page.
%%%



 
%%% ko
## 수평선

@<size:30>@<b>표현식 : ---@</b>@</size>
라인 선두에 '-' 문자를 최소 3개 이상 입력하면, markdown 과 동일하게 수평선이 입력됩니다.

예)
```txt
---
```
결과)
---
%%% en
## horizontal line

@<size:30>@<b>Expression : ---@</b>@</size>
If you enter at least three '-' characters at the beginning of a line, a horizontal line will be entered, just like in markdown.

ex)
```txt
---
```
Result)
---
%%%




 
%%% ko
## 언어코드 지정

@<size:30>@<b>표현식 : %%%@<color:FF0000>언어_코드명@</color>@</b>@</size>
'@<bookmark:@실행 방법>'에서 지정한 '-l' 옵션에서 지정한 '언어 코드명'과 비교합니다. \
만약 같지 않으면, 다시 언어코드를 변경하기 전까지 다음 문장들을 무시합니다. \
이로써 원하는 언어로 작성된 문장만 문서에 사용할 수 있습니다.
만약 언어 코드 없이 '%%%' 로 끝났다면, 다음 줄부터 모든 언어코드에 대해서 활성화됩니다.

:::NoteHeading
언어코드 이름은 대소문자를 구분하지 않으며, 언어 코드 이름 뒤에 '-', '=' 또는 '%' 문자들을 나열하여 가독성을 높일 수 있습니다.

%%% en
## Specify language code

@<size:30>@<b>Expression : %%%@<color:FF0000>language_code@</color>@</b>@</size>
Compare 'language_code' with the '-l' option specified one. Refer to '@<bookmark:@How to run>'. \
If they are not equal, the following sentences are ignored until the language code is changed again. \
This ensures that only sentences written in your desired language can be used in your document.
If it ends with '%%%' without a language code, it will be activated for all language codes from the next line.

:::NoteHeading
Language code names are case-insensitive, and you can improve readability by listing '-', '=', or '%' characters after the language code name.
%%%

%%% ko -------------------------
예)
```lua
%%% ko -------------------------
한글 출력입니다.
%%% en -------------------------
It's an english output.
%%% ----------------------------
모든 출력입니다.
```

결과) '-l ko' 옵션 사용시.
```txt
한글 출력입니다.
모든 출력입니다.
```

:::NoteHeading
기본은 모든 언어 코드가 활성된 상태입니다.
%%% en -------------------------
ex)
```lua
%%% ko -------------------------
한글 출력입니다.
%%% en -------------------------
It's an english output.
%%% ----------------------------
It's all output.
```
ex) When using the '-l en' option.
```txt
It's an english output.
It's all output.
```

:::NoteHeading
The default is enabled for all language codes.
%%% -------------------------
 




%%% ko
## Lua 함수 호출
@<size:30>@<b>표현식 : @@<lua:@<color:FF0000>lua_함수@</color>>@</b>@</size>
Lua 함수를 호출하여, 반환된 문자열을 표현식으로 다시 사용합니다.

예)
```lua
Lua 변수 'docgen_language'는 @<lua:docgen_language> 입니다.
```
결과)
Lua 변수 'docgen_language'는 @<lua:docgen_language> 입니다.
%%% en
## Lua function call
@<size:30>@<b>Expression : @@<lua:@<color:FF0000>lua_function@</color>>@</b>@</size>
Call a Lua function and reuse the returned string as an expression.

ex)
```lua
Lua variable 'docgen_language' is @<lua:docgen_language>
```
Result)
Lua variable 'docgen_language' is @<lua:docgen_language>
%%%
 





%%% ko
## 그림 삽입

@<size:30>@<b>표현식 : @@<img:@<color:FF0000>파일명@</color>;@<color:FF0000>스케일@</color>;@<color:FF0000>캡션@</color>>@</b>@</size>
그림은 jpg, png, bmp, gif, tif, svg, wmf, vsd/vsdx(Visio 설치 필요) 포멧을 지원하고 있습니다. @<color:FF0000>파일명@</color>은 표시할 파일명을 지정하게 되며, \
그림에 외각선 경계가 필요할 경우 "@<color:0000FF>#@</color>@<color:FF0000>파일명@</color>"으로 표시합니다. \
Visio 파일일 경우는 "@<color:FF0000>파일명@</color>[@<color:FF0000>페이지명@</color>]"으로 별도의 페이지 이름을 지정할 수 있으며, \
@<color:FF0000>페이지명@</color>이 지정되지 않을 때는 첫 페이지로 간주합니다.
또한 @<color:FF0000>스케일@</color>의 경우, 종이의 폭 기준 최대값 1.0으로 0 초과 1 이하의 값을 넣어 크기를 지정할 수 있습니다. \
지정되지 않을 경우 기본 1.0으로 간주 합니다.
@<color:FF0000>캡션@</color>은 말 그대로 캡션 내용을 지정하며, '@<bookmark:@상호 참조>'에서 사용될 수 있습니다. 지정되지 않을 경우 캡션은 삽입되지 않습니다.
%%% en
## Picture insertion

@<size:30>@<b>Expression : @@<img:@<color:FF0000>filename@</color>;@<color:FF0000>scale@</color>;@<color:FF0000>caption@</color>>@</b>@</size>
Pictures support jpg, png, bmp, gif, tif, svg, wmf, and vsd/vsdx (Visio installation required) formats. @<color:FF0000>file name@</color> specifies the file name to display, \
if the picture requires an outline border, indicate it as "@<color:0000FF>#@</color>@<color:FF0000>filename@</color>". \
In the case of a Visio file, a separate page name can be specified as "@<color:FF0000>filename@</color>[@<color:FF0000>page name@</color>]", and \
if @<color:FF0000>page name@</color> is not specified, it is considered the first page.

Additionally, in the case of @<color:FF0000>scale@</color>, the maximum value based on the width of the paper is 1.0, and you can specify the size by entering a value between 0 and 1. \
If not specified, the default 1.0 is assumed.
@<color:FF0000>caption@</color> literally specifies the caption content and can be used in '@<bookmark:@Cross-reference>'. If not specified, the caption will not be inserted.
%%%



 
%%% ko
예1) $$1/4$$ 크기의 경계선 있는 도넛 그림
```html
@<img:#media/donut.jpg;0.25;도넛 이미지 예시>
```
결과)
@<img:#media/donut.jpg;0.25;도넛 이미지 예시>

예2) 경계선 없는 벡터 방식 그림
```html
@<img:media/awesome_svg.svg;0.7;SVG 벡터 그림 예시>
```
결과)
@<img:media/awesome_svg.svg;0.7;SVG 벡터 그림 예시>

예3) Visio 특정 page를 벡터 이미지로 삽입
```html
@<img:media/test.vsdx[test_sample];0.8;Visio 벡터 그림 예시>
```
결과)
@<img:media/test.vsdx[test_sample];0.8;Visio 벡터 그림 예시>

예4) 그림 없이 캡션만 삽입
```html
@<img:nil;0;그림 내용 없이 캡션만 삽입>
```
결과)
@<img:nil;0;그림 내용 없이 캡션만 삽입>
%%% en
ex1) Illustration of a donut with border in size $$1/4$$
```html
@<img:#media/donut.jpg;0.25;Donut image example>
```
Result)
@<img:#media/donut.jpg;0.25;Donut image example>

ex2) Borderless vector image
```html
@<img:media/awesome_svg.svg;0.7;SVG vector image example>
```
Result)
@<img:media/awesome_svg.svg;0.7;SVG vector image example>

ex3) Insert Visio specific page as vector image
```html
@<img:media/test.vsdx[test_sample];0.8;Visio vector image example>
```
Result)
@<img:media/test.vsdx[test_sample];0.8;Visio vector image example>

ex4) Insert only a caption without a picture
```html
@<img:nil;0;Insert only a caption without picture content>
```
Result)
@<img:nil;0;Insert only a caption without picture content>
%%%



 
%%% ko
## 표 삽입
표는 엑셀에 의한 표현 방법과 Lua 스크립트에 의한 표현 방법 2가지를 지원합니다.
표가 매우 길게 기술되어 다음 페이지로 넘어갈 경우, 해더가 자동으로 반복 표기됩니다.
%%% en
## Table insertion
Tables support two expression methods: Excel and Lua script.
If the table is too long and you move to the next page, the header is automatically repeated.
%%%




 
%%% ko
### 엑셀 파일로부터 표 삽입

@<size:30>@<b>표현식 : @@<tbl:@<color:FF0000>파일명@</color>;@<color:FF0000>시트명@</color>;@<color:FF0000>캡션@</color>>@</b>@</size>

표 삽입의 경우 엑셀에서 작상된 내용을 옮겨 넣을 수 있으며, 엑셀상 Merge 된 셀들과 폭은 최대한 동일하게 적용됩니다.
표의 첫 줄은 상단 제목이며, 하위 데이터 열을 가운데 정렬을 하고자 할 때에는 제목 이름 앞에 '*'문자를 삽입합니다.

예1) 입력 엑셀 파일 : "media/table_sample.xlsx"
```html
@<tbl:media/table_sample.xlsx;Sheet1;엑셀 표 예시>
```
 
@<img:#media/table_sample.png;1;"media/table_sample.xlsx" 실제 엑셀 파일>

결과)
@<tbl:media/table_sample.xlsx;Sheet1;엑셀 표 예시>
 
예2) 테이블 내용 없이 캡션만 삽입
```html
@<tbl:nil;nil;표 내용 없이 캡션만 삽입>
```
결과)
@<tbl:nil;nil;표 내용 없이 캡션만 삽입>
%%% en
### Table insertion from excel file

@<size:30>@<b>Expression : @@<tbl:@<color:FF0000>file_name@</color>;@<color:FF0000>sheet_name@</color>;@<color:FF0000>caption@</color>>@</b>@</size>

In the case of inserting a table, you can move the content created in Excel, and the width will be the same as possible for the cells merged in Excel.
The first line of the table is the top title, and when you want to align lower data columns in the center, insert the character '*' front of the title name.

ex1) Table from excel file : "media/table_sample.xlsx"
```html
@<tbl:media/table_sample.xlsx;Sheet1;Excel table example>
```
 
@<img:#media/table_sample.png;1;"media/table_sample.xlsx" Actual excel file>

Result)
@<tbl:media/table_sample.xlsx;Sheet1;Excel table example>
 
ex2) Insert only captions without table content
```html
@<tbl:nil;nil;Insert only a caption without table content>
```
Result)
@<tbl:nil;nil;Insert only a caption without table content>
%%%
 



 
%%% ko
### Lua 표현으로부터 표 삽입
@<size:30>@<b>표현식 : @@<tbl:lua;@<color:FF0000>테이블_변수명@</color>;@<color:FF0000>캡션@</color>>@</b>@</size>

기본적으로 docgen 이 Lua 스크립트 기반으로 작성되기 때문에, Lua 변수로 테이블을 만들어 이를 활용하는 방법을 취할 수 있습니다.
본문에서는 이미 아래와 같이 lua 테이블이 정의되어 있는 것을 활용한 예입니다.

예) 선언된 lua 표현
```lua
lua_table_example = {
	HeaderCount = 2,
	{{"Head A", merge={1,2}}, {"Head B", merge={3,1}}, {width=300, style="TableTextCenter"}, ""},
	{"", "Sub A", "Sub B", "Sub C"},
	{"index1", "a", "1", "aa"},
	{"index2", "b", "2", "bb"},
	{"index3", {"c", merge={2,1}}, "", "@<color:ff0000>cc@</color>"}
}
```
 
예) paragraph 내 표현식
```html
@<tbl:lua;lua_table_example;Lua 표 예시>
```

결과)
@<tbl:lua;lua_table_example;Lua 표 예시>
 
Lua table 은 "{ { { ... }, ... }, ... }" 와 같이 다중의 중괄호로 표기한다. 맨 외곽의 중괄호는 Table을 기술함을 말하며, 다음 중괄호는 Row, 그 다음 중괄호는 Column 을 표기한다.
Column 과 Row 를 표기시 아래와 같은 속성을 지정할 수 있으며, Column 의 경우 문자열로만 기술할 경우 중괄호를 생략할 수 있다.

* Lua table Row 속성
** HeaderCount : 해더 줄 수를 지정한다. (기본 : 1)
* Lua table Column 속성
** data : 셀의 문자열 데이터를 지정한다. "data=" 를 생략하고, 문자열만으로 기술하는 것이 가능하다. 단 생략시 셀 기술의 첫 번째 속성이어야 한다. (기본 : " ")
** width : 하나의 컬럼 가로 길이를 지정한다. 모든 컬럼의 길이 총합에서 나뉘어 최종 상대적 길이가 지정된다. 이 속성은 첫 Row 에 기술된 Column 에 대해서만 유효하다. (기본 : 100)
** style : 컬럼에 적용할 텍스트 스타일 이름을 지정한다. 지정된 스타일은 하위 동일 컬럼에도 동일하게 적용된다. (기본 : "TableTextLeft", 스타일을 다시 기본으로 지정할 경우 nil 값이 아닌 "" 을 지정한다.)
** merge : 현재 셀 위치부터 일정 범위의 다중 셀을 병합하는 기능을 한다. 속성에서 범위 지정은 @<b>{width,height}@</b>로 지정할 수 있다.
%%% en
### Inserting table from Lua expressions
@<size:30>@<b>Expression : @@<tbl:lua;@<color:FF0000>table_variable_name@</color>;@<color:FF0000>caption@</color>>@</b>@</size>

Basically, since docgen is written based on Lua scripts, you can create a table with Lua variables and utilize it.
This is an example that utilizes the lua table already defined as shown below.

예) declared lua expression
```lua
lua_table_example = {
	HeaderCount = 2,
	{{"Head A", merge={1,2}}, {"Head B", merge={3,1}}, {width=300, style="TableTextCenter"}, ""},
	{"", "Sub A", "Sub B", "Sub C"},
	{"index1", "a", "1", "aa"},
	{"index2", "b", "2", "bb"},
	{"index3", {"c", merge={2,1}}, "", "@<color:ff0000>cc@</color>"}
}
```
 
예) Expression within paragraph
```html
@<tbl:lua;lua_table_example;Lua table example>
```

결과)
@<tbl:lua;lua_table_example;Lua table example>
 
Lua tables are expressed with multiple braces, such as "{ { { ... }, ... }, ... }". The outer curly braces indicate the table, the next brace indicates Row, and the next brace indicates Column.
When expressing Column and Row, you can specify the following properties. In the case of Column, if you describe it only as a string, the braces can be omitted.

* Lua table Row properties
** HeaderCount: Specifies the number of header lines. (Default: 1)
* Lua table Column properties
** data : Specifies the string data of the cell. It is possible to omit "data=" and describe only as a string. However, if omitted, it must be the first attribute of the cell technology. (default : " ")
** width : Specifies the horizontal length of one column. The final relative length is given by dividing it from the total length of all columns. This property is valid only for the Column described in the first Row. (Default: 100)
** style : Specifies the text style name to apply to the column. The specified style is equally applied to the same child columns. (Default: "TableTextLeft", when specifying the style back to default, specify "" rather than nil.)
** merge : It functions to merge multiple cells within a certain range starting from the current cell location. Range can be specified in properties as @<b>{width,height}@</b>.
%%%
