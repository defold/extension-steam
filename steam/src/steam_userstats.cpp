/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"


int SteamUserStats_OnLeaderboardFindResult(lua_State* L, void* data)
{
	LeaderboardFindResult_t* s = (LeaderboardFindResult_t*)data;
	lua_pushstring(L, "LeaderboardFindResult_t");

	lua_newtable(L);
	table_push_uint64(L, "m_hSteamLeaderboard", s->m_hSteamLeaderboard);
	table_push_boolean(L, "m_bLeaderboardFound", s->m_bLeaderboardFound);

	return 2;
}

int SteamUserStats_OnGlobalStatsReceived(lua_State* L, void* data)
{
	GlobalStatsReceived_t* s = (GlobalStatsReceived_t*)data;
	lua_pushstring(L, "GlobalStatsReceived_t");
	return 1;
}

int SteamUserStats_OnUserStatsReceived(lua_State* L, void* data)
{
	UserStatsReceived_t* s = (UserStatsReceived_t*)data;
	lua_pushstring(L, "UserStatsReceived_t");

	lua_newtable(L);
	table_push_uint64(L, "m_nGameID", s->m_nGameID);
	table_push_integer(L, "m_eResult", s->m_eResult);
	table_push_CSteamID(L, "m_steamIDUser", s->m_steamIDUser);

	return 2;
}

int SteamUserStats_OnLeaderboardScoresDownloaded(lua_State* L, void* data)
{
	LeaderboardScoresDownloaded_t* s = (LeaderboardScoresDownloaded_t*)data;
	lua_pushstring(L, "LeaderboardScoresDownloaded_t");

	lua_newtable(L);
	table_push_uint64(L, "m_hSteamLeaderboard", s->m_hSteamLeaderboard);
	table_push_uint64(L, "m_hSteamLeaderboardEntries", s->m_hSteamLeaderboardEntries);
	table_push_integer(L, "m_cEntryCount", s->m_cEntryCount);

	return 2;
}

int SteamUserStats_OnLeaderboardScoreUploaded(lua_State* L, void* data)
{
	LeaderboardScoreUploaded_t* s = (LeaderboardScoreUploaded_t*)data;
	lua_pushstring(L, "LeaderboardScoreUploaded_t");

	lua_newtable(L);
	table_push_uint64(L, "m_hSteamLeaderboard", s->m_hSteamLeaderboard);
	table_push_integer(L, "m_nScore", s->m_nScore);
	table_push_boolean(L, "m_bScoreChanged", s->m_bScoreChanged);
	table_push_integer(L, "m_nGlobalRankNew", s->m_nGlobalRankNew);
	table_push_integer(L, "m_nGlobalRankPrevious", s->m_nGlobalRankPrevious);

	return 2;
}

int SteamUserStats_OnLeaderboardUGCSet(lua_State* L, void* data)
{
	LeaderboardUGCSet_t* s = (LeaderboardUGCSet_t*)data;
	lua_pushstring(L, "LeaderboardUGCSet_t");

	lua_newtable(L);
	table_push_uint64(L, "m_hSteamLeaderboard", s->m_hSteamLeaderboard);
	table_push_integer(L, "m_eResult", s->m_eResult);

	return 2;
}


static ISteamUserStats* g_SteamUserStats = 0;

int SteamUserStats_Init(lua_State* L)
{
	g_SteamUserStats = SteamUserStats();
	return 0;
}


/** Get user stat as an integer.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat
 * @name user_stats_get_stat_int
 * @string id Id of the stat to get
 * @treturn ok Boolean
 * @treturn stat Number The stat or nil
 */
int SteamUserStats_GetStatInt(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	const char* id = luaL_checkstring(L, 1);
	int stat = 0;
	bool ok = g_SteamUserStats->GetStat(id, &stat);
	lua_pushboolean(L, ok);
	if (!ok)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_pushinteger(L, stat);
	}
	return 2;
}

/** Set user stat.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat
 * @name user_stats_set_stat_int
 * @string id Id of the stat to set
 * @number stat Number to set
 * @treturn boolean ok
 */
int SteamUserStats_SetStatInt(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	int stat = luaL_checknumber(L, 2);
	bool ok = g_SteamUserStats->SetStat(id, stat);
	lua_pushboolean(L, ok);
	return 1;
}

/** Get user stat as a floating point number.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat
 * @name user_stats_get_stat_float
 * @string id Id of the stat to get
 * @treturn Boolean ok
 * @treturn Number stat The stat
 */
