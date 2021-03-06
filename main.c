/*
 * File: main.c
 * 
 * MIT License
 * 
 * Copyright (c) 2019 Sydney "Lotte" Erickson
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "LotteLogs/LotteLogs.h"
#include "Lua5.3.5/lua.h"
#include "Lua5.3.5/lualib.h"
#include "Lua5.3.5/lauxlib.h"
#include "facil.io-0.7.3/lib/facil/http/http.h"
#include "facil.io-0.7.3/lib/facil/fiobj/fiobject.h"

// Define a handy function to get the size of a static string
#define strsizeof(str) (sizeof(str) - 1)
#define PATH_MAX_LEN 256

static FIOBJ HTTP_HEADER_SERVER;
static FIOBJ HTTP_HEADER_SERVER_VALUE;

// TODO: there's a memory leak somewhere, probably
void on_request(http_s *request) {
    // Generate the path to the Lua file to handle our request
    llog_trace("Check path");
    // TODO: sanatise path (make sure it doesn't escape the pages folder, etc)
    char* path = fiobj_obj2cstr(request->path).data;
    // The actual max path is MAX_LENGTH-1 (ex. 256-1), since it needs to be null terminated
    // TODO: take into account "pages" and ".lua"
    if(strlen(path) > PATH_MAX_LEN - 1) {
        http_send_error(request, 500);
        return;
    }
    char luaPath[PATH_MAX_LEN];
    memset(luaPath, 0, sizeof(luaPath));
    if (strcmp(path, "/") == 0) {
        // sizeof is useful here because we do want to include the null
        // character in the length
        snprintf(luaPath, sizeof(luaPath), "pages/index.lua");
    } else {
        // Combine "pages", path, and ".lua" to create our target path
        // The path variable should always start with a /
        snprintf(luaPath, sizeof(luaPath), "%s%s%s", "pages", path, ".lua");
    }
    llog_trace(luaPath);

    // Check if the Lua file does not exist
    llog_trace("Check exists");
    if (access(luaPath, F_OK) == -1) {
        http_send_error(request, 404);
        return;
    }

     // Check if it's actually a file, not a folder
    struct stat path_stat;
    stat(luaPath, &path_stat);
    if (!S_ISREG(path_stat.st_mode)) {
        http_send_error(request, 404);
        return;
    }

    // Initialise a Lua interpreter
    llog_trace("Init Lua");
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Import the file
    llog_trace("Run file");
    luaL_dofile(L, luaPath);

    // Run the doPage() function
    llog_trace("Run func");
    lua_getglobal(L, "doPage");
    lua_call(L, 0, 2);

    // Get the returning strings
    llog_trace("Parse returns");
    char* mime = (char*)lua_tostring(L, -1);
    char* result = (char*)lua_tostring(L, -2);
    lua_pop(L, 2);

    // Close the Lua interpreter session
    llog_trace("Cleanup Lua");
    lua_close(L);

    // Send result to client
    llog_trace("Return results");
    http_set_header(request, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find(mime, strlen(mime)));
    // TODO: not run fiobj_str_new every request for a static header
    http_set_header(
        request, 
        HTTP_HEADER_SERVER, 
        fiobj_dup(HTTP_HEADER_SERVER_VALUE)
    );
    http_send_body(request, result, strlen(result));
}

int main(void) {
    llog_info("LotteHTTP is starting up...");

    HTTP_HEADER_SERVER = fiobj_str_new("server", strsizeof("server"));
    HTTP_HEADER_SERVER_VALUE = fiobj_str_new("LotteHTTP/0.9.0", strsizeof("LotteHTTP/0.9.0"));

    http_listen("3000", NULL, .on_request = on_request, .log = 1);
    fio_start(.threads = 1);

    return 0;
}