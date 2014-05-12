#lua-mongoose
一个lua的http server库，基于[mongoose](https://github.com/cesanta/mongoose)。

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


local server1 = mongoose.create(8080, http_handle)
local server2 = mongoose.create(9090, http_handle)
while true do
	server1:poll()
	server2:poll()
end

```
看起来是这样的：
![image](https://raw.githubusercontent.com/shuax/lua-mongoose/master/screenshot.png)