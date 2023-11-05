#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamApps* g_Apps;

int SteamApps_Init(lua_State* L)
{
	g_Apps = SteamApps();
	return 0;
}

#endif
