#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamUserStats* g_SteamUserStats;

int SteamUserStats_Init(lua_State* L) {
	g_SteamUserStats = SteamUserStats();
	return 0;
}

int SteamUserStats_GetStatInt(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);

	const char* id = luaL_checkstring(L, 1);
	int stat = 0;
	bool ok = g_SteamUserStats->GetStat(id, &stat);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, stat);
	}
	return 1;
}

int SteamUserStats_SetStatInt(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	int stat = luaL_checknumber(L, 2);
	bool ok = g_SteamUserStats->SetStat(id, stat);
	lua_pushboolean(L, ok);
	return 1;
}

int SteamUserStats_GetStatFloat(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	float stat = 0;
	bool ok = g_SteamUserStats->GetStat(id, &stat);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, stat);
	}
	return 1;
}

int SteamUserStats_SetStatFloat(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	float stat = luaL_checknumber(L, 2);
	bool ok = g_SteamUserStats->SetStat(id, stat);
	lua_pushboolean(L, ok);
	return 1;
}

// Ask the server to send down this user's data and achievements for this game
int SteamUserStats_RequestCurrentStats(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	bool ok = g_SteamUserStats->RequestCurrentStats();
	lua_pushboolean(L, ok);
	return 1;
}

// Requests global stats data, which is available for stats marked as "aggregated".
// This call is asynchronous, with the results returned in GlobalStatsReceived_t.
// nHistoryDays specifies how many days of day-by-day history to retrieve in addition
// to the overall totals. The limit is 60.
int SteamUserStats_RequestGlobalStats(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	int historyDays = luaL_checknumber(L, 1);
	bool ok = g_SteamUserStats->RequestGlobalStats(historyDays);
	lua_pushboolean(L, ok);
	return 1;
}

// Store the current data on the server, will get a callback when set
// And one callback for every new achievement
//
// If the callback has a result of k_EResultInvalidParam, one or more stats 
// uploaded has been rejected, either because they broke constraints
// or were out of date. In this case the server sends back updated values.
// The stats should be re-iterated to keep in sync.
int SteamUserStats_StoreStats(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	bool ok = g_SteamUserStats->StoreStats();
	lua_pushboolean(L, ok);
	return 1;
}

// Reset stats 
int SteamUserStats_ResetAllStats(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	bool achievementsToo = lua_toboolean(L, 1);
	bool ok = g_SteamUserStats->ResetAllStats(achievementsToo);
	lua_pushboolean(L, ok);
	return 1;
}
int SteamUserStats_SetAchievement(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	bool ok = g_SteamUserStats->SetAchievement(name);
	lua_pushboolean(L, ok);
	return 1;
}
int SteamUserStats_GetAchievement(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	bool achieved = 0;
	bool ok = g_SteamUserStats->GetAchievement(name, &achieved);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushboolean(L, achieved);
	}
	return 1;
}
int SteamUserStats_ClearAchievement(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	bool ok = g_SteamUserStats->ClearAchievement(name);
	lua_pushboolean(L, ok);
	return 1;
}
// Used for iterating achievements. In general games should not need these functions because they should have a
// list of existing achievements compiled into them
int SteamUserStats_GetNumAchievements(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	int num = g_SteamUserStats->GetNumAchievements();
	lua_pushnumber(L, num);
	return 1;
}

// Get achievement name iAchievement in [0,GetNumAchievements)
int SteamUserStats_GetAchievementName(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	int index = luaL_checknumber(L, 1);
	const char* name = g_SteamUserStats->GetAchievementName(index);
	lua_pushstring(L, name);
	return 1;
}

// Get general attributes for an achievement. Accepts the following keys:
// - "name" and "desc" for retrieving the localized achievement name and description (returned in UTF8)
// - "hidden" for retrieving if an achievement is hidden (returns "0" when not hidden, "1" when hidden)
int SteamUserStats_GetAchievementDisplayAttribute(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	const char* key = luaL_checkstring(L, 2);
	const char* attribute = g_SteamUserStats->GetAchievementDisplayAttribute(name, key);
	lua_pushstring(L, attribute);
	return 1;
}

