local	Arg		= ArgTable("WOL(Wake On Lan) sender")

Arg:AddOptionInt	("port", 9, "p", nil, "port", "port number (default : 9)")
Arg:AddOptionString	("ip", "255.255.255.255", "i", nil, "ip", "IP address (ex : 192.168.10.2)")
Arg:AddOptionString	("mac", nil, nil, nil, "mac_address", "MAC address (ex : 1A:2B:3C:4D:5E:6F)")

if (Arg:DoParse() == false) then
	return
end

local	port		= Arg:GetOptionInt("port")
local	ip			= Arg:GetOptionString("ip")
local	mac_string	= Arg:GetOptionString("mac")

local mac = ''
for w in string.gmatch(mac_string, "[0-9A-Za-z][0-9A-Za-z]") do
  mac = mac .. string.char(tonumber(w, 16))
end

local udp = require("socket", "luasocket").udp()
udp:settimeout(1)
udp:setoption("broadcast", true)
udp:sendto(string.char(0xff):rep(6) .. mac:rep(16) , ip, port)

LOGI("Done!")
