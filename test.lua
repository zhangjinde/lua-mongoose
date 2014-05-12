local mongoose = require("mongoose")

local function http_handle(ctx)
	for k,v in pairs(ctx) do
		print(k,v)
	end
	print()
	return "hello mongoose"
end


local server1 = mongoose.create(8080, http_handle)
local server2 = mongoose.create(9090, http_handle)
while true do
	server1:poll()
	server2:poll()
end
