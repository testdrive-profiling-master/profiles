local socket = require("socket", "luasocket")

c = assert(socket.connect("localhost", 8080))
print("Connected! Please type stuff (empty line to stop):")
l = io.read()
while l and l ~= "" and not e do
	assert(c:send(l .. "\n"))
	l = io.read()
end
