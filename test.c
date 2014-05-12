#include <stdio.h>
#include <stdlib.h>

#include "lua-mongoose.h"

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    luaL_module(L, "mongoose", luaopen_mongoose);

    if( luaL_dofile(L, "test.lua")!=0 )
    {
        printf("%s\n", lua_tostring(L, -1));
    }

    lua_close(L);
    return 0;
}
