#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamScreenshots* g_SteamScreenshots = 0;

int SteamScreenshots_Init(lua_State* L)
{
	g_SteamScreenshots = SteamScreenshots();
	return 0;
}

#endif
