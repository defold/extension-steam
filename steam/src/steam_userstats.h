#ifndef DEFOLD_STEAMUSERSTATS_H
#define DEFOLD_STEAMUSERSTATS_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"

int SteamUserStats_OnUserStatsReceived(lua_State* L, void* s);
int SteamUserStats_OnGlobalStatsReceived(lua_State* L, void* s);

int SteamUserStats_Init(lua_State* L);
int SteamUserStats_GetStatInt(lua_State* L);
int SteamUserStats_SetStatInt(lua_State* L);
int SteamUserStats_GetStatFloat(lua_State* L);
int SteamUserStats_SetStatFloat(lua_State* L);

int SteamUserStats_RequestCurrentStats(lua_State* L);
int SteamUserStats_RequestGlobalStats(lua_State* L);
int SteamUserStats_StoreStats(lua_State* L);
int SteamUserStats_ResetAllStats(lua_State* L);

int SteamUserStats_SetAchievement(lua_State* L);
int SteamUserStats_GetAchievement(lua_State* L);
int SteamUserStats_ClearAchievement(lua_State* L);
int SteamUserStats_GetNumAchievements(lua_State* L);

int SteamUserStats_GetAchievementName(lua_State* L);
int SteamUserStats_GetAchievementDisplayAttribute(lua_State* L);
int SteamUserStats_GetAchievementAchievedPercent(lua_State* L);

int SteamUserStats_FindLeaderboard(lua_State* L);
int SteamUserStats_FindOrCreateLeaderboard(lua_State* L);
int SteamUserStats_GetLeaderboardName(lua_State* L);
int SteamUserStats_GetLeaderboardEntryCount(lua_State* L);
int SteamUserStats_DownloadLeaderboardEntries(lua_State* L);
int SteamUserStats_GetDownloadedLeaderboardEntry(lua_State* L);
int SteamUserStats_UploadLeaderboardScore(lua_State* L);

#endif

#endif