// Returns the percentage of users who have achieved the specified achievement.
int SteamUserStats_GetAchievementAchievedPercent(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	float percent = 0;
	bool ok = g_SteamUserStats->GetAchievementAchievedPercent(name, &percent);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, percent);
	}
	return 1;
}
int SteamUserStats_FindLeaderboard(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	SteamAPICall_t call = g_SteamUserStats->FindLeaderboard(name);
	push_uint64(L, call);
	return 1;
}
int SteamUserStats_GetLeaderboardName(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	const char* name = g_SteamUserStats->GetLeaderboardName(leaderboard);
	lua_pushstring(L, name);
	return 1;
}

// Asks the Steam back-end for a set of rows in the leaderboard.
// This call is asynchronous, with the result returned in LeaderboardScoresDownloaded_t
// LeaderboardScoresDownloaded_t will contain a handle to pull the results from GetDownloadedLeaderboardEntries() (below)
// You can ask for more entries than exist, and it will return as many as do exist.
// k_ELeaderboardDataRequestGlobal requests rows in the leaderboard from the full table, with nRangeStart & nRangeEnd in the range [1, TotalEntries]
// k_ELeaderboardDataRequestGlobalAroundUser requests rows around the current user, nRangeStart being negate
//   e.g. DownloadLeaderboardEntries( hLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -3, 3 ) will return 7 rows, 3 before the user, 3 after
// k_ELeaderboardDataRequestFriends requests all the rows for friends of the current user 
int SteamUserStats_DownloadLeaderboardEntries(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	ELeaderboardDataRequest request = (ELeaderboardDataRequest)luaL_checkinteger(L, 2);
	int start = luaL_checknumber(L, 3);
	int end = luaL_checknumber(L, 4);
	SteamAPICall_t call = g_SteamUserStats->DownloadLeaderboardEntries(leaderboard, request, start, end);
	push_uint64(L, call);
	return 1;
}

// Returns data about a single leaderboard entry
// use a for loop from 0 to LeaderboardScoresDownloaded_t::m_cEntryCount to get all the downloaded entries
// e.g.
//		void OnLeaderboardScoresDownloaded( LeaderboardScoresDownloaded_t *pLeaderboardScoresDownloaded )
//		{
//			for ( int index = 0; index < pLeaderboardScoresDownloaded->m_cEntryCount; index++ )
//			{
//				LeaderboardEntry_t leaderboardEntry;
//				int32 details[3];		// we know this is how many we've stored previously
//				GetDownloadedLeaderboardEntry( pLeaderboardScoresDownloaded->m_hSteamLeaderboardEntries, index, &leaderboardEntry, details, 3 );
//				assert( leaderboardEntry.m_cDetails == 3 );
//				...
//			}
// once you've accessed all the entries, the data will be free'd, and the SteamLeaderboardEntries_t handle will become invalid
int SteamUserStats_GetDownloadedLeaderboardEntry(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);

	SteamLeaderboardEntries_t hSteamLeaderboardEntries = check_uint64(L, 1);
	int index = luaL_checknumber(L, 2);
	int detailsMax = luaL_checknumber(L, 3);
	int32 details[detailsMax];

	LeaderboardEntry_t leaderboardEntry;
	bool ok = g_SteamUserStats->GetDownloadedLeaderboardEntry(hSteamLeaderboardEntries, index, &leaderboardEntry, details, detailsMax);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_newtable(L);
		lua_pushstring(L, "m_steamIDUser");
		push_CSteamID(L, leaderboardEntry.m_steamIDUser);
		lua_settable(L, -3);
		lua_pushstring(L, "m_nGlobalRank");
		lua_pushnumber(L, leaderboardEntry.m_nGlobalRank);
		lua_settable(L, -3);
		lua_pushstring(L, "m_nScore");
		lua_pushnumber(L, leaderboardEntry.m_nScore);
		lua_settable(L, -3);
		lua_pushstring(L, "m_cDetails");
		lua_pushnumber(L, leaderboardEntry.m_cDetails);
		lua_settable(L, -3);
		lua_pushstring(L, "m_hUGC");
		push_uint64(L, leaderboardEntry.m_hUGC);
		lua_settable(L, -3);
	}
	return 1;
}


#endif