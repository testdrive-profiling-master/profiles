-- 문서 속성들
property["Document_Name"]			= "TestDoc"
property["IP_Version"]				= "1.00"
property["Comprehensive_IP_Title"]	= "Some Title"
property["IP_Name_First_Page"]		= "Name_blabla~"
property["Business_Unit"]			= "foundry"
property["IP_Name_Header"]			= "Header_blabla~"
property["Ownership"]				= "TESTDRIVE"
property["Document_Name_Header"]	= "User guide"
property["Security_Level"]			= "BSD License."
property["Water_Mark"]				= "TESTDRIVE OPENSOURCED"


-- Revision history (버전, 연도, 월, 일, 설명) : 마지막 버전/날짜가 도큐먼트의 버전/날짜가 됩니다. 날짜 순으로 나열할 것!!!
AddRevision("0.01",	2021, 1,  1,	"Initial Draft")
AddRevision("0.02",	2022, 1,  2,	"Second release")
AddRevision("0.03",	2022, 1,  3,	"Third release")
AddRevision("1.02",	2022, 1, 26,	"Final release")

-- Term 추가
AddTerm("IoT", "Internet of things")
AddTerm("Gbps", "Gigabits per second")

-- 챕터, 표, 이미지, 하위 문서 추가 예제
AddParagraph(
"# 일반 표현\
## 하위 chapter\
### 동화\
#### 백설공주\
하얀 눈이 깃털처럼 흩날리던 날, 왕비는 흑단나무로 만든 자수틀로 작업을 하고 있었다. 작업을 하던 중, 하얀 눈을 감상하다 그만 손을 찔려 세 방울의 피를 눈 위에 떨어뜨렸다. 왕비는 떨어진 핏방울을 보며 눈처럼 하얗고, 피처럼 새빨갛고 흑단처럼 까만 아이를 가지길 소망했고, 소원대로 얼마 지나지 않아 피부가 눈처럼 하얗고, 입술은 피처럼 새빨갛고, 머리카락은 흑단처럼 까만 딸을 낳았다. 왕비는 아이의 이름을 백설이라 지었는데 기쁨도 잠시, 얼마 지나지 않아 산후병으로 세상을 떠나게 된다.\
몇 년 후, 왕은 재혼을 했다. 새 왕비는 아름다웠으나 오만했고, 인격이 바르지 못했다. 게다가 자신보다 미모가 더 뛰어난 사람이 있는 것을 참을 수 없어했다. 왕비는 진실만을 말하는 마법 거울에게 물으면서, 매일매일 자신이 이 세상에서 가장 아름답다는 사실을 확인하곤 했다. \"왕비마마보다 아름다운 사람은 없습니다.\"라는 답변을 받으면 만족해했다.\
어느덧 백설공주는 나이를 먹고 하루하루 예뻐져, 7살이 되어서는 왕비의 미모를 능가했다. 어느 날, 왕비는 평소처럼 거울에게 질문을 했는데, 거울이 \"백설공주가 더 아름답습니다.\"라고 답했다. 그 말을 듣자 질투심이 일어 화가 난 왕비는 사냥꾼을 불러 백설공주를 숲에 데리고 가서 죽이도록 명령했으나, 하지만 백설공주를 불쌍히 여긴 사냥꾼은 차마 백설공주를 죽이지 못하고, 숲 속으로 도망치게 했다."
)

AddPageBreak()

AddParagraph("## 테이블 표현")
AddParagraph("표(@<b>表@</b>, table)은 전체를 한눈에 쉽게 볼 수 있도록 꾸며 놓은 문서를 말한다. 네모 칸에 가로줄과 세로줄을 그어 작은 네모 칸으로 나누며, 가로줄은 행, 세로줄은 열이라고 한다. 기본적으로 스프레드시트의 시스템이며, 웬만한 워드프로세서에서 지원하는 기능이다.")
AddTable("Test table", "table_sample.xlsx", "Sheet1")


AddParagraph(
"# 수식 표현\
## 리스트\
* first\
** 링컨\
*** 모스\
**** 사자 : 무리의 수사자가 사냥을 잘 안 하는 이유는, 야생에서는 새끼의 생존이 매우 중요한데, 무리에서 사냥을 나갔을 때 새끼를 위협하는 천적인 다른 육식동물과 무리에 속하지 않는 수사자들로부터 새끼를 지키기 위해 힘을 아껴야 하기 때문이다. 암사자들도 자신의 새끼를 지킨다고 저항하지만 수사자와 암사자는 체급차가 워낙 커서 한계가 있다. 때문에 수사자가 사냥에 직접 나서서 힘을 소비하는 것을 최대한 자제하는 것이다. 그래선지 암컷들이 사냥을 해오면 가장 먼저 영양을 섭취한다. 불필요한 싸움을 피할 수 있기 때문에 이럴 때는 자신의 모습을 과시하는 갈기가 큰 도움이 된다.\
****> 다만 수사자도 먹이를 사냥할 때가 있다. 바로 무리 내에서 자신의 새끼를 가진 암사자가 출산했는데 무리가 작거나 무리가 미처 먹이를 구하지 못했을 경우. 여기에 다른 수사자들이 무리의 영역 내에서 알짱거릴 경우 암사자는 다른 수사자로부터 새끼를 지키고 젖도 줘야하기 때문에 선뜻 사냥을 나갈 수가 없다. 이럴 경우 수사자가 먹이를 구해온다.\
* second\
* third\
 \
## 문자열 표현\
expression : @<b>bold@</b> @<color:FF0000>colored @<i>italic@</i> text@</color> x@<sup>y@</sup>, ln@<sub>x@</sub>, @<s>strike@</s>, @<u>underline@</u>, @<bgcolor:FFFF00>background color@</bgcolor>, @<size:60>Big size@</size> text\
표현식은 : \"@@<...>\" 으로 사용됩니다.\
 \
## 상호 참조\
테이블 구현은 @<bookmark:Test table>를 참조하세요.\
첫번째 챕터는 \"@<bookmark:#일반 표현>. @<bookmark:일반 표현>\" 입니다.\
## 다음 줄 넘김\
page break~\
;;;\
## 가로선\
-----")


AddParagraph(" ")

AddParagraph("## 하위(서브, 외부 docx 참조) 문서")

AddSubDocument("sub_doc_sample.docx")			-- 하위 문서 셈플

AddParagraph("## 외부 markdown(modified) 하위 문서")
AddParagraph("[[markdown_sample.txt]]")	-- 외부 파일
