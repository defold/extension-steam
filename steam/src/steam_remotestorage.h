#ifndef DEFOLD_STEAMREMOTESTORAGE_H
#define DEFOLD_STEAMREMOTESTORAGE_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamRemoteStorage_Init(lua_State* L);

int SteamRemoteStorage_FileShare(lua_State* L);
int SteamRemoteStorage_GetFileCount(lua_State* L);
int SteamRemoteStorage_GetFileNameAndSize(lua_State* L);
int SteamRemoteStorage_GetQuota(lua_State* L);
int SteamRemoteStorage_FileWrite(lua_State* L);
int SteamRemoteStorage_FileRead(lua_State* L);

#endif

#endif
