cmake_minimum_required (VERSION 2.6)
project (LotteHTTP)

add_subdirectory(facil.io-0.7.1)
add_library(LotteLogs LotteLogs/LotteLogs.c)
add_library(Lua Lua5.3.5/lapi.c Lua5.3.5/lauxlib.c Lua5.3.5/lbaselib.c Lua5.3.5/lbitlib.c Lua5.3.5/lcode.c Lua5.3.5/lcorolib.c Lua5.3.5/lctype.c Lua5.3.5/ldblib.c Lua5.3.5/ldebug.c Lua5.3.5/ldo.c Lua5.3.5/ldump.c Lua5.3.5/lfunc.c Lua5.3.5/lgc.c Lua5.3.5/linit.c Lua5.3.5/liolib.c Lua5.3.5/llex.c Lua5.3.5/lmathlib.c Lua5.3.5/lmem.c Lua5.3.5/loadlib.c Lua5.3.5/lobject.c Lua5.3.5/lopcodes.c Lua5.3.5/loslib.c Lua5.3.5/lparser.c Lua5.3.5/lstate.c Lua5.3.5/lstring.c Lua5.3.5/lstrlib.c Lua5.3.5/ltable.c Lua5.3.5/ltablib.c Lua5.3.5/ltm.c Lua5.3.5/lundump.c Lua5.3.5/lutf8lib.c Lua5.3.5/lvm.c Lua5.3.5/lzio.c)

add_executable(LotteHTTP main.c)
target_link_libraries(LotteHTTP LotteLogs Lua facil.io)