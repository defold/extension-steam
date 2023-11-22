#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamNetworking* g_SteamNetworking = 0;

int SteamNetworking_Init(lua_State* L)
{
	g_SteamNetworking = SteamNetworking();
	return 0;
}

#endif
