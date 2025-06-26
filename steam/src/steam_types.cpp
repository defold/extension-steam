#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"


/*****************************
* (u)int64 (as string)
******************************/
uint64 check_uint64(lua_State* L, int index)
{
	if(lua_isstring(L, index))
	{
		const char * s = luaL_checkstring(L, index);
		return strtoull(s, NULL, 10);
	}
	return 0;
}
int64 check_int64(lua_State* L, int index)
{
	if(lua_isstring(L, index))
	{
		const char * s = luaL_checkstring(L, index);
		return strtoll(s, NULL, 10);
	}
	return 0;
}
void check_uint64_array(lua_State* L, int index, uint64 * arr, unsigned int size)
{
	if(lua_isnil(L, index) || lua_isnone(L, index))
	{
		if(size > 0)
		{
			luaL_error(L, "Size > 0 but no table provided");
		}
		return;
	}
	if(!lua_istable(L, index))
	{
		luaL_error(L, "Not a table");
	}
	int table_size = lua_objlen(L, index);
	if(table_size > size)
	{
		table_size = size;
	}
	for(int i=1; i<=table_size; i++)
	{
		lua_pushnumber(L, i);
		lua_gettable(L, index);
		arr[i] = check_uint64(L, -1);
	}
}
void push_int64(lua_State* L, int64 n)
{
	char buf[22];
	snprintf(buf, sizeof(buf), "%lld", n);
	lua_pushstring(L, buf);
}
void push_int64_array(lua_State* L, int64 arr[], unsigned int size)
{
	lua_newtable(L);
	for(int i=1; i <= size; i++)
	{
		lua_pushnumber(L, i);
		push_int64(L, arr[i]);
		lua_settable(L, -3);
	}
}
void push_uint64(lua_State* L, uint64 n)
{
	char buf[22];
	snprintf(buf, sizeof(buf), "%llu", n);
	lua_pushstring(L, buf);
}
void push_uint64_array(lua_State* L, uint64 arr[], unsigned int size)
{
	lua_newtable(L);
	for(int i=1; i <= size; i++)
	{
		lua_pushnumber(L, i);
		push_uint64(L, arr[i]);
		lua_settable(L, -3);
	}
}



/*****************************
* CSteamID
******************************/
CSteamID check_CSteamID(lua_State* L, int index)
{
	uint64 v = check_uint64(L, index);
	return CSteamID(v);
}
void push_CSteamID(lua_State* L, CSteamID steamId)
{
	push_uint64(L, steamId.ConvertToUint64());
}
void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size)
{
	lua_newtable(L);
	for(int i=1; i <= size; i++) {
		lua_pushnumber(L, i);
		push_CSteamID(L, steamId[i]);
		lua_settable(L, -3);
	}
}



/*****************************
* PUSH table key value pairs
******************************/
void table_push_string(lua_State* L, const char* key, const char* value)
{
	lua_pushstring(L, key);
	lua_pushstring(L, value);
	lua_settable(L, -3);
}
void table_push_number(lua_State* L, const char* key, lua_Number value)
{
	lua_pushstring(L, key);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}
void table_push_boolean(lua_State* L, const char* key, bool value)
{
	lua_pushstring(L, key);
	lua_pushboolean(L, value);
	lua_settable(L, -3);
}

void table_push_uint64(lua_State* L, const char* key, uint64 value)
{
	lua_pushstring(L, key);
	push_uint64(L, value);
	lua_settable(L, -3);
}

void table_push_CSteamID(lua_State* L, const char* key, CSteamID value)
{
	lua_pushstring(L, key);
	push_CSteamID(L, value);
	lua_settable(L, -3);
}


#endif