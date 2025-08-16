#ifndef DEFOLD_STEAMMATCHMAKING_H
#define DEFOLD_STEAMMATCHMAKING_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamMatchmaking_OnLobbyMatchList(lua_State* L, void* data);
int SteamMatchmaking_OnLobbyEnter(lua_State* L, void* data);

int SteamMatchmaking_Init(lua_State* L);
int SteamMatchmaking_RequestLobbyList(lua_State* L);
int SteamMatchmaking_GetLobbyByIndex(lua_State* L);
int SteamMatchmaking_GetLobbyOwner(lua_State* L);
int SteamMatchmaking_CreateLobby(lua_State* L);
int SteamMatchmaking_JoinLobby(lua_State* L);
int SteamMatchmaking_LeaveLobby(lua_State* L);
int SteamMatchmaking_GetNumLobbyMembers(lua_State* L);
int SteamMatchmaking_GetLobbyMemberByIndex(lua_State* L);

#endif

#endif
