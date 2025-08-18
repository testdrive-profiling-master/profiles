#lua
do
	local doc_tree = ProfileTree("Document|TestDrive")
	
	doc_tree:AddItem("report", "Verilator",									"verilator.sp")
	doc_tree:AddItem("report", "GTKWave",									"gtkwave.sp")
	doc_tree:AddItem("report", "Docgen",									"docgen.sp")
	doc_tree:AddItem("report", "Verigen",									"verigen.sp")
	doc_tree:AddItem("report", "TestDrive tutorial",						"tutorial.sp")
	doc_tree:AddItem("report", "TestDrive Script Language",					"testdrive_script.sp")
	doc_tree:AddItem("report", "aStyle - C/C++/Java automatic formatter",	"astyle.sp")
	doc_tree:AddItem("report", "iStyle - Verilog automatic formatter",		"istyle.sp")
end
