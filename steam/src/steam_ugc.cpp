/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamUGC* g_SteamUGC = 0;

int SteamUGC_Init(lua_State* L)
{
	g_SteamUGC = SteamUGC();
	return 0;
}

#endif
