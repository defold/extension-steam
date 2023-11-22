#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamVideo* g_SteamVideo = 0;

int SteamVideo_Init(lua_State* L)
{
	g_SteamVideo = SteamVideo();
	return 0;
}

#endif
