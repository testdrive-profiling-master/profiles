local	Arg				= ArgTable("Document Generator for TestDrive Profiling Master. v1.01")
local	sTemplatePath	= String(nil)

sTemplatePath:GetEnvironment("TESTDRIVE_PROFILE")

Arg:AddOptionString	("template", "", "t", nil, "template", "Document template name/file.")

do	-- list-up customized document template list
	local	bCustomTemplateIsExisted	= false
	for src_path in lfs.dir(sTemplatePath.s .. "common/bin/codegen") do
		local sName		= String(src_path)
		local sExt		= String(src_path)
		sExt:CutFront(".", true)
		
		if sName:CompareFront("docgen_template_") and (sExt.s == "docx") then
			if bCustomTemplateIsExisted == false then
				Arg:AddRemark(nil, "*** Installed docgen template list ***")
				bCustomTemplateIsExisted	= true
			end

			sName:CutFront("docgen_template_")
			sName:CutBack(".")
		
			local sDesc = ""
			local f = TextFile()
			if f:Open(sTemplatePath.s .. "common/bin/codegen/docgen_template_" .. sName.s .. ".txt") then
				sDesc	= String(f:Get())
				sDesc:CutBack("\r\n", true)
				sDesc:Trim(" \t")
				sDesc	= sDesc.s
			end
		
			Arg:AddRemark(nil, string.format("%-14s : %s", sName.s, sDesc))
		end
	end
end

Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua file")
Arg:AddOptionFile	("out_file", "", nil, nil, "output_file", "output Microsoft Word(.docx) file")


if (Arg:DoParse() == false) then
	return
end

local	sInFilename		= Arg:GetOptionFile("in_file", 0)
local	sDocTemplate	= String(Arg:GetOptionString("template", 0))
local	sOutFilename	= Arg:GetOptionFile("out_file", 0)
doc 					= DocWord()

if sDocTemplate.s == "" then
	sTemplatePath:Append("Common/bin/codegen/docgen_template_testdrive.docx")
elseif sDocTemplate:CompareBack(".docx") then
	sTemplatePath.s	= sDocTemplate.s
else
	sTemplatePath:Append("Common/bin/codegen/docgen_template_" .. sDocTemplate.s .. ".docx")
end


-- 문서 템플릿 열기
if doc:Open(sTemplatePath.s) == false then
	os.exit()		-- template document is not existed
end

local	sDate = String(nil)


property = {}
sDate:FormatDate("%Y", 0)
property["Year"]					= sDate.s		-- 라이센스 연도 반영
property["Document_Name"]			= "-"
sDate:FormatDate("%B %d, %Y", 0)
property["Revision_Date"]			= sDate.s
property["IP_Version"]				= "-"
property["Doc_Version"]				= "-"
property["Comprehensive_IP_Title"]	= "-"
property["IP_Name_First_Page"]		= "-"
property["Business_Unit"]			= "-"
property["IP_Name_Header"]			= "-"
property["Ownership"]				= "-"
property["Document_Name_Header"]	= "-"
property["Water_Mark"]				= ""

-- Revision 추가 함수
local	doc_node					= doc:GetNode("word/document.xml", false):child("w:document"):child("w:body")
local	tbl_revision_insert			= doc_node:child_in_depth("w:t", "Doc Revision"):parent("w:tr");
local	tbl_revision				= tbl_revision_insert:parent(nil);
local	tbl_term					= doc_node:child_in_depth("w:t", "List of Term"):parent("w:p"):next_sibling("w:tbl")


local	month_list	= {
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
}

-- 지역 변수들...
local	doc_last		= doc_node:last_child()
local	chapture_id		= 0
local	chapture_sid	= 0
local	table_id		= 0
local	figure_id		= 0
local	table_count		= 0
local	figure_count	= 0
local	term_count		= 0
local	bookmark_id		= 70
local	bookmark_list	= {}

do	-- delete latest "paragraph"
	local	last_pr	= doc_last:previous_sibling("w:p")
	last_pr:Destroy(1)
end

function GenerateChapter(level, title)
	do	-- trim off space
		local	sTitle	= String(title)
		sTitle:Trim(" \t")
		sTitle:Replace("<", "&lt;", true)
		sTitle:Replace(">", "&gt;", true)
		sTitle:ChangeCharsetToUTF8()
		title	= sTitle.s
	end

	local	iStyleList		= {1, 20, 30, 41, 51}
	local	reference_id	= 0
	
	if level == 1 then
		table_restart	= true
		chapture_id		= chapture_id + 1
		chapture_sid	= 0
		table_id		= 1
		figure_id		= 1
		reference_id	= 70000000 + (chapture_id*1000000)
	else
		reference_id	= 70000000 + (chapture_id*1000000) + (chapture_sid*10)
	end
	
	reference_id	= 70000000 + (chapture_id*1000000) + (chapture_sid*10)
	chapture_sid	= chapture_sid + 1
	
	bookmark_list[title]	= reference_id
	
	local	sTitle	= "<w:p>\
			<w:pPr>\
				<w:pStyle w:val=\"" .. iStyleList[level] .. "\"/>\
				<w:rPr><w:rFonts w:cs=\"Arial\"/></w:rPr>\
			</w:pPr>\
			<w:bookmarkStart w:id=\"" .. bookmark_id .. "\" w:name=\"_Ref" .. reference_id .. "\"/>\
			<w:r>"
				.. ((level == 1) and "<w:lastRenderedPageBreak/>" or "<w:rPr><w:rFonts w:cs=\"Arial\" w:hint=\"eastAsia\"/></w:rPr>") ..
				"<w:t>" .. title .. "</w:t>\
			</w:r>\
			<w:bookmarkEnd w:id=\"" .. bookmark_id .. "\"/>\
		</w:p>"
	bookmark_id		= bookmark_id + 1
	return sTitle
end

function GenerateCaption(sType, content)
	local	caption_id		= 0
	local	reference_id	= 0
	local	sID				= 0
	local	bFirst			= false
	
	do	-- trim off space
		local	sContent		= String(content)
		sContent:Trim(" ")
		sContent:Replace("<", "&lt;", true)
		sContent:Replace(">", "&gt;", true)
		sContent:ChangeCharsetToUTF8()
		content	= sContent.s
	end
	
	if sType == "Table" then
		caption_id		= 20000000 + (chapture_id*100000) + (table_id*10)
		bFirst			= (table_id == 1)
		table_id		= table_id + 1
		table_count		= table_count + 1
		sID				= table_id
	else
		caption_id		= 30000000 + (chapture_id*100000) + (figure_id*10)
		bFirst			= (figure_id == 1)
		figure_id		= figure_id + 1
		figure_count	= figure_count + 1
		sID				= figure_id
	end
	
	reference_id		= caption_id + 30000000
	bookmark_list[content]	= reference_id

	local sXML = "<w:p>\
		<w:pPr>\
			<w:pStyle w:val=\"" .. sType .. "Caption\"/>\
		</w:pPr>\
		<w:bookmarkStart w:id=\"" .. tostring(bookmark_id) .. "\" w:name=\"_Toc" .. tostring(caption_id) .. "\"/>\
		<w:bookmarkStart w:id=\"" .. tostring(bookmark_id + 1) .. "\" w:name=\"_Toc" .. tostring(caption_id + 1) .. "\"/>\
		<w:bookmarkStart w:id=\"" .. tostring(bookmark_id + 2) .. "\" w:name=\"_Ref" .. tostring(reference_id) .. "\"/>\
		<w:r><w:t xml:space=\"preserve\">" .. sType .. " </w:t></w:r>\
		<w:r>\
			<w:fldChar w:fldCharType=\"begin\"/>\
			<w:instrText xml:space=\"preserve\"> STYLEREF  \\s Heading1,H1 </w:instrText>\
			<w:fldChar w:fldCharType=\"separate\"/>\
			<w:t>" .. chapture_id .. "</w:t>\
			<w:fldChar w:fldCharType=\"end\"/>\
		</w:r>\
		<w:r><w:noBreakHyphen/></w:r>\
		<w:r>\
			<w:fldChar w:fldCharType=\"begin\"/>\
			<w:instrText xml:space=\"preserve\"> SEQ " .. sType .. " \\* ARABIC " .. (bFirst and "\\r" or "\\s") .. " 1 </w:instrText>\
			<w:fldChar w:fldCharType=\"separate\"/>\
			<w:t>" .. sID .. "</w:t>\
			<w:fldChar w:fldCharType=\"end\"/>\
		</w:r>\
		<w:bookmarkEnd w:id=\"" .. tostring(bookmark_id + 2) .. "\"/>\
		<w:r><w:t xml:space=\"preserve\">. </w:t></w:r>\
		<w:bookmarkEnd w:id=\"" .. tostring(bookmark_id) .. "\"/>\
		<w:r><w:t xml:space=\"preserve\">" .. content .. "</w:t></w:r>\
		<w:bookmarkEnd w:id=\"" .. tostring(bookmark_id + 1) .. "\"/>\
	</w:p>"
	bookmark_id		= bookmark_id + 3
	return sXML
