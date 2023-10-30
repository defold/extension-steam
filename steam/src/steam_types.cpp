#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"



/*****************************
* PUSH (u)int64 (to strings)
******************************/
static void push_int64_t(lua_State* L, int64_t n) {
	char buf[22];
	snprintf(buf, sizeof(buf), "%lld", n);
	lua_pushstring(L, buf);
}
static void push_int64(lua_State* L, int64 n) {
	char buf[22];
	snprintf(buf, sizeof(buf), "%lld", n);
	lua_pushstring(L, buf);
}
static void push_int64_array(lua_State* L, int64 arr[], unsigned int size) {
	lua_newtable(L);
	for(int i=1; i <= size; i++) {
		lua_pushnumber(L, i);
		push_int64(L, arr[i]);
		lua_settable(L, -3);
	}
}
static void push_uint64(lua_State* L, uint64 n) {
	char buf[22];
	snprintf(buf, sizeof(buf), "%llu", n);
	lua_pushstring(L, buf);
}
static void push_uint64_array(lua_State* L, uint64 arr[], unsigned int size) {
	lua_newtable(L);
	for(int i=1; i <= size; i++) {
		lua_pushnumber(L, i);
		push_uint64(L, arr[i]);
		lua_settable(L, -3);
	}
}


/*****************************
* PUSH CSteamID
******************************/
void push_CSteamID(lua_State* L, CSteamID steamId) {
	push_uint64(L, steamId.ConvertToUint64());
}
void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size) {
	lua_newtable(L);
	for(int i=1; i <= size; i++) {
		lua_pushnumber(L, i);
		push_CSteamID(L, steamId[i]);
		lua_settable(L, -3);
	}
}

#endif