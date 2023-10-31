#ifndef DEFOLD_STEAMNETWORKING_H
#define DEFOLD_STEAMNETWORKING_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamNetworking_Init(lua_State* L);

#endif

#endif
