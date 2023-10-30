#ifndef DEFOLD_STEAMTYPES_H
#define DEFOLD_STEAMTYPES_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

uint64 check_uint64(lua_State* L, int index);
int64 check_int64(lua_State* L, int index);

void push_int64(lua_State* L, int64 n);
void push_uint64(lua_State* L, uint64 n);

void push_CSteamID(lua_State* L, CSteamID steamId);
void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size);

#endif

#endif
