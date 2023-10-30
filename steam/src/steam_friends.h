#ifndef DEFOLD_STEAMFRIENDS_H
#define DEFOLD_STEAMFRIENDS_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamFriends_Init(lua_State* L);

#endif

#endif
