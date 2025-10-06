/** Steam API
 * Functions and constants for interacting with Steamworks.
 * @document
 * @name Steam
 * @namespace steam
 */

#define LIB_NAME "Steam"
#define MODULE_NAME "steam"

#ifndef DLIB_LOG_DOMAIN
#define DLIB_LOG_DOMAIN "steam"
#endif

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || (defined(DM_PLATFORM_LINUX) && !defined(__aarch64__))

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
	table_push_boolean(L, "m_bActive", s->m_bActive);
	return 2;
}


/*******************************************
 * LIFECYCLE
 *******************************************/

/** Initialize Steamworks.
 * @name init
 * @treturn Boolean success True if successful
 * @treturn String message Error message if unsuccessful.
 */
static int Init(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 2);
	if(!SteamAPI_Init())
	{
		lua_pushboolean(L, 0);
		lua_pushstring(L, "Error initialising SteamAPI");
		return 2;
	}
	if (!SteamAPI_IsSteamRunning())
	{
		lua_pushboolean(L, 0);
		lua_pushstring(L, "Steam is not running");
		return 2;
	}

	SteamAPI_ManualDispatch_Init();

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

	lua_pushboolean(L, 1);
	lua_pushnil(L);
	return 2;
}

/** Update Steamworks.
 * Call this from a script component.
 * @name update
 */
