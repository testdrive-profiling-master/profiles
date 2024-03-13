--[[
	read excel table from file.
	filename 		: excel file path
	sheet_name		: excel sheet name
	tag_name		: alignment tag name
					  nil		= aligned by number
					  "name"	= aligned by tag name, if not have 'name' column item in row will be disposed.
					  "#name"	= aligned by number, if not have 'name' column item in row will be disposed.
	auto_parsing	: auto value type parsing
	position_a		: set start table position (string or number[column])
					  string 	= ex) "A5"
					  number	= ex) 3, 4(position_b)
	position_b		: row number, if position_a is number
--]]
function excel_read_table(filename, sheet_name, tag_name, auto_parsing, position_a, position_b)
	local xls = DocExcel()
	
	if xls:Open(filename) then
		local sheet		= xls:GetSheet(sheet_name)
		local bNumID	= true
		
		if tag_name ~= nil then
			local sTag = String(tag_name)
			
			if sTag:CompareFront("#") then
				sTag:erase(0, 1)
				tag_name	= sTag.s
			else
				bNumID	= false
			end
		end

		if sheet ~= nil then
			local	contents	= {}
			
			-- read excel
			local	header		= {}
			local	col_count	= 0
			
			-- set position
			if position_a ~= nil then
				if type(position_a) == "string" then
					sheet:SetPosition(position_a)
				else
					sheet:SetPos(position_a, position_b)
				end
			end
			
			-- first row
			do
				sheet:GetRow()

				-- read tags
				while sheet:GetColumn() do
					header[#header + 1] = sheet:GetValue()
					col_count	= col_count + 1
				end
			end
			
			-- rest rows
			while sheet:GetRow() do
				local item	= {}

				-- read cols
				for i=1,col_count do
					sheet:GetColumn()
					local value = sheet:GetValue(true, true)
					local value_type = sheet:GetLatestValueType()
					
					-- auto parsing to type
					if auto_parsing == true then
						if value_type == "value" then
							value	= tonumber(value)
						elseif value_type == "boolean" then
							if (value == "True") or (value == "TRUE") then
								value = true
							elseif (value == "False") or (value == "FALSE") then
								value = false
							end
						elseif value_type == "string" then
							-- do nothing
						else
							value = nil
						end
					end
					
					item[header[i]]	= value
				end
				
				if bNumID then
					if tag_name ~= nil and (item[tag_name] == nil or #item[tag_name] == 0) then
						-- do nothing...
					else
						contents[#contents + 1] = item
					end
				else
					if contents[item[tag_name]] ~= nil then
						LOGW("read_excel_table of '" .. filename .. "'[" .. sheet_name .. "]'s tag '" .. tag_name .. "' is duplicated.")
					end
					contents[item[tag_name]] = item
				end
				
			end
			
			xls:Close()
			
			return contents
		end
	end
	
	return nil
end
