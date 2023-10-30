#ifndef DEFOLD_STEAMTYPES_H
#define DEFOLD_STEAMTYPES_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

void push_CSteamID(lua_State* L, CSteamID steamId);
void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size);

#endif

#endif
