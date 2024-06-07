local c = Clipper()

c:AddQRCode("https://testdrive-profiling-master.github.io/")

c:SaveToSVG("qrcode.svg")