int SteamUserStats_GetStatFloat(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	const char* id = luaL_checkstring(L, 1);
	float stat = 0;
	bool ok = g_SteamUserStats->GetStat(id, &stat);
	lua_pushboolean(L, ok);
	if (!ok)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_pushnumber(L, stat);
	}
	return 2;
}

/** Set user stat.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat
 * @name user_stats_set_stat_float
 * @string id Id of the stat to set
 * @number stat Number to set
 * @treturn Boolean ok
 */
int SteamUserStats_SetStatFloat(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* id = luaL_checkstring(L, 1);
	float stat = luaL_checknumber(L, 2);
	bool ok = g_SteamUserStats->SetStat(id, stat);
	lua_pushboolean(L, ok);
	return 1;
}

int SteamUserStats_RequestCurrentStats(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	dmLogInfo("SteamUserStats_RequestCurrentStats is no longer needed");
	return 0;
}

/** Requests global stats data, which is available for stats marked as "aggregated".
 * This call is asynchronous, with the results returned in GlobalStatsReceived_t.
 * nHistoryDays specifies how many days of day-by-day history to retrieve in addition
 * to the overall totals. The limit is 60.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalStats
 * @name user_stats_request_global_stats
 * @number history_days
 * @treturn Boolean ok 
 */
int SteamUserStats_RequestGlobalStats(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int historyDays = luaL_checknumber(L, 1);
	bool ok = g_SteamUserStats->RequestGlobalStats(historyDays);
	lua_pushboolean(L, ok);
	return 1;
}

/** Store the current data on the server.
 * Will get a callback when set and one callback for every new achievement
 *
 * If the callback has a result of k_EResultInvalidParam, one or more stats 
 * uploaded has been rejected, either because they broke constraints
 * or were out of date. In this case the server sends back updated values.
 * The stats should be re-iterated to keep in sync.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats
 * @name user_stats_store_stats
 * @treturn Boolean ok 
 */
int SteamUserStats_StoreStats(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	bool ok = g_SteamUserStats->StoreStats();
	lua_pushboolean(L, ok);
	return 1;
}

/** Reset stats.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats
 * @name user_stats_reset_all_stats
 * @boolean achievements True if achievements should be reset as well.
 * @treturn Boolean ok
 */
int SteamUserStats_ResetAllStats(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	bool achievementsToo = lua_toboolean(L, 1);
	bool ok = g_SteamUserStats->ResetAllStats(achievementsToo);
	lua_pushboolean(L, ok);
	return 1;
}

/** Set achievement.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement
 * @name user_stats_set_achievement
 * @string name
 * @treturn Boolean ok
 */
int SteamUserStats_SetAchievement(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	bool ok = g_SteamUserStats->SetAchievement(name);
	lua_pushboolean(L, ok);
	return 1;
}

/** Get achievement.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement
 * @name user_stats_get_achievement
 * @string name
 * @treturn Boolean ok
 * @treturn Boolean achieved
 */
int SteamUserStats_GetAchievement(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
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

/** Clear achievement.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement
 * @name user_stats_clear_achievement
 * @string name
 * @treturn Boolean ok
 */
int SteamUserStats_ClearAchievement(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	bool ok = g_SteamUserStats->ClearAchievement(name);
	lua_pushboolean(L, ok);
	return 1;
}

/** Used for iterating achievements.
 * In general games should not need these functions because they should have a
 * list of existing achievements compiled into them.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumAchievements
 * @name user_stats_get_num_achievements
 * @treturn Number num Number of achievements.
 */
int SteamUserStats_GetNumAchievements(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int num = g_SteamUserStats->GetNumAchievements();
	lua_pushinteger(L, num);
	return 1;
}

/** Get achievement name iAchievement in [0,GetNumAchievements)
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementName
 * @name user_stats_get_achievement_name
 * @number index
 * @treturn String name
 */
int SteamUserStats_GetAchievementName(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int index = luaL_checknumber(L, 1);
	const char* name = g_SteamUserStats->GetAchievementName(index);
	lua_pushstring(L, name);
	return 1;
}

/** Get general attributes for an achievement.
 * Accepts the following keys
 * * "name" and "desc" for retrieving the localized achievement name and description (returned in UTF8)
 * * "hidden" for retrieving if an achievement is hidden (returns "0" when not hidden, "1" when hidden)
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementDisplayAttribute
 * @name user_stats_get_achievement_display_attribute
 * @string name
 * @string key Either "name", "desc" or "hidden"
 * @treturn String attribute
 */
int SteamUserStats_GetAchievementDisplayAttribute(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = luaL_checkstring(L, 1);
	const char* key = luaL_checkstring(L, 2);
	const char* attribute = g_SteamUserStats->GetAchievementDisplayAttribute(name, key);
	lua_pushstring(L, attribute);
	return 1;
}

/** Returns the percentage of users who have achieved the specified achievement.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAchievedPercent
 * @name user_stats_get_achievement_achieved_percent
 * @treturn Boolean ok
 * @treturn Number percent
 */
int SteamUserStats_GetAchievementAchievedPercent(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	const char* name = luaL_checkstring(L, 1);
	float percent = 0;
	bool ok = g_SteamUserStats->GetAchievementAchievedPercent(name, &percent);
	lua_pushboolean(L, ok);
	if (!ok)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_pushnumber(L, percent);
	}
	return 2;
}

