#include "stdio.h"
#include "lua.hpp"
#include "tolua++.h"
int tolua_tarray_open(lua_State* tolua_S);
#include "tarray.h"
int g_Arr[10] = { 0 };

int main(int argc, char* argv[])
{
    lua_State * L = lua_open();
    int i = 0;
    for(i = 0; i < 10; i++)
		g_Arr[i] = i;

    luaopen_base(L);
    tolua_tarray_open(L);
    luaL_dofile(L, "../scripts/lua/array.lua");
    printf("now in c++, re-show Arr: ");
    for (i = 0; i < 10; i++)
		printf("%d ", g_Arr[i]);

    printf(" ");
    lua_close(L);
    return 0;
}


