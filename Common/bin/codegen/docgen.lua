RunScript("codegen_utils")

---------------------------------------------------------------------------------
-- Initialize configuration
---------------------------------------------------------------------------------
docgen					= {}
docgen.code_format		= "cpp"					-- default code format
docgen.code_bgcolor		= "F7F7F7"				-- code background color
docgen.boarder_color	= "AEAAAA"				-- table's boarder color
docgen.fixed_font		= "Cascadia Mono"		-- fixed font name
docgen.profile_path		= String(nil)
docgen.template_path	= String(nil)
docgen.profile_path:GetEnvironment("TESTDRIVE_PROFILE")
docgen.template_path.s	= docgen.profile_path.s

-- check installed document template
docgen.installed_template	= {}
docgen.default_template		= ""
do
	local	latest_modification_date	= nil
	
	for src_path in lfs.dir(docgen.template_path.s .. "common/bin/codegen") do
		local sName		= String(src_path)
		local sExt		= String(src_path)
		sExt:CutFront(".", true)
		sExt:MakeLower()
		
		if sName:CompareFront("docgen_template_") and (sExt.s == "docx") then
			sName:CutFront("docgen_template_")
			sName:CutBack(".")
		
			-- set default
			if #docgen.default_template == 0 then
				docgen.default_template		= sName.s
			end
		
			local f = TextFile()
			if f:Open(docgen.template_path.s .. "common/bin/codegen/docgen_template_" .. sName.s .. ".txt") then
				local sDesc	= String(f:Get())
				sDesc:CutBack("\r\n", true)
				sDesc:Trim(" \t")
				
				if sDesc:CompareFront("*") then
					sDesc:erase(0, 1)
					sDesc:TrimLeft(" ")
					
					local modification_date = os.date("%Y%m%d%H%M%S", lfs.attributes(src_path, "modification"))
					
					-- set explicit newer default
					if latest_modification_date == nil or (latest_modification_date < modification_date) then
						docgen.default_template		= sName.s
						latest_modification_date	= modification_date
					end
				end
				
				docgen.installed_template[sName.s]	= sDesc.s
			end
		end
	end
end

-- supported output file format
docgen.supported_format			= {}
docgen.supported_format["djvu"]	= "DjVu image based document"
docgen.supported_format["pdf"]	= "Portable Document Format"
docgen.supported_format["odt"]	= "OpenDocument Text format"
docgen.supported_format["rtf"]	= "Rich Text Format"
docgen.supported_format["html"]	= "Standard HTML format"
docgen.supported_format["xml"]	= "Extensible Markup Language format"
docgen.supported_format["txt"]	= "Plain text format (unicode)"

---------------------------------------------------------------------------------
-- argument handling
---------------------------------------------------------------------------------
local	Arg				= ArgTable("Document Generator for TestDrive Profiling Master. v1.8")

Arg:AddOptionString	("template", "", "t", nil, "template", "Document template name/file.")
-- list-up customized document template list
Arg:AddRemark(nil, "*** Installed docgen template list ***")
for name, desc in key_pairs(docgen.installed_template) do
	Arg:AddRemark(nil, string.format("%-14s : %s", name, desc))
end
Arg:AddRemark(nil, "(default : " .. docgen.default_template .. ")")

Arg:AddOptionString ("install", nil, "i", nil, "template_desc", "install new docgen template.")
Arg:AddRemark		(nil, "ex) docgen -t name -i \"description\" template.docx")

Arg:AddOptionString	("format", "", "f", nil, "format", "Extra output format.")
Arg:AddRemark		(nil, "- supported output format")
for ext, desc in key_pairs(docgen.supported_format) do
	Arg:AddRemark		(nil, string.format("%-4s", ext) .. " : " .. desc)
end
Arg:AddRemark		(nil, "(ex: -f pdf,html)")
Arg:AddOptionString	("language", "", "l", nil, "language", "Document language code string.")
Arg:AddRemark		(nil, "'docgen.language' variable in Lua")
Arg:AddRemark		(nil, "(default : 'en')")

Arg:AddOptionString	("run", "", "r", "run", "lua_code", "Run Lua snippet code")
Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input Lua or .md(markdown) file")
Arg:AddOptionFile	("out_file", "", nil, nil, "output_file", "output Microsoft Word(.docx) file")


if (Arg:DoParse() == false) then
	return
end

docgen.sInFilename		= Arg:GetOptionFile("in_file", 0)
docgen.sDocTemplate		= String(Arg:GetOptionString("template"))
docgen.sOutFilename		= Arg:GetOptionFile("out_file", 0)

docgen.language			= String(Arg:GetOptionString("language"))
docgen.language:MakeLower()
docgen.language			= docgen.language.s
if #docgen.language == 0 then
	docgen.language		= "en"
end

do	-- check output format
	local	sExtraFormatList	= String(Arg:GetOptionString("format"))
	
	if #sExtraFormatList.s ~= 0 then
		docgen.output_format	= {}
		local	sToken			= sExtraFormatList:Tokenize(",;:+/&\"")
		sToken:MakeLower()
		
		while sToken:IsEmpty() == false do
			if docgen.supported_format[sToken.s] == nil then
				LOGE("Unsupported output format : " .. sToken.s)
				os.exit(1)
			end
			docgen.output_format[sToken.s]	= true
			sToken			= sExtraFormatList:Tokenize(",;:+/&\"")
		end
	end
end

if docgen.sDocTemplate.s == "" then
	docgen.sDocTemplate.s		= docgen.default_template
end

do	-- run lua definition code
	local sCode = String(Arg:GetOptionString("run", i))
	
	sCode:Replace(";", "\n", true)
	
	if #sCode.s ~= 0 then
		load(sCode.s)()
	end
end

-- template source file path
if docgen.sDocTemplate:CompareBack(".docx") then
	docgen.template_path.s	= docgen.sDocTemplate.s
else
	docgen.template_path:Append("Common/bin/codegen/docgen_template_" .. docgen.sDocTemplate.s .. ".docx")
