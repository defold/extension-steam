#ifndef DEFOLD_STEAMUGC_H
#define DEFOLD_STEAMUGC_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamUGC_Init(lua_State* L);

#endif

#endif