static int Update(lua_State* L)
{
	// use a manual callback loop
	// see steam_api.h for details
	HSteamPipe steamPipe = SteamAPI_GetHSteamPipe();

	// perform period actions
	SteamAPI_ManualDispatch_RunFrame(steamPipe);

	// poll for callbacks
	CallbackMsg_t callback;
	while (SteamAPI_ManualDispatch_GetNextCallback(steamPipe, &callback))
	{
		int id = callback.m_iCallback;
		void* data = callback.m_pubParam;
		void* callResultData = 0;
		
		// handle SteamAPICall_t result
		// unpack the result struct
		if (id == SteamAPICallCompleted_t::k_iCallback)
		{
			SteamAPICallCompleted_t* callCompleted = (SteamAPICallCompleted_t*)data;
			callResultData = malloc(callCompleted->m_cubParam);
			bool failed;
			if (SteamAPI_ManualDispatch_GetAPICallResult(steamPipe, callCompleted->m_hAsyncCall, callResultData, callCompleted->m_cubParam, callCompleted->m_iCallback, &failed))
			{
				id = callCompleted->m_iCallback;
				data = callResultData;
				dmLogInfo("SteamAPICallCompleted_t %llu result %d", callCompleted->m_hAsyncCall, id);
			}
			else
			{
				free(callResultData);
				SteamAPI_ManualDispatch_FreeLastCallback(steamPipe);
				//dmLogInfo("SteamAPICallCompleted_t failed to get call result");
				return 0;
			}
		}

		// user stats
		if (id == LeaderboardFindResult_t::k_iCallback) SteamListener_Invoke(SteamUserStats_OnLeaderboardFindResult, data);
		else if (id == GlobalStatsReceived_t::k_iCallback) SteamListener_Invoke(SteamUserStats_OnGlobalStatsReceived, data);
		else if (id == UserStatsReceived_t::k_iCallback) SteamListener_Invoke(SteamUserStats_OnUserStatsReceived, data);
		else if (id == LeaderboardScoresDownloaded_t::k_iCallback) SteamListener_Invoke(SteamUserStats_OnLeaderboardScoresDownloaded, data);
		else if (id == LeaderboardScoreUploaded_t::k_iCallback) SteamListener_Invoke(SteamUserStats_OnLeaderboardScoreUploaded, data);
		else if (id == LeaderboardUGCSet_t::k_iCallback) SteamListener_Invoke(SteamUserStats_OnLeaderboardUGCSet, data);
		else if (id == UserStatsStored_t::k_iCallback) SteamListener_InvokeGeneric("UserStatsStored_t");
		else if (id == UserAchievementStored_t::k_iCallback) SteamListener_InvokeGeneric("UserAchievementStored_t");
		else if (id == NumberOfCurrentPlayers_t::k_iCallback) SteamListener_InvokeGeneric("NumberOfCurrentPlayers_t");
		else if (id == UserStatsUnloaded_t::k_iCallback) SteamListener_InvokeGeneric("UserStatsUnloaded_t");
		else if (id == UserAchievementIconFetched_t::k_iCallback) SteamListener_InvokeGeneric("UserAchievementIconFetched_t");
		else if (id == GlobalAchievementPercentagesReady_t::k_iCallback) SteamListener_InvokeGeneric("GlobalAchievementPercentagesReady_t");
		// user
		else if (id == MicroTxnAuthorizationResponse_t::k_iCallback) SteamListener_Invoke(SteamUser_OnMicroTxnAuthorizationResponse, data);
		else if (id == SteamServersConnected_t::k_iCallback) SteamListener_InvokeGeneric("SteamServersConnected_t");
		else if (id == SteamServerConnectFailure_t::k_iCallback) SteamListener_InvokeGeneric("SteamServerConnectFailure_t");
		else if (id == SteamServersDisconnected_t::k_iCallback) SteamListener_InvokeGeneric("SteamServersDisconnected_t");
		else if (id == ClientGameServerDeny_t::k_iCallback) SteamListener_InvokeGeneric("ClientGameServerDeny_t");
		else if (id == IPCFailure_t::k_iCallback) SteamListener_InvokeGeneric("IPCFailure_t");
		else if (id == LicensesUpdated_t::k_iCallback) SteamListener_InvokeGeneric("LicensesUpdated_t");
		else if (id == ValidateAuthTicketResponse_t::k_iCallback) SteamListener_InvokeGeneric("ValidateAuthTicketResponse_t");
		else if (id == EncryptedAppTicketResponse_t::k_iCallback) SteamListener_InvokeGeneric("EncryptedAppTicketResponse_t");
		else if (id == GetAuthSessionTicketResponse_t::k_iCallback) SteamListener_InvokeGeneric("GetAuthSessionTicketResponse_t");
		else if (id == GameWebCallback_t::k_iCallback) SteamListener_InvokeGeneric("GameWebCallback_t");
		else if (id == StoreAuthURLResponse_t::k_iCallback) SteamListener_InvokeGeneric("StoreAuthURLResponse_t");
		else if (id == MarketEligibilityResponse_t::k_iCallback) SteamListener_InvokeGeneric("MarketEligibilityResponse_t");
		else if (id == DurationControl_t::k_iCallback) SteamListener_InvokeGeneric("DurationControl_t");
		else if (id == GetTicketForWebApiResponse_t::k_iCallback) SteamListener_InvokeGeneric("GetTicketForWebApiResponse_t");
		// utils
		else if (id == GamepadTextInputDismissed_t::k_iCallback) SteamListener_Invoke(SteamUtils_OnGamepadTextInputDismissed, data);
		else if (id == FloatingGamepadTextInputDismissed_t::k_iCallback) SteamListener_Invoke(SteamUtils_OnFloatingGamepadTextInputDismissed, data);
		else if (id == IPCountry_t::k_iCallback) SteamListener_InvokeGeneric("IPCountry_t");
		else if (id == LowBatteryPower_t::k_iCallback) SteamListener_InvokeGeneric("LowBatteryPower_t");
		else if (id == SteamShutdown_t::k_iCallback) SteamListener_InvokeGeneric("SteamShutdown_t");
		else if (id == CheckFileSignature_t::k_iCallback) SteamListener_InvokeGeneric("CheckFileSignature_t");
		else if (id == FilterTextDictionaryChanged_t::k_iCallback) SteamListener_InvokeGeneric("FilterTextDictionaryChanged_t");
		// friends
		else if (id == GameOverlayActivated_t::k_iCallback) SteamListener_Invoke(Steam_OnGameOverlayActivated, data);
		else if (id == GameRichPresenceJoinRequested_t::k_iCallback) SteamListener_Invoke(SteamFriends_OnGameRichPresenceJoinRequested, data);
		else if (id == FriendRichPresenceUpdate_t::k_iCallback) SteamListener_Invoke(SteamFriends_OnFriendRichPresenceUpdate, data);
		else if (id == PersonaStateChange_t::k_iCallback) SteamListener_Invoke(SteamFriends_OnPersonaStateChange, data);
		else if (id == GameServerChangeRequested_t::k_iCallback) SteamListener_InvokeGeneric("GameServerChangeRequested_t");
		else if (id == GameLobbyJoinRequested_t::k_iCallback) SteamListener_InvokeGeneric("GameLobbyJoinRequested_t");
		else if (id == AvatarImageLoaded_t::k_iCallback) SteamListener_InvokeGeneric("AvatarImageLoaded_t");
		else if (id == ClanOfficerListResponse_t::k_iCallback) SteamListener_InvokeGeneric("ClanOfficerListResponse_t");
		else if (id == GameConnectedClanChatMsg_t::k_iCallback) SteamListener_InvokeGeneric("GameConnectedClanChatMsg_t");
		else if (id == GameConnectedChatJoin_t::k_iCallback) SteamListener_InvokeGeneric("GameConnectedChatJoin_t");
		else if (id == GameConnectedChatLeave_t::k_iCallback) SteamListener_InvokeGeneric("GameConnectedChatLeave_t");
		else if (id == DownloadClanActivityCountsResult_t::k_iCallback) SteamListener_InvokeGeneric("DownloadClanActivityCountsResult_t");
		else if (id == JoinClanChatRoomCompletionResult_t::k_iCallback) SteamListener_InvokeGeneric("JoinClanChatRoomCompletionResult_t");
		else if (id == GameConnectedFriendChatMsg_t::k_iCallback) SteamListener_InvokeGeneric("GameConnectedFriendChatMsg_t");
		else if (id == FriendsGetFollowerCount_t::k_iCallback) SteamListener_InvokeGeneric("FriendsGetFollowerCount_t");
		else if (id == FriendsIsFollowing_t::k_iCallback) SteamListener_InvokeGeneric("FriendsIsFollowing_t");
		else if (id == FriendsEnumerateFollowingList_t::k_iCallback) SteamListener_InvokeGeneric("FriendsEnumerateFollowingList_t");
		else if (id == UnreadChatMessagesChanged_t::k_iCallback) SteamListener_InvokeGeneric("UnreadChatMessagesChanged_t");
		else if (id == OverlayBrowserProtocolNavigation_t::k_iCallback) SteamListener_InvokeGeneric("OverlayBrowserProtocolNavigation_t");
		else if (id == EquippedProfileItemsChanged_t::k_iCallback) SteamListener_InvokeGeneric("EquippedProfileItemsChanged_t");
		else if (id == EquippedProfileItems_t::k_iCallback) SteamListener_InvokeGeneric("EquippedProfileItems_t");
		// matchmaking
		else if (id == LobbyMatchList_t::k_iCallback) SteamListener_Invoke(SteamMatchmaking_OnLobbyMatchList, data);
		else if (id == LobbyEnter_t::k_iCallback) SteamListener_Invoke(SteamMatchmaking_OnLobbyEnter, data);
		else if (id == FavoritesListChanged_t::k_iCallback) SteamListener_InvokeGeneric("FavoritesListChanged_t");
		else if (id == LobbyInvite_t::k_iCallback) SteamListener_InvokeGeneric("LobbyInvite_t");
		else if (id == LobbyDataUpdate_t::k_iCallback) SteamListener_Invoke(SteamMatchmaking_OnLobbyDataUpdate, data);
		else if (id == LobbyChatUpdate_t::k_iCallback) SteamListener_Invoke(SteamMatchmaking_OnLobbyChatUpdate, data);
		else if (id == LobbyChatMsg_t::k_iCallback) SteamListener_Invoke(SteamMatchmaking_OnLobbyChatMsg, data);
		else if (id == LobbyGameCreated_t::k_iCallback) SteamListener_InvokeGeneric("LobbyGameCreated_t");
		else if (id == LobbyKicked_t::k_iCallback) SteamListener_InvokeGeneric("LobbyKicked_t");
		else if (id == LobbyCreated_t::k_iCallback) SteamListener_Invoke(SteamMatchmaking_OnLobbyCreated, data);
		else if (id == FavoritesListAccountsUpdated_t::k_iCallback) SteamListener_InvokeGeneric("FavoritesListAccountsUpdated_t");
		else if (id == SearchForGameProgressCallback_t::k_iCallback) SteamListener_InvokeGeneric("SearchForGameProgressCallback_t");
		else if (id == SearchForGameResultCallback_t::k_iCallback) SteamListener_InvokeGeneric("SearchForGameResultCallback_t");
		else if (id == RequestPlayersForGameProgressCallback_t::k_iCallback) SteamListener_InvokeGeneric("RequestPlayersForGameProgressCallback_t");
		else if (id == RequestPlayersForGameResultCallback_t::k_iCallback) SteamListener_InvokeGeneric("RequestPlayersForGameResultCallback_t");
		else if (id == RequestPlayersForGameFinalResultCallback_t::k_iCallback) SteamListener_InvokeGeneric("RequestPlayersForGameFinalResultCallback_t");
		else if (id == SubmitPlayerResultResultCallback_t::k_iCallback) SteamListener_InvokeGeneric("SubmitPlayerResultResultCallback_t");
		else if (id == EndGameResultCallback_t::k_iCallback) SteamListener_InvokeGeneric("EndGameResultCallback_t");
		else if (id == JoinPartyCallback_t::k_iCallback) SteamListener_InvokeGeneric("JoinPartyCallback_t");
		else if (id == CreateBeaconCallback_t::k_iCallback) SteamListener_InvokeGeneric("CreateBeaconCallback_t");
		else if (id == ReservationNotificationCallback_t::k_iCallback) SteamListener_InvokeGeneric("ReservationNotificationCallback_t");
		else if (id == ChangeNumOpenSlotsCallback_t::k_iCallback) SteamListener_InvokeGeneric("ChangeNumOpenSlotsCallback_t");
		else if (id == AvailableBeaconLocationsUpdated_t::k_iCallback) SteamListener_InvokeGeneric("AvailableBeaconLocationsUpdated_t");
		else if (id == ActiveBeaconsUpdated_t::k_iCallback) SteamListener_InvokeGeneric("ActiveBeaconsUpdated_t");
		// networking
		else if (id == SteamNetworkingMessagesSessionFailed_t::k_iCallback) SteamListener_Invoke(SteamNetworking_OnSteamNetworkingMessagesSessionFailed, data);
		else if (id == SteamNetworkingMessagesSessionRequest_t::k_iCallback) SteamListener_Invoke(SteamNetworking_OnSteamNetworkingMessagesSessionRequest, data);
		else if (id == SteamRelayNetworkStatus_t::k_iCallback) SteamListener_InvokeGeneric("SteamRelayNetworkStatus_t");
		else if (id == SteamNetAuthenticationStatus_t::k_iCallback) SteamListener_InvokeGeneric("SteamNetAuthenticationStatus_t");
		else if (id == SteamNetworkingMessagesSessionFailed_t::k_iCallback) SteamListener_InvokeGeneric("SteamNetworkingMessagesSessionFailed_t");
		// remote storage
		else if (id == RemoteStorageFileShareResult_t::k_iCallback) SteamListener_Invoke(SteamRemoteStorage_OnRemoteStorageFileShareResult, data);
		else if (id == RemoteStoragePublishFileResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStoragePublishFileResult_t");
		else if (id == RemoteStorageDeletePublishedFileResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageDeletePublishedFileResult_t");
		else if (id == RemoteStorageEnumerateUserPublishedFilesResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageEnumerateUserPublishedFilesResult_t");
		else if (id == RemoteStorageSubscribePublishedFileResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageSubscribePublishedFileResult_t");
		else if (id == RemoteStorageEnumerateUserSubscribedFilesResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageEnumerateUserSubscribedFilesResult_t");
		else if (id == RemoteStorageUnsubscribePublishedFileResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageUnsubscribePublishedFileResult_t");
		else if (id == RemoteStorageUpdatePublishedFileResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageUpdatePublishedFileResult_t");
		else if (id == RemoteStorageDownloadUGCResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageDownloadUGCResult_t");
		else if (id == RemoteStorageGetPublishedFileDetailsResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageGetPublishedFileDetailsResult_t");
		else if (id == RemoteStorageEnumerateWorkshopFilesResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageEnumerateWorkshopFilesResult_t");
		else if (id == RemoteStorageGetPublishedItemVoteDetailsResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageGetPublishedItemVoteDetailsResult_t");
		else if (id == RemoteStoragePublishedFileSubscribed_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStoragePublishedFileSubscribed_t");
		else if (id == RemoteStoragePublishedFileUnsubscribed_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStoragePublishedFileUnsubscribed_t");
		else if (id == RemoteStoragePublishedFileDeleted_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStoragePublishedFileDeleted_t");
		else if (id == RemoteStorageUpdateUserPublishedItemVoteResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageUpdateUserPublishedItemVoteResult_t");
		else if (id == RemoteStorageUserVoteDetails_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageUserVoteDetails_t");
		else if (id == RemoteStorageEnumerateUserSharedWorkshopFilesResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageEnumerateUserSharedWorkshopFilesResult_t");
		else if (id == RemoteStorageSetUserPublishedFileActionResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageSetUserPublishedFileActionResult_t");
		else if (id == RemoteStorageEnumeratePublishedFilesByUserActionResult_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageEnumeratePublishedFilesByUserActionResult_t");
		else if (id == RemoteStoragePublishFileProgress_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStoragePublishFileProgress_t");
		else if (id == RemoteStoragePublishedFileUpdated_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStoragePublishedFileUpdated_t");
		else if (id == RemoteStorageFileWriteAsyncComplete_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageFileWriteAsyncComplete_t");
		else if (id == RemoteStorageFileReadAsyncComplete_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageFileReadAsyncComplete_t");
		else if (id == RemoteStorageLocalFileChange_t::k_iCallback) SteamListener_InvokeGeneric("RemoteStorageLocalFileChange_t");
		else
		{
			//dmLogInfo("Unhandled callback with id %d", id);
		}
		free(callResultData);
		SteamAPI_ManualDispatch_FreeLastCallback(steamPipe);
	}
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
 * @name final
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
	{ "utils_get_image_size", SteamUtils_GetImageSize },
	{ "utils_get_image_rgba", SteamUtils_GetImageRGBA },
	{ "utils_get_server_real_time", SteamUtils_GetServerRealTime },
	{ "utils_show_floating_gamepad_text_input", SteamUtils_ShowFloatingGamepadTextInput },
	{ "utils_show_gamepad_text_input", SteamUtils_ShowGamepadTextInput },

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
	{ "user_stats_get_achievement", SteamUserStats_GetAchievement },
	{ "user_stats_clear_achievement", SteamUserStats_ClearAchievement },
	{ "user_stats_get_num_achievements", SteamUserStats_GetNumAchievements },
	{ "user_stats_get_achievement_name", SteamUserStats_GetAchievementName },
	{ "user_stats_get_achievement_display_attribute", SteamUserStats_GetAchievementDisplayAttribute },
	{ "user_stats_get_achievement_achieved_percent", SteamUserStats_GetAchievementAchievedPercent },
	
	// USERSTATS - leaderboard
	{ "user_stats_find_leaderboard", SteamUserStats_FindLeaderboard },
	{ "user_stats_find_or_create_leaderboard", SteamUserStats_FindOrCreateLeaderboard },
	{ "user_stats_get_leaderboard_name", SteamUserStats_GetLeaderboardName },
	{ "user_stats_get_leaderboard_entry_count", SteamUserStats_GetLeaderboardEntryCount },
	{ "user_stats_get_leaderboard_sort_method", SteamUserStats_GetLeaderboardSortMethod },
	{ "user_stats_get_leaderboard_display_type", SteamUserStats_GetLeaderboardDisplayType },
	{ "user_stats_download_leaderboard_entries", SteamUserStats_DownloadLeaderboardEntries },
	{ "user_stats_get_downloaded_leaderboard_entry", SteamUserStats_GetDownloadedLeaderboardEntry },
	{ "user_stats_get_achievement", SteamUserStats_GetAchievement },
	{ "user_stats_upload_leaderboard_score", SteamUserStats_UploadLeaderboardScore },
	{ "user_stats_attach_leadboard_ugc", SteamUserStats_UploadLeaderboardScore },

	// FRIENDS
	{ "friends_get_persona_name", SteamFriends_GetPersonaName },
	{ "friends_get_persona_state", SteamFriends_GetPersonaState },
	{ "friends_get_friend_count", SteamFriends_GetFriendCount },
	{ "friends_get_friend_by_index", SteamFriends_GetFriendByIndex },
	{ "friends_get_friend_persona_state", SteamFriends_GetFriendPersonaState },
	{ "friends_get_friend_steam_level", SteamFriends_GetFriendSteamLevel },
	{ "friends_get_friend_relationship", SteamFriends_GetFriendRelationship },
	{ "friends_get_friend_persona_name", SteamFriends_GetFriendPersonaName },
	{ "friends_get_small_friend_avatar", SteamFriends_GetSmallFriendAvatar },
	{ "friends_get_medium_friend_avatar", SteamFriends_GetMediumFriendAvatar },
	{ "friends_get_large_friend_avatar", SteamFriends_GetLargeFriendAvatar },
	{ "friends_activate_game_overlay_to_store", SteamFriends_ActivateGameOverlayToStore },
	{ "friends_activate_game_overlay_to_web_page", SteamFriends_ActivateGameOverlayToWebPage },
	{ "friends_set_rich_presence", SteamFriends_SetRichPresence },
	{ "friends_clear_rich_presence", SteamFriends_ClearRichPresence },
	{ "friends_invite_user_to_game", SteamFriends_InviteUserToGame },

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
	{ "user_get_auth_session_ticket", SteamUser_GetAuthSessionTicket },

	// MATCHMAKING
	{ "matchmaking_request_lobby_list", SteamMatchmaking_RequestLobbyList },
	{ "matchmaking_add_request_lobby_list_string_filter", SteamMatchmaking_AddRequestLobbyListStringFilter },
	{ "matchmaking_add_request_lobby_list_numerical_filter", SteamMatchmaking_AddRequestLobbyListNumericalFilter },
	{ "matchmaking_add_request_lobby_list_near_value_filter", SteamMatchmaking_AddRequestLobbyListNearValueFilter },
	{ "matchmaking_add_request_lobby_list_filter_slots_available", SteamMatchmaking_AddRequestLobbyListFilterSlotsAvailable },
	{ "matchmaking_add_request_lobby_list_distance_filter", SteamMatchmaking_AddRequestLobbyListDistanceFilter },
	{ "matchmaking_add_request_lobby_list_result_count_filter", SteamMatchmaking_AddRequestLobbyListResultCountFilter },
	{ "matchmaking_add_request_lobby_list_compatible_members_filter", SteamMatchmaking_AddRequestLobbyListCompatibleMembersFilter },

	{ "matchmaking_get_lobby_by_index", SteamMatchmaking_GetLobbyByIndex },
	{ "matchmaking_create_lobby", SteamMatchmaking_CreateLobby },
	{ "matchmaking_join_lobby", SteamMatchmaking_JoinLobby },
	{ "matchmaking_leave_lobby", SteamMatchmaking_LeaveLobby },
	{ "matchmaking_get_lobby_owner", SteamMatchmaking_GetLobbyOwner },
	{ "matchmaking_get_num_lobby_members", SteamMatchmaking_GetNumLobbyMembers },
	{ "matchmaking_get_lobby_member_by_index", SteamMatchmaking_GetLobbyMemberByIndex },
	{ "matchmaking_set_lobby_member_data", SteamMatchmaking_SetLobbyMemberData },
	{ "matchmaking_get_lobby_member_data", SteamMatchmaking_GetLobbyMemberData },
	{ "matchmaking_set_lobby_owner", SteamMatchmaking_SetLobbyOwner },
	{ "matchmaking_set_lobby_type", SteamMatchmaking_SetLobbyType },
	{ "matchmaking_set_lobby_joinable", SteamMatchmaking_SetLobbyJoinable },
	{ "matchmaking_set_lobby_member_limit", SteamMatchmaking_SetLobbyMemberLimit },
	{ "matchmaking_get_lobby_member_limit", SteamMatchmaking_GetLobbyMemberLimit },

	{ "matchmaking_set_lobby_data", SteamMatchmaking_SetLobbyData },
	{ "matchmaking_get_lobby_data", SteamMatchmaking_GetLobbyData },
	{ "matchmaking_get_lobby_data_count", SteamMatchmaking_GetLobbyDataCount },
	{ "matchmaking_get_lobby_data_by_index", SteamMatchmaking_GetLobbyDataByIndex },
	{ "matchmaking_send_lobby_chat_message", SteamMatchmaking_SendLobbyChatMsg },
	{ "matchmaking_get_lobby_chat_entry", SteamMatchmaking_GetLobbyChatEntry },

	// NETWORKING
	{ "networking_send_message_to_user", SteamNetworking_SendMessageToUser },
	{ "networking_receive_messages_on_channel", SteamNetworking_ReceiveMessagesOnChannel },
	{ "networking_accept_session_with_user", SteamNetworking_AcceptSessionWithUser },
	{ "networking_close_session_with_user", SteamNetworking_CloseSessionWithUser },
	{ "networking_close_channel_with_user", SteamNetworking_CloseChannelWithUser },
	{ "networking_get_session_connection_info", SteamNetworking_GetSessionConnectionInfo },

	// REMOTESTORAGE
	{ "remote_storage_file_share", SteamRemoteStorage_FileShare },
	{ "remote_storage_file_write", SteamRemoteStorage_FileWrite },
	{ "remote_storage_file_read", SteamRemoteStorage_FileRead },
	{ "remote_storage_get_file_count", SteamRemoteStorage_GetFileCount },
	{ "remote_storage_get_file_name_and_size", SteamRemoteStorage_GetFileNameAndSize },
	{ "remote_storage_get_quota", SteamRemoteStorage_GetQuota },

	{ 0, 0 }
};


static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);

	#define SETCONSTANT(name, val) \
	lua_pushinteger(L, (lua_Number) val); \
	lua_setfield(L, -2, #name);

	//
	// EFloatingGamepadTextInputMode
	//
	/**
	 * Enter dismisses the keyboard
	 * @field EFloatingGamepadTextInputModeModeSingleLine
	 */
	SETCONSTANT(EFloatingGamepadTextInputModeModeSingleLine, k_EFloatingGamepadTextInputModeModeSingleLine);
	/**
	 * User needs to explicitly dismiss the keyboard
	 * @field EFloatingGamepadTextInputModeModeMultipleLines
	 */
	SETCONSTANT(EFloatingGamepadTextInputModeModeMultipleLines, k_EFloatingGamepadTextInputModeModeMultipleLines);
	/**
	 * Keyboard is displayed in a special mode that makes it easier to enter emails
	 * @field EFloatingGamepadTextInputModeModeEmail
	 */
	SETCONSTANT(EFloatingGamepadTextInputModeModeEmail, k_EFloatingGamepadTextInputModeModeEmail);
	/**
	 * Numeric keypad is shown
	 * @field EFloatingGamepadTextInputModeModeNumeric
	 */
	SETCONSTANT(EFloatingGamepadTextInputModeModeNumeric, k_EFloatingGamepadTextInputModeModeNumeric);


	//
	// EGamepadTextInputMode
	//
	/**
	 * Normal text input
	 * @field EGamepadTextInputModeNormal
	 */
	SETCONSTANT(EGamepadTextInputModeNormal, k_EGamepadTextInputModeNormal);
	/**
	 * Password text input
	 * @field EGamepadTextInputModePassword
	 */
	SETCONSTANT(EGamepadTextInputModePassword, k_EGamepadTextInputModePassword);


	//
	// EGamepadTextInputLineMode
	//
	/**
	 * Single line text
	 * @field EGamepadTextInputLineModeSingleLine
	 */
	SETCONSTANT(EGamepadTextInputLineModeSingleLine, k_EGamepadTextInputLineModeSingleLine);
	/**
	 * Multi line text
	 * @field EGamepadTextInputLineModeMultipleLines
	 */
	SETCONSTANT(EGamepadTextInputLineModeMultipleLines, k_EGamepadTextInputLineModeMultipleLines);


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
	// ELeaderboardSortMethod
	//
	/** 
	 * @field ELeaderboardSortMethodNone
	 */
	SETCONSTANT(ELeaderboardSortMethodNone, k_ELeaderboardSortMethodNone);
	/** top-score is lowest number
	 * @field ELeaderboardSortMethodAscending
	 */
	SETCONSTANT(ELeaderboardSortMethodAscending, k_ELeaderboardSortMethodAscending);
	/** top-score is highest number
	 * @field ELeaderboardSortMethodDescending
	 */
	SETCONSTANT(ELeaderboardSortMethodDescending, k_ELeaderboardSortMethodDescending);


	//
	// ELeaderboardUploadScoreMethod
	//
	/**
	 * @field ELeaderboardUploadScoreMethodNone
	 */
	SETCONSTANT(ELeaderboardUploadScoreMethodNone, k_ELeaderboardUploadScoreMethodNone);
	/** Leaderboard will keep user's best score
	 * @field ELeaderboardUploadScoreMethodKeepBest
	 */
	SETCONSTANT(ELeaderboardUploadScoreMethodKeepBest, k_ELeaderboardUploadScoreMethodKeepBest);
	/** Leaderboard will always replace score with specified
	 * @field ELeaderboardUploadScoreMethodForceUpdate
	 */
	SETCONSTANT(ELeaderboardUploadScoreMethodForceUpdate, k_ELeaderboardUploadScoreMethodForceUpdate);


	//
	// ELeaderboardDisplayType
	//
	/**
	 * @field ELeaderboardDisplayTypeNone
	 */
	SETCONSTANT(ELeaderboardDisplayTypeNone, k_ELeaderboardDisplayTypeNone);
	/** Simple numerical score
	 * @field ELeaderboardDisplayTypeNumeric
	 */
	SETCONSTANT(ELeaderboardDisplayTypeNumeric, k_ELeaderboardDisplayTypeNumeric);
	/** The score represents a time, in seconds
	 * @field ELeaderboardDisplayTypeTimeSeconds
	 */
	SETCONSTANT(ELeaderboardDisplayTypeTimeSeconds, k_ELeaderboardDisplayTypeTimeSeconds);
	/** The score represents a time, in milliseconds
	 * @field ELeaderboardDisplayTypeTimeMilliSeconds
	 */
	SETCONSTANT(ELeaderboardDisplayTypeTimeMilliSeconds, k_ELeaderboardDisplayTypeTimeMilliSeconds);


	//
	// EOverlayToStoreFlag
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

	//
	// EActivateGameOverlayToWebPageMode
	//
	/** Passed as parameter to ActivateGameOverlayToWebPage
	 * @field EActivateGameOverlayToWebPageMode_Default
	 */
	SETCONSTANT(EActivateGameOverlayToWebPageMode_Default, k_EActivateGameOverlayToWebPageMode_Default);
	/** Passed as parameter to ActivateGameOverlayToWebPage
	 * @field EActivateGameOverlayToWebPageMode_Modal
	 */
	SETCONSTANT(EActivateGameOverlayToWebPageMode_Modal, k_EActivateGameOverlayToWebPageMode_Modal);


	//
	// EPersonaState
	//
	/** friend is not currently logged on
	 * @field EPersonaStateOffline
	 */
	SETCONSTANT(EPersonaStateOffline, k_EPersonaStateOffline);
	/** friend is logged on
	 * @field EPersonaStateOnline
	 */
	SETCONSTANT(EPersonaStateOnline, k_EPersonaStateOnline);
	/** user is on, but busy
	 * @field EPersonaStateBusy
	 */
	SETCONSTANT(EPersonaStateBusy, k_EPersonaStateBusy);
	/** auto-away feature
	 * @field EPersonaStateAway
	 */
	SETCONSTANT(EPersonaStateAway, k_EPersonaStateAway);
	/** auto-away for a long time
	 * @field EPersonaStateSnooze
	 */
	SETCONSTANT(EPersonaStateSnooze, k_EPersonaStateSnooze);
	/** Online, trading
	 * @field EPersonaStateLookingToTrade
	 */
	SETCONSTANT(EPersonaStateLookingToTrade, k_EPersonaStateLookingToTrade);
	/** Online, wanting to play
	 * @field EPersonaStateLookingToPlay
	 */
	SETCONSTANT(EPersonaStateLookingToPlay, k_EPersonaStateLookingToPlay);
	/** Online, but appears offline to friends.  This status is never published to clients.
	 * @field EPersonaStateInvisible
	 */
	SETCONSTANT(EPersonaStateInvisible, k_EPersonaStateInvisible);

	//
	// EFriendFlags
	//
	/**
	 * EFriendFlagNone
	 * @field EFriendFlagNone
	 */
	SETCONSTANT(EFriendFlagNone, k_EFriendFlagNone);
	/**
	 * EFriendFlagBlocked
	 * @field EFriendFlagBlocked
	 */
	SETCONSTANT(EFriendFlagBlocked, k_EFriendFlagBlocked);
	/**
	 * EFriendFlagFriendshipRequested
	 * @field EFriendFlagFriendshipRequested
	 */
	SETCONSTANT(EFriendFlagFriendshipRequested, k_EFriendFlagFriendshipRequested);
	/**
	 * EFriendFlagImmediate
	 * @field EFriendFlagImmediate
	 */
	SETCONSTANT(EFriendFlagImmediate, k_EFriendFlagImmediate);
	/**
	 * EFriendFlagClanMember
	 * @field EFriendFlagClanMember
	 */
	SETCONSTANT(EFriendFlagClanMember, k_EFriendFlagClanMember);
	/**
	 * EFriendFlagOnGameServer
	 * @field EFriendFlagOnGameServer
	 */
	SETCONSTANT(EFriendFlagOnGameServer, k_EFriendFlagOnGameServer);
	/**
	 * EFriendFlagRequestingFriendship
	 * @field EFriendFlagRequestingFriendship
	 */
	SETCONSTANT(EFriendFlagRequestingFriendship, k_EFriendFlagRequestingFriendship);
	/**
	 * EFriendFlagRequestingInfo
	 * @field EFriendFlagRequestingInfo
	 */
	SETCONSTANT(EFriendFlagRequestingInfo, k_EFriendFlagRequestingInfo);
	/**
	 * EFriendFlagIgnored
	 * @field EFriendFlagIgnored
	 */
	SETCONSTANT(EFriendFlagIgnored, k_EFriendFlagIgnored);
	/**
	 * EFriendFlagIgnoredFriend
	 * @field EFriendFlagIgnoredFriend
	 */
	SETCONSTANT(EFriendFlagIgnoredFriend, k_EFriendFlagIgnoredFriend);
	/**
	 * EFriendFlagChatMember
	 * @field EFriendFlagChatMember
	 */
	SETCONSTANT(EFriendFlagChatMember, k_EFriendFlagChatMember);
	/**
	 * EFriendFlagAll
	 * @field EFriendFlagAll
	 */
	SETCONSTANT(EFriendFlagAll, k_EFriendFlagAll);

	/**
	 * SteamNetworkingSend_Unreliable
	 * @field SteamNetworkingSend_Unreliable
	 */
	SETCONSTANT(SteamNetworkingSend_Unreliable, k_nSteamNetworkingSend_Unreliable);
	/**
	 * SteamNetworkingSend_NoNagle
	 * @field SteamNetworkingSend_NoNagle
	 */
	SETCONSTANT(SteamNetworkingSend_NoNagle, k_nSteamNetworkingSend_NoNagle);
	/**
	 * SteamNetworkingSend_UnreliableNoNagle
	 * @field SteamNetworkingSend_UnreliableNoNagle
	 */
	SETCONSTANT(SteamNetworkingSend_UnreliableNoNagle, k_nSteamNetworkingSend_UnreliableNoNagle);
	/**
	 * SteamNetworkingSend_NoDelay
	 * @field SteamNetworkingSend_NoDelay
	 */
	SETCONSTANT(SteamNetworkingSend_NoDelay, k_nSteamNetworkingSend_NoDelay);
	/**
	 * SteamNetworkingSend_UnreliableNoDelay
	 * @field SteamNetworkingSend_UnreliableNoDelay
	 */
	SETCONSTANT(SteamNetworkingSend_UnreliableNoDelay, k_nSteamNetworkingSend_UnreliableNoDelay);
	/**
	 * SteamNetworkingSend_Reliable
	 * @field SteamNetworkingSend_Reliable
	 */
	SETCONSTANT(SteamNetworkingSend_Reliable, k_nSteamNetworkingSend_Reliable);
	/**
	 * SteamNetworkingSend_ReliableNoNagle
	 * @field SteamNetworkingSend_ReliableNoNagle
	 */
	SETCONSTANT(SteamNetworkingSend_ReliableNoNagle, k_nSteamNetworkingSend_ReliableNoNagle);
	/**
	 * SteamNetworkingSend_UseCurrentThread
	 * @field SteamNetworkingSend_UseCurrentThread
	 */
	SETCONSTANT(SteamNetworkingSend_UseCurrentThread, k_nSteamNetworkingSend_UseCurrentThread);
	/**
	 * SteamNetworkingSend_AutoRestartBrokenSession
	 * @field SteamNetworkingSend_AutoRestartBrokenSession
	 */
	SETCONSTANT(SteamNetworkingSend_AutoRestartBrokenSession, k_nSteamNetworkingSend_AutoRestartBrokenSession);

	/**
	 * ESteamNetConnectionEnd_Invalid
	 * @field ESteamNetConnectionEnd_Invalid
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Invalid, k_ESteamNetConnectionEnd_Invalid);
	/**
	 * ESteamNetConnectionEnd_App_Generic
	 * @field ESteamNetConnectionEnd_App_Generic
	 */
	SETCONSTANT(ESteamNetConnectionEnd_App_Generic, k_ESteamNetConnectionEnd_App_Generic);
	/**
	 * ESteamNetConnectionEnd_AppException_Generic
	 * @field ESteamNetConnectionEnd_AppException_Generic
	 */
	SETCONSTANT(ESteamNetConnectionEnd_AppException_Generic, k_ESteamNetConnectionEnd_AppException_Generic);
	/**
	 * ESteamNetConnectionEnd_Local_OfflineMode
	 * @field ESteamNetConnectionEnd_Local_OfflineMode
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Local_OfflineMode, k_ESteamNetConnectionEnd_Local_OfflineMode);
	/**
	 * ESteamNetConnectionEnd_Local_ManyRelayConnectivity
	 * @field ESteamNetConnectionEnd_Local_ManyRelayConnectivity
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Local_ManyRelayConnectivity, k_ESteamNetConnectionEnd_Local_ManyRelayConnectivity);
	/**
	 * ESteamNetConnectionEnd_Local_HostedServerPrimaryRelay
	 * @field ESteamNetConnectionEnd_Local_HostedServerPrimaryRelay
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Local_HostedServerPrimaryRelay, k_ESteamNetConnectionEnd_Local_HostedServerPrimaryRelay);
	/**
	 * ESteamNetConnectionEnd_Local_NetworkConfig
	 * @field ESteamNetConnectionEnd_Local_NetworkConfig
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Local_NetworkConfig, k_ESteamNetConnectionEnd_Local_NetworkConfig);
	/**
	 * ESteamNetConnectionEnd_Local_Rights
	 * @field ESteamNetConnectionEnd_Local_Rights
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Local_Rights, k_ESteamNetConnectionEnd_Local_Rights);
	/**
	 * ESteamNetConnectionEnd_Local_P2P_ICE_NoPublicAddresses
	 * @field ESteamNetConnectionEnd_Local_P2P_ICE_NoPublicAddresses
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Local_P2P_ICE_NoPublicAddresses, k_ESteamNetConnectionEnd_Local_P2P_ICE_NoPublicAddresses);
	/**
	 * ESteamNetConnectionEnd_Remote_Timeout
	 * @field ESteamNetConnectionEnd_Remote_Timeout
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Remote_Timeout, k_ESteamNetConnectionEnd_Remote_Timeout);
	/**
	 * ESteamNetConnectionEnd_Remote_BadCrypt
	 * @field ESteamNetConnectionEnd_Remote_BadCrypt
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Remote_BadCrypt, k_ESteamNetConnectionEnd_Remote_BadCrypt);
	/**
	 * ESteamNetConnectionEnd_Remote_BadCert
	 * @field ESteamNetConnectionEnd_Remote_BadCert
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Remote_BadCert, k_ESteamNetConnectionEnd_Remote_BadCert);
	/**
	 * ESteamNetConnectionEnd_Remote_BadProtocolVersion
	 * @field ESteamNetConnectionEnd_Remote_BadProtocolVersion
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Remote_BadProtocolVersion, k_ESteamNetConnectionEnd_Remote_BadProtocolVersion);
	/**
	 * ESteamNetConnectionEnd_Remote_P2P_ICE_NoPublicAddresses
	 * @field ESteamNetConnectionEnd_Remote_P2P_ICE_NoPublicAddresses
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Remote_P2P_ICE_NoPublicAddresses, k_ESteamNetConnectionEnd_Remote_P2P_ICE_NoPublicAddresses);
	/**
	 * ESteamNetConnectionEnd_Misc_Generic
	 * @field ESteamNetConnectionEnd_Misc_Generic
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_Generic, k_ESteamNetConnectionEnd_Misc_Generic);
	/**
	 * ESteamNetConnectionEnd_Misc_InternalError
	 * @field ESteamNetConnectionEnd_Misc_InternalError
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_InternalError, k_ESteamNetConnectionEnd_Misc_InternalError);
	/**
	 * ESteamNetConnectionEnd_Misc_Timeout
	 * @field ESteamNetConnectionEnd_Misc_Timeout
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_Timeout, k_ESteamNetConnectionEnd_Misc_Timeout);
	/**
	 * ESteamNetConnectionEnd_Misc_SteamConnectivity
	 * @field ESteamNetConnectionEnd_Misc_SteamConnectivity
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_SteamConnectivity, k_ESteamNetConnectionEnd_Misc_SteamConnectivity);
	/**
	 * ESteamNetConnectionEnd_Misc_NoRelaySessionsToClient
	 * @field ESteamNetConnectionEnd_Misc_NoRelaySessionsToClient
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_NoRelaySessionsToClient, k_ESteamNetConnectionEnd_Misc_NoRelaySessionsToClient);
	/**
	 * ESteamNetConnectionEnd_Misc_P2P_Rendezvous
	 * @field ESteamNetConnectionEnd_Misc_P2P_Rendezvous
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_P2P_Rendezvous, k_ESteamNetConnectionEnd_Misc_P2P_Rendezvous);
	/**
	 * ESteamNetConnectionEnd_Misc_P2P_NAT_Firewall
	 * @field ESteamNetConnectionEnd_Misc_P2P_NAT_Firewall
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_P2P_NAT_Firewall, k_ESteamNetConnectionEnd_Misc_P2P_NAT_Firewall);
	/**
	 * ESteamNetConnectionEnd_Misc_PeerSentNoConnection
	 * @field ESteamNetConnectionEnd_Misc_PeerSentNoConnection
	 */
	SETCONSTANT(ESteamNetConnectionEnd_Misc_PeerSentNoConnection, k_ESteamNetConnectionEnd_Misc_PeerSentNoConnection);


	/**
	 * ESteamNetworkingConnectionState_None
	 * @field ESteamNetworkingConnectionState_None
	 */
	SETCONSTANT(ESteamNetworkingConnectionState_None, k_ESteamNetworkingConnectionState_None);
	/**
	 * ESteamNetworkingConnectionState_Connecting
	 * @field ESteamNetworkingConnectionState_Connecting
	 */
	SETCONSTANT(ESteamNetworkingConnectionState_Connecting, k_ESteamNetworkingConnectionState_Connecting);
	/**
	 * ESteamNetworkingConnectionState_FindingRoute
	 * @field ESteamNetworkingConnectionState_FindingRoute
	 */
	SETCONSTANT(ESteamNetworkingConnectionState_FindingRoute, k_ESteamNetworkingConnectionState_FindingRoute);
	/**
	 * ESteamNetworkingConnectionState_Connected
	 * @field ESteamNetworkingConnectionState_Connected
	 */
	SETCONSTANT(ESteamNetworkingConnectionState_Connected, k_ESteamNetworkingConnectionState_Connected);
	/**
	 * ESteamNetworkingConnectionState_ClosedByPeer
	 * @field ESteamNetworkingConnectionState_ClosedByPeer
	 */
	SETCONSTANT(ESteamNetworkingConnectionState_ClosedByPeer, k_ESteamNetworkingConnectionState_ClosedByPeer);
	/**
	 * ESteamNetworkingConnectionState_ProblemDetectedLocally
	 * @field ESteamNetworkingConnectionState_ProblemDetectedLocally
	 */
	SETCONSTANT(ESteamNetworkingConnectionState_ProblemDetectedLocally, k_ESteamNetworkingConnectionState_ProblemDetectedLocally);


	/**
	 * ELobbyTypePrivate
	 * only way to join the lobby is to invite to someone else
	 * @field ELobbyTypePrivate
	 */
	SETCONSTANT(ELobbyTypePrivate, k_ELobbyTypePrivate);
	/**
	 * ELobbyTypeFriendsOnly
	 * shows for friends or invitees, but not in lobby list
	 * @field ELobbyTypeFriendsOnly
	 */
	SETCONSTANT(ELobbyTypeFriendsOnly, k_ELobbyTypeFriendsOnly);
	/**
	 * ELobbyTypePublic
	 * visible for friends and in lobby list
	 * @field ELobbyTypePublic
	 */
	SETCONSTANT(ELobbyTypePublic, k_ELobbyTypePublic);
	/**
	 * ELobbyTypeInvisible
	 * returned by search, but not visible to other friends 
	 * @field ELobbyTypeInvisible
	 */
	SETCONSTANT(ELobbyTypeInvisible, k_ELobbyTypeInvisible);
	/**
	 * ELobbyTypePrivateUnique
	 * private, unique and does not delete when empty
	 * @field ELobbyTypePrivateUnique
	 */
	SETCONSTANT(ELobbyTypePrivateUnique, k_ELobbyTypePrivateUnique);

	// ELobbyDistanceFilter
	SETCONSTANT(ELobbyDistanceFilterClose, k_ELobbyDistanceFilterClose);
	SETCONSTANT(ELobbyDistanceFilterDefault, k_ELobbyDistanceFilterDefault);
	SETCONSTANT(ELobbyDistanceFilterFar, k_ELobbyDistanceFilterFar);
	SETCONSTANT(ELobbyDistanceFilterWorldwide, k_ELobbyDistanceFilterWorldwide);

	// EMatchMakingLobbyComparison
	SETCONSTANT(ELobbyComparisonEqual, k_ELobbyComparisonEqual);
	SETCONSTANT(ELobbyComparisonNotEqual, k_ELobbyComparisonNotEqual);
	SETCONSTANT(ELobbyComparisonGreaterThan, k_ELobbyComparisonGreaterThan);
	SETCONSTANT(ELobbyComparisonGreaterThanOrEqual, k_ELobbyComparisonEqualToOrGreaterThan);
	SETCONSTANT(ELobbyComparisonLessThan, k_ELobbyComparisonLessThan);
	SETCONSTANT(ELobbyComparisonLessThanOrEqual, k_ELobbyComparisonEqualToOrLessThan);

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
