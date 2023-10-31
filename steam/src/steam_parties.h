#ifndef DEFOLD_STEAMPARTIES_H
#define DEFOLD_STEAMPARTIES_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamParties_Init(lua_State* L);

#endif

#endif
