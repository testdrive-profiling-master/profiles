local	Arg		= ArgTable("Convert LibreOffice draw(.odg) to .SVG for TestDrive Profiling Master")

Arg:AddOptionString	("page_name", "", "p", nil, "page_name", "Specify page name")
Arg:AddRemark		(nil, "(default : first page)")
Arg:AddOptionFile	("out_file", "", "o", nil, "output_file", "Specify output .svg file name")
Arg:AddOptionFile	("input_file", nil, nil, nil, "input_file", "input .odg file name")

if (Arg:DoParse() == false) then
	return
end

page_name		= Arg:GetOptionString("page_name")
in_file_name	= Arg:GetOptionFile("input_file", 0)
out_file_name	= Arg:GetOptionFile("out_file", 0)
office_path	= nil

function ERROR(msg)
	LOGE(msg)
	run("rm -rf .odg2svg")
	os.exit(1)
end

-- 1st : get libreoffice path
for i=0,7 do	-- 'C'~'H' drive
	local sPath = string.char(67+i) .. ":\\Program Files\\LibreOffice\\program\\soffice.exe"

	if lfs.IsExist(sPath) then
		office_path = String(sPath)
		office_path:Replace("\\", "/", true)
		office_path	= office_path.s
		break
	end
end

if office_path == nil then
	ERROR("Libreoffice must be installed first.")
end

-- Get page name list
run(".run \"" .. office_path .. "\" --draw --headless --convert-to xml --outdir .odg2svg \"" .. in_file_name .. "\"")

local	page_list	= {}
do
	local sXML	= String()
	do	-- read from file
		local	f				= TextFile()
		local xml_file_name		= String(in_file_name)
		xml_file_name:CutFront("/", true)
		xml_file_name:CutFront("\\", true)
		xml_file_name:DeleteBack("odg")
		xml_file_name:Append("xml")
		
		if f:Open(".odg2svg/" .. xml_file_name.s) == false then
			ERROR("Information check is failed.")
		end
		sXML.s	= f:GetAll()
		f:Close()
	end
	
	local	iPos	= 0
	local	iID		= 1
	while (iPos >= 0) do
		iPos	= sXML:find("<draw:page")
		if iPos > 0 then
			sXML:erase(0, iPos)
			sXML:CutFront("draw:name")
			local name = sXML:Tokenize("=\"").s
			if #page_name == 0 then
				page_name	= name
			end
			page_list[name]	= iID
			iID = iID + 1
		end
	end
end

-- check page exist
if page_list[page_name] == nil then
	ERROR("No page existed : '" .. page_name .. "'")
end

-- Make SVG
run(".run \"" .. office_path .. "\" --draw --headless --convert-to svg:\"draw_svg_Export\" --outdir .odg2svg \"" .. in_file_name .. "\"")

