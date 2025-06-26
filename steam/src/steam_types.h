#ifndef DEFOLD_STEAMTYPES_H
#define DEFOLD_STEAMTYPES_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

uint64 check_uint64(lua_State* L, int index);
int64 check_int64(lua_State* L, int index);
CSteamID check_CSteamID(lua_State* L, int index);

void push_int64(lua_State* L, int64 n);
void push_uint64(lua_State* L, uint64 n);
void push_CSteamID(lua_State* L, CSteamID steamId);
void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size);


void table_push_string(lua_State* L, const char* key, const char* value);
void table_push_number(lua_State* L, const char* key, lua_Number value);
void table_push_boolean(lua_State* L, const char* key, bool value);
void table_push_uint64(lua_State* L, const char* key, uint64 value);
void table_push_CSteamID(lua_State* L, const char* key, CSteamID value);

#endif

#endif
