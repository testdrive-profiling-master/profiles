local	Arg		= ArgTable("WOL(Wake On Lan) sender, (currently local network only.)")

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
local mac_address = ''
for w in string.gmatch(mac_string, "[0-9A-Za-z][0-9A-Za-z]") do
  mac = mac .. string.char(tonumber(w, 16))
  mac_address = mac_address .. ((#mac_address ~= 0) and ":" or "") .. string.format("%02X", tonumber(w, 16))
end

print("* Target *")
print("\tIP          : " .. ip)
print("\tMAC address : " .. mac_address)
print("\tPort number : " .. port)

-- https://tst2005.github.io/lua-socket/udp.html
local udp = require("socket", "luasocket").udp()
udp:settimeout(1)
udp:setoption("broadcast", true)
assert(udp:sendto(string.char(0xff):rep(6) .. mac:rep(16) , ip, port))

LOGI("Done!")
