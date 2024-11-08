-- 주요 폴더 리스트 얻기
list = CreateFileList("../..", -1, true, "cpp;h")	 -- C 관련 파일만 검색

-- 리스트 출력
while(list:Size() ~= 0)
do
	local	v = list:Pop()
	print("file : "..v.data)
end
	