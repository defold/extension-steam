#define LIB_NAME "Steam"
#define MODULE_NAME "steam"

#ifndef DLIB_LOG_DOMAIN
#define DLIB_LOG_DOMAIN "steam"
#endif

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "steam_api.h"

#include "steam_types.h"
#include "steam_listener.h"

#include "steam_apps.h"
#include "steam_friends.h"
#include "steam_game_search.h"
#include "steam_input.h"
#include "steam_inventory.h"
#include "steam_matchmaking.h"
#include "steam_music.h"
#include "steam_musicremote.h"
#include "steam_parties.h"
#include "steam_networking.h"
#include "steam_remotestorage.h"
#include "steam_screenshots.h"
#include "steam_ugc.h"
#include "steam_user.h"
#include "steam_userstats.h"
#include "steam_utils.h"
#include "steam_video.h"


struct SteamBootstrap
{
	SteamBootstrap()
	{
		SteamAPI_Init();
	}
} g_SteamBootstrap;


int Steam_OnGameOverlayActivated(lua_State* L, void* data)
{
	GameOverlayActivated_t* s = (GameOverlayActivated_t*)data;
	lua_pushstring(L, "GameOverlayActivated_t");
	lua_newtable(L);
	lua_pushstring(L, "m_bActive");
	lua_pushboolean(L, s->m_bActive);
	lua_settable(L, -3);
	return 2;
}

class SteamCallbackWrapper
{
	public:
		SteamCallbackWrapper();
		STEAM_CALLBACK(SteamCallbackWrapper, OnGameOverlayActivated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);
};
SteamCallbackWrapper::SteamCallbackWrapper() :
	m_CallbackGameOverlayActivated(this, &SteamCallbackWrapper::OnGameOverlayActivated)
{}
void SteamCallbackWrapper::OnGameOverlayActivated(GameOverlayActivated_t *s)
{
	SteamListener_Invoke(Steam_OnGameOverlayActivated, s);
}

static SteamCallbackWrapper *g_SteamCallbackWrapper = new SteamCallbackWrapper();





/*******************************************
 * LIFECYCLE
 *******************************************/

/** Initialize Steamworks
 * @name init
 */
static int Init(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	if(!SteamAPI_Init())
	{
		luaL_error(L, "Error initialising SteamAPI");
	}
	if (!SteamAPI_IsSteamRunning())
	{
		luaL_error(L, "Steam is not running");
	}

	SteamApps_Init(L);
	SteamFriends_Init(L);
	SteamGameSearch_Init(L);
	SteamInput_Init(L);
	SteamInventory_Init(L);
	SteamMatchmaking_Init(L);
	SteamMusic_Init(L);
	SteamMusicRemote_Init(L);
	SteamNetworking_Init(L);
	SteamParties_Init(L);
	SteamRemoteStorage_Init(L);
	SteamScreenshots_Init(L);
	SteamUGC_Init(L);
	SteamUser_Init(L);
	SteamUserStats_Init(L);
	SteamUtils_Init(L);
	SteamVideo_Init(L);
	return 0;
}

/** Update Steamworks.
 * Call this from a script component.
 * @name update
 */
static int Update(lua_State* L)
{
	SteamAPI_RunCallbacks();
	return 0;
}

/** Restart Steamworks.
 * @name restart
 * @number appid
 * @tresult Boolean ok
 */
static int Restart(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 1);
	uint32_t appid = (uint32_t)luaL_checknumber(L, 1);
	int result = SteamAPI_RestartAppIfNecessary(appid);
	lua_pushboolean(L, result);
	return 1;
}

/** Finalize Steamworks.
 * @name finalize
 */
static int Final(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	SteamAPI_Shutdown();
	return 0;
}




