-- wifi		= WIFI:S:<SSID>;T:<WEP|WPA|blank>;P:<PASSWORD>;H:<true|false|blank>;;
-- Vcard	= MECARD:N:<NAME>;TEL:<PHONE_NUMBER>;URL:<URL>;EMAIL:<EMAIL>;ADR:<ADDRESS>;;
local c = Clipper()

c:AddQRCode("https://testdrive-profiling-master.github.io/")

local s = SVGBuilder()
s:AppPaths(c, {stoke_width=0,fill=0xFF000000})
s:ExportToFile("qrcode.svg", {margin=5})
