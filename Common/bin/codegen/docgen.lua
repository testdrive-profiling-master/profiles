RunScript("codegen_utils")

---------------------------------------------------------------------------------
-- Initialize configuration
---------------------------------------------------------------------------------
docgen							= {}
docgen.contents_only			= false					-- only contents, delete all first page, table list...
docgen.code_format				= "cpp"					-- default code format
docgen.code_bgcolor				= "F7F7F7"				-- code background color
docgen.code_header_bgcolor		= "EDEDED"				-- code header color
docgen.boarder_color			= "AEAAAA"				-- table's boarder color
docgen.fixed_font				= "Cascadia Mono"		-- fixed font name
docgen.profile_path				= String()
docgen.template_path			= String()
docgen.profile_path:GetEnvironment("TESTDRIVE_PROFILE")
docgen.template_path.s			= docgen.profile_path.s
docgen.max_console_chars		= 110
docgen.style_default			= "BodyText10"

docgen.table_header				= {}
docgen.table_header.text_color	= "FFFFFF"				-- table's header text color
docgen.table_header.bgcolor		= "808080"				-- table's background color
docgen.table_header.height		= 0						-- default auto

docgen.table_content			= {}
docgen.table_content.height		= 0						-- default auto
docgen.table_content.scale		= 1.0					-- table scale

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
				sDesc:Replace("\r", "", true)
				sDesc:CutBack("\n", true)
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
local	Arg				= ArgTable("Document Generator for TestDrive Profiling Master. v1.12")

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

-- Run Document dependent lua code
do
	local sExtraLuaFile = String(docgen.template_path.s)
	sExtraLuaFile:DeleteBack(".docx")
	sExtraLuaFile:Append(".lua")

	if lfs.IsExist(sExtraLuaFile.s) then	-- existed
		RunScript(sExtraLuaFile.s)
	end
end

---------------------------------------------------------------------------------
-- new document
---------------------------------------------------------------------------------
-- 문서 템플릿 열기
docgen.doc	 	= DocWord()

if docgen.doc:Open(docgen.template_path.s) then
	LOGI("Current template document(" .. docgen.sDocTemplate.s .. ") path.\n    - " .. docgen.template_path.s)
else
	LOGE("Invalid template document : " .. docgen.sDocTemplate.s)
	os.exit(1)		-- template document is not existed
end

do
	local	sDate = String()
	docgen.property								= {}
	property									= docgen.property	-- deprecated : 1년간 유지할 예정임.(~2025/7)
	sDate:FormatDate("%Y", 0)
	docgen.property["Year"]						= sDate.s		-- 라이센스 연도 반영
	docgen.property["Document_Name"]			= ""
	sDate:FormatDate("%B %d, %Y", 0)
	docgen.property["Revision_Date"]			= sDate.s
	docgen.property["Doc_Version"]				= ""
	docgen.property["Main_Title"]				= ""
	docgen.property["Sub_Title"]				= ""
	docgen.property["IP_Name_Header"]			= ""
	docgen.property["Ownership"]				= ""
	docgen.property["Document_Name_Header"]		= ""
	docgen.property["Water_Mark"]				= ""			-- 지정할 경우 워터마크가 overlay 된다.
end

-- Revision 추가 함수
docgen.doc_body						= docgen.doc:GetNode("word/document.xml", false):child("w:document"):child("w:body")
docgen.doc_styles					= docgen.doc:GetNode("word/styles.xml", false):child("w:styles")

-- page 속성
do
	local child	= docgen.doc_body:child_in_depth("w:pgSz", nil)
	if child:empty() == false then	-- page 크기
		docgen.page_size			= {}
		docgen.page_size.width		= tonumber(child:get_attribute("w:w"))
		docgen.page_size.height		= tonumber(child:get_attribute("w:h"))
	end
	child	= docgen.doc_body:child_in_depth("w:pgMar", nil)
	if child:empty() == false then	-- page 마진
		docgen.page_margin			= {}
		docgen.page_margin.top		= tonumber(child:get_attribute("w:top"))
		docgen.page_margin.right	= tonumber(child:get_attribute("w:right"))
		docgen.page_margin.bottom	= tonumber(child:get_attribute("w:bottom"))
		docgen.page_margin.left		= tonumber(child:get_attribute("w:left"))
		docgen.page_margin.header	= tonumber(child:get_attribute("w:header"))
		docgen.page_margin.footer	= tonumber(child:get_attribute("w:footer"))
		docgen.page_margin.gutter	= tonumber(child:get_attribute("w:gutter"))
	end
end

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
	["cyan"]	= 8,
	-- [undocumented color name below from this.]
	["Maroon"]	= 9,
	["puple"]	= 10,
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

docgen.color		= {}
docgen.color.db		= {
	["alice_blue"] = "F0F8FF",
	["antique_white"] = "FAEBD7",
	["aqua"] = "00FFFF",
	["aquamarine"] = "7FFFD4",
	["azure"] = "F0FFFF",
	["beige"] = "F5F5DC",
	["bisque"] = "FFE4C4",
	["black"] = "000000",
	["blanched_almond"] = "FFEBCD",
	["blue"] = "0000FF",
	["blue_violet"] = "8A2BE2",
	["brown"] = "A52A2A",
	["burly_wood"] = "DEB887",
	["cadet_blue"] = "5F9EA0",
	["chartreuse"] = "7FFF00",
	["chocolate"] = "D2691E",
	["coral"] = "FF7F50",
	["cornflower_blue"] = "6495ED",
	["cornsilk"] = "FFF8DC",
	["crimson"] = "DC143C",
	["cyan"] = "00FFFF",
	["dark_blue"] = "00008B",
	["dark_cyan"] = "008B8B",
	["dark_goldenrod"] = "B8860B",
	["dark_gray"] = "A9A9A9",
	["dark_green"] = "006400",
	["dark_khaki"] = "BDB76B",
	["dark_magenta"] = "8B008B",
	["dark_olive_green"] = "556B2F",
	["dark_orange"] = "FF8C00",
	["dark_orchid"] = "9932CC",
	["dark_red"] = "8B0000",
	["dark_salmon"] = "E9967A",
	["dark_sea_green"] = "8FBC8F",
	["dark_slate_blue"] = "483D8B",
	["dark_slate_gray"] = "2F4F4F",
	["dark_turquoise"] = "00CED1",
	["dark_violet"] = "9400D3",
	["deep_pink"] = "FF1493",
	["deep_sky_blue"] = "00BFFF",
	["dim_gray"] = "696969",
	["dodger_blue"] = "1E90FF",
	["firebrick"] = "B22222",
	["floral_white"] = "FFFAF0",
	["forest_green"] = "228B22",
	["fuchsia"] = "FF00FF",
	["gainsboro"] = "DCDCDC",
	["ghost_white"] = "F8F8FF",
	["gold"] = "FFD700",
	["goldenrod"] = "DAA520",
	["gray"] = "808080",
	["green"] = "008000",
	["green_yellow"] = "ADFF2F",
	["honeydew"] = "F0FFF0",
	["hot_pink"] = "FF69B4",
	["indian_red"] = "CD5C5C",
	["indigo"] = "4B0082",
	["ivory"] = "FFFFF0",
	["khaki"] = "F0E68C",
	["lavender"] = "E6E6FA",
	["lavender_blush"] = "FFF0F5",
	["lawn_green"] = "7CFC00",
	["lemon_chiffon"] = "FFFACD",
	["light_blue"] = "ADD8E6",
	["light_coral"] = "F08080",
	["light_cyan"] = "E0FFFF",
	["light_goldenrod_yellow"] = "FAFAD2",
	["light_gray"] = "D3D3D3",
	["light_green"] = "90EE90",
	["light_pink"] = "FFB6C1",
	["light_salmon"] = "FFA07A",
	["light_sea_green"] = "20B2AA",
	["light_sky_blue"] = "87CEFA",
	["light_slate_gray"] = "778899",
	["light_steel_blue"] = "B0C4DE",
	["light_yellow"] = "FFFFE0",
	["lime"] = "00FF00",
	["lime_green"] = "32CD32",
	["linen"] = "FAF0E6",
	["magenta"] = "FF00FF",
	["maroon"] = "800000",
	["medium_aquamarine"] = "66CDAA",
	["medium_blue"] = "0000CD",
	["medium_orchid"] = "BA55D3",
	["medium_purple"] = "9370DB",
	["medium_sea_green"] = "3CB371",
	["medium_slate_blue"] = "7B68EE",
	["medium_spring_green"] = "00FA9A",
	["medium_turquoise"] = "48D1CC",
	["medium_violet_red"] = "C71585",
	["midnight_blue"] = "191970",
	["mint_cream"] = "F5FFFA",
	["misty_rose"] = "FFE4E1",
	["moccasin"] = "FFE4B5",
	["navajo_white"] = "FFDEAD",
	["navy"] = "000080",
	["old_lace"] = "FDF5E6",
	["olive"] = "808000",
	["olive_drab"] = "6B8E23",
	["orange"] = "FFA500",
	["orange_red"] = "FF4500",
	["orchid"] = "DA70D6",
	["pale_goldenrod"] = "EEE8AA",
	["pale_green"] = "98FB98",
	["pale_turquoise"] = "AFEEEE",
	["pale_violet_red"] = "DB7093",
	["papaya_whip"] = "FFEFD5",
	["peach_puff"] = "FFDAB9",
	["peru"] = "CD853F",
	["pink"] = "FFC0CB",
	["plum"] = "DDA0DD",
	["powder_blue"] = "B0E0E6",
	["purple"] = "800080",
	["red"] = "FF0000",
	["rosy_brown"] = "BC8F8F",
	["royal_blue"] = "4169E1",
	["saddle_brown"] = "8B4513",
	["salmon"] = "FA8072",
	["sandy_brown"] = "F4A460",
	["sea_green"] = "2E8B57",
	["sea_shell"] = "FFF5EE",
	["sienna"] = "A0522D",
	["silver"] = "C0C0C0",
	["sky_blue"] = "87CEEB",
	["slate_blue"] = "6A5ACD",
	["slate_gray"] = "708090",
	["snow"] = "FFFAFA",
	["spring_green"] = "00FF7F",
	["steel_blue"] = "4682B4",
	["tan"] = "D2B48C",
	["teal"] = "008080",
	["thistle"] = "D8BFD8",
	["tomato"] = "FF6347",
	["turquoise"] = "40E0D0",
	["violet"] = "EE82EE",
	["wheat"] = "F5DEB3",
	["white"] = "FFFFFF",
	["white_smoke"] = "F5F5F5",
	["yellow"] = "FFFF00",
	["yellow_green"] = "9ACD32"
}
docgen.color.validate = function(s)
	if #s == 0 then
		print("")
		LOGW("Empty color format.\n")
		return s
	end
	local sColor	= docgen.color.db[s]
	
	if sColor == nil then
		local num	= tonumber(s,16)
		if num == nil then
			error("Not a color formated string : " .. s)
		end
		sColor = string.format("%06X", tonumber(s,16))
	end
	
	return sColor