/**
 * Find a leaderboard.
 * Will return leaderboard asynchronously.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#FindLeaderboard
 * @name user_stats_find_leaderboard
 * @string name
 */
int SteamUserStats_FindLeaderboard(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	const char* name = luaL_checkstring(L, 1);
	g_SteamUserStats->FindLeaderboard(name);
	return 0;
}

/** Gets a leaderboard by name, it will create it if it's not yet created.
 * This call is asynchronous, with the result returned in a listener callback
 * with event set to LeaderboardFindResult_t.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#FindOrCreateLeaderboard
 * @name user_stats_find_or_create_leaderboard
 * @string leaderboard_name The name of the leaderboard to find or create.
 * @tparam ELeaderboardSortMethod eLeaderboardSortMethod The sort order of the new leaderboard if it's created.
 * @tparam ELeaderboardDisplayType eLeaderboardDisplayType The display type (used by the Steam Community web site) of the new leaderboard if it's created.
 */
int SteamUserStats_FindOrCreateLeaderboard(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 0);

	const char* leaderboardName = luaL_checkstring(L, 1);
	ELeaderboardSortMethod eLeaderboardSortMethod = (ELeaderboardSortMethod)luaL_checknumber(L, 2);
	ELeaderboardDisplayType eLeaderboardDisplayType = (ELeaderboardDisplayType)luaL_checknumber(L, 3);

	g_SteamUserStats->FindOrCreateLeaderboard(leaderboardName, eLeaderboardSortMethod, eLeaderboardDisplayType);
	return 0;
}

/** Get the name of a leaderboard.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardName
 * @name user_stats_get_leaderboard_name
 * @string leaderboard
 * @treturn string name
 */
int SteamUserStats_GetLeaderboardName(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	const char* name = g_SteamUserStats->GetLeaderboardName(leaderboard);
	lua_pushstring(L, name);
	return 1;
}

/** Get the total number of entries in a leaderboard, as of the last request.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardEntryCount
 * @name user_stats_get_leaderboard_entry_count
 * @string leaderboard
 * @treturn number count
 */
int SteamUserStats_GetLeaderboardEntryCount(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	int count = g_SteamUserStats->GetLeaderboardEntryCount(leaderboard);
	lua_pushinteger(L, count);
	return 1;
}

/** Returns the sort method of the leaderboard
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardSortMethod
 * @name user_stats_get_leaderboard_sort_method
 * @string leaderboard
 * @treturn number sort order
 */
int SteamUserStats_GetLeaderboardSortMethod(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	ELeaderboardSortMethod order = g_SteamUserStats->GetLeaderboardSortMethod(leaderboard);
	lua_pushinteger(L, order);
	return 1;
}


/** Returns the display type of a leaderboard handle.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardDisplayType
 * @name user_stats_get_leaderboard_display_type
 * @string leaderboard
 * @treturn number display type
 */
int SteamUserStats_GetLeaderboardDisplayType(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	ELeaderboardDisplayType displayType = g_SteamUserStats->GetLeaderboardDisplayType(leaderboard);
	lua_pushinteger(L, displayType);
	return 1;
}

