#ifndef DEFOLD_STEAMFRIENDS_H
#define DEFOLD_STEAMFRIENDS_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamFriends_Init(lua_State* L);
int SteamFriends_GetPersonaName(lua_State* L);
int SteamFriends_GetPersonaState(lua_State* L);
int SteamFriends_GetFriendCount(lua_State* L);
int SteamFriends_GetFriendByIndex(lua_State* L);
int SteamFriends_GetFriendPersonaState(lua_State* L);
int SteamFriends_GetFriendSteamLevel(lua_State* L);
int SteamFriends_GetFriendPersonaName(lua_State* L);
int SteamFriends_GetFriendRelationship(lua_State* L);
int SteamFriends_GetSmallFriendAvatar(lua_State* L);
int SteamFriends_ActivateGameOverlayToStore(lua_State* L);
int SteamFriends_ActivateGameOverlayToWebPage(lua_State* L);

#endif

#endif
