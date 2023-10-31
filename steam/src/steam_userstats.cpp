/** Provides functions for accessing and submitting stats, achievements, and
 * leaderboards.
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"
#include "steam_listener.h"


int SteamUserStats_OnLeaderboardFindResult(lua_State* L, void* data) {
	LeaderboardFindResult_t* s = (LeaderboardFindResult_t*)data;
	lua_pushstring(L, "LeaderboardFindResult_t");

	lua_newtable(L);
	lua_pushstring(L, "m_hSteamLeaderboard");
	push_uint64(L, s->m_hSteamLeaderboard);
	lua_settable(L, -3);
	lua_pushstring(L, "m_bLeaderboardFound");
	lua_pushboolean(L, s->m_bLeaderboardFound);
	lua_settable(L, -3);

	return 2;
}

int SteamUserStats_OnGlobalStatsReceived(lua_State* L, void* data) {
	GlobalStatsReceived_t* s = (GlobalStatsReceived_t*)data;
	lua_pushstring(L, "GlobalStatsReceived_t");
	return 1;
}

int SteamUserStats_OnUserStatsReceived(lua_State* L, void* data)
{
	UserStatsReceived_t* s = (UserStatsReceived_t*)data;
	lua_pushstring(L, "UserStatsReceived_t");

	lua_newtable(L);
	lua_pushstring(L, "m_nGameID");
	push_uint64(L, s->m_nGameID);
	lua_settable(L, -3);
	lua_pushstring(L, "m_eResult");
	lua_pushnumber(L, s->m_eResult);
	lua_settable(L, -3);
	lua_pushstring(L, "m_steamIDUser");
	push_CSteamID(L, s->m_steamIDUser);
	lua_settable(L, -3);

	return 2;
}

int SteamUserStats_OnLeaderboardScoresDownloaded(lua_State* L, void* data)
{
	LeaderboardScoresDownloaded_t* s = (LeaderboardScoresDownloaded_t*)data;
	lua_pushstring(L, "LeaderboardScoresDownloaded_t");

	lua_newtable(L);
	lua_pushstring(L, "m_hSteamLeaderboard");
	push_uint64(L, s->m_hSteamLeaderboard);
	lua_settable(L, -3);
	lua_pushstring(L, "m_hSteamLeaderboardEntries");
	push_uint64(L, s->m_hSteamLeaderboardEntries);
	lua_settable(L, -3);
	lua_pushstring(L, "m_cEntryCount");
	lua_pushnumber(L, s->m_cEntryCount);
	lua_settable(L, -3);

	return 2;
}

class SteamUserStatsCallbacks
{
	public:
		SteamUserStatsCallbacks();
		STEAM_CALLBACK(SteamUserStatsCallbacks, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamUserStatsCallbacks, OnGlobalStatsReceived, GlobalStatsReceived_t, m_CallbackGlobalStatsReceived);

		CCallResult<SteamUserStatsCallbacks, UserStatsReceived_t> m_CallResultUserStatsReceived_t;
		void TrackSteamAPICallUserStatsReceived_t(SteamAPICall_t steamAPICall) {
			m_CallResultUserStatsReceived_t.Set(steamAPICall, this, &SteamUserStatsCallbacks::OnUserStatsReceived);
		}
		void OnUserStatsReceived(UserStatsReceived_t *pResult, bool bIOFailure) {
			SteamListener_Invoke(SteamUserStats_OnUserStatsReceived, pResult);
		}

		CCallResult<SteamUserStatsCallbacks, LeaderboardFindResult_t> m_CallResultLeaderboardFindResult_t;
		void TrackSteamAPICallLeaderboardFindResult_t(SteamAPICall_t steamAPICall) {
			m_CallResultLeaderboardFindResult_t.Set(steamAPICall, this, &SteamUserStatsCallbacks::OnLeaderboardFindResult);
		}
		void OnLeaderboardFindResult(LeaderboardFindResult_t *pResult, bool bIOFailure) {
			SteamListener_Invoke(SteamUserStats_OnLeaderboardFindResult, pResult);
		}

		CCallResult<SteamUserStatsCallbacks, LeaderboardScoresDownloaded_t> m_CallResultLeaderboardScoresDownloadResult_t;
		void TrackSteamAPICallLeaderboardScoresDownloaded_t(SteamAPICall_t steamAPICall) {
			m_CallResultLeaderboardScoresDownloadResult_t.Set(steamAPICall, this, &SteamUserStatsCallbacks::OnLeaderboardScoresDownloaded);
		}
		void OnLeaderboardScoresDownloaded(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure) {
			SteamListener_Invoke(SteamUserStats_OnLeaderboardScoresDownloaded, pResult);
		}
};
SteamUserStatsCallbacks::SteamUserStatsCallbacks() :
	m_CallbackUserStatsReceived(this, &SteamUserStatsCallbacks::OnUserStatsReceived),
	m_CallbackGlobalStatsReceived(this, &SteamUserStatsCallbacks::OnGlobalStatsReceived)
{}
void SteamUserStatsCallbacks::OnUserStatsReceived(UserStatsReceived_t *s)
{
	SteamListener_Invoke(SteamUserStats_OnUserStatsReceived, s);
}
void SteamUserStatsCallbacks::OnGlobalStatsReceived(GlobalStatsReceived_t *s)
{
	SteamListener_Invoke(SteamUserStats_OnGlobalStatsReceived, s);
}


static SteamUserStatsCallbacks* g_SteamUserStatsCallbacks = new SteamUserStatsCallbacks();

static ISteamUserStats* g_SteamUserStats;

int SteamUserStats_Init(lua_State* L) {
	g_SteamUserStats = SteamUserStats();
	return 0;
}


/** Get user stat as an integer.
 * @name user_stats_get_stat_int
 * @string id Id of the stat to get
 * @treturn ok Boolean
 * @treturn stat Number The stat or nil
 */
