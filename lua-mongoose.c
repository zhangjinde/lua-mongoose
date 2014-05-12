#include "lua-mongoose.h"

static void build_param(lua_State *L, struct mg_connection *conn)
{
    lua_newtable(L);

    lua_pushstring(L, conn->uri);
    lua_setfield(L, -2, "uri");

    lua_pushstring(L, conn->query_string);
    lua_setfield(L, -2, "query_string");

    lua_pushlstring(L, conn->content, conn->content_len);
    lua_setfield(L, -2, "content");

    lua_pushstring(L, conn->remote_ip);
    lua_setfield(L, -2, "remote_ip");

    lua_pushstring(L, conn->request_method);
    lua_setfield(L, -2, "request_method");
}

static int event_handler(struct mg_connection *conn, enum mg_event ev)
{
    if (ev == MG_REQUEST)
    {
        struct mg_context *ctx = conn->server_param;
        lua_State *L = ctx->vm;

        lua_rawgeti(L, LUA_REGISTRYINDEX , ctx->callback);
        build_param(L, conn);

        if(lua_pcall(L, 1, 1, 0))
        {
            printf("%s\n", lua_tostring(L, -1));
            mg_send_data(conn, "", 0);
        }
        else
        {
            size_t len;
            const char* ret = luaL_tolstring(L, -1, &len);

            mg_send_data(conn, ret, len);
        }
    }
    return MG_TRUE;
}

static int mg_poll(lua_State *L)
{
    struct mg_context *ctx = luaL_checkudata(L, 1, MONGOOSE_NAME);
    mg_poll_server(ctx->server, 10);
    return 0;
}

static int mg_destroy(lua_State *L)
{
    struct mg_context *ctx = luaL_checkudata(L, 1, MONGOOSE_NAME);
    mg_destroy_server(&ctx->server);
    return 0;
}

static int mg_create(lua_State *L)
{
    const char* port = luaL_checkstring(L, 1);

    luaL_checktype(L, 2, LUA_TFUNCTION);
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);

    struct mg_context *ctx = lua_newuserdata(L, sizeof(struct mg_context) );

    struct mg_server *server = mg_create_server(ctx, event_handler);
    const char* err = mg_set_option(server, "listening_port", port);
    if(err)
    {
        luaL_error(L, "%s %s", err, port);
    }

    luaL_getmetatable(L, MONGOOSE_NAME);
    lua_setmetatable(L, -2);

    ctx->server = server;
    ctx->vm = L;
    ctx->callback = ref;

    return 1;
}

int luaopen_mongoose(lua_State *L)
{
    static const luaL_Reg mongoose_meta[] =
    {
        {"poll", mg_poll},
        {"__gc", mg_destroy},
        {NULL, NULL}
    };

    static const luaL_Reg mongoose[] =
    {
        {"create", mg_create},
        {NULL, NULL}
    };

    luaL_newmetatable(L, MONGOOSE_NAME);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, mongoose_meta, 0);

    luaL_newlib(L, mongoose);
    return 1;
}

void luaL_module(lua_State *L, const char *name, lua_CFunction f)
{
    luaL_getsubtable(L, LUA_REGISTRYINDEX, "_PRELOAD");
    lua_pushcfunction(L, f);
    lua_setfield(L, -2, name);
}
