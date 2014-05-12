#lua-mongoose
一个lua的http server库，基于mongoose。

用起来是这样的：
```lua
local mongoose = require("mongoose")

local function http_handle(ctx)
	for k,v in pairs(ctx) do
		print(k,v)
	end
	print()
	return "hello mongoose"
end


local server = mongoose.create(80, http_handle)
while true do
	server:poll()
end
```
看起来是这样的：
![image](https://raw.githubusercontent.com/shuax/lua-mongoose/master/screenshot.png)