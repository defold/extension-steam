#ifndef DEFOLD_STEAMMATCHMAKING_H
#define DEFOLD_STEAMMATCHMAKING_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamMatchmaking_Init(lua_State* L);

#endif

#endif