int SteamUserStats_GetStatInt(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 2);

	const char* id = luaL_checkstring(L, 1);
	int stat = 0;
	bool ok = g_SteamUserStats->GetStat(id, &stat);
	lua_pushboolean(L, ok);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, stat);
	}
	return 2;
}

/** Set user stat.
 * @name user_stats_set_stat_int
 * @string id Id of the stat to set
 * @number stat Number to set
 * @treturn boolean ok
 */
int SteamUserStats_SetStatInt(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	int stat = luaL_checknumber(L, 2);
	bool ok = g_SteamUserStats->SetStat(id, stat);
	lua_pushboolean(L, ok);
	return 1;
}

/** Get user stat as a floating point number.
 * @name user_stats_get_stat_float
 * @string id Id of the stat to get
 * @treturn Boolean ok
 * @treturn Number stat The stat
 */
int SteamUserStats_GetStatFloat(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 2);
	const char* id = luaL_checkstring(L, 1);
	float stat = 0;
	bool ok = g_SteamUserStats->GetStat(id, &stat);
	lua_pushboolean(L, ok);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, stat);
	}
	return 2;
}

/** Set user stat.
 * @name user_stats_set_stat_float
 * @string id Id of the stat to set
 * @number stat Number to set
 * @treturn Boolean ok
 */
int SteamUserStats_SetStatFloat(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	float stat = luaL_checknumber(L, 2);
	bool ok = g_SteamUserStats->SetStat(id, stat);
	lua_pushboolean(L, ok);
	return 1;
}

// 

/** Ask the server to send down this user's data and achievements for this game.
 * @treturn Boolean ok True if successful
 */
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
	DM_LUA_STACK_CHECK(L, 2);
	const char* name = luaL_checkstring(L, 1);
	bool achieved = 0;
	bool ok = g_SteamUserStats->GetAchievement(name, &achieved);
	lua_pushboolean(L, ok);
	if (!ok)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_pushboolean(L, achieved);
	}
	return 2;
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
	DM_LUA_STACK_CHECK(L, 2);
	const char* name = luaL_checkstring(L, 1);
	float percent = 0;
	bool ok = g_SteamUserStats->GetAchievementAchievedPercent(name, &percent);
	lua_pushboolean(L, ok);
	if (!ok) {
		lua_pushnil(L);
	}
	else {
		lua_pushnumber(L, percent);
	}
	return 2;
}
int SteamUserStats_FindLeaderboard(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	SteamAPICall_t call = g_SteamUserStats->FindLeaderboard(name);
	g_SteamUserStatsCallbacks->TrackSteamAPICallLeaderboardFindResult_t(call);
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
	g_SteamUserStatsCallbacks->TrackSteamAPICallLeaderboardScoresDownloaded_t(call);
	push_uint64(L, call);
	return 1;
}

/** Returns data about a single leaderboard entry
 * @name user_stats_get_downloaded_leaderboard_entry
 * @string leaderboard Leaderboard handle
 * @number index Which entry to get
 * @treturn Boolean ok
 * @treturn Table entry The requested leaderboard entry.
 */
int SteamUserStats_GetDownloadedLeaderboardEntry(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 2);

	SteamLeaderboardEntries_t hSteamLeaderboardEntries = check_uint64(L, 1);
	int index = luaL_checknumber(L, 2);
	int detailsMax = luaL_checknumber(L, 3);
	int32 details[detailsMax];

	LeaderboardEntry_t leaderboardEntry;
	bool ok = g_SteamUserStats->GetDownloadedLeaderboardEntry(hSteamLeaderboardEntries, index, &leaderboardEntry, details, detailsMax);
	lua_pushboolean(L, ok);
	if (!ok)
	{
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
	return 2;
}


#endif