#ifndef DEFOLD_STEAMAPPS_H
#define DEFOLD_STEAMAPPS_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamApps_Init(lua_State* L);

#endif

#endif
