local c = Clipper()

c:AddQRCode("https://testdrive-profiling-master.github.io/")

c:SaveToSVG("qrcode.svg", {stoke_width=0,fill=0xFF000000})
