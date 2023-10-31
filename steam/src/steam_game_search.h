#ifndef DEFOLD_STEAMGAMESEARCH_H
#define DEFOLD_STEAMGAMESEARCH_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamGameSearch_Init(lua_State* L);

#endif

#endif