end

RunScript("docgen/emoji")	-- support emoji functions

local table_wrapper	= {}	-- table 이나 figure 인용시 bookmark 와 함께 wrapper 시킴
table_wrapper.prefix	= "<w:tbl>\
<w:tblPr>\
	<w:tblStyle w:val=\"ac\"/>\
	<w:tblW w:w=\"5020\" w:type=\"pct\"/>\
	<w:tblBorders>\
		<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
		<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
		<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
		<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
		<w:insideH w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
		<w:insideV w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
	</w:tblBorders>\
	<w:tblLook w:val=\"04A0\" w:firstRow=\"1\" w:lastRow=\"0\" w:firstColumn=\"1\" w:lastColumn=\"0\" w:noHBand=\"0\" w:noVBand=\"1\"/>\
</w:tblPr>\
<w:tblGrid>\
	<w:gridCol w:w=\"9224\"/>\
</w:tblGrid>\
<w:tr>\
	<w:trPr>\
		<w:cantSplit/>\
	</w:trPr>\
	<w:tc>\
	<w:tcPr>\
		<w:tcW w:w=\"9224\" w:type=\"dxa\"/>\
		<w:tcBorders>\
			<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
			<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
			<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
			<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
			<w:insideH w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
			<w:insideV w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"auto\"/>\
		</w:tcBorders>\
		<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"auto\"/>\
	</w:tcPr>"
table_wrapper.postfix	= "</w:tc></w:tr></w:tbl>"

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

