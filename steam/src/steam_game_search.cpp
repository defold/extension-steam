/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamGameSearch* g_SteamGameSearch = 0;

int SteamGameSearch_Init(lua_State* L)
{
	g_SteamGameSearch = SteamGameSearch();
	return 0;
}

#endif