end

local	img_id			= 3000
local	temp_visio_file_list	= {}
function GenerateFigure(sFileName, fRatio)
	local cx			= 0
	local cy			= 0
	local width			= 0
	local height		= 0
	sFileName			= String(sFileName)
	local sPageName		= ""	-- for Visio
	local bSolidBox		= sFileName:CompareFront("#")
	if bSolidBox then
		sFileName:erase(0,1)
	end
	if sFileName:CompareBack("]") then
		sPageName	= String(sFileName.s)
		sFileName:CutBack("[", true)
		sFileName:Trim(" ")
		sPageName:CutFront("[", true)
		sPageName:CutBack("]", true)
		sPageName:Trim(" ")
		sPageName	= sPageName.s

		-- convert visio to svg
		if sFileName:CompareBack(".vsd") or sFileName:CompareBack(".vsdx") then
			os.execute("vsd2svg \"" .. sFileName.s .. "\" \"" .. sPageName .. "\"")
			
			if #sPageName ~= 0 then
				sFileName:Append("." .. sPageName)
			end
			sFileName:Append(".svg")
			temp_visio_file_list[#temp_visio_file_list + 1]		= sFileName.s
		end
	end
	
	sFileName	= sFileName.s
	
	local sExt			= String(sFileName)
	local sID			= doc:AddMedia(sFileName)
	local sExtraExt		= ""
	local sBlipAttr		= ""
	
	sExt:CutFront(".", true)
	sExt:MakeLower()
	sExt		= sExt.s
	
	do	-- get image width & height
		local sInfo		= String((sExt == "wmf") and exec("identify " .. sFileName) or exec("identify -size x " .. sFileName))
		sInfo:CutFront(" ", false)
		sInfo:CutFront(" ", false)
		sInfo:CutBack(" ", true)
		local	sx		= String(sInfo.s)
		local	sy		= String(sInfo.s)
		sx:CutBack("x", false)
		sy:CutFront("x", false)
		width	= tonumber(sx.s)
		height	= tonumber(sy.s)
		
		if height == nil then
			LOGE("Invalid image : \"" .. sFileName .. "\"")
			os.exit()
		end
		
		cx	= math.floor(fRatio * 6400000)
		cy	= math.floor(fRatio * 6400000 * height / width)
	end
	
	if sExt == "svg" then
		sExtraExt	= "\
		<a:extLst>\
			<a:ext uri=\"{96DAC541-7B7A-43D3-8B79-37D633B846F1}\">\
				<asvg:svgBlip xmlns:asvg=\"http://schemas.microsoft.com/office/drawing/2016/SVG/main\" r:embed=\"" .. sID .. "\"/>\
			</a:ext>\
		</a:extLst>"
		sBlipAttr	= "cstate=\"print\""
	end

	local	sXML	= "<w:p>\
		<w:pPr>\
			<w:suppressAutoHyphens w:val=\"0\"/>\
			<w:adjustRightInd/>\
			<w:snapToGrid/>\
			<w:spacing w:after=\"160\" w:line=\"259\" w:lineRule=\"auto\"/>\
			<w:jc w:val=\"center\"/>\
		</w:pPr>\
		<w:r>"
	do
		if sExt == "wmf" then
			local	pt_x	= width * 4.23 * fRatio
			local	pt_y	= height * 4.23 * fRatio
			sXML	= sXML .. "<w:object>\
						<v:shape style=\"width:" .. pt_x .. "pt;height:" .. pt_y .. "pt\" \
							o:bordertopcolor=\"this\" o:borderleftcolor=\"this\" o:borderbottomcolor=\"this\" o:borderrightcolor=\"this\" path=\"\" stroked=\"f\">\
							<v:fill opacity=\"0\"/>\
							<v:imagedata r:id=\"" .. sID .. "\" o:title=\"\"/>\
							<v:path o:connecttype=\"segments\"/>"
							.. (bSolidBox and "<w10:bordertop type=\"single\" width=\"4\"/><w10:borderleft type=\"single\" width=\"4\"/><w10:borderbottom type=\"single\" width=\"4\"/><w10:borderright type=\"single\" width=\"4\"/>" or "") ..
						"</v:shape>\
					</w:object>"
		else
			sXML	= sXML .. "<w:drawing>\
					<wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">\
						<wp:extent cx=\"" .. cx .. "\" cy=\"" .. cy .. "\"/>\
						<wp:effectExtent l=\"19050\" t=\"19050\" r=\"24130\" b=\"15240\"/>\
						<wp:docPr id=\"" .. tostring(img_id) .. "\" name=\"그림 " .. tostring(img_id) .. "\"/>\
						<wp:cNvGraphicFramePr>\
							<a:graphicFrameLocks xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" noChangeAspect=\"1\"/>\
						</wp:cNvGraphicFramePr>\
						<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
							<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">\
								<pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">\
									<pic:nvPicPr>\
										<pic:cNvPr id=\"" .. tostring(img_id) .. "\" name=\"그림 " .. tostring(img_id) .. "\"/>\
										<pic:cNvPicPr/>\
									</pic:nvPicPr>\
									<pic:blipFill>\
										<a:blip r:embed=\"" .. sID .. "\" " .. sBlipAttr .. ">\
										".. sExtraExt .. "\
											<a:clrChange>\
												<a:clrFrom>\
													<a:srgbClr val=\"FFFFFF\"/>\
												</a:clrFrom>\
												<a:clrTo>\
													<a:srgbClr val=\"FFFFFF\">\
														<a:alpha val=\"0\"/>\
													</a:srgbClr>\
												</a:clrTo>\
											</a:clrChange>\
										</a:blip>\
										<a:stretch>\
											<a:fillRect/>\
										</a:stretch>\
									</pic:blipFill>\
									<pic:spPr>\
										<a:xfrm>\
											<a:off x=\"0\" y=\"0\"/>\
											<a:ext cx=\"" .. cx .. "\" cy=\"" .. cy .. "\"/>\
										</a:xfrm>\
										<a:prstGeom prst=\"rect\">\
											<a:avLst/>\
										</a:prstGeom>"
										.. (bSolidBox and "<a:ln><a:solidFill><a:schemeClr val=\"tx1\"/></a:solidFill></a:ln>" or "") ..
									"</pic:spPr>\
								</pic:pic>\
							</a:graphicData>\
						</a:graphic>\
					</wp:inline>\
				</w:drawing>"
		end
		sXML	= sXML .. "</w:r></w:p>"
	end

	img_id	= img_id + 1
	
	return sXML
end

function GenerateTable(sExcelFileName, sSheetName)
	-- 엑셀 열기
	local	xls		= DocExcel()
	
	if xls:Open(sExcelFileName) == false then
		LOGE("Can't open excel file : " .. sExcelFileName)
		os.exit()
	end
	
	-- 시트 열기
	local	sheet			= xls:GetSheet(sSheetName)
	local	col_count		= 0
	local	col_total_width	= 0
	local	col_list		= {}
	
	if sheet == nil then
		LOGE("Can't open sheet table : " .. sExcelFileName .. "(" .. sSheetName .. ")")
		os.exit()
	end
	
	-- 첫 줄 목록 얻기
	if sheet:GetRow(false) then
		while sheet:GetColumn(false) do
			local	width			= sheet:GetColumnWidth()
			local	sName			= String(sheet:GetValue())
			local	bCenter			= sName:CompareFront("*")
			if bCenter then
				sName:erase(0, 1)
			end
			col_count				= col_count + 1
			col_list[col_count]		= {sName.s, width, bCenter, ""}		-- sValue, column width, center style, merge
			col_total_width			= col_total_width + width
		end
		
		for i=1, col_count do
			col_list[i][2]	= math.floor((col_list[i][2] * 3900) / col_total_width);
		end
	else
		LOGW("No table data. : " .. sExcelFileName .. "(" .. sSheetName .. ")")
		return
	end

	local	table_code	= String("\
	<w:tbl>\
		<w:tblPr>\
			<w:tblStyle w:val=\"ac\"/>\
			<w:tblW w:w=\"5000\"\
					w:type=\"pct\"/>\
			<w:tblBorders>\
				<w:top w:val=\"single\"\
					   w:sz=\"4\"\
					   w:space=\"0\"\
					   w:color=\"AEAAAA\"\
					   w:themeColor=\"background2\"\
					   w:themeShade=\"BF\"/>\
				<w:left w:val=\"single\"\
						w:sz=\"4\"\
						w:space=\"0\"\
						w:color=\"AEAAAA\"\
						w:themeColor=\"background2\"\
						w:themeShade=\"BF\"/>\
				<w:bottom w:val=\"single\"\
						  w:sz=\"4\"\
						  w:space=\"0\"\
						  w:color=\"AEAAAA\"\
						  w:themeColor=\"background2\"\
						  w:themeShade=\"BF\"/>\
				<w:right w:val=\"single\"\
						 w:sz=\"4\"\
						 w:space=\"0\"\
						 w:color=\"AEAAAA\"\
						 w:themeColor=\"background2\"\
						 w:themeShade=\"BF\"/>\
				<w:insideH w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"AEAAAA\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
				<w:insideV w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"AEAAAA\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
			</w:tblBorders>\
			<w:tblLayout w:type=\"fixed\"/>\
			<w:tblLook w:val=\"06A0\"\
					   w:firstRow=\"1\"\
					   w:lastRow=\"0\"\
					   w:firstColumn=\"1\"\
					   w:lastColumn=\"0\"\
					   w:noHBand=\"1\"\
					   w:noVBand=\"1\"/>\
		</w:tblPr>\
		<w:tblGrid>")

	for i=1, col_count do
		table_code:Append("<w:gridCol w:w=\"" .. col_list[i][2] .. "\"/>")
	end

	-- header
	table_code:Append("\
		</w:tblGrid>\
		<w:tr>\
			<w:trPr>\
				<w:cnfStyle w:val=\"100000000000\"\
							w:firstRow=\"1\"\
							w:lastRow=\"0\"\
							w:firstColumn=\"0\"\
							w:lastColumn=\"0\"\
							w:oddVBand=\"0\"\
							w:evenVBand=\"0\"\
							w:oddHBand=\"0\"\
							w:evenHBand=\"0\"\
							w:firstRowFirstColumn=\"0\"\
							w:firstRowLastColumn=\"0\"\
							w:lastRowFirstColumn=\"0\"\
							w:lastRowLastColumn=\"0\"/>\
				<w:trHeight w:val=\"45\"/>\
				<w:tblHeader/>\
			</w:trPr>"
		)
		
	for i=1, col_count do
		table_code:Append("\
			<w:tc>\
				<w:tcPr>\
					<w:tcW w:w=\"" .. col_list[i][2] .. "\"\
						   w:type=\"dxa\"/>\
					<w:tcBorders>\
						<w:top w:val=\"single\"\
							   w:sz=\"4\"\
							   w:space=\"0\"\
							   w:color=\"AEAAAA\"\
							   w:themeColor=\"background2\"\
							   w:themeShade=\"BF\"/>\
						<w:left w:val=\"single\"\
								w:sz=\"4\"\
								w:space=\"0\"\
								w:color=\"AEAAAA\"\
								w:themeColor=\"background2\"\
								w:themeShade=\"BF\"/>\
						<w:bottom w:val=\"single\"\
								  w:sz=\"4\"\
								  w:space=\"0\"\
								  w:color=\"AEAAAA\"\
								  w:themeColor=\"background2\"\
								  w:themeShade=\"BF\"/>\
						<w:right w:val=\"single\"\
								 w:sz=\"4\"\
								 w:space=\"0\"\
								 w:color=\"AEAAAA\"\
								 w:themeColor=\"background2\"\
								 w:themeShade=\"BF\"/>\
					</w:tcBorders>\
					<w:shd w:val=\"clear\"\
						   w:color=\"auto\"\
						   w:fill=\"7F7F7F\"\
						   w:themeFill=\"text1\"\
						   w:themeFillTint=\"80\"/>\
				</w:tcPr>"
				.. EncodeParagraph(col_list[i][1], {pPr="<w:pStyle w:val=\"TableColumn\"/>\
				<w:rPr>\
					<w:color w:val=\"FFFFFF\"\
							 w:themeColor=\"background1\"/>\
					<w:sz w:val=\"22\"/>\
					<w:szCs w:val=\"22\"/>\
				</w:rPr>", rPr="\
				<w:rFonts w:hint=\"eastAsia\"/>\
				<w:color w:val=\"FFFFFF\" w:themeColor=\"background1\"/>\
				<w:sz w:val=\"22\"/>\
				<w:szCs w:val=\"22\"/>"}) ..
			"</w:tc>"
		)
	end
	table_code:Append("</w:tr>")
	
	-- 데이터 줄 얻기
	if sheet:GetRow(false) then
		local	bLast	= false
		-- 한 줄 읽기
		while bLast == false do
			for i=1, col_count do
				sheet:GetColumn(false)
				col_list[i][1]	= sheet:GetValue()
				col_list[i][4]	= sheet:GetMergeCellPos()
				if #(col_list[i][1]) == 0 then col_list[i][1] = " " end	-- cell must be filled
			end
			bLast	= (sheet:GetRow(false) == false)
			
			-- 한줄 채우기
			table_code:Append("\
			<w:tr>")
			-- 컬럼 채우기
			for i=1, col_count do
				local	bMerge		= (#col_list[i][4] ~= 0)
				local	bMergeDown	= false
							
				table_code:Append("\
				<w:tc>\
					<w:tcPr>\
						<w:tcW w:w=\"" .. col_list[i][2] .. "\" w:type=\"dxa\"/>")
			
				if bMerge then	-- merge case
					local	sMerge	= String(col_list[i][4])
					local	x		= tonumber(sMerge:Tokenize(",").s)
					local	y		= tonumber(sMerge:Tokenize(",").s)
					local	width	= tonumber(sMerge:Tokenize(",").s)
					local	height	= tonumber(sMerge:Tokenize(",").s)
					
					if width ~= 1 then
						table_code:Append("<w:hMerge ")
						if x == 0 then
							table_code:Append("w:val=\"restart\"")
						else
							bMergeDown	= true
							if x ~= (width-1) then
								table_code:Append("w:val=\"continue\"")
								
							end
						end
						table_code:Append("/>")
					end
					
					if height ~= 1 then
						table_code:Append("<w:vMerge ")
						if y == 0 then
							table_code:Append("w:val=\"restart\"")
						else
							bMergeDown	= true
							if y ~= (height-1) then
								table_code:Append("w:val=\"continue\"")
								
							end
						end
						table_code:Append("/>")
					end
				end
				
				table_code:Append("\
						<w:tcBorders>\
							<w:top w:val=\"single\"\
								   w:sz=\"4\"\
								   w:space=\"0\"\
								   w:color=\"AEAAAA\"\
								   w:themeColor=\"background2\"\
								   w:themeShade=\"BF\"/>"
				)
				
				if i == 1 then	-- first column
					table_code:Append("<w:left w:val=\"nil\"/>")
				end
				if i == col_count then	-- last column
					table_code:Append("<w:right w:val=\"nil\"/>")
				end
				
				table_code:Append("\
						</w:tcBorders>\
					</w:tcPr>"
					.. EncodeParagraph(col_list[i][1],
					{
						pPr=("<w:pStyle w:val=\"" .. (col_list[i][3] and "TableTextCenter" or "TableTextLeft") .. "\"/>"),
						rPr="<w:rFonts w:hint=\"eastAsia\"/>"
					}) .. 
				"</w:tc>")
			end
			table_code:Append("</w:tr>")
		end
	end
	table_code:Append("</w:tbl>")
	
	return table_code.s
end

function GenerateTableFromLua(sLuaTable)
	local lua_table = _G[sLuaTable]
	
	if lua_table == nil then
		LOGI("Invalid lua variable '" .. sLuaTable .. "'")
		os.exit(1)
	end
	
	local	col_count		= 0
	local	col_width_list	= {}
	local	col_merge		= {}
	local	col_style		= {}
	local	col_string		= nil

	local	table_code	= String("\
	<w:tbl>\
		<w:tblPr>\
			<w:tblStyle w:val=\"ac\"/>\
			<w:tblW w:w=\"5000\"\
					w:type=\"pct\"/>\
			<w:tblBorders>\
				<w:top w:val=\"single\"\
					   w:sz=\"4\"\
					   w:space=\"0\"\
					   w:color=\"AEAAAA\"\
					   w:themeColor=\"background2\"\
					   w:themeShade=\"BF\"/>\
				<w:left w:val=\"single\"\
						w:sz=\"4\"\
						w:space=\"0\"\
						w:color=\"AEAAAA\"\
						w:themeColor=\"background2\"\
						w:themeShade=\"BF\"/>\
				<w:bottom w:val=\"single\"\
						  w:sz=\"4\"\
						  w:space=\"0\"\
						  w:color=\"AEAAAA\"\
						  w:themeColor=\"background2\"\
						  w:themeShade=\"BF\"/>\
				<w:right w:val=\"single\"\
						 w:sz=\"4\"\
						 w:space=\"0\"\
						 w:color=\"AEAAAA\"\
						 w:themeColor=\"background2\"\
						 w:themeShade=\"BF\"/>\
				<w:insideH w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"AEAAAA\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
				<w:insideV w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"AEAAAA\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
			</w:tblBorders>\
			<w:tblLayout w:type=\"fixed\"/>\
			<w:tblLook w:val=\"06A0\"\
					   w:firstRow=\"1\"\
					   w:lastRow=\"0\"\
					   w:firstColumn=\"1\"\
					   w:lastColumn=\"0\"\
					   w:noHBand=\"1\"\
					   w:noVBand=\"1\"/>\
		</w:tblPr>\
		<w:tblGrid>")
	for i=1, col_count do
		table_code:Append("<w:gridCol w:w=\"" .. col_width_list[i] .. "\"/>")
	end
	table_code:Append("</w:tblGrid>")

	-- 데이터 줄 얻기
	if #lua_table >= 1 then
		local	iRowCount		= 0
		local	HeaderCount		= 1
		local	HeaderWidth		= 3900
		
		if lua_table.HeaderCount ~= nil then
			HeaderCount	= lua_table.HeaderCount
		end
		
		if lua_table.HeaderWidth ~= nil then
			HeaderWidth	= lua_table.HeaderWidth
		end
		
		-- 한 줄 읽기
		for iRow=1, #lua_table do
			local	row_data		= lua_table[iRow]
			
			if type(row_data) == "table" then
				local	bHeader		= (iRowCount < HeaderCount)
				iRowCount			= iRowCount + 1
				
				-- 첫 줄일 때, 먼저 총 column width 계산
				if iRowCount == 1 then
					local	col_total_width	= 0
					col_count				= #row_data
					
					for i=1, col_count do
						local	col_width	= 100
						local	col_data	= row_data[i]
						col_style[i]		= nil
						col_merge[i]		= {x=0, y=0, w=0, h=0}

						if type(col_data) == "table" then						
							if col_data.width ~= nil then
								col_width			= col_data.width
							end
						end
						
						col_width_list[i]	= col_width
						col_total_width		= col_total_width + col_width
					end
					
					for i=1, col_count do
						col_width_list[i]	= math.floor((col_width_list[i] * HeaderWidth) / col_total_width);
					end
				end

				-- mergedown 갱신
				for i=1, col_count do
					if col_merge[i].y ~= 0 then
						col_merge[i].y	= col_merge[i].y + 1
						
						if col_merge[i].y > col_merge[i].h then
							col_merge[i].x	= 0
							col_merge[i].y	= 0
							col_merge[i].w	= 0
							col_merge[i].h	= 0
						end
					end
				end

				-- 줄 시작
				table_code:Append("<w:tr>")
				if bHeader then
					table_code:Append("\
						<w:trPr>\
						<w:cnfStyle w:val=\"100000000000\"\
								w:firstRow=\"1\"\
								w:lastRow=\"0\"\
								w:firstColumn=\"0\"\
								w:lastColumn=\"0\"\
								w:oddVBand=\"0\"\
								w:evenVBand=\"0\"\
								w:oddHBand=\"0\"\
								w:evenHBand=\"0\"\
								w:firstRowFirstColumn=\"0\"\
								w:firstRowLastColumn=\"0\"\
								w:lastRowFirstColumn=\"0\"\
								w:lastRowLastColumn=\"0\"/>\
						<w:trHeight w:val=\"45\"/>\
						<w:tblHeader/>\
						</w:trPr>")
				end
				-- 컬럼 채우기
				for i=1, col_count do
					local	col_data		= row_data[i]
					
					if type(col_data) == "string" then
						col_string	= col_data
					else	-- table
						if col_data == nil then
							col_string	= " "
						elseif col_data.data == nil then
							if type(col_data[1]) == "string" then
								col_string	= col_data[1]
							else
								col_string	= " "
							end
						else
							col_string	= col_data.data
						end

						if col_data.style ~= nil then
							col_style[i]		= col_data.style
						end
						
						if col_data.merge ~= nil then
							for t=1, col_data.merge[1] do
								local index	= t + i - 1
								col_merge[index].x = t
								col_merge[index].y = 1
								col_merge[index].w = col_data.merge[1]
								col_merge[index].h = col_data.merge[2]
							end
						end
					end
					
					if #col_string == 0 then col_string = " " end	-- cell must be filled

					table_code:Append("\
					<w:tc>\
						<w:tcPr>\
							<w:tcW w:w=\"" .. col_width_list[i] .. "\" w:type=\"dxa\"/>")
				
					if col_merge[i].x ~= 0 then	-- merge case
						
						if col_merge[i].w ~= 1 then
							table_code:Append("<w:hMerge ")
							if col_merge[i].x == 1 then
								table_code:Append("w:val=\"restart\"")
							else
								if col_merge[i].x ~= col_merge[i].w then
									table_code:Append("w:val=\"continue\"")
								end
							end
							table_code:Append("/>")
						end
						
						if col_merge[i].h ~= 1 then
							table_code:Append("<w:vMerge ")
							if col_merge[i].y == 1 then
								table_code:Append("w:val=\"restart\"")
							else
								if col_merge[i].y ~= col_merge[i].h then
									table_code:Append("w:val=\"continue\"")
								end
							end
							table_code:Append("/>")
						end
					end
					
					if bHeader then
						table_code:Append("<w:tcBorders>\
							<w:top w:val=\"single\"\
								   w:sz=\"4\"\
								   w:space=\"0\"\
								   w:color=\"AEAAAA\"\
								   w:themeColor=\"background2\"\
								   w:themeShade=\"BF\"/>\
							<w:left w:val=\"single\"\
									w:sz=\"4\"\
									w:space=\"0\"\
									w:color=\"AEAAAA\"\
									w:themeColor=\"background2\"\
									w:themeShade=\"BF\"/>\
							<w:bottom w:val=\"single\"\
									  w:sz=\"4\"\
									  w:space=\"0\"\
									  w:color=\"AEAAAA\"\
									  w:themeColor=\"background2\"\
									  w:themeShade=\"BF\"/>\
							<w:right w:val=\"single\"\
									 w:sz=\"4\"\
									 w:space=\"0\"\
									 w:color=\"AEAAAA\"\
									 w:themeColor=\"background2\"\
									 w:themeShade=\"BF\"/>\
						</w:tcBorders>\
						<w:shd w:val=\"clear\"\
							   w:color=\"auto\"\
							   w:fill=\"7F7F7F\"\
							   w:themeFill=\"text1\"\
							   w:themeFillTint=\"80\"/>")
					else
						table_code:Append("\
							<w:tcBorders>\
								<w:top w:val=\"single\"\
									   w:sz=\"4\"\
									   w:space=\"0\"\
									   w:color=\"AEAAAA\"\
									   w:themeColor=\"background2\"\
									   w:themeShade=\"BF\"/>"
						)
						
						if i == 1 then	-- first column
							table_code:Append("<w:left w:val=\"nil\"/>")
						end
						if i == col_count then	-- last column
							table_code:Append("<w:right w:val=\"nil\"/>")
						end
						table_code:Append("</w:tcBorders>")
					end
					table_code:Append("</w:tcPr>")
					
					if bHeader then
						table_code:Append(EncodeParagraph(col_string, {pPr="<w:pStyle w:val=\"TableColumn\"/>\
						<w:rPr>\
							<w:color w:val=\"FFFFFF\"\
									 w:themeColor=\"background1\"/>\
							<w:sz w:val=\"22\"/>\
							<w:szCs w:val=\"22\"/>\
						</w:rPr>", rPr="\
						<w:rFonts w:hint=\"eastAsia\"/>\
						<w:color w:val=\"FFFFFF\" w:themeColor=\"background1\"/>\
						<w:sz w:val=\"22\"/>\
						<w:szCs w:val=\"22\"/>"}))
					else
						table_code:Append(EncodeParagraph(col_string,
								{
									pPr=("<w:pStyle w:val=\"" .. (((col_style[i] == nil) or (col_style[i] == "")) and "TableTextLeft" or col_style[i]) .. "\"/>"),
									rPr="<w:rFonts w:hint=\"eastAsia\"/>"
								}))
					end
					table_code:Append("</w:tc>")
				end
				table_code:Append("</w:tr>")
			end
		end
		
		if iRowCount == 0 then
			LOGE("No table data in Lua variable : " .. sLuaTable)
			os.exit(1)
		end
	end

	table_code:Append("</w:tbl>")

	return table_code.s
end

ReadText = function(sFile)
	local	txt	= TextFile()
	if txt:Open(sFile) == false then
		LOGE("Text file '" .. sFile .. ". is not existed.")
		os.exit()
	end
	return txt:GetAll(false)
end

local	bInline		= false

function EncodeParagraph(sText, sExtra)
	local	sPara	= String(sText)
	local	sResult	= String("")
	
	if sPara:CompareFront("[[") and sPara:CompareBack("]]") then
		sPara:erase(0,2)
		sPara:CutBack("]]", false)
		sPara:ChangeCharsetToANSI()
		local	txt	= TextFile()
		if txt:Open(sPara.s) == false then
			LOGE("Text file '" .. sPara.s .. ". is not existed.")
			os.exit()
		end
		sPara	= String(txt:GetAll(false))
		sPara:Replace("  \\\n", " ", true);
		sPara:Replace("  \\\r\n", " ", true);
		sPara:Replace(" \\\n", " ", true);
		sPara:Replace(" \\\r\n", " ", true);
		txt:Close()
	end
	
	sPara:ChangeCharsetToUTF8()
	sPara:Replace("@@", "&#64;", true)
	sPara:Replace("\\$", "&#36;", true)
	
	while true do
		local	sLine	= sPara:Tokenize("\r\n")	-- enter code process
		local	s_pPr	= ""

		if sPara.TokenizePos < 0 then
			break
		end
		sResult:Append("<w:p>")
		if sExtra ~= nil and sExtra.pPr ~= nil then
			s_pPr	= sExtra.pPr
		end

::new_line::
		sLine:TrimRight(" \t")
		
		if bInline == false then
			-- math macro
			do
				local	iPos	= sLine:find("$$", 0)
				if iPos >= 0 then
					sLine:erase(iPos, 2)
					local	iEndPos	= sLine:find("$$", 0)
					
					while iEndPos < 0 do
						sLine:Append(sPara:Tokenize("\r\n").s)
						iEndPos	= sLine:find("$$", iPos)
						if sPara.TokenizePos < 0 then break end
					end
					
					if iEndPos > 0 then
						local	sContent	= String(sLine.s)
						sContent:erase(iEndPos, -1)
						if iPos > 0 then
							sContent:erase(0, iPos)
						end
						sLine:erase(iPos, iEndPos - iPos + 2)
						sContent:Trim(" \t")
						sContent	= "<m:oMath><m:r><m:rPr><m:sty m:val=\"p\"/></m:rPr><w:rPr><w:rFonts w:ascii=\"Cambria Math\" w:eastAsia=\"Cambria Math\" w:hAnsi=\"Cambria Math\" w:cs=\"Cambria Math\"/></w:rPr><m:t>" .. sContent.s .. "</m:t></m:r></m:oMath>"
						if sLine:Length() ~= 0 then
							sContent	= "<m:oMathPara>" .. sContent .. "</m:oMathPara>"
						end
						sContent	= "@<*>" .. sContent .. "@</*>"
						sLine:insert(iPos, sContent)
					end
				end
			end
			
			-- horizontal line
			if sLine:CompareFront("---") then
				sLine:CutFront("-", true)
				s_pPr	= s_pPr .. "\
					<w:pBdr>\
						<w:bottom w:val=\"single\"\
								  w:sz=\"6\"\
								  w:space=\"1\"\
								  w:color=\"auto\"/>\
					</w:pBdr>"
			-- code reference
			elseif sLine:CompareFront("```") then
				local	bLine	= false
				sResult:CutBack("<w:p>", false)
				sLine:CutFront("`", true)
				sLine:Trim(" ")
				if sLine:CompareFront("#") then
					bLine	= true
					sLine:erase(0,1)
				end
				local	sCodeType	= sLine.s
				local	sIndent		= ""
				-- cut off front
				sPara:erase(0, sPara.TokenizePos)
				sPara.TokenizePos	= 0
				-- get code length
				local	iCodeLen	= 0
				while sPara.TokenizePos >= 0 do
					sLine	= sPara:Tokenize("\r\n")
					if sLine:CompareFront("```") then break end
					iCodeLen	= sPara.TokenizePos
				end
				-- make contents
				local	sContent	= String(sPara.s)
				if iCodeLen == 0 then	-- no contents
					goto continue
				else
					sContent:erase(iCodeLen, -1)
					sContent:Replace("\r", "", true)
					sContent:Replace("\n\n", "\n \n", true)
					sContent:TrimLeft("\n")
					sContent:TrimRight(" \n")
					sContent:Replace("@```", "```", true)
				end
				
				do	-- make highlight contents
					local txt = TextFile()
					if txt:Create(".docgen_code_highlight.tmp") then
					else
						LOGE("Create temporary file is failed.")
						os.exit()
					end
					txt:Put(sContent.s)
					txt:Close()
					sContent	= exec("code_highlighter --ilang=" .. sCodeType .. " --olang=docgen " .. (bLine and "-n " or "") .. ".docgen_code_highlight.tmp")
					exec("rm -f .docgen_code_highlight.tmp")
					
					if bLine then
						local sIndentString	= String(sContent)
						sIndentString:CutBack(":@<", true)
						sIndentString:CutFront(">", true)
						local	iIndent	= (sIndentString:Length()+2) * 110	-- chars + ": "
						sIndent	= "<w:ind w:left=\"" .. iIndent .. "\" w:hangingChars=\"" .. (iIndent/2) .. "\" w:hanging=\"" .. iIndent .. "\"/>"
					end
				end
				bInline	= true
				sResult:Append(
				"<w:tbl>\
				<w:tblPr>\
					<w:tblStyle w:val=\"ae\"/>\
					<w:tblW w:w=\"0\" w:type=\"auto\"/>\
					<w:tblBorders>\
						<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F2F2F2\"/>\
						<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F2F2F2\"/>\
						<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F2F2F2\"/>\
						<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F2F2F2\"/>\
					</w:tblBorders>\
					<w:tblCellMar>\
						<w:top w:w=\"32\" w:type=\"dxa\"/>\
						<w:bottom w:w=\"52\" w:type=\"dxa\"/>\
					</w:tblCellMar>\
				</w:tblPr>\
				<w:tblGrid>\
					<w:gridCol w:w=\"10094\"/>\
				</w:tblGrid>\
				<w:tr>\
					<w:tc>\
						<w:tcPr>\
							<w:tcW w:w=\"10094\" w:type=\"dxa\"/>\
							<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"F2F2F2\" w:themeFill=\"background1\" w:themeFillShade=\"F2\"/>\
						</w:tcPr>"
				.. EncodeParagraph(sContent, {pPr=("<w:jc w:val=\"left\"/><w:spacing w:after=\"0\"/>" .. sIndent), rPr="<w:rFonts w:ascii=\"돋움체\" w:eastAsia=\"돋움체\" w:hAnsi=\"돋움체\"/>"}) ..
				"</w:tc></w:tr></w:tbl>"
				)
				bInline	= false
				goto continue
			elseif sLine:CompareFront(";;;") then
				sLine:CutFront(";", true)
				sResult:Append("<w:br w:type=\"page\"/>")
			elseif sLine:CompareFront(":::") then
				sLine:CutFront(":", true)
				sLine:Trim(" \t")
				s_pPr	= String(s_pPr)
				s_pPr:DeleteBlock("<w:pStyle*/>", 0)
				s_pPr:Append("<w:pStyle w:val=\"" .. sLine.s .. "\"/>")
				s_pPr	= s_pPr.s
				sLine	= sPara:Tokenize("\r\n")
				goto new_line
			else	-- intended list
				local	ilevel			= 0
				
				if sLine:CompareFront("*") then
					local	bIntend			= true
					local	iIndentWidth	= 760
					sLine:erase(0,1)
					
					while sLine:CompareFront("*") do
						sLine:erase(0,1)
						ilevel		= ilevel + 1
						if ilevel == 1 then
							iIndentWidth	= 1200
						else
							iIndentWidth	= iIndentWidth + 400
						end
					end
					sLine:Trim(" \t")

					-- set intend style
					s_pPr	= s_pPr .. "\
					<w:pStyle w:val=\"BodyText10\"/>"

					local	sLeftIntend	= ""
					if sLine:CompareFront(">") then
						sLine:CutFront(">", false)
						sLine:TrimLeft(" \t")
						sLeftIntend	= "w:left=\"" .. iIndentWidth .. "\" w:firstLine=\"0\" "
					else
						s_pPr	= s_pPr .. "<w:numPr>\
							<w:ilvl w:val=\"" .. ilevel .. "\"/>\
							<w:numId w:val=\"33\"/>\
						</w:numPr>"
					end

					s_pPr	= s_pPr .. "<w:adjustRightInd/>\
					<w:snapToGrid/>\
					<w:jc w:val=\"both\"/>\
					<w:spacing w:after=\"80\" w:line=\"200\" w:lineRule=\"auto\"/>\
					<w:ind ".. sLeftIntend .. "w:firstLineChars=\"0\"/>"
					
				elseif sLine:CompareFront("#") then
					sLine:erase(0,1)
					ilevel	= 1
					while sLine:CompareFront("#") do
						sLine:erase(0,1)
						ilevel		= ilevel + 1
					end
					sLine:Trim(" \t")
					sResult:CutBack("<w:p>", false)
					sResult:Append(GenerateChapter(ilevel, sLine.s))
					goto continue
				end
			end
		end

		if #s_pPr ~= 0 then
			sResult:Append("<w:pPr>" .. s_pPr .. "</w:pPr>")
		end

		do
			local		bBold			= false
			local		bItalic			= false
			local		bUnderline		= false
			local		bStrike			= false
			local		bSuperscript	= false
			local		bSubscript		= false
			local		bFixed			= false
			local		bColor			= false
			local		sColor			= ""
			local		bBgColor		= false
			local		sBgColor		= ""
			local		bSize			= false
			local		iSize			= 0
			sLine.TokenizePos	= 0
			
			while sLine.TokenizePos >= 0 do
				local	sWord	= sLine:TokenizeVariable("@<*>")
				local	s_rPr	= ""

				sWord:Replace("<", "&lt;", true)
				sWord:Replace(">", "&gt;", true)

				if #(sWord.s) ~= 0 then
					sResult:Append("<w:r>")
					
					if sExtra ~= nil and sExtra.rPr ~= nil then
						s_rPr	= sExtra.rPr
					end
					
					if bBold then
						s_rPr	= s_rPr .. "<w:b/><w:bCs/>"
					end
					if bItalic then
						s_rPr	= s_rPr .. "<w:i/><w:iCs/>"
					end
					if bUnderline then
						s_rPr	= s_rPr .. "<w:u w:val=\"single\"/>"
					end
					if bStrike then
						s_rPr	= s_rPr .. "<w:strike/>"
					end
					if bSuperscript then
						s_rPr	= s_rPr .. "<w:vertAlign w:val=\"superscript\"/>"
					elseif bSubscript then
						s_rPr	= s_rPr .. "<w:vertAlign w:val=\"subscript\"/>"
					end
					if bFixed then
						s_rPr	= s_rPr .. "<w:rFonts w:ascii=\"돋움체\" w:eastAsia=\"돋움체\" w:hAnsi=\"돋움체\"/>"
					end
					if bColor then
						s_rPr	= s_rPr .. "<w:color w:val=\"" .. sColor .. "\"/>"
					end
					if bBgColor then
						s_rPr	= s_rPr .. "<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" .. sBgColor .. "\"/>"
					end
					if bSize then
						s_rPr	= s_rPr .. "<w:sz w:val=\"" .. iSize .. "\"/><w:szCs w:val=\"" .. iSize .. "\"/>"
					end
					
					if #s_rPr ~= 0 then
						sResult:Append("<w:rPr>" .. s_rPr .. "</w:rPr>")
					end
					
					sResult:Append("<w:t xml:space=\"preserve\">" .. sWord.s .. "</w:t>")
					sResult:Append("</w:r>")
				end
				
				local	sVar	= sLine:GetVariable()

				if #(sVar.s) ~= 0 then	-- paragraph parsing
					local	bSet	= true
					
					if sVar:CompareFront("/") then
						bSet	= false
						sVar:erase(0, 1)
					end
					
					if sVar:Length() == 1 then
						if sVar.s == "b" then
							bBold	= bSet
						elseif sVar.s == "i" then
							bItalic	= bSet
						elseif sVar.s == "u" then
							bUnderline	= bSet
						elseif sVar.s == "s" then
							bStrike		= bSet
						elseif sVar.s == "*" then	-- bypass XML expression
							sLine:erase(0, sLine.TokenizePos)
							local	sContents	= String(sLine.s)
							sContents:CutBack("@</*>", true)
							sContents:Trim(" \r\n\t")
							sLine:CutFront("@</*>", false)
							
							if sContents:CompareFront("<w:p>") then
								sResult:CutBack("<w:p>", false)
								sResult:Append(sContents.s)
								goto continue
							else
								sResult:Append(sContents.s)
							end
						else
							LOGW("Invalid paragraph command : " .. sVar.s .. "\n\t Line : " .. sLine.s)
						end
					else
						local	sTag	= sVar:Tokenize(": ")
						sTag:MakeLower()
						if sTag.s == "sup" then
							bSuperscript	= bSet
						elseif sTag.s == "sub" then
							bSubscript		= bSet
						elseif sTag.s == "fixed" then
							bFixed			= bSet
						elseif sTag.s == "color" then
							bColor			= bSet
							if bColor then
								sColor		= sVar:Tokenize(": ").s
							end
						elseif sTag.s == "bgcolor" then
							bBgColor		= bSet
							if bBgColor then
								sBgColor	= sVar:Tokenize(": ").s
							end
						elseif sTag.s == "size" then
							bSize			= bSet
							if bSize then
								iSize		= tonumber(sVar:Tokenize(": ").s)
							end
						elseif sTag.s == "img" then
							local	sFileName	= sVar:Tokenize(";")
							local	fRatio		= tonumber(sVar:Tokenize("; ").s)
							local	sPageName	= ""
							sFileName:erase(0,1)
							sFileName:Trim(" ")
							sVar:erase(0, sVar.TokenizePos)
							sVar:Trim(" \t;")
							local	sCaption	= sVar.s
							if fRatio == nil then
								fRatio	= 1.0
							end
							sResult:CutBack("<w:p>", false)
							sResult:Append(GenerateFigure(sFileName.s, fRatio))
							
							if sCaption ~= nil and sCaption ~= "" then
								sResult:Append(GenerateCaption("Figure", sCaption))
							end

							goto continue
						elseif sTag.s == "tbl" then
							local	sMainName	= sVar:Tokenize("; ")
							local	sSubName	= sVar:Tokenize(";")
							sMainName:erase(0,1)
							sVar:erase(0, sVar.TokenizePos)
							sVar:Trim(" \t;")
							sSubName:Trim(" \t")
							local	sCaption	= sVar.s
							sResult:CutBack("<w:p>", false)
							
							if sCaption ~= nil and sCaption ~= "" then
								sResult:Append(GenerateCaption("Table", sCaption))
							end
							
							if sMainName.s == "lua" then
								sResult:Append(GenerateTableFromLua(sSubName.s))
							else
								sResult:Append(GenerateTable(sMainName.s, sSubName.s))	-- file_name, sheet_name
							end
							goto continue
						elseif sTag.s == "property" then
							local	sName	= sVar:Tokenize("")
							sName:TrimLeft(" :")
							sName:TrimRight(" ")
							sLine:insert(sLine.TokenizePos, String(property[sName.s]).s)
						elseif sTag.s == "bookmark" then
							local	sBookmark	= sVar:Tokenize("")
							sBookmark:TrimLeft(" :")
							sBookmark:TrimRight(" ")
							local	bAll		= sBookmark:CompareFront("@")
							if bAll then	-- recursive '#' + 'title'
								sBookmark:erase(0, 1)
								sLine:insert(sLine.TokenizePos, ". @<bookmark:" .. sBookmark.s .. ">")
								sBookmark:insert(0, "#")
							end
							sResult:Append("\
								<w:r><w:fldChar w:fldCharType=\"begin\"/></w:r>\
								<w:r><w:instrText xml:space=\"preserve\">BOOKMARK</w:instrText><w:t>" .. sBookmark.s .."</w:t></w:r>")
							if #s_rPr ~= 0 then
								sResult:Append("<w:r><w:instrText xml:space=\"preserve\"> \\* MERGEFORMAT </w:instrText></w:r>")
							end
							sResult:Append("<w:r><w:fldChar w:fldCharType=\"separate\"/></w:r><w:r>")
							if #s_rPr ~= 0 then
								sResult:Append("<w:rPr>" .. s_rPr .. "</w:rPr>")
							end
							sResult:Append("<w:t xml:space=\"preserve\">[" .. sBookmark.s .. "]</w:t></w:r>")
							sResult:Append("<w:r><w:fldChar w:fldCharType=\"end\"/></w:r>")
						elseif sTag.s == "link" then
							local	sLink		= sVar:Tokenize(";")
							local	sComment	= sVar:Tokenize(";")
							sLink:CutFront(":", false)
							if sComment:Length() == 0 then
								sComment:Append(sLink.s)
							end
							sResult:Append("<w:hyperlink r:id=\"" .. doc:AddHyperlink(sLink.s) .. "\"><w:r>\
									<w:rPr><w:rStyle w:val=\"af0\"/></w:rPr>\
									<w:t>"  .. sComment.s ..  "</w:t>\
								</w:r></w:hyperlink>")
						else
							LOGE("Can't recognize paragraph command : " .. sTag.s)
							os.exit()
						end
					end
				end
			end
		end

		sResult:Append("</w:p>")
		::continue::
	end
	return sResult.s
end

AddRevision = function(sVer, iYear, iMonth, iDay, sDescription)
	property["Doc_Version"]		= sVer
	property["Revision_Date"]	= month_list[iMonth] .. " " .. tostring(iDay) .. ", " .. tostring(iYear)
	tbl_revision:AddChildAfterFromBuffer(tbl_revision_insert, "<w:tr>\
				<w:tc>\
					<w:tcPr><w:tcW w:w=\"1985\" w:type=\"dxa\"/></w:tcPr>"
					.. EncodeParagraph(sVer, {pPr="<w:pStyle w:val=\"tabletext\"/><w:jc w:val=\"center\"/>", rPr="<w:color w:val=\"auto\"/>"}) ..
				"</w:tc>\
				<w:tc>\
					<w:tcPr><w:tcW w:w=\"2249\" w:type=\"dxa\"/></w:tcPr>"
					.. EncodeParagraph(property["Revision_Date"], {pPr="<w:pStyle w:val=\"tabletext\"/><w:jc w:val=\"center\"/>", rPr="<w:color w:val=\"auto\"/>"}) ..
				"</w:tc>\
				<w:tc>\
					<w:tcPr><w:tcW w:w=\"5834\" w:type=\"dxa\"/></w:tcPr>"
					.. EncodeParagraph(sDescription, {pPr="<w:pStyle w:val=\"tabletext\"/>", rPr="<w:color w:val=\"auto\"/>"}) ..
				"</w:tc>\
			</w:tr>");
end

AddTerm = function(sTag, sDescription)
	tbl_term:AddChildFromBuffer("\
	<w:tr>\
		<w:tc>\
			<w:tcPr>\
				<w:tcW w:w=\"1798\" w:type=\"dxa\"/>\
				<w:hideMark/>\
			</w:tcPr>"
			.. EncodeParagraph(sTag, {pPr="<w:pStyle w:val=\"TableTextCenter\"/><w:spacing w:after=\"0\"/>"}) ..
		"</w:tc>\
		<w:tc>\
			<w:tcPr>\
				<w:tcW w:w=\"8304\" w:type=\"dxa\"/>\
				<w:hideMark/>\
			</w:tcPr>"
			.. EncodeParagraph(sDescription, {pPr="<w:spacing w:after=\"0\"/>"}) ..
		"</w:tc>\
	</w:tr>")

	term_count	= term_count + 1
end

AddPageBreak = function()
	doc_node:AddChildBeforeFromBuffer(doc_last,"\
		<w:p>\
			<w:r>\
				<w:br w:type=\"page\"/>\
			</w:r>\
		</w:p>")
end

AddParagraph = function(content)
	doc_node:AddChildBeforeFromBuffer(doc_last, EncodeParagraph(content, {pPr="<w:pStyle w:val=\"BodyText10\"/>"}))
end

AddTable = function(sCaption, sExcelFileName, sSheetName)
	if sCaption ~= nil and sCaption ~= "" then
		doc_node:AddChildBeforeFromBuffer(doc_last, GenerateCaption("Table", sCaption))
	end
	
	doc_node:AddChildBeforeFromBuffer(doc_last, GenerateTable(sExcelFileName, sSheetName))
end

AddSubDocument = function(sDocFileName)
	local	id	= doc:AddSubDocument(sDocFileName)
	doc_node:AddChildBeforeFromBuffer(doc_last, "\
		<w:p>\
			<w:subDoc r:id=\"" .. id .. "\"/>\
		</w:p>"
	)
end

-- 사용자 소스 실행
if RunScript(sInFilename) == false then
	return
end

-- 속성 갱신
for key, value in pairs(property) do
	if key ~= "Water_Mark" then
		doc:SetProperty(key, value)
	end
end


-- 테이블/그림 목차 없을시 제거
if table_count == 0 then
	doc_node:child_in_depth("w:t", "List of Tables"):parent("w:p"):Destroy(5)
end

if figure_count == 0 then
	doc_node:child_in_depth("w:t", "List of Figures"):parent("w:p"):Destroy(5)
end

if term_count == 0 then
	doc_node:child_in_depth("w:t", "List of Terms"):parent("w:p"):Destroy(4)
end

-- 북마크 갱신
while true do
	local	bookmark_node	= doc_node:child_in_depth("w:instrText", "BOOKMARK")
	
	if bookmark_node:empty() then break end
	local	bookmark_text	= bookmark_node:next_sibling("w:t")
	local	sTarget			= String(bookmark_text:text())
	local	bNumber			= false
	local	bPage			= false
	
	if sTarget:CompareFront("#") then
		bNumber	= true
		sTarget:erase(0, 1)
	elseif sTarget:CompareFront("&") then
		bPage	= true
		sTarget:erase(0, 1)
	end
	
	local	bookmark_num	= bookmark_list[sTarget.s]
	
	if bookmark_num == nil then
		LOGE("bookmark \"" .. sTarget.s .. "\" is not found.")
		os.exit()
	end
	
	bookmark_text:Destroy(1)
	
	bookmark_node:set_string(" " .. (bPage and "PAGEREF" or "REF") .. " _Ref" .. bookmark_num .. (bNumber and " \\w" or "") .. " \\h ")
end

-- 파일 저장
do
	-- 저장 이름 없을 시 생성
	if #sOutFilename == 0 then
		sOutFilename	= property["Ownership"] .. "_" .. property["Business_Unit"] .. "_" .. property["IP_Name_Header"]  .. "_" .. property["Document_Name_Header"] .. "_rev" .. property["Doc_Version"] .. ".docx"
	end

	sOutFilename	= String(sOutFilename)
	sOutFilename:Trim(" \"")

	if sOutFilename:CompareBack(".docx") == false then
		sOutFilename:Append(".docx")
	end

	local sOutFilename_PDF	= String(sOutFilename.s)
	sOutFilename_PDF:CutBack(".docx", false)
	sOutFilename_PDF:Append(".pdf")
		
	-- 기존 doc/pdf 파일 제거
	if os.execute("rm -f \"" .. sOutFilename.s .. "\"") == false then
		LOGE("file '" .. sOutFilename.s .. "' is locked.")
		os.exit()
	end
	if os.execute("rm -f \"" .. sOutFilename_PDF.s .. "\"") == false then
		LOGE("file \"" .. sOutFilename_PDF.s .. "\" is locked.")
		os.exit()
	end
	
	-- 결과 저장
	LOGI("Build document : " .. sOutFilename.s)
	if doc:Save(sOutFilename.s) == false then
		LOGE("Can't create : " .. sOutFilename.s)
		os.exit()
	end
	doc:Close()
	
	-- 중간 생성 파일들 제거
	for i=1, #temp_visio_file_list do
		os.execute("rm -f \"" .. temp_visio_file_list[1] .. "\"")
	end	
	
	-- Field 갱신
	LOGI("Fields calculation & Saving to PDF output : " .. sOutFilename_PDF.s)
	os.execute("doc2pdf \"" .. sOutFilename.s .. "\" \"" .. property["Water_Mark"] .. "\" True")
	
	os.exit()
end
