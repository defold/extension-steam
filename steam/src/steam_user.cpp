#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamUser* g_SteamUser;


int SteamUser_Init(lua_State* L) {
	g_SteamUser = SteamUser();
	return 0;
}

int SteamUser_GetSteamId(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	push_CSteamID(L, g_SteamUser->GetSteamID());
	return 1;
}

int SteamUser_GetPlayerSteamLevel(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushnumber(L, g_SteamUser->GetPlayerSteamLevel());
	return 1;
}

int SteamUser_GetGameBadgeLevel(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	int series = luaL_checknumber(L, 1);
	bool foil = lua_toboolean(L, 2);
	lua_pushnumber(L, g_SteamUser->GetGameBadgeLevel(series, foil));
	return 1;
}

int SteamUser_LoggedOn(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BLoggedOn());
	return 1;
}

int SteamUser_IsBehindNAT(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsBehindNAT());
	return 1;
}

int SteamUser_IsPhoneVerified(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsPhoneVerified());
	return 1;
}

int SteamUser_IsPhoneIdentifying(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsPhoneIdentifying());
	return 1;
}

int SteamUser_IsPhoneRequiringVerification(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsPhoneRequiringVerification());
	return 1;
}

int SteamUser_IsTwoFactorEnabled(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsTwoFactorEnabled());
	return 1;
}

#endif
