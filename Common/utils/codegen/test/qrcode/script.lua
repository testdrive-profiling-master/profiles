local c = Clipper()

c:AddQRCode("https://testdrive-profiling-master.github.io/")

local s = SVGBuilder()
s:AppPaths(c, {stoke_width=0,fill=0xFF000000})
s:ExportToFile("qrcode.svg", {margin=5})
