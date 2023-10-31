#ifndef DEFOLD_STEAMINPUT_H
#define DEFOLD_STEAMINPUT_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamInput_Init(lua_State* L);

#endif

#endif
