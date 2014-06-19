#ifndef LUA_MONGOOSE
#define LUA_MONGOOSE

/*
#define MONGOOSE_NO_FILESYSTEM
#define MONGOOSE_NO_WEBSOCKET
可以减少不需要的模块
*/
#include "mongoose.h"

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#define MONGOOSE_NAME "mongoose_server"

struct mg_context {
    struct mg_server *server;
    lua_State *vm;
    int callback;
};

void luaL_module(lua_State *L, const char *name, lua_CFunction f);
int luaopen_mongoose(lua_State *L);

#endif // LUA_MONGOOSE
