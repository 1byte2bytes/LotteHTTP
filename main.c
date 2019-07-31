#include "LotteLogs/LotteLogs.h"
#include "Lua5.3.2/lua.h"
#include "Lua5.3.2/lualib.h"
#include "Lua5.3.2/lauxlib.h"
#include "facil.io-0.7.1/lib/facil/http/http.h"

lua_State* L;

void on_request(http_s *request) {
    http_send_body(request, "Hello World!\r\n", 14);
}

int main(void) {
    llog_info("LotteHTTP is starting up...");

    llog_debug("Starting to init Lua");
    llog_trace("Creating Lua state and initialising interpreter");
    L = luaL_newstate();
    llog_trace("Loading Lua libraries");
    luaL_openlibs(L);
    llog_debug("Lua is now initialised!");

    llog_debug("Running test Lua file");
    luaL_dofile(L, "test.lua");

    llog_debug("Starting facil.io server");
    llog_trace("Facil.io listen");
    http_listen("3000", NULL, .on_request = on_request, .log = 1);
    llog_trace("Facil.io run");
    llog_info("LotteHTTP started, listening to port 3000!");
    fio_start(.threads = 1);

    llog_trace("Closing Lua state");
    lua_close(L);
    llog_debug("Exiting program");
    return 0;
}