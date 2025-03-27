local	Arg		= ArgTable("Convert draw.io(.drawio) to .png for TestDrive Profiling Master")

Arg:AddOptionString	("page_name", "", "p", nil, "page_name", "Specify page name")
Arg:AddRemark		(nil, "(default : first page)")
Arg:AddOptionFile	("out_file", "", "o", nil, "output_file", "Specify output .png file name")
Arg:AddOptionFile	("input_file", nil, nil, nil, "input_file", "input .drawio file name")

if (Arg:DoParse() == false) then
	return
end

page_name		= Arg:GetOptionString("page_name")
in_file_name	= Arg:GetOptionFile("input_file", 0)
out_file_name	= Arg:GetOptionFile("out_file", 0)

if lfs.IsExist(in_file_name) == false then
	LOGE("Input file is not found : " .. in_file_name)
	os.exit(1)
end

function ERROR(msg)
	LOGE(msg)
	os.exit(1)
end

RunScript("check_drawio")

local	sProfilePath		= String()
sProfilePath:GetEnvironment("TESTDRIVE_PROFILE")
sProfilePath:Append("common/bin/")

-- Get page name list
exec(".run \"" .. sProfilePath.s .. "drawio/draw.io.exe\" -x -f xml -u -o \"" .. in_file_name .. ".drawio2png.xml\" \"" .. in_file_name .. "\"")

local	page_list	= {}
local	page_count	= 0
do
	local xml = DocXML()
	do	-- read from file
		local xml_file_name		= String(in_file_name)
		xml_file_name:CutFront("/", true)
		xml_file_name:CutFront("\\", true)
		xml_file_name:DeleteBack("odg")
		xml_file_name:Append("xml")
		if xml:LoadFromFile(in_file_name .. ".drawio2png.xml") == false then
			exec("rm -f \"" .. in_file_name .. ".drawio2png.xml\"")
			ERROR("Information check is failed.")
		end
		exec("rm -f \"" .. in_file_name .. ".drawio2png.xml\"")
	end
	
	local node	= xml:Node("mxfile")
	local child = node:child("diagram")
	
	while child:empty() == false do
		page_list[child:get_attribute("name")]	= page_count
		page_count	= page_count + 1	-- start at '0'
		if #page_name == 0 then	-- default page name (first page)
			page_name = child:get_attribute("name")
		end
		child:Destroy(1)
		child = node:child("diagram")
	end
end

-- check page exist
if page_list[page_name] == nil then
	ERROR("No page existed : '" .. page_name .. "'")
end

local page_number = page_list[page_name]

page_name	= String(page_name)
page_name:Replace(" ", "_", true)
page_name:ChangeCharsetToANSI()	-- fix for command line
	
if #out_file_name == 0 then
	out_file_name	= in_file_name .. "." .. page_name.s .. ".png"
end

exec(".run \"" .. sProfilePath.s .. "drawio/draw.io.exe\" -x --crop --width 3200 -f png -p " .. page_number .. " -o \"" .. out_file_name .. "\" \"" .. in_file_name .. "\"")
