#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamRemoteStorage* g_SteamRemoteStorage;

int SteamRemoteStorage_Init(lua_State* L) {
	g_SteamRemoteStorage = SteamRemoteStorage();
	return 0;
}

#endif
