#ifndef DEFOLD_STEAMMUSICREMOTE_H
#define DEFOLD_STEAMMUSICREMOTE_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamMusicRemote_Init(lua_State* L);

#endif

#endif
