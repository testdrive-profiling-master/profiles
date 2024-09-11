local	Arg		= ArgTable("convert LibreOffice draw(.odg) to .SVG")

Arg:AddOptionString	("page_name", "", "p", nil, "page_name", "Specify page name")
Arg:AddRemark		(nil, "(default : first page)")
Arg:AddOptionFile	("odg_file", nil, nil, nil, "input_file", "input .odg file")

if (Arg:DoParse() == false) then
	return
end

page_name	= Arg:GetOptionString("page_name")
file_name	= Arg:GetOptionFile("odg_file", 0)
office_path	= nil

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
	LOGE("Libreoffice must be installed first.")
	os.exit(1)
end

-- Get page name list
run(".run \"" .. office_path .. "\" --draw --headless --convert-to xml --outdir .odg2svg \"" .. file_name .. "\"")

local	page_list	= {}
do
	local sXML	= String()
	do	-- read from file
		local	f				= TextFile()
		local xml_file_name		= String(file_name)
		xml_file_name:DeleteBack("odg")
		xml_file_name:Append("xml")
		
		if f:Open(".odg2svg/" .. xml_file_name.s) == false then
			LOGE("Information check is failed.")
			os.exit(1)
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
	
	for key, value in pairs(page_list) do
		print("Page[" .. key .. "] = " .. value)
	end
end

-- check page exist
if page_list[page_name] == nil then
	LOGE("No page existed : '" .. page_name .. "'")
end

print("*I: Retrieving page : '" .. page_name .. "'")

-- Make SVG
run(".run \"" .. office_path .. "\" --draw --headless --convert-to svg:\"draw_svg_Export\" --outdir .odg2svg \"" .. file_name .. "\"")

do
	local xml = DocXML()

	do
		local xml_file_name		= String(file_name)
		xml_file_name:DeleteBack("odg")
		xml_file_name:Append("svg")
		
		if xml:Open(".odg2svg/" .. xml_file_name.s) == false then
			LOGE("SVG file can't create.")
			os.exit(1)
		end
	end
	local node = xml:Node()
	print("node[" .. node:name() .. "] = " .. tostring(node:empty()))
	node = node:child(nil)
	node = xml:Node():child("svg"):child_by_attribute("g", "class", "SlideGroup")
	print("d : " .. node:name() .. " : " .. tostring(node:empty()))
end
