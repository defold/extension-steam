#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"

static ISteamUtils* g_SteamUtils = 0;

extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char *pchDebugText)
{
	if (nSeverity == 0)
	{
		dmLogInfo("%s", pchDebugText);
	}
	else
	{
		dmLogWarning("%s", pchDebugText);
	}
}

int SteamUtils_Init(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	g_SteamUtils = SteamUtils();
	g_SteamUtils->SetWarningMessageHook(&SteamAPIDebugTextHook);
	return 0;
}

/** Returns the appID of the current process.
 * @name utils_get_app_id
 * @treturn Number app_id
 */
int SteamUtils_GetAppId(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushnumber(L, g_SteamUtils->GetAppID());
	return 1;
}

/** Return the number of seconds since the user.
 * @name utils_get_seconds_since_app_active
 * @treturn Number seconds
 */
int SteamUtils_GetSecondsSinceAppActive(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushnumber(L, g_SteamUtils->GetSecondsSinceAppActive());
	return 1;
}

/** Returns true if currently running on the Steam Deck device.
 * @name utils_is_steam_running_on_steam_deck
 * @treturn Boolean running_on_steamdeck
 */
int SteamUtils_IsSteamRunningOnSteamDeck(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushboolean(L, g_SteamUtils->IsSteamRunningOnSteamDeck());
	return 1;
}


/** Get size of image
 * @name utils_get_image_size
 * @number image Image handle
 * @treturn Boolean ok True if size of image was read successfully
 * @treturn Number width Image width or nil
 * @treturn Number height Image height or nil
 */
int SteamUtils_GetImageSize(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 3);

	int iImage = luaL_checknumber(L, 1);
	uint32 pnWidth = 0;
	uint32 pnHeight = 0;

	bool ok = g_SteamUtils->GetImageSize(iImage, &pnWidth, &pnHeight);
	lua_pushboolean(L, ok);
	if (ok)
	{
		lua_pushnumber(L, pnWidth);
		lua_pushnumber(L, pnWidth);
	}
	else
	{
		lua_pushnil(L);
		lua_pushnil(L);
	}
	return 3;
}

/** Get image in RGBA format.
 * @name utils_get_image_rgba
 * @number image Image handle
 * @number size Size of image
 * @treturn Boolean ok True if size of image was read successfully
 * @treturn String Image
 */
int SteamUtils_GetImageRGBA(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	int iImage = luaL_checknumber(L, 1);
	int imageSize = luaL_checknumber(L, 2);
	int nDestBufferSize = sizeof(uint8) * 4 * imageSize;
	uint8* pubDest = (uint8*)malloc(nDestBufferSize);
	bool ok = g_SteamUtils->GetImageRGBA(iImage, pubDest, nDestBufferSize);
	lua_pushboolean(L, ok);
	if (ok)
	{
		lua_pushlstring(L, (char*)pubDest, nDestBufferSize);
	}
	else
	{
		lua_pushnil(L);
	}
	free(pubDest);
	return 2;
}


/** Returns the Steam server time in Unix epoch format. (Number of seconds since Jan 1, 1970 UTC)
 * @name utils_get_server_real_time
 * @treturn number Server time
 */
int SteamUtils_GetServerRealTime(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	uint32 time = g_SteamUtils->GetServerRealTime();
	lua_pushnumber(L, time);
	return 1;
}


#endif