do
	local xml = DocXML()

	do
		local xml_file_name		= String(in_file_name)
		xml_file_name:CutFront("/", true)
		xml_file_name:CutFront("\\", true)
		xml_file_name:DeleteBack("odg")
		xml_file_name:Append("svg")
		
		if xml:LoadFromFile(".odg2svg/" .. xml_file_name.s) == false then
			ERROR("SVG file can't create.")
		end
	end
	
	local page_id	= page_list[page_name]
	local node		= xml:Node("svg"):child_by_attribute("g", "class", "SlideGroup")
	
	if node:empty() then
		ERROR("SVG file can't create.")
	end

	-- delete other pages
	local	child		= node:first_child()
	do
		local	slide_id	= 1
		while (child:empty() == false) do
			if slide_id ~= page_id then
				child:Destroy()
			else
				child		= child:next_sibling()
			end
			slide_id	= slide_id + 1
		end
	end
	
	-- calculate view port area
	do
		local	BoundingBox			= {}
		BoundingBox.iCount			= 0
		BoundingBox.sx				= 1000000000000000000000
		BoundingBox.sy				= 1000000000000000000000
		BoundingBox.ex				= -1000000000000000000000
		BoundingBox.ey				= -1000000000000000000000
		child						= node:child_in_depth_by_attribute("rect", "class", "BoundingBox")
		
		-- 모든 boundingbox 조사
		while child:empty() == false do
			local	sx		= tonumber(child:get_attribute("x"))
			local	sy		= tonumber(child:get_attribute("y"))
			local	ex		= sx + tonumber(child:get_attribute("width"))
			local	ey		= sy + tonumber(child:get_attribute("height"))
			child:set_attribute("class", "BoundingBox_done")
			child	= node:child_in_depth_by_attribute("rect", "class", "BoundingBox")
			
			if sx < BoundingBox.sx then BoundingBox.sx = sx end
			if sy < BoundingBox.sy then BoundingBox.sy = sy end
			if ex > BoundingBox.ex then BoundingBox.ex = ex end
			if ey > BoundingBox.ey then BoundingBox.ey = ey end
			BoundingBox.iCount	= BoundingBox.iCount + 1
		end

		-- 임시 수정한 내용 원복
		child						= node:child_in_depth_by_attribute("rect", "class", "BoundingBox_done")
		while child:empty() == false do
			child:set_attribute("class", "BoundingBox")
			child	= node:child_in_depth_by_attribute("rect", "class", "BoundingBox_done")
		end
		
		if BoundingBox.sx < 0 then BoundingBox.sx = 0 end
		if BoundingBox.sy < 0 then BoundingBox.sy = 0 end

		-- 사이즈 적용
		if BoundingBox.iCount > 0 then
			local	width	= BoundingBox.ex - BoundingBox.sx
			local	height	= BoundingBox.ey - BoundingBox.sy
			node		= xml:Node("svg"):child_in_depth_by_attribute("defs", "class", "ClipPathGroup")

			if node:empty() == false then
				local rect	= node:child_in_depth_by_attribute("clipPath", "id", "presentation_clip_path_shrink")
				if rect:empty() == false then
					rect	= rect:child("rect")
					rect:set_attribute("x", tostring(BoundingBox.sx))
					rect:set_attribute("y", tostring(BoundingBox.sy))
					rect:set_attribute("width", tostring(width))
					rect:set_attribute("height", tostring(height))
				end
			end
			
			-- 마진 적용
			local	margin	= math.ceil((width / 2) * 0.01)	-- 1% 마진
			BoundingBox.sx	= BoundingBox.sx - margin
			BoundingBox.sy	= BoundingBox.sy - margin
			BoundingBox.ex	= BoundingBox.ex + margin
			BoundingBox.ey	= BoundingBox.ey + margin
			width			= BoundingBox.ex - BoundingBox.sx
			height			= BoundingBox.ey - BoundingBox.sy
			if node:empty() == false then
				local rect	= node:child_in_depth_by_attribute("clipPath", "id", "presentation_clip_path")
				if rect:empty() == false then
					rect	= rect:child("rect")
					rect:set_attribute("x", tostring(BoundingBox.sx))
					rect:set_attribute("y", tostring(BoundingBox.sy))
					rect:set_attribute("width", tostring(width))
					rect:set_attribute("height", tostring(height))
				end
			end
			node		= xml:Node("svg")
			node:set_attribute("viewBox", tostring(BoundingBox.sx) .. " " .. tostring(BoundingBox.sy) .. " " .. tostring(width) .. " " .. tostring(height))
			node:set_attribute("width", tostring(math.ceil(width/100)) .. "mm")
			node:set_attribute("height", tostring(math.ceil(height/100)) .. "mm")
		end
	end
	
	-- save to file
	page_name	= String(page_name)
	page_name:Replace(" ", "_", true)
	
	if #out_file_name == 0 then
		out_file_name	= in_file_name .. "." .. page_name.s .. ".svg"
	end
	
	if xml:SaveToFile(out_file_name) == false then
		ERROR("Save to '" .. out_file_name .. "' is failed.")
	end

	run("rm -rf .odg2svg")
end
