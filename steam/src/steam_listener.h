#ifndef DEFOLD_STEAMLISTENER_H
#define DEFOLD_STEAMLISTENER_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

void SteamListener_Invoke(int (*fn)(lua_State*, void*), void* data);
void SteamListener_InvokeGeneric(const char* id);
void SteamListener_Destroy();
int SteamListener_Set(lua_State* L);

#endif

#endif
