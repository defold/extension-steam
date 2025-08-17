/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamParties* g_SteamParties = 0;

int SteamParties_Init(lua_State* L)
{
	g_SteamParties = SteamParties();
	return 0;
}

#endif