end

do	-- install new template
	local	new_template_desc	= Arg:GetOptionString("install")
	
	if #new_template_desc ~= 0 then
		if docgen.sDocTemplate.s == "testdrive" then
			LOGE("Specify other template name, 'testdrive' is system's default template.")
			os.exit(1)
		end
		
		if (docgen.sDocTemplate:find(".", 0) >= 0) or (docgen.sDocTemplate:find(" ", 0) >= 0) then
			LOGE("Template must be a name, not a file. : " .. docgen.sDocTemplate.s)
			os.exit(1)
		end
		
		if #docgen.sOutFilename ~= 0 then
			LOGE("output file argument is not needed to install template document.")
			os.exit(1)
		end
		
		docgen.sInFilename	= String(docgen.sInFilename)
		
		if docgen.sInFilename:CompareBack(".docx") == false then
			LOGE("Input file must be a template document(.docx) file. : " .. docgen.sInFilename.s)
			os.exit(1)
		end
		
		LOGI("Set new template : " .. docgen.sDocTemplate.s)
		
		if lfs.IsExist(docgen.sInFilename.s) == false then
			LOGE("Input document template file is not existed : " .. docgen.sInFilename.s)
			os.exit(1)
		end
		
		if lfs.IsExist(docgen.template_path.s) then
			LOGW("The template document '" .. docgen.sDocTemplate.s .. "' is already existed. It will be overwritten.")
		end
		
		if #exec("cp -f \"" .. docgen.sInFilename.s .. "\" \"" .. docgen.template_path.s .. "\"") ~= 0 then
			LOGE("Template file copy is failed.")
			os.exist(1)
		end
		
		do	-- create description file
			local f = TextFile()
			if f:Create(docgen.profile_path.s .. "common/bin/codegen/docgen_template_" .. docgen.sDocTemplate.s .. ".txt") then
				f:Put(new_template_desc)
				f:Close()
			else
				LOGE("Can't create description file for template '" .. docgen.sDocTemplate.s .. "'")
				os.exit(1)
			end
		end
		
		LOGI("Template(" .. docgen.sDocTemplate.s .. ") installtion is done.")
		os.exit(0)
	end
end


---------------------------------------------------------------------------------
-- new document
---------------------------------------------------------------------------------
-- 문서 템플릿 열기
docgen.doc	 	= DocWord()

if docgen.doc:Open(docgen.template_path.s) == false then
	os.exit()		-- template document is not existed
end

local	sDate = String(nil)

property = {}
sDate:FormatDate("%Y", 0)
property["Year"]					= sDate.s		-- 라이센스 연도 반영
property["Document_Name"]			= ""
sDate:FormatDate("%B %d, %Y", 0)
property["Revision_Date"]			= sDate.s
property["Doc_Version"]				= ""
property["Main_Title"]				= ""
property["Sub_Title"]				= ""
property["IP_Name_Header"]			= ""
property["Ownership"]				= ""
property["Document_Name_Header"]	= ""
property["Water_Mark"]				= ""			-- 지정할 경우 워터마크가 overlay 된다.

-- Revision 추가 함수
docgen.doc_body						= docgen.doc:GetNode("word/document.xml", false):child("w:document"):child("w:body")
docgen.doc_styles					= docgen.doc:GetNode("word/styles.xml", false):child("w:styles")

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

local	excel_named_color_table = {
	["black"]	= 1,
	["white"]	= 2,
	["red"]		= 3,
	["green"]	= 4,
	["blue"]	= 5,
	["yellow"]	= 6,
	["magenta"]	= 7,
	["cyan"]	= 8
}

local	excel_numbered_color_codes = {
	"000000",
	"FFFFFF",
	"FF0000",
	"00FF00",
	"0000FF",
	"FFFF00",
	"FF00FF",
	"00FFFF",
	"800000",
	"008000",
	"000080",
	"808000",
	"800080",
	"008080",
	"C0C0C0",
	"808080",
	"9999FF",
	"993366",
	"FFFFCC",
	"CCFFFF",
	"660066",
	"FF8080",
	"0066CC",
	"CCCCFF",
	"000080",
	"FF00FF",
	"FFFF00",
	"00FFFF",
	"800080",
	"800000",
	"008080",
	"0000FF",
	"00CCFF",
	"CCFFFF",
	"CCFFCC",
	"FFFF99",
	"99CCFF",
	"FF99CC",
	"CC99FF",
	"FFCC99",
	"3366FF",
	"33CCCC",
	"99CC00",
	"FFCC00",
	"FF9900",
	"FF6600",
	"666699",
	"969696",
	"003366",
	"339966",
	"003300",
	"333300",
	"993300",
	"993366",
	"333399",
	"333333"
}

function GetExcelColorCode(sName)
	local	sColorName	= String(sName)
	local	iColorCode	= -1
	
	if #sName > 0 then
		if sColorName:CompareFront("color") then
			sColorName:erase(0, 5)
			iColorCode	= tonumber(sColorName.s)
		else
			iColorCode	= excel_named_color_table[sName]
			
			if iColorCode == nil then
				return ""
			end
		end
		
		if iColorCode >= 1 and iColorCode <= 57 then
			return excel_numbered_color_codes[iColorCode]
		end
	end
	
	return ""
end

local function ConvertStyleIDString(StyleName)
	-- find from style string
	local t			= docgen.doc_styles:child_by_attribute("w:style/w:name", "w:val", StyleName)
	local IdName	= t:parent("w:style"):get_attribute("w:styleId")
	
	-- find from style ID
	if IdName == nil then
		t 		= docgen.doc_styles:child_by_attribute("w:style", "w:styleId", StyleName)
		if t:empty() then
			error("Can't find style : " .. StyleName)
		end
		IdName	= StyleName
	end
	
	return IdName
end

