#ifndef DEFOLD_STEAMMATCHMAKING_H
#define DEFOLD_STEAMMATCHMAKING_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamMatchmaking_OnLobbyMatchList(lua_State* L, void* data);
int SteamMatchmaking_OnLobbyEnter(lua_State* L, void* data);
int SteamMatchmaking_OnLobbyCreated(lua_State* L, void* data);
int SteamMatchmaking_OnLobbyChatMsg(lua_State* L, void* data);
int SteamMatchmaking_OnLobbyDataUpdate(lua_State* L, void* data);
int SteamMatchmaking_OnLobbyChatUpdate(lua_State* L, void* data);

int SteamMatchmaking_Init(lua_State* L);
int SteamMatchmaking_RequestLobbyList(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListStringFilter(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListNumericalFilter(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListNearValueFilter(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListFilterSlotsAvailable(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListDistanceFilter(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListResultCountFilter(lua_State* L);
int SteamMatchmaking_AddRequestLobbyListCompatibleMembersFilter(lua_State* L);
int SteamMatchmaking_GetLobbyByIndex(lua_State* L);
int SteamMatchmaking_GetLobbyOwner(lua_State* L);
int SteamMatchmaking_SetLobbyOwner(lua_State* L);
int SteamMatchmaking_SetLobbyType(lua_State* L);
int SteamMatchmaking_SetLobbyJoinable(lua_State* L);
int SteamMatchmaking_SetLobbyMemberLimit(lua_State* L);
int SteamMatchmaking_GetLobbyMemberLimit(lua_State* L);
int SteamMatchmaking_CreateLobby(lua_State* L);
int SteamMatchmaking_JoinLobby(lua_State* L);
int SteamMatchmaking_LeaveLobby(lua_State* L);
int SteamMatchmaking_GetNumLobbyMembers(lua_State* L);
int SteamMatchmaking_GetLobbyMemberByIndex(lua_State* L);
int SteamMatchmaking_SetLobbyMemberData(lua_State* L);
int SteamMatchmaking_GetLobbyMemberData(lua_State* L);
int SteamMatchmaking_SetLobbyData(lua_State* L);
int SteamMatchmaking_GetLobbyData(lua_State* L);
int SteamMatchmaking_GetLobbyDataCount(lua_State* L);
int SteamMatchmaking_GetLobbyDataByIndex(lua_State* L);
int SteamMatchmaking_SendLobbyChatMsg(lua_State* L);
int SteamMatchmaking_GetLobbyChatEntry(lua_State* L);

#endif

#endif
