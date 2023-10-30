#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"

static ISteamUtils *utils;

extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText )
{
	if (nSeverity == 0) {
		dmLogInfo("%s", pchDebugText);
	}
	else {
		dmLogWarning("%s", pchDebugText);
	}
}

int SteamUtils_Init(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	utils = SteamUtils();
	utils->SetWarningMessageHook(&SteamAPIDebugTextHook);
	return 0;
}

int SteamUtils_GetAppId(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushnumber(L, utils->GetAppID());
	return 1;
}

int SteamUtils_GetSecondsSinceAppActive(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushnumber(L, utils->GetSecondsSinceAppActive());
	return 1;
}

int SteamUtils_IsSteamRunningOnSteamDeck(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushboolean(L, utils->IsSteamRunningOnSteamDeck());
	return 1;
}

#endif