docgen.revision			= {}
docgen.revision.title	= "Document Revision History"
docgen.revision.count	= 0
docgen.revision.tbl_top = docgen.doc_body:child_by_text("w:p", "w:t", docgen.revision.title):next_sibling("w:tbl"):child("w:tr")
docgen.revision.tbl		= docgen.revision.tbl_top:parent("w:tbl")

if docgen.revision.tbl_top:empty() then
	LOGW("Can't find table of Document revision : " .. docgen.revision.title)
end

docgen.revision.insert	= function(sVer, iYear, iMonth, iDay, sDescription)
	property["Doc_Version"]		= sVer
	property["Revision_Date"]	= month_list[iMonth] .. " " .. tostring(iDay) .. ", " .. tostring(iYear)
	docgen.revision.tbl:AddChildAfterFromBuffer(docgen.revision.tbl_top, "<w:tr>\
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
	docgen.revision.count	= docgen.revision.count + 1
end
AddRevision	= docgen.revision.insert	-- set global

docgen.terms			= {}
docgen.terms.title		= "List of Terms"
docgen.terms.count		= 0
docgen.terms.tbl		= docgen.doc_body:child_by_text("w:p", "w:t", docgen.terms.title):next_sibling("w:tbl")

if docgen.terms.tbl:empty() then
	LOGW("Can't find table of terms")
end

docgen.terms.insert		= function(sTag, sDescription)
	docgen.terms.tbl:AddChildFromBuffer("\
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

	docgen.terms.count	= docgen.terms.count + 1
end
AddTerm	= docgen.terms.insert

-- 지역 변수들...
local	doc_last		= docgen.doc_body:last_child()
local	chapture_id		= 0
local	chapture_sid	= 0
local	table_id		= 0
local	figure_id		= 0
local	table_count		= 0
local	figure_count	= 0
local	bookmark_id		= 70
local	bookmark_list	= {}

do	-- delete latest "paragraph"
	local	last_pr	= doc_last:previous_sibling("w:p")
	last_pr:Destroy(1)
end

function GenerateChapter(level, title)
	-- make log title
	local sChapterTitle = String(title)
	sChapterTitle:Trim(" \t")
	sChapterTitle:ChangeCharsetToUTF8()
	for i=3,level do
		sChapterTitle:insert(0, "    ")
	end
	sChapterTitle	= sChapterTitle.s

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
		
		print(chapture_id .. ". " .. sChapterTitle)
	else
		local sNum		= "" .. chapture_id .. ". "
		local sSpace	= ""
		for i = 1,#sNum do
			sSpace	= sSpace .. " "
		end
		print(sSpace .. sChapterTitle)
		
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

local	img_id				= 3000
local	temporary_file_list	= {}
function GenerateFigure(sFileName, fRatio)
	local cx			= 0
	local cy			= 0
	local width			= 0
	local height		= 0
	sFileName			= String(sFileName)
	local sPageName		= ""	-- for Visio/Excel
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

		if sFileName:CompareBack(".vsd") or sFileName:CompareBack(".vsdx") then
			-- convert visio to svg
			os.execute("vsd2svg \"" .. sFileName.s .. "\" \"" .. sPageName .. "\"")
			
			if #sPageName ~= 0 then
				sFileName:Append("." .. sPageName)
			end
			sFileName:Append(".svg")
			
			do	-- whitespace bug fix. convert " " to "_"
				local sFixedFileName = String(sFileName.s)
				sFixedFileName:CutFront("\\", true)
				sFixedFileName:CutFront("/", true)
				sFixedFileName:Replace(" ", "_", true)
				sFileName:erase(#sFileName.s - #sFixedFileName.s, -1);
				sFileName:Append(sFixedFileName.s)
			end
			
			if lfs.attributes(sFileName.s) == false then	-- no visio
				LOGE("Visio is not installed.")
				sFileName.s	= docgen.profile_path.s .. "common/bin/codegen/media/no_visio.svg"
			else
				temporary_file_list[#temporary_file_list + 1]		= sFileName.s	-- clean up list.
			end
		elseif sFileName:CompareBack(".xls") or sFileName:CompareBack(".xlsx") then
			-- convert Excel chart to svg
			if #sPageName == 0 then
				error("Excel sheet name of chart must be existed.")
			end
			
			os.execute("xlsx_chart \"" .. sFileName.s .. "\" \"" .. sPageName .. "\"")
			sPageName	= String(sPageName)
			sPageName:Replace(":", ".", true)
			sFileName:Append("." .. sPageName.s .. ".svg")
			do	-- whitespace bug fix. convert " " to "_"
				local sFixedFileName = String(sFileName.s)
				sFixedFileName:CutFront("\\", true)
				sFixedFileName:CutFront("/", true)
				sFixedFileName:Replace(" ", "_", true)
				sFileName:erase(#sFileName.s - #sFixedFileName.s, -1);
				sFileName:Append(sFixedFileName.s)
			end
			
			if lfs.attributes(sFileName.s) then	-- existed
				temporary_file_list[#temporary_file_list + 1]		= sFileName.s	-- clean up list.
			end
		end
	end
	
	sFileName	= sFileName.s
	
	local sExt			= String(sFileName)
	local sID			= docgen.doc:AddMedia(sFileName)
	local sExtraExt		= ""
	local sBlipAttr		= ""
	
	sExt:CutFront(".", true)
	sExt:MakeLower()
	sExt		= sExt.s
	
	do	-- get image width & height
		local sInfo		= String((sExt == "wmf") and exec("magick identify \"" .. sFileName .. "\"") or exec("magick identify -size x \"" .. sFileName .. "\""))

		sInfo:CutBack("sRGB", false)
		sInfo:CutFront(".", true)
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
			error("[ImageMagick] Invalid image : \"" .. sFileName .. "\"")
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
	
	if xls:Open(sExcelFileName, true) == false then
		error("Can't open excel file : " .. sExcelFileName)
	end
	
	-- 시트 열기
	local	sheet			= xls:GetSheet(sSheetName)
	local	col_count		= 0
	local	col_total_width	= 0
	local	col_cells		= {}
	local	col_width		= {}
	
	if sheet == nil then
		error("Can't open sheet table : " .. sExcelFileName .. "(" .. sSheetName .. ")")
	end
	
	local function GetTableCellData(sheet, bGetWidth)
		local col	= {}
		col.text	= sheet:GetValue()
		if bGetWidth == true then
			col.width	= sheet:GetColumnWidth()
		end
		col.align	= ""
		do
			local sMerge		= String(sheet:GetMergeCellPos())
			col.merge			= {}
			col.merge.doc		= ""
			col.merge.enable	= (#sMerge.s ~= 0)
			col.merge.downed	= false
			-- make doc text
		
			if col.merge.enable then
				col.merge.x			= tonumber(sMerge:Tokenize(",").s)
				col.merge.y			= tonumber(sMerge:Tokenize(",").s)
				col.merge.width		= tonumber(sMerge:Tokenize(",").s)
				col.merge.height	= tonumber(sMerge:Tokenize(",").s)
			
				-- 가로는 span 으로 세로는 vMerge 로 기술한다.
				if col.merge.width ~= 1 then
					col.merge.doc = col.merge.doc .. "<w:gridSpan w:val=\"" .. col.merge.width .. "\"/>"
					if col.merge.x ~= 0 then
						col.merge.downed	= true
					end
				end
				
				if col.merge.height ~= 1 then
					col.merge.doc = col.merge.doc .. "<w:vMerge "
					if col.merge.y == 0 then
						col.merge.doc = col.merge.doc .. "w:val=\"restart\""
					elseif col.merge.y ~= (col.merge.height - 1) then
						col.merge.doc = col.merge.doc .. "w:val=\"continue\""
					end
					col.merge.doc = col.merge.doc .. "/>"
				end
			else
				col.merge.x			= 0
				col.merge.y			= 0
				col.merge.width		= 0
				col.merge.height	= 0
			end
		end
		
		col.style				= sheet:GetStyle()
		
		-- for formated text color
		col.formated_color		= GetExcelColorCode(sheet:GetLatestValueColor())	-- formated color
		return col
	end
	
	-- 첫 줄 목록 얻기
	if sheet:GetRow(false) then
		while sheet:GetColumn(false) do
			col_count				= col_count + 1
			col_cells[col_count]	= GetTableCellData(sheet, true)
			col_total_width			= col_total_width + col_cells[col_count].width
		end

		-- col grid width 재계산
		for i=1, col_count do
			col_width[i]	= math.floor((col_cells[i].width * 10094) / col_total_width);
		end
	else
		LOGW("No table data. : " .. sExcelFileName .. "(" .. sSheetName .. ")")
		return
	end

	local	table_code	= String("\
	<w:tbl>\
		<w:tblPr>\
			<w:tblStyle w:val=\"ac\"/>\
			<w:tblW w:w=\"10094\"\
					w:type=\"auto\"/>\
			<w:tblBorders>\
				<w:top w:val=\"single\"\
					   w:sz=\"4\"\
					   w:space=\"0\"\
					   w:color=\"" .. docgen.boarder_color .. "\"\
					   w:themeColor=\"background2\"\
					   w:themeShade=\"BF\"/>\
				<w:left w:val=\"single\"\
						w:sz=\"4\"\
						w:space=\"0\"\
						w:color=\"" .. docgen.boarder_color .. "\"\
						w:themeColor=\"background2\"\
						w:themeShade=\"BF\"/>\
				<w:bottom w:val=\"single\"\
						  w:sz=\"4\"\
						  w:space=\"0\"\
						  w:color=\"" .. docgen.boarder_color .. "\"\
						  w:themeColor=\"background2\"\
						  w:themeShade=\"BF\"/>\
				<w:right w:val=\"single\"\
						 w:sz=\"4\"\
						 w:space=\"0\"\
						 w:color=\"" .. docgen.boarder_color .. "\"\
						 w:themeColor=\"background2\"\
						 w:themeShade=\"BF\"/>\
				<w:insideH w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"" .. docgen.boarder_color .. "\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
				<w:insideV w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"" .. docgen.boarder_color .. "\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
			</w:tblBorders>\
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
		table_code:Append("<w:gridCol w:w=\"" .. col_width[i] .. "\"/>")
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
		if col_cells[i].merge.downed == false then
			local cell_width = 0
			for t = 0, ((col_cells[i].merge.width == 0) and 0 or (col_cells[i].merge.width - 1)) do
				cell_width	= cell_width + col_width[i + t]
			end
		
			table_code:Append("\
				<w:tc>\
					<w:tcPr>\
						<w:tcW w:w=\"" .. cell_width .. "\"\
							   w:type=\"dxa\"/>" .. col_cells[i].merge.doc ..
						"<w:tcBorders>\
							<w:top w:val=\"single\"\
								   w:sz=\"4\"\
								   w:space=\"0\"\
								   w:color=\"" .. docgen.boarder_color .. "\"\
								   w:themeColor=\"background2\"\
								   w:themeShade=\"BF\"/>\
							<w:left w:val=\"single\"\
									w:sz=\"4\"\
									w:space=\"0\"\
									w:color=\"" .. docgen.boarder_color .. "\"\
									w:themeColor=\"background2\"\
									w:themeShade=\"BF\"/>\
							<w:bottom w:val=\"single\"\
									  w:sz=\"4\"\
									  w:space=\"0\"\
									  w:color=\"" .. docgen.boarder_color .. "\"\
									  w:themeColor=\"background2\"\
									  w:themeShade=\"BF\"/>\
							<w:right w:val=\"single\"\
									 w:sz=\"4\"\
									 w:space=\"0\"\
									 w:color=\"" .. docgen.boarder_color .. "\"\
									 w:themeColor=\"background2\"\
									 w:themeShade=\"BF\"/>\
						</w:tcBorders>\
						<w:shd w:val=\"clear\"\
							   w:color=\"auto\"\
							   w:fill=\"7F7F7F\"\
							   w:themeFill=\"text1\"\
							   w:themeFillTint=\"80\"/>\
					</w:tcPr>"
					.. EncodeParagraph(col_cells[i].text, {pPr="<w:pStyle w:val=\"TableColumn\"/>\
					<w:rPr>\
						<w:color w:val=\"FFFFFF\"\
								 w:themeColor=\"background1\"/>\
						<w:sz w:val=\"22\"/>\
						<w:szCs w:val=\"22\"/>\
					</w:rPr>", rPr="\
					<w:rFonts w:hint=\"eastAsia\"/>\
					<w:color w:val=\"FFFFFF\" w:themeColor=\"background1\"/>\
					<w:sz w:val=\"22\"/>\
					<w:szCs w:val=\"22\"/>", bDontIgnoreEmpty=true}) ..
				"</w:tc>"
			)
		end
	end
	table_code:Append("</w:tr>")
	
	-- 데이터 줄 얻기
	if sheet:GetRow(false) then
		local	bLast	= false
		-- 한 줄 읽기
		while bLast == false do
			for i=1, col_count do
				sheet:GetColumn(false)
				col_cells[i]			= GetTableCellData(sheet, false)
			end
			bLast	= (sheet:GetRow(false) == false)
			
			-- 한줄 채우기
			table_code:Append("\
			<w:tr>")
			-- 컬럼 채우기
			for i=1, col_count do
				if col_cells[i].merge.downed == false then
					local cell_width = 0
					for t = 0, (col_cells[i].merge.width - 1) do
						cell_width	= cell_width + col_width[i + t]
					end
			
					table_code:Append("\
					<w:tc>\
						<w:tcPr>\
							<w:tcW w:w=\"" .. cell_width .. "\" w:type=\"dxa\"/>")
				
					table_code:Append(col_cells[i].merge.doc)
					
					table_code:Append("\
							<w:tcBorders>\
								<w:top w:val=\"single\"\
									   w:sz=\"4\"\
									   w:space=\"0\"\
									   w:color=\"" .. docgen.boarder_color .. "\"\
									   w:themeColor=\"background2\"\
									   w:themeShade=\"BF\"/>"
					)
					
					-- 좌/우 경계선 비우기
					if i == 1 then	-- first column
						table_code:Append("<w:left w:val=\"nil\"/>")
					end
					if i == (col_count - ((col_cells[i].merge.width ~= 0) and (col_cells[i].merge.width-1) or 0)) then	-- last column
						table_code:Append("<w:right w:val=\"nil\"/>")
					end
					
					local back_color_doc	= ""
					-- set horizontal alignment
					local cell_alignment	= "TableTextLeft"
					
					if col_cells[i].style ~= nil then
						if col_cells[i].style:AlignmentHorizontal() == "center"  then
							cell_alignment		= "TableTextCenter"
						elseif col_cells[i].style:AlignmentHorizontal() == "right" then
							cell_alignment		= "TableTextRight"
						end

						-- background color
						local	cell_backcolor = col_cells[i].style:BackgroundColor()
						
						if #cell_backcolor ~= 0 then
							back_color_doc = "\
							<w:shd w:val=\"clear\"\
						       w:color=\"auto\"\
						       w:fill=\"" .. cell_backcolor .. "\"/>"
						end
					elseif col_cells[i][3] then
						cell_alignment		= "TableTextCenter"
					end
					
					local color_field	= ""
					
					if #col_cells[i].formated_color ~= 0 then	-- color 설정
						color_field		= "<w:color w:val=\"" .. col_cells[i].formated_color .. "\"/>"
					end
					
					table_code:Append("\
							</w:tcBorders>" .. back_color_doc ..
						"</w:tcPr>"
						.. EncodeParagraph(col_cells[i].text,
						{
							pPr=("<w:pStyle w:val=\"" .. cell_alignment .. "\"/>"),
							rPr="<w:rFonts w:hint=\"eastAsia\"/>" .. color_field,
							bDontIgnoreEmpty=true
						}) .. 
					"</w:tc>")
				end
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
			<w:tblW w:w=\"HeaderWidth\"\
					w:type=\"auto\"/>\
			<w:tblBorders>\
				<w:top w:val=\"single\"\
					   w:sz=\"4\"\
					   w:space=\"0\"\
					   w:color=\"" .. docgen.boarder_color .. "\"\
					   w:themeColor=\"background2\"\
					   w:themeShade=\"BF\"/>\
				<w:left w:val=\"single\"\
						w:sz=\"4\"\
						w:space=\"0\"\
						w:color=\"" .. docgen.boarder_color .. "\"\
						w:themeColor=\"background2\"\
						w:themeShade=\"BF\"/>\
				<w:bottom w:val=\"single\"\
						  w:sz=\"4\"\
						  w:space=\"0\"\
						  w:color=\"" .. docgen.boarder_color .. "\"\
						  w:themeColor=\"background2\"\
						  w:themeShade=\"BF\"/>\
				<w:right w:val=\"single\"\
						 w:sz=\"4\"\
						 w:space=\"0\"\
						 w:color=\"" .. docgen.boarder_color .. "\"\
						 w:themeColor=\"background2\"\
						 w:themeShade=\"BF\"/>\
				<w:insideH w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"" .. docgen.boarder_color .. "\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
				<w:insideV w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"" .. docgen.boarder_color .. "\"\
						   w:themeColor=\"background2\"\
						   w:themeShade=\"BF\"/>\
			</w:tblBorders>\
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
		local	HeaderWidth		= 10094
		
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
								   w:color=\"" .. docgen.boarder_color .. "\"\
								   w:themeColor=\"background2\"\
								   w:themeShade=\"BF\"/>\
							<w:left w:val=\"single\"\
									w:sz=\"4\"\
									w:space=\"0\"\
									w:color=\"" .. docgen.boarder_color .. "\"\
									w:themeColor=\"background2\"\
									w:themeShade=\"BF\"/>\
							<w:bottom w:val=\"single\"\
									  w:sz=\"4\"\
									  w:space=\"0\"\
									  w:color=\"" .. docgen.boarder_color .. "\"\
									  w:themeColor=\"background2\"\
									  w:themeShade=\"BF\"/>\
							<w:right w:val=\"single\"\
									 w:sz=\"4\"\
									 w:space=\"0\"\
									 w:color=\"" .. docgen.boarder_color .. "\"\
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
									   w:color=\"" .. docgen.boarder_color .. "\"\
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
			error("No table data in Lua variable : " .. sLuaTable)
		end
	end

	table_code:Append("</w:tbl>")

	return table_code.s
end

local function GenerateHighlightedCodes(sCodeFormat, sContent, bLine)
	local txt = TextFile()
	if txt:Create(".docgen_code_highlight.tmp") then
	else
		error("Create temporary file is failed.")
	end
	txt:Put(sContent)
	txt:Close()
	local sResult	= exec("code_highlighter --ilang=" .. sCodeFormat .. " --olang=docgen " .. (bLine and "-n " or "") .. ".docgen_code_highlight.tmp")
	exec("rm -f .docgen_code_highlight.tmp")
	
	return sResult
end

ReadText = function(sFile)
	local	txt	= TextFile()
	if txt:Open(sFile) == false then
		error("Text file '" .. sFile .. ". is not existed.")
	end
	return txt:GetAll(false)
end

local	bInline		= false

function EncodeParagraph(sText, sExtra)
	local	sPara			= String(sText)
	local	sResult			= String("")
	local	bBypass			= false				-- 내용 무시, docgen.language 일치하지 않음
	local	bBypassCodeRef	= false				-- code reference in bypass mode
	
	if sPara:CompareFront("[[") and sPara:CompareBack("]]") then
		sPara:erase(0,2)
		sPara:CutBack("]]", false)
		sPara:ChangeCharsetToANSI()
		local	txt	= TextFile()
		if txt:Open(sPara.s) == false then
			error("Text file '" .. sPara.s .. ". is not existed.")
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

	-- empty paragraph, but do not ignore
	if sExtra ~= nil then
		if (#sPara.s == 0) and sExtra.bDontIgnoreEmpty then
			sResult:Append("<w:p>")
			sResult:Append("<w:rPr>")
			if sExtra.pPr ~= nil then
				sResult:Append(sExtra.pPr)
			end
			sResult:Append("</w:rPr>")
			sResult:Append("<w:r>")
			sResult:Append("<w:rPr>")
			if sExtra.rPr ~= nil then
				sResult:Append(sExtra.rPr)
			end
			sResult:Append("</w:rPr>")
			sResult:Append("</w:r>")
			sResult:Append("</w:p>")
			return sResult.s
		end
	end

	while true do
		local	sLine	= sPara:Tokenize("\r\n")	-- enter code process
		local	s_pPr	= ""

		if sPara.TokenizePos < 0 then
			break
		end
		
		if bBypass and sLine:CompareFront("```") then	-- check in code reference
			bBypassCodeRef	= (bBypassCodeRef == false)
			if bBypass then
				goto continue
			end
		end
		
		if bBypass and ((sLine:CompareFront("%%%") == false) or bBypassCodeRef) then	-- bypass : language is not matching
			goto continue
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
				local	bScript	= false
				sResult:CutBack("<w:p>", false)
				-- code type 얻기
				sLine:CutFront("`", true)
				sLine:Trim(" ")
				sLine:Replace(" ", "", true)	-- 내부 space 모두 제거 "[lua]" 구별을 위해
				sLine:MakeLower()
				if sLine:CompareFront("#") then
					bLine	= true
					sLine:erase(0,1)
				else
					bScript		= sLine:CompareFront("[") and sLine:CompareBack("]")	-- highlight contents 가 아닌 진짜 코드임.
					
					if bScript then
						sLine:DeleteFront("[")
						sLine:DeleteBack("]")
					end
				end
				
				local	sCodeFormat	= sLine.s
				local	sIndent		= ""
				-- cut off front
				sPara:erase(0, sPara.TokenizePos)
				sPara.TokenizePos	= 0
				-- get code length
				local	iCodeLen	= 0
				while sPara.TokenizePos >= 0 do
					sLine	= sPara:Tokenize("\r\n")
					if sLine:CompareFront("```") then
						sLine:Trim(" \t")
						if sLine.s ~= "```" then
							error("Not a end of code reference.")
						end
						break
					end
					iCodeLen	= sPara.TokenizePos
				end
				-- make contents
				local	sContent	= String(sPara.s)
				if iCodeLen == 0 then	-- no contents
					goto continue
				else
					if iCodeLen < 0 then
						error("Not found of end of code reference \"```\".")
					end
					sContent:erase(iCodeLen, -1)
					if bScript == false then
						sContent:Replace("\r", "", true)
						sContent:Replace("\n\n", "\n \n", true)
						sContent:TrimLeft("\n")
						sContent:TrimRight(" \n")
						sContent:Replace("@```", "```", true)
					end
				end
				
				if bScript then
					if sCodeFormat == "lua" then
						if RunString(sContent.s, "inline") == false then
							error("Got error on inline lua script.")
						end
					else
						error("Can't execute '" .. sCodeFormat .. "' script.")
					end
				else
					do	-- make highlight contents
						sContent = GenerateHighlightedCodes(sCodeFormat, sContent.s, bLine)
						
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
							<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F7F7F7\"/>\
							<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F7F7F7\"/>\
							<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F7F7F7\"/>\
							<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"F7F7F7\"/>\
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
								<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"F8F8F8\" w:themeFill=\"background1\" w:themeFillShade=\"F8\"/>\
							</w:tcPr>"
					.. EncodeParagraph(sContent, {pPr=("<w:jc w:val=\"left\"/><w:spacing w:after=\"0\"/>" .. sIndent), rPr="<w:rFonts w:ascii=\"" .. docgen.fixed_font .. "\" w:eastAsia=\"" .. docgen.fixed_font .. "\" w:hAnsi=\"" .. docgen.fixed_font .. "\"/>"}) ..
					"</w:tc></w:tr></w:tbl>"
					)
					bInline	= false
				end
				goto continue
			elseif sLine:CompareFront(";;;") then
				sLine:CutFront(";", true)
				sResult:Append("<w:br w:type=\"page\"/>")
			elseif sLine:CompareFront(":::") then
				sLine:CutFront(":", true)
				sLine:CutBack("\t", true)
				sLine:CutBack("//", true)
				sLine:CutBack("--", true)
				sLine:CutBack(";", true)
				sLine:Trim(" \t")
				s_pPr	= String(s_pPr)
				s_pPr:DeleteBlock("<w:pStyle*/>", 0)
				s_pPr:Append("<w:pStyle w:val=\"" .. ConvertStyleIDString(sLine.s) .. "\"/>")
				s_pPr	= s_pPr.s
				sLine	= sPara:Tokenize("\r\n")
				goto new_line
			elseif sLine:CompareFront("%%%") then
				sLine:CutFront("%", true)
				sLine:Trim(" \t%-=")
				sResult:CutBack("<w:p>", false)

				if #sLine.s ~= 0 then
					if sLine:CompareFront("(") then
						if sLine:CompareBack(")") == false then
							error("The ')' symbol is required at the end of the Lua expression : '" .. sLine.s .. "'")
						end
						
						local	ReturnCode	= load("return (" .. sLine.s .. ")")()

						if ReturnCode == nil then
							bBypass	= true
						else
							bBypass	= (ReturnCode == false)
						end
					else
						bBypass	= (sLine.s ~= docgen.language)
					end
				else
					bBypass	= false
				end
				goto continue
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
						s_rPr	= s_rPr .. "<w:rFonts w:ascii=\"" .. docgen.fixed_font .. "\" w:eastAsia=\"" .. docgen.fixed_font .. "\" w:hAnsi=\"" .. docgen.fixed_font .. "\"/>"
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
							if sFileName.s ~= "nil" then
								sResult:Append(GenerateFigure(sFileName.s, fRatio))
							end
							
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
							
							if sMainName.s ~= "nil" then
								if sMainName.s == "lua" then
									sResult:Append(GenerateTableFromLua(sSubName.s))
								else
									sResult:Append(GenerateTable(sMainName.s, sSubName.s))	-- file_name, sheet_name
								end
							end
							goto continue
						elseif sTag.s == "code" then	-- inline code block
							if bSet then
								local	sCodeFormat	= sVar:Tokenize(":; ")
								sCodeFormat:MakeLower()
								sCodeFormat			= sCodeFormat.s
								if #sCodeFormat == 0 then
									sCodeFormat			= docgen.code_format	-- override with default code format
								end
								
								sLine:erase(0, sLine.TokenizePos)
								
								local	sContents	= String(sLine.s)
								sContents:CutBack("@</code>", true)
								
								sLine:CutFront("@</code>", false)
								
								sContents:Trim(" \r\n\t")
								
								sContents.s = GenerateHighlightedCodes(sCodeFormat, sContents.s, false)
								
								sLine:insert(sLine.TokenizePos, "@<fixed>@<bgcolor:" .. docgen.code_bgcolor .. ">" .. sContents.s .. "@</bgcolor>@</fixed>")
							else
								error("invalid use '</code>'")
							end
						elseif sTag.s == "property" then
							local	sName	= sVar:Tokenize("")
							sName:TrimLeft(" :")
							sName:TrimRight(" ")
							do
								local sProperty = property[sName.s]
								if sProperty == nil then
									LOGW("property[" .. sName.s .. "] is not found.")
									sProperty	= ""
								end
								sLine:insert(sLine.TokenizePos, sProperty)
							end
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
							sResult:Append("<w:hyperlink r:id=\"" .. docgen.doc:AddHyperlink(sLink.s) .. "\"><w:r>\
									<w:rPr><w:rStyle w:val=\"af0\"/></w:rPr>\
									<w:t>"  .. sComment.s ..  "</w:t>\
								</w:r></w:hyperlink>")
						elseif sTag.s == "lua" then
							local	sLuaCode	= sVar:Tokenize("")
							sLuaCode:TrimLeft(" :")
							
							local	ReturnCode	= load("return (" .. sLuaCode.s .. ")")()
							
							if type(ReturnCode) == "string" then
								sLine:insert(sLine.TokenizePos, ReturnCode)
							else
								sLine:insert(sLine.TokenizePos, tostring(ReturnCode))
							end
						else
							error("Can't recognize paragraph command : " .. sTag.s)
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

AddPageBreak = function()
	docgen.doc_body:AddChildBeforeFromBuffer(doc_last,"\
		<w:p>\
			<w:r>\
				<w:br w:type=\"page\"/>\
			</w:r>\
		</w:p>")
end

AddParagraph = function(content)
	docgen.doc_body:AddChildBeforeFromBuffer(doc_last, EncodeParagraph(content, {pPr="<w:pStyle w:val=\"BodyText10\"/>"}))
end

AddTable = function(sCaption, sExcelFileName, sSheetName)
	if sCaption ~= nil and sCaption ~= "" then
		docgen.doc_body:AddChildBeforeFromBuffer(doc_last, GenerateCaption("Table", sCaption))
	end
	
	docgen.doc_body:AddChildBeforeFromBuffer(doc_last, GenerateTable(sExcelFileName, sSheetName))
end

AddSubDocument = function(sDocFileName)
	local	id	= docgen.doc:AddSubDocument(sDocFileName)
	docgen.doc_body:AddChildBeforeFromBuffer(doc_last, "\
		<w:p>\
			<w:subDoc r:id=\"" .. id .. "\"/>\
		</w:p>"
	)
end

-- 사용자 소스 실행
do
	local __sExt	= String(docgen.sInFilename)
	
	__sExt:CutFront(".", true)
	__sExt:MakeLower()
	
	if __sExt.s == "md" then
		AddParagraph("[[" .. docgen.sInFilename .. "]]")
	else
		if RunScript(docgen.sInFilename) == false then
			return
		end
	end
end

-- 속성 갱신
for key, value in pairs(property) do
	if key ~= "Water_Mark" then
		docgen.doc:SetProperty(key, value)
	end
end


-- 테이블/그림/Terms 목차 없을시 제거
local function DeleteDocSection(sPara)
	local node = docgen.doc_body:child_by_text("w:p", "w:t", sPara)

	if node:empty() then
		error("Can't find page paragraph : '" .. sPara .. "'")
	else
		-- search to top paragraph
		while node:child_in_depth("w:lastRenderedPageBreak", nil):empty() do
			node	= node:previous_sibling(nil)
			if node:empty() then
				error("It's first page paragraph. : " .. sPara)
				break;
			end
		end

		-- delete first paragraph (because it includes a page break code)
		node:Destroy(1)
		
		-- delete paragraph until page break
		while node:child_in_depth("w:lastRenderedPageBreak", nil):empty() do
			if node:Destroy(1) == false then
				error("It's last page paragraph. : " .. sPara)
				break;
			end
		end
	end
end

if docgen.revision.count == 0 then
	DeleteDocSection(docgen.revision.title)
end

if table_count == 0 then
	DeleteDocSection("List of Tables")
end

if figure_count == 0 then
	DeleteDocSection("List of Figures")
end

if docgen.terms.count == 0 then
	DeleteDocSection(docgen.terms.title)
end

-- 북마크 갱신
LOGI("Link all bookmarks.")
while true do
	local	bookmark_node	= docgen.doc_body:child_in_depth("w:instrText", "BOOKMARK")
	
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
		error("bookmark \"" .. sTarget.s .. "\" is not found.")
	end
	
	bookmark_text:Destroy(1)
	
	bookmark_node:set_string(" " .. (bPage and "PAGEREF" or "REF") .. " _Ref" .. bookmark_num .. (bNumber and " \\w" or "") .. " \\h ")
end

-- 파일 저장
do
	-- 저장 이름 없을 시 생성
	if #docgen.sOutFilename == 0 then
		if(#property["IP_Name_Header"] > 0) then
			docgen.sOutFilename	= docgen.sOutFilename .. property["IP_Name_Header"] .. "_"
		end

		if(#property["Document_Name_Header"] > 0) then
			docgen.sOutFilename	= docgen.sOutFilename .. property["Document_Name_Header"] .. "_"
		end

		if(#property["Doc_Version"] > 0) then
			docgen.sOutFilename	= docgen.sOutFilename .. "rev" .. property["Doc_Version"]
		end
		
		do	-- cut last '_' charactor
			local s = String(docgen.sOutFilename)
			s:Replace(" ", "_", true)
			s:TrimRight("_")
			docgen.sOutFilename = s.s
		end
		
		if(#docgen.sOutFilename == 0)  then
			docgen.sOutFilename = "untitled_document"
		end
		
		docgen.sOutFilename	= docgen.sOutFilename .. ".docx"
	end

	docgen.sOutFilename	= String(docgen.sOutFilename)
	docgen.sOutFilename:Trim(" \"")

	if docgen.sOutFilename:CompareBack(".docx") == false then
		docgen.sOutFilename:Append(".docx")
	end

	local sOutFilename_PDF	= String(docgen.sOutFilename.s)
	sOutFilename_PDF:CutBack(".docx", false)
	sOutFilename_PDF:Append(".pdf")
		
	-- 기존 doc/pdf 파일 제거
	if os.execute("rm -f \"" .. docgen.sOutFilename.s .. "\"") == false then
		error("file '" .. docgen.sOutFilename.s .. "' is locked.")
	end
	if os.execute("rm -f \"" .. sOutFilename_PDF.s .. "\"") == false then
		error("file \"" .. sOutFilename_PDF.s .. "\" is locked.")
	end
	
	-- 결과 저장
	LOGI("Build document : " .. docgen.sOutFilename.s)
	if docgen.doc:Save(docgen.sOutFilename.s) == false then
		error("Can't create : " .. docgen.sOutFilename.s)
	end
	docgen.doc:Close()
	
	-- 중간 생성 파일들 제거
	for i=1, #temporary_file_list do
		os.execute("rm -f \"" .. temporary_file_list[i] .. "\"")
	end	
	
	if docgen.output_format == nil then
		LOGI("Fields calculation...")
		os.execute("doc2pdf \"" .. docgen.sOutFilename.s .. "\" \"" .. property["Water_Mark"] .. "\" *")
	else
		-- make pdf first
		if (docgen.output_format["pdf"] == true) or (docgen.output_format["djvu"] == true) then
			if docgen.output_format["pdf"] == true then
				LOGI("Save as 'pdf' file...")
			else
				LOGI("Fields calculation...")
			end
			docgen.output_format["pdf"]	= nil
			os.execute("doc2pdf \"" .. docgen.sOutFilename.s .. "\" \"" .. property["Water_Mark"] .. "\"")
		end
		-- the others save as
		for ext, bOut in key_pairs(docgen.output_format) do
			if bOut then
				if ext == "pdf" then
				elseif ext == "djvu" then
					local sDJVU_filename	= String(sOutFilename_PDF.s)
					sDJVU_filename:DeleteBack("pdf")
					sDJVU_filename:Append("djvu")
					LOGI("Save as '" .. ext .. "' file...")
					exec("pdf2djvu -j 2 -d 600 \"" .. sOutFilename_PDF.s .. "\" -o \"" .. sDJVU_filename.s .. "\"")	-- 600 dpi output
					if docgen.output_format["pdf"] ~= true then
						exec("rm -f \"" .. sOutFilename_PDF.s .. "\"")
					end
				else
					LOGI("Save as '" .. ext .. "' file...")
					os.execute("doc2save \"" .. docgen.sOutFilename.s .. "\" " .. ext)
				end
			end
		end
	end
end
