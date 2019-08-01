#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "LotteLogs/LotteLogs.h"
#include "Lua5.3.2/lua.h"
#include "Lua5.3.2/lualib.h"
#include "Lua5.3.2/lauxlib.h"
#include "facil.io-0.7.1/lib/facil/http/http.h"
#include "facil.io-0.7.1/lib/facil/fiobj/fiobject.h"

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void on_request(http_s *request) {
    // Generate the path to the Lua file to handle our request
    char* path = fiobj_obj2cstr(request->path).data;
    char *luaPath;
    if (strcmp(path, "") == 0) {
        luaPath = "pages/index.lua";
    } else {
        luaPath = concat(concat("pages", path), ".lua");
    }

    // Check if the Lua file does not exist
    if( access( luaPath, F_OK ) == -1 ) {
        http_send_body(request, "404 Not Found", 13);
        return;
    }

    // Initialise a Lua interpreter
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Import the file
    luaL_dofile(L, luaPath);

    // Run the doPage() function
    lua_getglobal(L, "doPage");
    lua_call(L, 0, 1);

    // Get the returning string
    char* result = (char*)lua_tostring(L, -1);
    lua_pop(L, 1);

    // Close the Lua interpreter session
    lua_close(L);

    // Get the size of our result
    int end = 0;
    int i = 0;
    while (end == 0) { if (result[i] == '\0') {end = 1;} else {i++;} }

    // Send result to client
    http_send_body(request, result, i);
}

int main(void) {
    llog_info("LotteHTTP is starting up...");

    http_listen("3000", NULL, .on_request = on_request, .log = 1);
    fio_start(.threads = 1);

    return 0;
}