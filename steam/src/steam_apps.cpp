/**
 * @document
 * @namespace steam
 */

 #if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamApps* g_SteamApps = 0;

int SteamApps_Init(lua_State* L)
{
	g_SteamApps = SteamApps();
	return 0;
}

/** Takes AppID of DLC and checks if the user owns the DLC & if the DLC is installed.
 * @name apps_is_dlc_installed
 * @number app_id
 * @treturn bool installed
 */
int SteamApps_IsDlcInstalled(lua_State* L)
{
	if (!g_SteamApps) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	AppId_t appID = luaL_checknumber(L, 1);
	bool installed = g_SteamApps->BIsDlcInstalled(appID);
	lua_pushboolean(L, installed);
	return 1;
}

#endif
