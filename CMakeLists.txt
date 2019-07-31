cmake_minimum_required (VERSION 2.6)
project (LotteHTTP)

add_library(LotteLogs LotteLogs/LotteLogs.c)
add_library(Lua Lua5.3.2/lapi.c Lua5.3.2/lauxlib.c Lua5.3.2/lbaselib.c Lua5.3.2/lbitlib.c Lua5.3.2/lcode.c Lua5.3.2/lcorolib.c Lua5.3.2/lctype.c Lua5.3.2/ldblib.c Lua5.3.2/ldebug.c Lua5.3.2/ldo.c Lua5.3.2/ldump.c Lua5.3.2/lfunc.c Lua5.3.2/lgc.c Lua5.3.2/linit.c Lua5.3.2/liolib.c Lua5.3.2/llex.c Lua5.3.2/lmathlib.c Lua5.3.2/lmem.c Lua5.3.2/loadlib.c Lua5.3.2/lobject.c Lua5.3.2/lopcodes.c Lua5.3.2/loslib.c Lua5.3.2/lparser.c Lua5.3.2/lstate.c Lua5.3.2/lstring.c Lua5.3.2/lstrlib.c Lua5.3.2/ltable.c Lua5.3.2/ltablib.c Lua5.3.2/ltm.c Lua5.3.2/lundump.c Lua5.3.2/lutf8lib.c Lua5.3.2/lvm.c Lua5.3.2/lzio.c)

add_executable(LotteHTTP main.c)
target_link_libraries(LotteHTTP LotteLogs Lua)