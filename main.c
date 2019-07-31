#include "LotteLogs/LotteLogs.h"
#include "Lua5.3.2/lua.h"
#include "Lua5.3.2/lualib.h"
#include "Lua5.3.2/lauxlib.h"

int main(void) {
    llog_info("LotteHTTP is starting up...");

    llog_debug("Starting to init Lua");
    llog_trace("Creating Lua state and initialising interpreter");
    lua_State* L;
    L = luaL_newstate();
    llog_trace("Loading Lua libraries");
    luaL_openlibs(L);
    llog_debug("Lua is now initialised!");

    llog_debug("Running test Lua file");
    luaL_dofile(L, "test.lua");

    llog_trace("Closing Lua state");
    lua_close(L);
    llog_debug("Exiting program");
    return 0;
}