/** Asks the Steam back-end for a set of rows in the leaderboard.
 * This call is asynchronous, with the result returned in a listener callback
 * with event set to LeaderboardScoresDownloaded_t.
 * LeaderboardScoresDownloaded_t will contain a handle to pull the results from
 * GetDownloadedLeaderboardEntries().
 * You can ask for more entries than exist, and it will return as many as do
 * exist.
 * * k_ELeaderboardDataRequestGlobal requests rows in the leaderboard from the full table, with nRangeStart & nRangeEnd in the range [1, TotalEntries]
 * * k_ELeaderboardDataRequestGlobalAroundUser requests rows around the current user, nRangeStart being negate
 *   e.g. DownloadLeaderboardEntries( hLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -3, 3 ) will return 7 rows, 3 before the user, 3 after
 * * k_ELeaderboardDataRequestFriends requests all the rows for friends of the current user 
 * https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntries
 * @name user_stats_download_leaderboard_entries
 * @string leaderboard
 * @tparam ELeaderboardDataRequest request
 * @number start
 * @number end
 */
int SteamUserStats_DownloadLeaderboardEntries(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	ELeaderboardDataRequest request = (ELeaderboardDataRequest)luaL_checkinteger(L, 2);
	int start = luaL_checknumber(L, 3);
	int end = luaL_checknumber(L, 4);
	g_SteamUserStats->DownloadLeaderboardEntries(leaderboard, request, start, end);
	return 0;
}

/** Returns data about a single leaderboard entry
 * https://partner.steamgames.com/doc/api/ISteamUserStats#GetDownloadedLeaderboardEntry
 * @name user_stats_get_downloaded_leaderboard_entry
 * @string hSteamLeaderboardEntries Leaderboard entries handle
 * @number index Which entry to get
 * @treturn Boolean ok
 * @treturn Table entry The requested leaderboard entry.
 */
int SteamUserStats_GetDownloadedLeaderboardEntry(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 2);

	SteamLeaderboardEntries_t hSteamLeaderboardEntries = check_uint64(L, 1);
	int index = luaL_checknumber(L, 2);
	int detailsMax = 1;
	int32 details[detailsMax];

	LeaderboardEntry_t leaderboardEntry;
	bool ok = g_SteamUserStats->GetDownloadedLeaderboardEntry(hSteamLeaderboardEntries, index, &leaderboardEntry, details, detailsMax);
	lua_pushboolean(L, ok);
	if (!ok)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_newtable(L);
		lua_pushstring(L, "m_steamIDUser");
		push_CSteamID(L, leaderboardEntry.m_steamIDUser);
		lua_settable(L, -3);
		lua_pushstring(L, "m_nGlobalRank");
		lua_pushinteger(L, leaderboardEntry.m_nGlobalRank);
		lua_settable(L, -3);
		lua_pushstring(L, "m_nScore");
		lua_pushinteger(L, leaderboardEntry.m_nScore);
		lua_settable(L, -3);
		lua_pushstring(L, "m_cDetails");
		lua_pushinteger(L, leaderboardEntry.m_cDetails);
		lua_settable(L, -3);
		lua_pushstring(L, "m_hUGC");
		push_uint64(L, leaderboardEntry.m_hUGC);
		lua_settable(L, -3);
	}
	return 2;
}


/** Uploads a user score to a specified leaderboard.
 * This call is asynchronous, with the result returned in a listener callback
 * with event set to LeaderboardScoreUploaded_t.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore
 * @name user_stats_upload_leaderboard_score
 * @string leaderboard
 * @tparam ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod
 * @number nScore
 */
int SteamUserStats_UploadLeaderboardScore(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 0);

	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	ELeaderboardUploadScoreMethod eLeaderboardUploadScoreMethod = (ELeaderboardUploadScoreMethod)luaL_checknumber(L, 2);
	int32 nScore = luaL_checknumber(L, 3);
	// TODO score details not supported yet
	const int32 *pScoreDetails = 0;
	int cScoreDetailsCount = 0;

	g_SteamUserStats->UploadLeaderboardScore(leaderboard, eLeaderboardUploadScoreMethod, nScore, pScoreDetails, cScoreDetailsCount);
	return 0;
}



/** Attaches a piece of user generated content the current user's entry on a
 * leaderboard.
 * https://partner.steamgames.com/doc/api/ISteamUserStats#AttachLeaderboardUGC
 * @name user_stats_attach_leadboard_ugc
 * @string leaderboard
 * @string ugc_handle
 * @treturn string Steam API call id
 */
int SteamUserStats_AttachLeaderboardUGC(lua_State* L)
{
	if (!g_SteamUserStats) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	SteamLeaderboard_t leaderboard = check_uint64(L, 1);
	UGCHandle_t ugc = check_uint64(L, 2);
	SteamAPICall_t call = g_SteamUserStats->AttachLeaderboardUGC(leaderboard, ugc);
	push_uint64(L, call);

	return 1;
}

#endif
