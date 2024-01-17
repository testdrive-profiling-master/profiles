local Arg = ArgTable("ArgTable test.")

Arg:AddOption		("power", "p", "power", "enable power")
Arg:AddOption		("force", "f", "force", "forced enable")
Arg:AddRemark		(nil, "additional description")
Arg:AddOptionFile	("in_file", nil, nil, nil, "input_file", "input file")
Arg:AddOptionFile	("out_file", nil, nil, nil, "output_file", "output file")
if (Arg:DoParse() == false) then
	return
end

-- sample inputs
print("Option forced : ".. tostring(Arg:GetOption("force")))
print("Option power  : ".. tostring(Arg:GetOption("power")))
print("Input file    : ".. Arg:GetOptionFile("in_file", 0))
print("Output file   : ".. Arg:GetOptionFile("out_file", 0))