static const luaL_reg Module_methods[] = {
	{ "init", Init },
	{ "restart", Restart },
	{ "update", Update },
	{ "final", Final },
	{ "set_listener", SteamListener_Set },
	
	// APPS
	{ "apps_is_dlc_installed", SteamApps_IsDlcInstalled },
	
	// UTILS
	{ "utils_get_app_id", SteamUtils_GetAppId },
	{ "utils_get_seconds_since_app_active", SteamUtils_GetSecondsSinceAppActive },
	{ "utils_is_steam_running_on_steam_deck", SteamUtils_IsSteamRunningOnSteamDeck },
	
	// USERSTATS - stats
	{ "user_stats_get_stat_int", SteamUserStats_GetStatInt },
	{ "user_stats_set_stat_int", SteamUserStats_SetStatInt },
	{ "user_stats_get_stat_float", SteamUserStats_GetStatFloat },
	{ "user_stats_set_stat_float", SteamUserStats_SetStatFloat },
	{ "user_stats_request_current_stats", SteamUserStats_RequestCurrentStats },
	{ "user_stats_request_global_stats", SteamUserStats_RequestGlobalStats },
	{ "user_stats_store_stats", SteamUserStats_StoreStats },
	{ "user_stats_reset_all_stats", SteamUserStats_ResetAllStats },
	
	// USERSTATS - achievements
	{ "user_stats_set_achievement", SteamUserStats_SetAchievement },
	{ "user_stats_get_num_achievements", SteamUserStats_GetNumAchievements },
	{ "user_stats_get_achievement_name", SteamUserStats_GetAchievementName },
	{ "user_stats_get_achievement", SteamUserStats_GetAchievement },
	{ "user_stats_get_achievement_display_attribute", SteamUserStats_GetAchievementDisplayAttribute },
	{ "user_stats_get_achievement_achieved_percent", SteamUserStats_GetAchievementAchievedPercent },
	
	// USERSTATS - leaderboard
	{ "user_stats_find_leaderboard", SteamUserStats_FindLeaderboard },
	{ "user_stats_get_leaderboard_name", SteamUserStats_GetLeaderboardName },
	{ "user_stats_download_leaderboard_entries", SteamUserStats_DownloadLeaderboardEntries },
	{ "user_stats_get_downloaded_leaderboard_entry", SteamUserStats_GetDownloadedLeaderboardEntry },
	{ "user_stats_get_achievement", SteamUserStats_GetAchievement },

	// FRIENDS
	{ "friends_get_friend_persona_name", SteamFriends_GetFriendPersonaName },
	{ "friends_activate_game_overlay_to_store", SteamFriends_ActivateGameOverlayToStore },

	// USER
	{ "user_get_steam_id", SteamUser_GetSteamId },
	{ "user_get_player_steam_level", SteamUser_GetPlayerSteamLevel },
	{ "user_get_game_badge_level", SteamUser_GetGameBadgeLevel },
	{ "user_logged_on", SteamUser_LoggedOn },
	{ "user_is_behind_nat", SteamUser_IsBehindNAT },
	{ "user_is_phone_verified", SteamUser_IsPhoneVerified },
	{ "user_is_phone_identifying", SteamUser_IsPhoneIdentifying },
	{ "user_is_phone_requiring_verification", SteamUser_IsPhoneRequiringVerification },
	{ "user_is_two_factor_enabled", SteamUser_IsTwoFactorEnabled },
	{ 0, 0 }
};


static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);

	#define SETCONSTANT(name, val) \
	lua_pushnumber(L, (lua_Number) val); \
	lua_setfield(L, -2, #name);

	//
	// ELeaderboardDataRequest
	//
	/** Requests rows in the leaderboard from the full table
	 * @field ELeaderboardDataRequestGlobal
	 */
	SETCONSTANT(ELeaderboardDataRequestGlobal, k_ELeaderboardDataRequestGlobal);
	/** Requests rows in the leaderboard from rows around the user
	 * @field ELeaderboardDataRequestGlobalAroundUser
	 */
	SETCONSTANT(ELeaderboardDataRequestGlobalAroundUser, k_ELeaderboardDataRequestGlobalAroundUser);
	/** Requests all the rows for friends of the current user
	 * @field ELeaderboardDataRequestFriends
	 */
	SETCONSTANT(ELeaderboardDataRequestFriends, k_ELeaderboardDataRequestFriends);
	SETCONSTANT(ELeaderboardDataRequestUsers, k_ELeaderboardDataRequestUsers);

	//
	// EOverlayToStoreFlag
	// These values are passed as parameters to the store
	//
	/** Passed as parameter to the store
	 * @field EOverlayToStoreFlag_None
	 */
	SETCONSTANT(EOverlayToStoreFlag_None, k_EOverlayToStoreFlag_None);
	/** Passed as parameter to the store
	 * @field EOverlayToStoreFlag_AddToCart
	 */
	SETCONSTANT(EOverlayToStoreFlag_AddToCart, k_EOverlayToStoreFlag_AddToCart);
	/** Passed as parameter to the store
	 * @field EOverlayToStoreFlag_AddToCartAndShow
	 */
	SETCONSTANT(EOverlayToStoreFlag_AddToCartAndShow, k_EOverlayToStoreFlag_AddToCartAndShow);
	#undef SETCONSTANT

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeSteam(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeSteam(dmExtension::Params* params)
{
	LuaInit(params->m_L);
	dmLogInfo("Registered %s Extension", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeSteam(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeSteam(dmExtension::Params* params)
{
	SteamListener_Destroy();
	return dmExtension::RESULT_OK;
}

#else

static dmExtension::Result AppInitializeSteam(dmExtension::AppParams* params)
{
	dmLogWarning("Registered %s (null) Extension", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeSteam(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeSteam(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeSteam(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(steam, LIB_NAME, AppInitializeSteam, AppFinalizeSteam, InitializeSteam, 0, 0, FinalizeSteam)
