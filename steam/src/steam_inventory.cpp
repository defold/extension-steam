#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamInventory* g_SteamInventory = 0;

int SteamInventory_Init(lua_State* L)
{
	g_SteamInventory = SteamInventory();
	return 0;
}

#endif
