#include <dmsdk/sdk.h>

void DumpStack(lua_State* L)
{
	for (int i = 1; i <= lua_gettop(L); i++)
	{
		int type = lua_type(L, i);
		const char* name = lua_typename(L, type);
		switch (type)
		{
			case LUA_TNIL:
			case LUA_TTABLE:
			case LUA_TFUNCTION:
			case LUA_TUSERDATA:
			case LUA_TTHREAD:
			case LUA_TLIGHTUSERDATA:
			default:
				dmLogInfo("%d = %s", i, name);
				break;
			case LUA_TNUMBER:
				dmLogInfo("%d = %f (%s)", i, luaL_checknumber(L, i), name);
				break;
			case LUA_TBOOLEAN:
				dmLogInfo("%d = %d (%s)", i, lua_toboolean(L, i), name);
				break;
			case LUA_TSTRING:
				dmLogInfo("%d = %s (%s)", i, luaL_checkstring(L, i), name);
				break;
		}
	}
}
