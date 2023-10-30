#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamFriends* g_SteamFriends;

int SteamFriends_Init(lua_State* L) {
	g_SteamFriends = SteamFriends();
	return 0;
}

#endif
