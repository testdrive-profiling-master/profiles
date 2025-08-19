#lua
do
	local regmap_tree	= ProfileTree("System|Register map")
	regmap_tree:AddItem("report", "MTSP (Meitner)", "main.sp")
	regmap_tree:Expand()
end
