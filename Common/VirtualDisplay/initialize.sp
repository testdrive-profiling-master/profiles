#lua
do
	local system_tree = ProfileTree("System")
	system_tree:AddItem("screen", "Display", "main.sp")
	system_tree:Expand()
end

System.CallProfile("main.sp")	-- run default
