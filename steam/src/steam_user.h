#ifndef DEFOLD_STEAMUSER_H
#define DEFOLD_STEAMUSER_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamUser_OnMicroTxnAuthorizationResponse(lua_State* L, void* data);

int SteamUser_Init(lua_State* L);
int SteamUser_GetSteamId(lua_State* L);
int SteamUser_GetPlayerSteamLevel(lua_State* L);
int SteamUser_GetGameBadgeLevel(lua_State* L);
int SteamUser_LoggedOn(lua_State* L);
int SteamUser_IsBehindNAT(lua_State* L);
int SteamUser_IsPhoneVerified(lua_State* L);
int SteamUser_IsPhoneIdentifying(lua_State* L);
int SteamUser_IsPhoneRequiringVerification(lua_State* L);
int SteamUser_IsTwoFactorEnabled(lua_State* L);
int SteamUser_GetAuthSessionTicket(lua_State* L);

#endif

#endif
