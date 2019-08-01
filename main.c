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
    if (strcmp(path, "/") == 0) {
        luaPath = "pages/index.lua";
    } else {
        luaPath = concat(concat("pages", path), ".lua");
    }

    // Check if the Lua file does not exist
    if( access( luaPath, F_OK ) == -1 ) {
        http_send_error(request, 404);
        return;
    }

    // Initialise a Lua interpreter
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // Import the file
    luaL_dofile(L, luaPath);

    // Run the doPage() function
    lua_getglobal(L, "doPage");
    lua_call(L, 0, 2);

    // Get the returning strings
    char* mime = (char*)lua_tostring(L, -1);
    char* result = (char*)lua_tostring(L, -2);
    lua_pop(L, 2);

    // Close the Lua interpreter session
    lua_close(L);

    // Get the size of our body result
    int end = 0;
    int resultlen = 0;
    while (end == 0) { if (result[resultlen] == '\0') {end = 1;} else {resultlen++;} }

    // Get the size of our mime result
    end = 0;
    int mimelen = 0;
    while (end == 0) { if (mime[mimelen] == '\0') {end = 1;} else {mimelen++;} }

    // Send result to client
    FIOBJ SERVER = fiobj_str_new("server", 6);
    FIOBJ SERVER_STR = fiobj_str_new("LotteHTTP/1.0.0", 15);
    http_set_header(request, HTTP_HEADER_CONTENT_TYPE, http_mimetype_find(mime, mimelen));
    http_set_header(request, SERVER, SERVER_STR);
    http_send_body(request, result, resultlen);
}

int main(void) {
    llog_info("LotteHTTP is starting up...");

    http_listen("3000", NULL, .on_request = on_request, .log = 1);
    fio_start(.threads = 1);

    return 0;
}