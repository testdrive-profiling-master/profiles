---------------------------------------------------------------------------------
-- argument handling
---------------------------------------------------------------------------------
local	Arg				= ArgTable("TSCP auto-update server")

Arg:AddOptionString	("path", nil, nil, nil, "path", "Remote path to update.")

if (Arg:DoParse() == false) then
	return
end