docgen.revision.insert	= function(sVer, iYear, iMonth, iDay, sDescription)
	docgen.property["Doc_Version"]		= sVer
	docgen.property["Revision_Date"]	= month_list[iMonth] .. " " .. tostring(iDay) .. ", " .. tostring(iYear)
	docgen.revision.tbl:AddChildAfterFromBuffer(docgen.revision.tbl_top, "<w:tr>\
				<w:tc>\
					<w:tcPr><w:tcW w:w=\"1985\" w:type=\"dxa\"/></w:tcPr>"
					.. EncodeParagraph(sVer, {pPr="<w:pStyle w:val=\"tabletext\"/><w:jc w:val=\"center\"/>", rPr="<w:color w:val=\"auto\"/>"}) ..
				"</w:tc>\
				<w:tc>\
					<w:tcPr><w:tcW w:w=\"2249\" w:type=\"dxa\"/></w:tcPr>"
					.. EncodeParagraph(docgen.property["Revision_Date"], {pPr="<w:pStyle w:val=\"tabletext\"/><w:jc w:val=\"center\"/>", rPr="<w:color w:val=\"auto\"/>"}) ..
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
	docgen.terms.tbl	= nil
end

docgen.terms.insert		= function(sTag, sDescription)
	if docgen.terms.tbl ~= nil then
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
	end

	docgen.terms.count	= docgen.terms.count + 1
end
AddTerm	= docgen.terms.insert

-- 문서 끝
docgen.doc_last			= docgen.doc_body:child_by_text("w:p", "w:t", "Contents on Here")
if docgen.doc_last:empty() then
	docgen.doc_last			= docgen.doc_body:last_child()	-- 문서 끝에 추가될 수 있도록
		do	-- delete latest "paragraph" : 빈 paragraph 를 제거한다.
		local	last_pr	= docgen.doc_last:previous_sibling("w:p")
		last_pr:Destroy(1)
	end
else
	docgen.doc_last:Destroy(1)	-- 가이드 포인트 'Contents on Here' 를 지운다.
end

docgen.doc_previous		= docgen.doc_last:previous_sibling()	-- contents 바로 앞 마지막 수식

-- 지역 변수들...
-- chapters
docgen.chapter			= {}
docgen.chapter[0]		= 0		-- first chapter ID
docgen.chapter.level	= 0
docgen.chapter.sid		= 0		-- bookmark reference id
-- tables
docgen.table			= {}
docgen.table.title		= "List of Tables"
docgen.table.count		= 0
docgen.table.id			= 0
-- figures
docgen.figure			= {}
docgen.figure.title		= "List of Figures"
docgen.figure.count		= 0
docgen.figure.id		= 0
-- bookmark
docgen.bookmark			= {}
docgen.bookmark.id		= 70	-- initial gap
docgen.bookmark.__list	= {}
docgen.bookmark.Set		= function (type_part, num_part, name, id)
	if docgen.bookmark.__list[name] ~= nil then
		LOGW("'" .. name .. "'(" .. docgen.bookmark.__list[name].type_part .. ") is duplicated with '" .. name .. "'(" .. type_part .. ").")
	end
	docgen.bookmark.__list[name]			= {}
	docgen.bookmark.__list[name].type_part	= type_part
	docgen.bookmark.__list[name].num_part	= num_part
	docgen.bookmark.__list[name].name		= name
	docgen.bookmark.__list[name].id			= id
end
docgen.bookmark.Get		= function (name)
	return docgen.bookmark.__list[name]
end
docgen.bookmark.always_bold		= false		-- bookmark 표현을 항상 굵은 글씨로 표현함.

function GenerateChapter(level, title)
	-- make log title
	local sChapterTitle = String(title)
	sChapterTitle:Trim(" \t")
	sChapterTitle:ChangeCharsetToUTF8()
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
	
	-- chapter level ID 값 증가 갱신
	if docgen.chapter.level ~= level then
		if docgen.chapter.level	< level then
			if (docgen.chapter.level + 1) ~= level then
				LOGW("The chapter's stage has risen abnormally.")
			end
			for i=(docgen.chapter.level+1),level do
				docgen.chapter[i] = 0
			end
		end
		if docgen.chapter[level] == nil then
			docgen.chapter[level]	= 0
		end
		
		docgen.chapter.level	= level
	end
	docgen.chapter[level]	= docgen.chapter[level] + 1
	
	-- chapter 번호 생성
	local	chapter_num			= ""
	for i=1,level do
		if i > 2 then
			chapter_num	= chapter_num .. "."
		end
		chapter_num	= chapter_num .. docgen.chapter[i]
		if i == 1 then
			chapter_num	= chapter_num .. "."
		end
	end
	
	-- 이전 출력 상태 지우기
	io.write((' '):rep(docgen.max_console_chars) .. "\r")
	
	if level == 1 then
		table_restart			= true
		docgen.chapter.sid		= 0
		docgen.table.id			= 1
		docgen.figure.id		= 1
		reference_id			= 70000000 + (docgen.chapter[1]*1000000)
		
		LOGI("@^0    " .. chapter_num .. " ")
		print(sChapterTitle)
		LOGI("@0")
	else
		local sSpace	= string.char(0x20):rep(level * 4)
		print(sSpace .. chapter_num .. " " .. sChapterTitle)
		
		reference_id	= 70000000 + (docgen.chapter[1]*1000000) + (docgen.chapter.sid*10)
	end
	
	reference_id		= 70000000 + (docgen.chapter[1]*1000000) + (docgen.chapter.sid*10)
	docgen.chapter.sid	= docgen.chapter.sid + 1
	
	docgen.bookmark.Set("Chapter", chapter_num, title, reference_id)
	
	local	sTitle	= "<w:p>\
			<w:pPr>\
				<w:pStyle w:val=\"" .. iStyleList[level] .. "\"/>\
				<w:rPr><w:rFonts w:cs=\"Arial\"/></w:rPr>\
			</w:pPr>\
			<w:bookmarkStart w:id=\"" .. docgen.bookmark.id .. "\" w:name=\"_Ref" .. reference_id .. "\"/>\
			<w:r>"
				.. ((level == 1) and "<w:lastRenderedPageBreak/>" or "<w:rPr><w:rFonts w:cs=\"Arial\" w:hint=\"eastAsia\"/></w:rPr>") ..
				"<w:t>" .. title .. "</w:t>\
			</w:r>\
			<w:bookmarkEnd w:id=\"" .. docgen.bookmark.id .. "\"/>\
		</w:p>"
	docgen.bookmark.id		= docgen.bookmark.id + 1
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
	
	local	sNumPart		= ""
	local	spPr			= ""	-- 앞부분 tab 문제를 제거
	
	if sType == "Table" then
		caption_id				= 20000000 + (docgen.chapter[1]*100000) + (docgen.table.id*10)
		sNumPart				= tostring(docgen.chapter[1]) .. "-" .. tostring(docgen.table.id)
		bFirst					= (docgen.table.id == 1)
		docgen.table.id			= docgen.table.id + 1
		docgen.table.count		= docgen.table.count + 1
		sID						= docgen.table.id
	else
		caption_id				= 30000000 + (docgen.chapter[1]*100000) + (docgen.figure.id*10)
		sNumPart				= tostring(docgen.chapter[1]) .. "-" .. tostring(docgen.figure.id)
		bFirst					= (docgen.figure.id == 1)
		docgen.figure.id		= docgen.figure.id + 1
		docgen.figure.count		= docgen.figure.count + 1
		sID						= docgen.figure.id
		spPr					= "<w:numPr><w:ilvl w:val=\"0\"/><w:numId w:val=\"0\"/></w:numPr>"
	end
	
	reference_id		= caption_id + 30000000
	docgen.bookmark.Set(sType, sNumPart, content, reference_id)

	local sXML = "<w:p>\
		<w:pPr>\
			<w:pStyle w:val=\"" .. sType .. "Caption\"/>" .. spPr ..
		"</w:pPr>\
		<w:bookmarkStart w:id=\"" .. tostring(docgen.bookmark.id) .. "\" w:name=\"_Toc" .. tostring(caption_id) .. "\"/>\
		<w:bookmarkStart w:id=\"" .. tostring(docgen.bookmark.id + 1) .. "\" w:name=\"_Toc" .. tostring(caption_id + 1) .. "\"/>\
		<w:bookmarkStart w:id=\"" .. tostring(docgen.bookmark.id + 2) .. "\" w:name=\"_Ref" .. tostring(reference_id) .. "\"/>\
		<w:r><w:t xml:space=\"preserve\">" .. sType .. " </w:t></w:r>\
		<w:r>\
			<w:fldChar w:fldCharType=\"begin\"/>\
			<w:instrText xml:space=\"preserve\"> STYLEREF  \\s Heading1,H1 </w:instrText>\
			<w:fldChar w:fldCharType=\"separate\"/>\
			<w:t>" .. docgen.chapter[1] .. "</w:t>\
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
		<w:bookmarkEnd w:id=\"" .. tostring(docgen.bookmark.id + 2) .. "\"/>\
		<w:r><w:t xml:space=\"preserve\">. </w:t></w:r>\
		<w:bookmarkEnd w:id=\"" .. tostring(docgen.bookmark.id) .. "\"/>\
		<w:r><w:t xml:space=\"preserve\">" .. content .. "</w:t></w:r>\
		<w:bookmarkEnd w:id=\"" .. tostring(docgen.bookmark.id + 1) .. "\"/>\
	</w:p>"
	docgen.bookmark.id		= docgen.bookmark.id + 3
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

		if sFileName:CompareBack(".vsd") or sFileName:CompareBack(".vsdx") then	-- visio
			if #sPageName == 0 then
				error("The page names in the visio file(" .. sFileName.s .. ") must be specified.")
			end
			
			if lfs.IsExist(sFileName.s) == false then
				error("File is not found : " .. sFileName.s)
			end
			
			-- convert visio to svg
			os.execute("vsd2svg \"" .. sFileName.s .. "\" \"" .. sPageName .. "\"")
			
			sFileName:Append("." .. sPageName .. ".svg")
			
			do	-- whitespace bug fix. convert " " to "_"
				local sFixedFileName = String(sFileName.s)
				sFixedFileName:CutFront("\\", true)
				sFixedFileName:CutFront("/", true)
				sFixedFileName:Replace(" ", "_", true)
				sFileName:erase(#sFileName.s - #sFixedFileName.s, -1);
				sFileName:Append(sFixedFileName.s)
			end
			
			if lfs.IsExist(sFileName.s) == false then	-- no visio
				error("Can't create .svg file from Visio file(" .. sFileName.s .. "[".. sPageName .. ").\nis not installed.Make sure Visio is installed properly.")
			end
			
			temporary_file_list[#temporary_file_list + 1]		= sFileName.s	-- clean up list.
		elseif sFileName:CompareBack(".odg") then	-- libreoffice draw
			if lfs.IsExist(sFileName.s) == false then
				error("File is not found : " .. sFileName.s)
			end
			
			local sOutFileName = String(sFileName.s)
			
			do	-- whitespace bug fix. convert " " to "_"
				local sFixedFileName = String(sFileName.s)
				sFixedFileName:CutFront("\\", true)
				sFixedFileName:CutFront("/", true)
				sOutFileName:erase(#sFileName.s - #sFixedFileName.s, -1);
				sOutFileName:Append(sFixedFileName.s .. "." .. sPageName .. ".svg")
				sOutFileName:Replace(" ", "_", true)
			end
			-- convert odg to svg
			if #sPageName == 0 then
				os.execute("odg2svg \"" .. sFileName.s .. "\" -o \"" .. sOutFileName.s .. "\"")
			else
				os.execute("odg2svg \"" .. sFileName.s .. "\" -p \"" .. sPageName .. "\" -o \"" .. sOutFileName.s .. "\"")
			end
			
			if lfs.IsExist(sOutFileName.s) == false then	-- no visio
				error("Can't create .svg file from OpenDocument file(" .. sFileName.s .. "[".. sPageName .. "]).\nMake sure LibreOffice is installed properly.")
			end
			
			sFileName.s = sOutFileName.s
			temporary_file_list[#temporary_file_list + 1]		= sFileName.s
		elseif sFileName:CompareBack(".drawio") then	-- drawio
			if lfs.IsExist(sFileName.s) == false then
				error("File is not found : " .. sFileName.s)
			end
			
			local sOutFileName = String(sFileName.s)
			
			do	-- whitespace bug fix. convert " " to "_"
				local sFixedFileName = String(sFileName.s)
				sFixedFileName:CutFront("\\", true)
				sFixedFileName:CutFront("/", true)
				sOutFileName:erase(#sFileName.s - #sFixedFileName.s, -1);
				sOutFileName:Append(sFixedFileName.s .. "." .. sPageName .. ".png")
				sOutFileName:Replace(" ", "_", true)
			end
			-- convert drawio to png
			if #sPageName == 0 then
				os.execute("drawio2png \"" .. sFileName.s .. "\" -o \"" .. sOutFileName.s .. "\"")
			else
				os.execute("drawio2png \"" .. sFileName.s .. "\" -p \"" .. sPageName .. "\" -o \"" .. sOutFileName.s .. "\"")
			end
			
			if lfs.IsExist(sOutFileName.s) == false then	-- no visio
				error("Can't create .png file from draw.io file(" .. sFileName.s .. "[".. sPageName .. "]).\nMake sure draw.io is installed properly.")
			end
			
			sFileName.s = sOutFileName.s
			temporary_file_list[#temporary_file_list + 1]		= sFileName.s
		elseif sFileName:CompareBack(".xls") or sFileName:CompareBack(".xlsx") then
			if #sPageName == 0 then
				error("Excel sheet name of chart must be existed.")
			end
			
			-- convert Excel chart to svg
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
			
			if lfs.IsExist(sFileName.s) then	-- existed
				temporary_file_list[#temporary_file_list + 1]		= sFileName.s	-- clean up list.
			end
		end
	else	-- advanced image file conversion
		if sFileName:CompareBack(".jxl") or sFileName:CompareBack(".bmp") or sFileName:CompareBack(".psd") or sFileName:CompareBack(".tiff") or sFileName:CompareBack(".pcx") or sFileName:CompareBack(".tga") or sFileName:CompareBack(".webp") then
			if lfs.IsExist(sFileName.s) == false then
				error("File is not found : " .. sFileName.s)
			end
			-- convert JXL image to PNG
			os.execute("magick \"" .. sFileName.s .. "\" \"" .. sFileName.s .. ".docgen.png\"")
			sFileName:Append(".docgen.png")
			temporary_file_list[#temporary_file_list + 1]		= sFileName.s	-- clean up list.
		end
	end
	
	if lfs.IsExist(sFileName.s) == false then
		error("File is not found : " .. sFileName.s)
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
	
	local function GetSheetValue(sheet)		-- table 내에서 오류를 일으킬 표현법 제거
		local s = String(sheet:GetValue())
		if s:CompareFront("#") then			-- 절표시 제거
			s:insert(0, "@")
		end
		s:Replace("\n#","\n@#", true)
		while s:CompareFront(";;;") do		-- 페이지 나눔 제거
			s:insert(0, "@")
		end
		s:Replace("\n;;;","\n@;;;", true)
		while s:CompareFront("---") do		-- 수평선 제거
			s:insert(0, "@")
		end
		s:Replace("\n---","\n@---", true)
		s:Replace("@<tbl:","<tbl:", true)	-- 테이블 안에 테이블 허용 안함
		return s.s
	end
	
	local function GetTableCellData(sheet, bGetWidth)
		local col	= {}
		col.text	= GetSheetValue(sheet)
		col.type	= sheet:GetLatestValueType()
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
	
	-- pane split 나누기 얻기
	local header_rows	= sheet:GetPanePosY()
	
	-- 테이블 너비
	local table_width	= math.floor(docgen.table_content.scale * 10094)
	

	-- 첫 줄 목록 얻기
	if sheet:GetRow(false) then
		while sheet:GetColumn(false) do
			col_count				= col_count + 1
			col_cells[col_count]	= GetTableCellData(sheet, true)
			col_total_width			= col_total_width + col_cells[col_count].width
		end

		-- col grid width 재계산
		for i=1, col_count do
			col_width[i]	= math.floor((col_cells[i].width * table_width) / col_total_width);
		end
	else
		LOGW("No table data. : " .. sExcelFileName .. "(" .. sSheetName .. ")")
		return
	end

	local	table_code	= String("\
	<w:tbl>\
		<w:tblPr>\
			<w:tblStyle w:val=\"ac\"/>\
			<w:tblW w:w=\"" .. table_width .. "\"\
					w:type=\"auto\"/>\
			<w:tblBorders>\
				<w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
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
				<w:trHeight w:val=\"" .. ((docgen.table_header.height == 0) and "auto" or tostring(docgen.table_header.height)) .. "\"/>\
				<w:tblHeader/>\
			</w:trPr>"
		)
		
	for i=1, col_count do
		if col_cells[i].merge.downed == false then
			local cell_width = 0
			for t = 0, ((col_cells[i].merge.width == 0) and 0 or (col_cells[i].merge.width - 1)) do
				cell_width	= cell_width + col_width[i + t]
			end

			local text_rotate_doc	= ""
			
			if col_cells[i].style ~= nil then
				-- text rotation
				local	cell_textRotation = col_cells[i].style:TextRotation()
				
				if cell_textRotation ~= 0 then
					if cell_textRotation == 90 then
						text_rotate_doc	= "<w:textDirection w:val=\"btLr\"/>"
					elseif cell_textRotation == -90 then
						text_rotate_doc	= "<w:textDirection w:val=\"tbRl\"/>"
					elseif cell_textRotation == 255 then	-- vertical (not identical...)
						text_rotate_doc	= "<w:textDirection w:val=\"tbRlV\"/>"
					else
						LOGW("Excel cell's custom text rotation(" .. cell_textRotation .. ") is not supported. (only 90 or -90 is supported.)")
					end
				end
			end

			table_code:Append("\
				<w:tc>\
					<w:tcPr>\
						<w:tcW w:w=\"" .. cell_width .. "\"\
							   w:type=\"dxa\"/>" .. col_cells[i].merge.doc ..
						"<w:tcBorders>\
							<w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
						</w:tcBorders>\
						<w:shd w:val=\"clear\"\
							   w:color=\"auto\"\
							   w:fill=\"" .. docgen.table_header.bgcolor.. "\"/>" .. text_rotate_doc ..
					"</w:tcPr>"
					.. EncodeParagraph(col_cells[i].text, {
						pPr="<w:pStyle w:val=\"TableColumn\"/>",
						text_color=docgen.table_header.text_color,
						bDontIgnoreEmpty=true}) ..
				"</w:tc>"
			)
		end
	end
	table_code:Append("</w:tr>")
	header_rows = header_rows - 1
	
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
			table_code:Append("<w:tr><w:trPr><w:trHeight w:val=\"" .. ((docgen.table_content.height == 0) and "auto" or tostring(docgen.table_content.height)) ..  "\"/>")
			local bHeader	= (header_rows > 0)
			header_rows		= header_rows - 1
			if bHeader then table_code:Append("<w:tblHeader/>") end
			table_code:Append("</w:trPr>")
			-- 컬럼 채우기
			for i=1, col_count do
				if col_cells[i].merge.downed == false then
					local cell_width = 0
					
					if col_cells[i].merge.enable then
						for t = 0, (col_cells[i].merge.width - 1) do
							cell_width	= cell_width + col_width[i + t]
						end
					else
						cell_width	= col_width[i]
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
									   w:color=\"" .. docgen.boarder_color .. "\"/>"
					)
					
					-- 좌/우 경계선 비우기
					if i == 1 then	-- first column
						table_code:Append("<w:left w:val=\"nil\"/>")
					elseif bHeader then
						table_code:Append("<w:left w:val=\"single\" w:color=\"" .. docgen.boarder_color .. "\"/>")
					end
					if i == (col_count - ((col_cells[i].merge.width ~= 0) and (col_cells[i].merge.width-1) or 0)) then	-- last column
						table_code:Append("<w:right w:val=\"nil\"/>")
					elseif bHeader then
						table_code:Append("<w:right w:val=\"single\" w:color=\"" .. docgen.boarder_color .. "\"/>")
					end
					
					if bHeader then
						table_code:Append("<w:bottom w:val=\"single\" w:color=\"" .. docgen.boarder_color .. "\"/>")
					end
					
					local back_color_doc	= ""
					local text_rotate_doc	= ""
					-- set horizontal alignment
					local cell_alignment	= (col_cells[i].type == "value") and "TableTextRight" or "TableTextLeft"
					
					if col_cells[i].style ~= nil then
						if col_cells[i].style:AlignmentHorizontal() == "center"  then
							cell_alignment		= "TableTextCenter"
						elseif col_cells[i].style:AlignmentHorizontal() == "right" then
							cell_alignment		= "TableTextRight"
						elseif col_cells[i].style:AlignmentHorizontal() == "left" then
							cell_alignment		= "TableTextLeft"
						end

						-- background color
						local	cell_backcolor = col_cells[i].style:BackgroundColor()
						
						if #cell_backcolor ~= 0 then
							back_color_doc = "\
							<w:shd w:val=\"clear\"\
						       w:color=\"auto\"\
						       w:fill=\"" .. cell_backcolor .. "\"/>"
						end
						
						-- text rotation
						local	cell_textRotation = col_cells[i].style:TextRotation()
						
						if cell_textRotation ~= 0 then
							if cell_textRotation == 90 then
								text_rotate_doc	= "<w:textDirection w:val=\"btLr\"/>"
							elseif cell_textRotation == -90 then
								text_rotate_doc	= "<w:textDirection w:val=\"tbRl\"/>"
							elseif cell_textRotation == 255 then	-- vertical (not identical...)
								text_rotate_doc	= "<w:textDirection w:val=\"tbRlV\"/>"
							else
								LOGW("Excel cell's custom text rotation(" .. cell_textRotation .. ") is not supported. (only 90 or -90 is supported.)")
							end
						end
					elseif col_cells[i][3] then
						cell_alignment		= "TableTextCenter"
					end
					
					local color_field	= ""
					
					if #col_cells[i].formated_color ~= 0 then	-- color 설정
						color_field		= "<w:color w:val=\"" .. col_cells[i].formated_color .. "\"/>"
					end
					
					table_code:Append("\
							</w:tcBorders>" .. back_color_doc .. text_rotate_doc ..
						"</w:tcPr>"
						.. EncodeParagraph(col_cells[i].text,
						{
							pPr="<w:pStyle w:val=\"" .. cell_alignment .. "\"/>",
							rPr=color_field,
							bDontIgnoreEmpty=true
						}) .. 
					"</w:tc>")
				end
			end
			table_code:Append("</w:tr>")
		end
	end
	table_code:Append("</w:tbl>")
	
	docgen.table_header.height		= 0
	docgen.table_content.height		= 0
	docgen.table_content.scale		= 1.0
	
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
					   w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:left w:val=\"single\"\
						w:sz=\"4\"\
						w:space=\"0\"\
						w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:bottom w:val=\"single\"\
						  w:sz=\"4\"\
						  w:space=\"0\"\
						  w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:right w:val=\"single\"\
						 w:sz=\"4\"\
						 w:space=\"0\"\
						 w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:insideH w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"" .. docgen.boarder_color .. "\"/>\
				<w:insideV w:val=\"single\"\
						   w:sz=\"4\"\
						   w:space=\"0\"\
						   w:color=\"" .. docgen.boarder_color .. "\"/>\
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
	
	local table_width	= math.floor(docgen.table_content.scale * 10094)

	-- 데이터 줄 얻기
	if #lua_table >= 1 then
		local	iRowCount		= 0
		local	HeaderCount		= 1
		local	HeaderWidth		= table_width
		
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
						<w:trHeight w:val=\"auto\"/>\
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
								   w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:left w:val=\"single\"\
									w:sz=\"4\"\
									w:space=\"0\"\
									w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:bottom w:val=\"single\"\
									  w:sz=\"4\"\
									  w:space=\"0\"\
									  w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:right w:val=\"single\"\
									 w:sz=\"4\"\
									 w:space=\"0\"\
									 w:color=\"" .. docgen.boarder_color .. "\"/>\
						</w:tcBorders>\
						<w:shd w:val=\"clear\"\
							   w:color=\"auto\"\
							   w:fill=\"" .. docgen.table_header.bgcolor .. "\"/>")
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
						table_code:Append(EncodeParagraph(col_string, {pPr="<w:pStyle w:val=\"TableColumn\"/>", rPr="\
						<w:rFonts w:hint=\"eastAsia\"/>\
						<w:color w:val=\"FFFFFF\" w:themeColor=\"background1\"/>"}))
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
	local sResult	= String(exec("code_highlighter --ilang=" .. sCodeFormat .. " --olang=docgen " .. (bLine and "-n " or "") .. ".docgen_code_highlight.tmp"))

	-- 줄 이어붙이기 막기 ('\' 로 끝나는 줄 없애기)
	sResult:Replace("\r", "", true)
	sResult:Replace("\\\n", "\\  \n", true)
	sResult = sResult.s
	
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

-- 한글 접미사 자동 수정
docgen.hangul					= {}
docgen.hangul.auto_suffix		= true
docgen.hangul.suffix_list		= {}
docgen.hangul.suffix_list[1]	= "을"
docgen.hangul.suffix_list[2]	= "를"
docgen.hangul.suffix_list[3]	= "은"
docgen.hangul.suffix_list[4]	= "는"
docgen.hangul.suffix_list[5]	= "이"
docgen.hangul.suffix_list[6]	= "가"
docgen.hangul.suffix_list[7]	= "과"
docgen.hangul.suffix_list[8]	= "와"
docgen.hangul.suffix_retrieve 	= function(s)
	for i, v in pairs (docgen.hangul.suffix_list) do
		if v == s then return i end
	end
	return -1
end
docgen.hangul.suffix_enable		= false
docgen.hangul.suffix_hint		= nil
docgen.hangul.suffix_bookmark	= nil
docgen.hangul.suffix_deferred	= {}		-- 나중을 위한 접미사 리스트
docgen.hangul.suffix_disable	= function ()
	docgen.hangul.suffix_enable	= false
end
docgen.hangul.postfix_set_hint	= function (s)
	if docgen.hangul.auto_suffix then
		docgen.hangul.suffix_hint		= s
		docgen.hangul.suffix_bookmark	= nil	
		docgen.hangul.suffix_enable		= true
	end
end
docgen.hangul.postfix_set_bookmark	= function (s)
	if docgen.hangul.auto_suffix then
		docgen.hangul.suffix_hint		= nil
		docgen.hangul.suffix_bookmark	= s	
		docgen.hangul.suffix_enable		= true
	end
end
-- ASCII 에서의 자음 접미사가 오는 것들 = 밭침이 없는 발음, (대문자는 모두 자음 접미사)
docgen.hangul.postfix_ascii_vowels		= {}
docgen.hangul.postfix_ascii_vowels["2"]	= 1
docgen.hangul.postfix_ascii_vowels["4"]	= 1
docgen.hangul.postfix_ascii_vowels["5"]	= 1
docgen.hangul.postfix_ascii_vowels["9"]	= 1
docgen.hangul.postfix_ascii_vowels["a"]	= 1
docgen.hangul.postfix_ascii_vowels["e"]	= 1
docgen.hangul.postfix_ascii_vowels["i"]	= 1
docgen.hangul.postfix_ascii_vowels["o"]	= 1
docgen.hangul.postfix_fix = function (sHint, iSuffix)
	local do_suffix = function(sHint)
		local s = String(sHint)
		s:TrimRight(" .")

		if #s.s > 0 then
			if string.byte(s.s, #s.s) < 128 then	-- case : ascii
				s:erase(0, #s.s - 1)
				if ((string.byte(s.s) >= string.byte("A")) and (string.byte(s.s) >= string.byte("Z"))) or (docgen.hangul.postfix_ascii_vowels[s.s] ~= nil) then
					return 1
				end
				return 0
			elseif #s.s >= 3 then				-- case : utf-8 (한글 표현 -> 3bytes)
				s:erase(0, #s.s - 3)			-- 마지막 3글자만 남김 
				
				if string.byte(s.s) >= 224 then	-- utf-8 type 임
					local	ich = ((string.byte(s.s) & 0x1F) << 12) | ((string.byte(s.s, 2) & 0x3F) << 6) | (string.byte(s.s, 3) & 0x3F)
					
					if (ich >= 0x3130) and (ich <= 0x318F) then	-- 완성형 자모 표현
						return (s.s == "ㅇ") and 0 or 1
					end
					if (ich >= 0xAC00) and (ich <= 0xD7A3) then	-- 완성형 글자
						return (((ich - 0xAC00) % 28) ~= 0) and 0 or 1
					end
				end
			end
		end
		return -1
	end
	
	local i_val = do_suffix(sHint)
	
	if i_val ~= -1 then
		iSuffix	= (((iSuffix - 1) & 0xFE) + i_val + 1)
	end
	
	return iSuffix
end

function EncodeParagraph(sText, config, sSourceTarget, sSourceLine)
	local	sPara			= String(sText)
	local	sResult			= String()
	local	bBypass			= false				-- 내용 무시, docgen.language 일치하지 않음
	local	bBypassCodeRef	= false				-- code reference in bypass mode
	local	iIngoreLines	= 0
	
	-- initialize configuration
	if config == nil then config = {} end
	if config.pPr == nil then config.pPr = "" end
	if config.rPr == nil then config.rPr = "" end
	
	-- line 얻기 구현
	local	line		= {}
	line.count			= 0
	line.source			= "str"
	line.get = function(opt)
		local	sTokStart	= sPara.TokenizePos
		local	bExtra		= false
		local	sLine		= nil
		
		sLine = sPara:Tokenize("\n")
		if opt == nil then
			opt = {}
		end
		
		
		if (bBypassCodeRef == false) and (opt.code_block ~= true) then
			while sLine:CompareBack(" \\") do -- 다음라인 이어 붙이기
				sLine:DeleteBack(" \\")
				sLine:TrimRight(" \t")
				sLine:Append(" ")
				sLine:Append(sPara:Tokenize("\n").s)
			end
		end
		
		if bExtra == false then
			-- ignore front enter code
			while sPara:at(sTokStart) == '\n' do
				if iIngoreLines > 0 then
					iIngoreLines	= iIngoreLines - 1
				else
					line.count	= line.count + 1
				end
				sTokStart = sTokStart + 1
			end
			
			local	sTokEnd		= sPara.TokenizePos	-- end of token

			-- show current line shortly
			if sLine:IsEmpty() == false then
				local	sStatus	= String(line.current() .. sLine.s)
				sStatus:Replace("\t", "    ", true)
				if sStatus:Length() > (docgen.max_console_chars - 5) then
					sStatus:erase((docgen.max_console_chars - 5), -1)
					sStatus:Append("...")
				end
				
				io.write(sStatus.s .. (' '):rep(docgen.max_console_chars - sStatus:Length()) .. "\r")
			end
			
			-- enter code counting...
			while sTokStart >= 0 do
				sTokStart	= sPara:find("\n", sTokStart)

				if (sTokStart < 0) or (sTokStart >= sTokEnd) then
					break
				end
				
				if iIngoreLines > 0 then
					iIngoreLines	= iIngoreLines - 1
				else
					line.count	= line.count + 1
				end
				sTokStart		= sTokStart + 1
			end
		end

		return sLine
	end
	line.current = function()
		local s = "[" .. line.source .. ":" .. (line.count + 1) .. "] "
		return s
	end
	
	if sPara:CompareFront("[[") and sPara:CompareBack("]]") then
		sPara:erase(0,2)
		sPara:CutBack("]]", false)
		sPara:ChangeCharsetToANSI()
		local	txt	= TextFile()
		if txt:Open(sPara.s) == false then
			error("Text file '" .. sPara.s .. "' is not existed.")
		end
		-- 파일 이름만 따옴
		sPara:CutFront("\\", true)
		sPara:CutFront("/", true)
		line.source	= sPara.s
		-- 전체 text 얻기
		sPara		= String(txt:GetAll(false))
		-- [Space + '\' + Enter] 표현은 다음줄과 이어 붙인다.
		txt:Close()
	end
	
	-- 사용자 지정 소스 내용
	if sSourceTarget ~= nil then
		line.source			= sSourceTarget
		if sSourceLine ~= nil then
			line.count			= sSourceLine
		end
	end
	
	sPara:ChangeCharsetToUTF8()
	sPara:Replace("@@", "&#64;", true)		-- '@' 문자 대체 표현
	sPara:Replace("@#", "&#35;", true)		-- '#' 문자 대체 표현
	sPara:Replace("@-", "&#45;", true)		-- '-' 문자 대체 표현
	sPara:Replace("@;", "&#59;", true)		-- ';' 문자 대체 표현
	sPara:Replace("\\$", "&#36;", true)		-- '$' 문자 대체 표현
	sPara:Replace("\r", "", true);			-- line feed 모두 제거

	-- empty paragraph, but do not ignore
	if (#sPara.s == 0) and (config.bDontIgnoreEmpty == true) then
		sPara.s = " "
	end

	while true do
		local	sLine	= line.get()	-- enter code process
		local	s_pPr	= config.pPr

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
						sLine:Append(line.get().s)
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
				sLine:CutBack("//", true)	-- comment out
				sLine:Trim(" ")
				if sLine:CompareFront("#") then
					bLine	= true
					sLine:erase(0,1)
				else
					bScript		= sLine:CompareFront("[") and sLine:CompareBack("]")	-- highlight contents 가 아닌 진짜 코드임.
					
					if bScript then
						sLine:DeleteFront("[")
						sLine:DeleteBack("]")
						sLine:Trim(" ")
					end
				end
				
				local	sCodeFormat	= sLine:Tokenize(":")
				local	sTitle		= sLine:Tokenize("")
				local	sIndent		= ""
				
				sCodeFormat:Trim(" ")
				sCodeFormat:MakeLower()
				sCodeFormat	= sCodeFormat.s
				
				if #sTitle.s > 0 then
					sTitle:DeleteFront(":")
					sTitle:Trim(" ")
				end
				sTitle	= sTitle.s
				
				-- cut off front
				sPara:erase(0, sPara.TokenizePos)
				sPara.TokenizePos	= 0
				-- get code length
				local	iCodeLen	= 0
				while sPara.TokenizePos >= 0 do
					sLine	= line.get({code_block = true})
					if sLine:CompareFront("```") then
						local s = String(sLine.s)
						s:CutFront("`", true)
						s:Trim(" \t")
						if s:IsEmpty() == false then
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
						sContent:Replace("\n\n", "\n \n", true)	-- for EncodeParagraph's to keep a enter code
						sContent:TrimLeft("\n")
						sContent:TrimRight(" \n")
						if sContent:CompareBack("\\") then	-- 마지막 끝이 '\' 로 끝나면 라인이 삭제된다. 이를 방지
							sContent:Append(" ")
						end
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
					
					if #sTitle ~= 0 then
						sTitle = "<w:tr><w:trPr><w:tblHeader/></w:trPr><w:tc>\
							<w:tcPr>\
								<w:tcW w:w=\"10094\" w:type=\"dxa\"/>\
								<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" .. docgen.code_header_bgcolor .. "\"/>\
							</w:tcPr>" ..
							EncodeParagraph(sTitle, {pPr=("<w:jc w:val=\"center\"/><w:spacing w:after=\"0\"/>" .. sIndent), rPr="<w:rFonts w:ascii=\"" .. docgen.fixed_font .. "\" w:eastAsia=\"" .. docgen.fixed_font .. "\" w:hAnsi=\"" .. docgen.fixed_font .. "\"/>"}) ..
							"</w:tc></w:tr>"
					end
					
					sResult:Append(
					"<w:tbl>\
					<w:tblPr>\
						<w:tblStyle w:val=\"ae\"/>\
						<w:tblW w:w=\"0\" w:type=\"auto\"/>\
						<w:tblBorders>\
							<w:top w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:left w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:bottom w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:right w:val=\"none\" w:sz=\"0\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:insideH w:val=\"none\"  w:sz=\"0\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
							<w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"" .. docgen.boarder_color .. "\"/>\
						</w:tblBorders>\
						<w:tblCellMar>\
							<w:top w:w=\"32\" w:type=\"dxa\"/>\
							<w:bottom w:w=\"52\" w:type=\"dxa\"/>\
						</w:tblCellMar>\
					</w:tblPr>\
					<w:tblGrid>\
						<w:gridCol w:w=\"10094\"/>\
					</w:tblGrid>" ..
					sTitle ..
					"<w:tr>\
						<w:tc>\
							<w:tcPr>\
								<w:tcW w:w=\"10094\" w:type=\"dxa\"/>\
								<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" .. docgen.code_bgcolor .. "\"/>\
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
				sLine:Trim(" \t")
				
				do
					local sTok = sLine:Tokenize(",;/")
				
					while sTok ~= nil do
						sTok:Trim(" \t")
						if sTok:IsEmpty() == false then
							s_pPr	= String(s_pPr)
							if (sTok.s == "left") or (sTok.s == "right") or (sTok.s == "center") then	-- alignment
								s_pPr:DeleteBlock("<w:jc */>", 0)
								s_pPr:Append("<w:jc w:val=\"" .. sTok.s .. "\"/>")
							else
								s_pPr:DeleteBlock("<w:pStyle*/>", 0)
								s_pPr:Append("<w:pStyle w:val=\"" .. ConvertStyleIDString(sTok.s) .. "\"/>")	-- style
							end
							s_pPr	= s_pPr.s
						else
							break
						end
						sTok = sLine:Tokenize(",;/")
					end
					sLine	= line.get()
				end
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
			elseif sLine:CompareFront("===") then	-- comment out
				sResult:CutBack("<w:p>", false)
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
					<w:pStyle w:val=\"" .. docgen.style_default .. "\"/>"

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
			local		sUnderline		= "single"
			local		bStrike			= false
			local		bSuperscript	= false
			local		bSubscript		= false
			local		bFixed			= false
			local		bColor			= (config.text_color ~= nil)
			local		sColor			= (config.text_color ~= nil) and (config.text_color) or ""
			local		bBgColor		= false
			local		sBgColor		= ""
			local		bSize			= false
			local		iSize			= 0
			local		bFont			= false
			local		sFont			= ""
			
			docgen.hangul.suffix_disable()
			sLine.TokenizePos			= 0
			
			while sLine.TokenizePos >= 0 do
				local	sWord	= sLine:TokenizeVariable("@<*>")
				local	s_rPr	= config.rPr

				sWord:Replace("<", "&lt;", true)
				sWord:Replace(">", "&gt;", true)

				if #(sWord.s) ~= 0 then
					sResult:Append("<w:r>")
					
					if bBold then
						s_rPr	= s_rPr .. "<w:b/><w:bCs/>"
					end
					if bItalic then
						s_rPr	= s_rPr .. "<w:i/><w:iCs/>"
					end
					if bUnderline then
						s_rPr	= s_rPr .. "<w:u w:val=\"" .. sUnderline .. "\"/>"
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
						s_rPr	= s_rPr .. "<w:rFonts w:ascii=\"" .. docgen.fixed_font .. "\" w:hAnsi=\"" .. docgen.fixed_font .. "\"/>"
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
					if bFont then
						s_rPr	= s_rPr .. "<w:rFonts w:ascii=\"" .. sFont .. "\" w:hAnsi=\"" .. sFont .. "\"/>"
					end
					
					if #s_rPr ~= 0 then
						sResult:Append("<w:rPr>" .. s_rPr .. "</w:rPr>")
					end
					
					if docgen.hangul.suffix_enable then	-- 한글 접미사 체크
						local s = String(sWord.s)
						s:TrimLeft(" }])'\"")
						local iSuffix	= docgen.hangul.suffix_retrieve(string.sub(s.s, 1, 3))
						
						if iSuffix > 0 then
							if docgen.hangul.suffix_hint == nil then
								-- bookmark 에 의한 참조 나중으로 미룸.
								local	t	= {}
								t.hint		= docgen.hangul.suffix_bookmark
								t.iSuffix	= iSuffix
								docgen.hangul.suffix_deferred[#docgen.hangul.suffix_deferred + 1]	= t
								
								sWord:Replace(docgen.hangul.suffix_list[iSuffix], "[[::han_suffix::" .. #docgen.hangul.suffix_deferred .. "]]")
							else
								-- 바로 고칠 수 있음
								local iSuffix_fixed = docgen.hangul.postfix_fix(docgen.hangul.suffix_hint, iSuffix)

								if iSuffix_fixed > 0 and iSuffix_fixed ~= iSuffix then
									sWord:Replace(docgen.hangul.suffix_list[iSuffix], docgen.hangul.suffix_list[iSuffix_fixed])
								end
							end
						end
						docgen.hangul.suffix_disable()
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
							sUnderline	= "single"
						elseif sVar.s == "U" then
							bUnderline	= bSet
							sUnderline	= "double"
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
							bColor			= bSet or (config.text_color ~= nil)
							if bSet and (config.text_color == nil) then
								sColor		= docgen.color.validate(sVar:Tokenize(": ").s)
								if #sColor == 0 then
									bColor		= false
								end
							end
						elseif sTag.s == "bgcolor" then
							bBgColor		= bSet
							if bSet then
								sBgColor	= docgen.color.validate(sVar:Tokenize(": ").s)
								if #sBgColor == 0 then
									bBgColor		= false
								end
							end
						elseif sTag.s == "size" then
							bSize			= bSet
							if bSet then
								iSize		= tonumber(sVar:Tokenize(": ").s)
							end
						elseif sTag.s == "font" then
							bFont			= bSet
							if bSet then
								sFont			= sVar:Tokenize(":")
								sFont:Trim(" ")
								sFont			= sFont.s
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
							
							sResult:Append(table_wrapper.prefix)
							
							if sFileName.s ~= "nil" then
								sResult:Append(GenerateFigure(sFileName.s, fRatio))
							end
							
							if sCaption ~= nil and sCaption ~= "" then
								sResult:Append(GenerateCaption("Figure", sCaption))
							end
							
							sResult:Append(table_wrapper.postfix)
							docgen.hangul.suffix_disable()

							goto continue
						elseif sTag.s == "tbl" then
							local	sMainName	= sVar:Tokenize("; ")
							local	sSubName	= sVar:Tokenize(";")
							sMainName:erase(0,1)
							sVar:erase(0, sVar.TokenizePos)
							sVar:Trim(" \t;")
							sSubName:Trim(" \t")
							local	bUnGroup	= sMainName:CompareFront("#")	-- 테이블 크기에 따른 페이지 넘김 무시
							local	sCaption	= sVar.s
							sResult:CutBack("<w:p>", false)
							
							if bUnGroup then
								sMainName:erase(0, 1)
							end
							
							if bUnGroup == false then
								sResult:Append(table_wrapper.prefix)
							end
							
							if sCaption ~= nil and sCaption ~= "" then
								sResult:Append(GenerateCaption("Table", sCaption))
							end
							
							if sMainName.s ~= "nil" then
								if sMainName.s == "lua" then
									sResult:Append(GenerateTableFromLua(sSubName.s))
								else
									sResult:Append(GenerateTable(sMainName.s, sSubName.s))	-- file_name, sheet_name
								end
								if bUnGroup == false then
									sResult:Append("<w:p/>")	-- tc 마지막에 없으면 table 에러 발생
								end
							end
							
							if bUnGroup == false then
								sResult:Append(table_wrapper.postfix)
							end
							docgen.hangul.suffix_disable()
							
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

								-- ZERO WIDTH SPACE(UTF-8 &#8203;) insertion
								do
									sContents:Replace("://", ":\b", true)	-- '://' 예외
									sContents:Replace("@</", "@\b", true)	-- '@</' 예외
									sContents:Replace("/", "/&#8203;", true)
									sContents:Replace(".", ".&#8203;", true)
									sContents:Replace(",", ",&#8203;", true)
									sContents:Replace("\\.", "\\.&#8203;", true)
									sContents:Replace("@\b", "@</", true)	-- '@</' 원복
									sContents:Replace(":\b", "://", true)	-- '://' 원복
								end
								
								sLine:insert(sLine.TokenizePos, "@<fixed>@<bgcolor:" .. docgen.code_bgcolor .. ">" .. sContents.s .. "@</bgcolor>@</fixed>")
							else
								error("invalid use '</code>'")
							end
						elseif sTag.s == "property" then
							local	sName	= sVar:Tokenize("")
							sName:TrimLeft(" :")
							sName:TrimRight(" ")
							do
								local sProperty = docgen.property[sName.s]
								if sProperty == nil then
									LOGW("docgen.property[" .. sName.s .. "] is not found.")
									sProperty	= ""
								end
								sLine:insert(sLine.TokenizePos, sProperty)
							end
							docgen.hangul.postfix_set_hint(sProperty)
						elseif sTag.s == "bookmark" then
							local	sBookmark		= sVar:Tokenize("")
							local	sBookmark_rPr	= ""
							sBookmark:TrimLeft(" :")
							sBookmark:TrimRight(" ")
							local	bAll		= sBookmark:CompareFront("@")
							if bAll then	-- recursive '#' + 'title' (number 를 먼저 찍고, 이름을 나중에 또 찍기 위해.)
								sBookmark:erase(0, 1)
								sLine:insert(sLine.TokenizePos, ". @<bookmark:" .. sBookmark.s .. ">")
								sBookmark:insert(0, "#")
							else
								docgen.hangul.postfix_set_bookmark(sBookmark.s)
							end
							
							do
								if bBold or (docgen.bookmark.always_bold and (sBookmark:CompareFront("&") == false)) then
									sBookmark_rPr	= sBookmark_rPr .. "<w:b/><w:bCs/>"
								end
								if bItalic then
									sBookmark_rPr	= sBookmark_rPr .. "<w:i/><w:iCs/>"
								end
								if bUnderline then
									sBookmark_rPr	= sBookmark_rPr .. "<w:u w:val=\"single\"/>"
								end
								if bStrike then
									sBookmark_rPr	= sBookmark_rPr .. "<w:strike/>"
								end
								if bFixed then
									sBookmark_rPr	= sBookmark_rPr .. "<w:rFonts w:ascii=\"" .. docgen.fixed_font .. "\" w:hAnsi=\"" .. docgen.fixed_font .. "\"/>"
								end
								if bColor then
									sBookmark_rPr	= sBookmark_rPr .. "<w:color w:val=\"" .. sColor .. "\"/>"
								end
								if bBgColor then
									sBookmark_rPr	= sBookmark_rPr .. "<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"" .. sBgColor .. "\"/>"
								end
								if bSize then
									sBookmark_rPr	= sBookmark_rPr .. "<w:sz w:val=\"" .. iSize .. "\"/><w:szCs w:val=\"" .. iSize .. "\"/>"
								end
								if bFont then
									sBookmark_rPr	= sBookmark_rPr .. "<w:rFonts w:ascii=\"" .. sFont .. "\" w:hAnsi=\"" .. sFont .. "\"/>"
								end
								
								if #sBookmark_rPr ~= 0 then
									sBookmark_rPr	= "<w:rPr>" .. sBookmark_rPr .. "</w:rPr>"
								end
							end
							
							sResult:Append("\
								<w:r>" .. sBookmark_rPr .. "<w:fldChar w:fldCharType=\"begin\"/></w:r>\
								<w:r>" .. sBookmark_rPr .. "<w:instrText xml:space=\"preserve\">BOOKMARK</w:instrText><w:t>" .. sBookmark.s .."</w:t></w:r>")
							sResult:Append("<w:r>" .. sBookmark_rPr .. "<w:instrText xml:space=\"preserve\"> \\* MERGEFORMAT </w:instrText></w:r>")
							sResult:Append("<w:r>" .. sBookmark_rPr .. "<w:fldChar w:fldCharType=\"separate\"/></w:r>")
							sResult:Append("<w:r>" .. sBookmark_rPr .. "<w:t xml:space=\"preserve\">[" .. sBookmark.s .. "]</w:t></w:r>")
							sResult:Append("<w:r>" .. sBookmark_rPr .. "<w:fldChar w:fldCharType=\"end\"/></w:r>")
						elseif sTag.s == "link" then
							local	sLink		= sVar:Tokenize(";")
							local	sComment	= sVar:Tokenize(";")
							sLink:CutFront(":", false)
							if sComment:Length() == 0 then
								sComment:Append(sLink.s)
								docgen.hangul.postfix_set_hint(sLink.s)
							else
								docgen.hangul.postfix_set_hint(sComment.s)
							end
							sResult:Append("<w:hyperlink r:id=\"" .. docgen.doc:AddHyperlink(sLink.s) .. "\"><w:r>\
									<w:rPr><w:rStyle w:val=\"af0\"/></w:rPr>\
									<w:t>"  .. sComment.s ..  "</w:t>\
								</w:r></w:hyperlink>")
						elseif (sTag.s == "fas") or (sTag.s == "far") or (sTag.s == "fab") then	-- emoji Font Awesome
							local	sUnicode	= docgen.emoji.get(sTag.s, sVar:Tokenize(" :").s)
							sLine:insert(sLine.TokenizePos, sUnicode)
						elseif sTag.s == "lua" then
							local	sLuaCode	= sVar:Tokenize("")
							sLuaCode:TrimLeft(" :")
							
							local	ReturnCode	= load("return (" .. sLuaCode.s .. ")")()
							
							if type(ReturnCode) == "string" then
								local sExtra = String(ReturnCode)
								sExtra:ChangeCharsetToUTF8()
								sExtra:Replace("@@", "&#64;", true)		-- '@' 문자 대체 표현
								sExtra:Replace("@#", "&#35;", true)		-- '#' 문자 대체 표현
								sExtra:Replace("@-", "&#45;", true)		-- '-' 문자 대체 표현
								sExtra:Replace("@;", "&#59;", true)		-- ';' 문자 대체 표현
								sExtra:Replace("\\$", "&#36;", true)	-- '$' 문자 대체 표현
								sExtra:Replace("\r", "", true);			-- line feed 모두 제거
								
								if sExtra:find("\n") >= 0 then
									sLine:erase(0, sLine.TokenizePos)
									sLine.TokenizePos = 0
									sLine:insert(0, sLine.s)
									
									if sExtra:CompareFront("\n") then
										sLine:clear()
										
									else
										sLine:insert(0, sExtra:Tokenize("\n").s)
									end
									sExtra = sExtra:Tokenize("")
									local iPos = 0	-- 무시할 enter code 계산
									while iPos >= 0 do
										iPos	= sPara:find("\n", iPos)

										if (iPos < 0) then break end
										
										iIngoreLines = iIngoreLines + 1
										iPos = iPos + 1
									end

									sPara:insert(sPara.TokenizePos, sExtra.s)
									
									sResult:CutBack("<w:p>", false)
									goto continue
								else
									sLine:insert(sLine.TokenizePos, ReturnCode)
									docgen.hangul.postfix_set_hint(ReturnCode)
								end
							else
								sLine:insert(sLine.TokenizePos, tostring(ReturnCode))
								docgen.hangul.postfix_set_hint(tostring(ReturnCode))
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
	docgen.doc_body:AddChildBeforeFromBuffer(docgen.doc_last,"\
		<w:p>\
			<w:r>\
				<w:br w:type=\"page\"/>\
			</w:r>\
		</w:p>")
end

AddParagraph = function(content, sSourceTarget)
	docgen.doc_body:AddChildBeforeFromBuffer(docgen.doc_last, EncodeParagraph(content, {pPr="<w:pStyle w:val=\"" .. docgen.style_default .. "\"/>"}), sSourceTarget)
end

AddTable = function(sCaption, sExcelFileName, sSheetName)
	if sCaption ~= nil and sCaption ~= "" then
		docgen.doc_body:AddChildBeforeFromBuffer(docgen.doc_last, GenerateCaption("Table", sCaption))
	end
	
	docgen.doc_body:AddChildBeforeFromBuffer(docgen.doc_last, GenerateTable(sExcelFileName, sSheetName))
end

AddSubDocument = function(sDocFileName)
	local	id	= docgen.doc:AddSubDocument(sDocFileName)
	docgen.doc_body:AddChildBeforeFromBuffer(docgen.doc_last, "\
		<w:p>\
			<w:subDoc r:id=\"" .. id .. "\"/>\
		</w:p>"
	)
end

-- Run user defined initialization
if docgen.OnInitialize ~= nil then
	if docgen.OnInitialize() == false then
		LOGE("Error on document initialization...")
		os.exit(1)
	end
end

-- 사용자 소스 실행
do
	local __sExt	= String(docgen.sInFilename)
	
	__sExt:CutFront(".", true)
	__sExt:MakeLower()
	
	LOGI("@1\n[[ Contents ]]\n")
	
	if __sExt.s == "md" then
		AddParagraph("[[" .. docgen.sInFilename .. "]]")
	else
		if RunScript(docgen.sInFilename) == false then
			return
		end
	end

	-- 현재 라인 상태 모두 지우기
	print((' '):rep(docgen.max_console_chars) .. "\r")	-- end of document
end

-- Run user defined finalization
if docgen.OnFinalize ~= nil then
	if docgen.OnFinalize() == false then
		LOGE("Error on document finalization...")
		os.exit(1)
	end
end

-- 속성 갱신
for key, value in pairs(docgen.property) do
	if key ~= "Water_Mark" then
		docgen.doc:SetProperty(key, value)
	end
end


-- 테이블/그림/Terms 목차 없을시 제거
local function DeleteDocSection(sPara)
	if sPara == nil then
		return
	end
	local node = docgen.doc_body:child_by_text("w:p", "w:t", sPara)

	if node:empty() then	-- not found
		LOGW("Doc section '" .. sPara .. "' is not found.");
		return
	else
		-- search to top paragraph (반드시 'w:lastRenderedPageBreak' 이 존재하는 것은 아니다.)
		--[[while node:child_in_depth("w:lastRenderedPageBreak", nil):empty() do
			node	= node:previous_sibling(nil)
			if node:empty() then
				error("It's first page paragraph. : " .. sPara)
				break;
			end
		end
		--]]

		-- delete first paragraph (because it includes a page break code)
		node:Destroy(1)
		
		-- delete paragraph until page break
		while node:child_in_depth("w:lastRenderedPageBreak", nil):empty() and node:child_in_depth_by_attribute("w:pStyle", "w:val", "TOCHeading"):empty() do
		
			-- 새로운 색션(페이지 나눔 포함) 시작일 경우 이전 세션 정보를 위로 올려줘야 한다.
			if node:child_in_depth("w:br", nil):empty() == false then	-- page break
				local w_type	= node:child_in_depth("w:br", nil):get_attribute("w:type")
				local section	= node:child_in_depth("w:sectPr", nil):as_xml()
				local prev_para	= node:previous_sibling("w:p")
				local prev_rex	= prev_para:child("w:r")
				local prev_pPr	= prev_para:child("w:pPr")

				-- set page break
				if prev_rex:empty() then
					prev_rex	= prev_para:append_child("w:r")
				end
				if prev_rex:child("w:br"):empty() then
					prev_rex:append_child("w:br"):set_attribute("w:type", w_type)
				else
					prev_rex:child("w:br"):set_attribute("w:type", w_type)
				end
				
				-- set section description
				if prev_pPr:child("w:sectPr"):empty() == false then	-- if already exist section description, must delete it
					prev_pPr:child("w:sectPr"):Destroy()
				end
				prev_pPr:AddChildFromBuffer(section)
			end
		
			if node:Destroy(1) == false then
				error("It's last page paragraph. : " .. sPara)
				break
			end
		end
	end
end


if docgen.contents_only then	-- 내용을 제외한 모두 제거.
	while(true) do
		local	last_pr	= docgen.doc_previous:previous_sibling()
		
		if last_pr:empty() then
			break
		end
		last_pr:Destroy(1)
	end
	docgen.doc_previous:Destroy(1)
else
	if docgen.revision.count == 0 then
		DeleteDocSection(docgen.revision.title)
	end

	if docgen.table.count == 0 then
		DeleteDocSection(docgen.table.title)
	end

	if docgen.figure.count == 0 then
		DeleteDocSection(docgen.figure.title)
	end

	if docgen.terms.count == 0 then
		DeleteDocSection(docgen.terms.title)
	end
end

-- 북마크 갱신
LOGI("*0Link all bookmarks.")
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
	
	local	bookmark	= docgen.bookmark.Get(sTarget.s)
	
	if bookmark == nil then
		error("bookmark \"" .. sTarget.s .. "\" is not found.")
	end
	
	bookmark_text:Destroy(1)

	bookmark_node:set_string(" " .. (bPage and "PAGEREF" or "REF") .. " _Ref" .. bookmark.id .. (bNumber and " \\w" or "") .. " \\h ")
end

-- 한글 접미사 수정
if #docgen.hangul.suffix_deferred > 0 then
	-- 먼저 올바른 postfix 찾기
	for i, v in ipairs(docgen.hangul.suffix_deferred) do
		local	name		= String(v.hint)
		
		if name:CompareFront("&") == false then	-- page 는 무시
			local	bNumber	= name:CompareFront("#")	-- num part 만
			if bNumber then	
				name:erase(0, 1)
			end
			local	bookmark	= docgen.bookmark.Get(name.s)
			
			if bookmark ~= nil then
				if bookmark.type_part ~= "Chapter" then
					bNumber	= true
				end
				v.iSuffix = docgen.hangul.postfix_fix((bNumber and bookmark.num_part or name.s), v.iSuffix)
			end
		end
	end

	-- 모두 바꾸기
	while true do
		local	text_node	= docgen.doc_body:child_in_depth("w:t", "[[::han_suffix::")
		
		if text_node:empty() then break end

		local s = String(text_node:text())

		while true do
			local iStart	= s:find("[[::han_suffix::")
			local iEnd		= s:find("]]", iStart + 16)

			if iStart < 0 or iEnd < 0 then
				break
			end

			local	iNum	= tonumber(string.sub(s.s, iStart + 17, iEnd))
			s:erase(iStart, iEnd - iStart + 2)
			local	node	= docgen.hangul.suffix_deferred[iNum]
			
			if node == nil then
				error("Got unexpected error on hangul suffix autofix.")
			end
			
			s:insert(iStart, docgen.hangul.suffix_list[node.iSuffix])
		end

		text_node:set_string(s.s)
	end
end

-- 파일 저장
do
	-- 저장 이름 없을 시 생성
	if #docgen.sOutFilename == 0 then
		if(#docgen.property["IP_Name_Header"] > 0) then
			docgen.sOutFilename	= docgen.sOutFilename .. docgen.property["IP_Name_Header"] .. "_"
		end

		if(#docgen.property["Document_Name_Header"] > 0) then
			docgen.sOutFilename	= docgen.sOutFilename .. docgen.property["Document_Name_Header"] .. "_"
		end

		if(#docgen.property["Doc_Version"] > 0) then
			docgen.sOutFilename	= docgen.sOutFilename .. "rev" .. docgen.property["Doc_Version"]
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

	-- should be change to ANSI formatted file name (must not UTF-8)
	docgen.sOutFilename:ChangeCharsetToANSI()

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
	do
		sutf8_Name	= String(docgen.sOutFilename.s)
		sutf8_Name:ChangeCharsetToUTF8()
		LOGI("@0*I: Build document : ")
		print(sutf8_Name.s)
		if docgen.doc:Save(docgen.sOutFilename.s) == false then
			LOGI("@2*E: Can't create : ")
			print(sutf8_Name.s)
			os.exit(1)
		end
	end
	docgen.doc:Close()
	
	-- 중간 생성 파일들 제거
	for i=1, #temporary_file_list do
		os.execute("rm -f \"" .. temporary_file_list[i] .. "\"")
	end
	
	if docgen.output_format == nil then
		LOGI("*0Fields calculation...")
		os.execute("doc2pdf \"" .. docgen.sOutFilename.s .. "\" \"" .. docgen.property["Water_Mark"] .. "\" *")	-- no save as
	else
		-- make pdf first
		if (docgen.output_format["pdf"] == true) or (docgen.output_format["djvu"] == true) then
			if docgen.output_format["pdf"] == true then
				LOGI("*0Save as 'pdf'(" .. docgen.supported_format["pdf"] .. ") file...")
			else
				LOGI("*0Fields calculation...")
			end
			os.execute("doc2pdf \"" .. docgen.sOutFilename.s .. "\" \"" .. docgen.property["Water_Mark"] .. "\"")
		end
		-- then save as others
		for ext, bOut in key_pairs(docgen.output_format) do
			if bOut then
				if ext == "pdf" then
				elseif ext == "djvu" then
					local sDJVU_filename	= String(sOutFilename_PDF.s)
					sDJVU_filename:DeleteBack("pdf")
					sDJVU_filename:Append("djvu")
					LOGI("*0Save as '" .. ext .. "'(" .. docgen.supported_format[ext] .. ") file...")
					exec("pdf2djvu -j 2 -d 600 \"" .. sOutFilename_PDF.s .. "\" -o \"" .. sDJVU_filename.s .. "\"")	-- 600 dpi output
					if docgen.output_format["pdf"] ~= true then
						exec("rm -f \"" .. sOutFilename_PDF.s .. "\"")
					end
				else
					LOGI("*0Save as '" .. ext .. "'(" .. docgen.supported_format[ext] .. ") file...")
					os.execute("doc2save \"" .. docgen.sOutFilename.s .. "\" " .. ext)
				end
			end
		end
	end
end
