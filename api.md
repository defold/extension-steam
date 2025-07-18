# API reference

* src
  * [steam](#steam)
  * [steam_apps](#steam_apps)
  * [steam_friends](#steam_friends)
  * [steam_listener](#steam_listener)
  * [steam_matchmaking](#steam_matchmaking)
  * [steam_networking](#steam_networking)
  * [steam_remotestorage](#steam_remotestorage)
  * [steam_user](#steam_user)
  * [steam_userstats](#steam_userstats)
  * [steam_utils](#steam_utils)

# src

## steam
*File: `steam/src/steam.cpp`*




### init()
Initialize Steamworks. 


RETURNS
* `success` [`Boolean`] - True if successful
* `message` [`String`] - Error message if unsuccessful.


### update()
Update Steamworks. Call this from a script component.



### restart(appid)
Restart Steamworks. 


PARAMS
* `appid` [`number`] - 


### final()
Finalize Steamworks. 



### ELeaderboardDataRequestGlobal [`ELeaderboardDataRequestGlobal`]
Requests rows in the leaderboard from the full table 



### ELeaderboardDataRequestGlobalAroundUser [`ELeaderboardDataRequestGlobalAroundUser`]
Requests rows in the leaderboard from rows around the user 



### ELeaderboardDataRequestFriends [`ELeaderboardDataRequestFriends`]
Requests all the rows for friends of the current user 



### ELeaderboardSortMethodAscending [`ELeaderboardSortMethodAscending`]
Top-score is lowest number 



### ELeaderboardSortMethodDescending [`ELeaderboardSortMethodDescending`]
Top-score is highest number 



### ELeaderboardUploadScoreMethodKeepBest [`ELeaderboardUploadScoreMethodKeepBest`]
Leaderboard will keep user&#x27;s best score 



### ELeaderboardUploadScoreMethodForceUpdate [`ELeaderboardUploadScoreMethodForceUpdate`]
Leaderboard will always replace score with specified 



### ELeaderboardDisplayTypeNumeric [`ELeaderboardDisplayTypeNumeric`]
Simple numerical score 



### ELeaderboardDisplayTypeTimeSeconds [`ELeaderboardDisplayTypeTimeSeconds`]
The score represents a time, in seconds 



### ELeaderboardDisplayTypeTimeMilliSeconds [`ELeaderboardDisplayTypeTimeMilliSeconds`]
The score represents a time, in milliseconds 



### EOverlayToStoreFlag_None [`EOverlayToStoreFlag_None`]
Passed as parameter to the store 



### EOverlayToStoreFlag_AddToCart [`EOverlayToStoreFlag_AddToCart`]
Passed as parameter to the store 



### EOverlayToStoreFlag_AddToCartAndShow [`EOverlayToStoreFlag_AddToCartAndShow`]
Passed as parameter to the store 



### EActivateGameOverlayToWebPageMode_Default [`EActivateGameOverlayToWebPageMode_Default`]
Passed as parameter to ActivateGameOverlayToWebPage 



### EActivateGameOverlayToWebPageMode_Modal [`EActivateGameOverlayToWebPageMode_Modal`]
Passed as parameter to ActivateGameOverlayToWebPage 



### EPersonaStateOffline [`EPersonaStateOffline`]
Friend is not currently logged on 



### EPersonaStateOnline [`EPersonaStateOnline`]
Friend is logged on 



### EPersonaStateBusy [`EPersonaStateBusy`]
User is on, but busy 



### EPersonaStateAway [`EPersonaStateAway`]
Auto-away feature 



### EPersonaStateSnooze [`EPersonaStateSnooze`]
Auto-away for a long time 



### EPersonaStateLookingToTrade [`EPersonaStateLookingToTrade`]
Online, trading 



### EPersonaStateLookingToPlay [`EPersonaStateLookingToPlay`]
Online, wanting to play 



### EPersonaStateInvisible [`EPersonaStateInvisible`]
Online, but appears offline to friends.  This status is never published to clients. 



---

## steam_apps
*File: `steam/src/steam_apps.cpp`*




### apps_is_dlc_installed(app_id)
Takes AppID of DLC and checks if the user owns the DLC &amp; if the DLC is installed. 


PARAMS
* `app_id` [`number`] - 

RETURNS
* `installed` [`bool`] - 


---

## steam_friends
*File: `steam/src/steam_friends.cpp`*

Interface to access information about individual users and interact with the Steam Overlay.


### friends_get_friend_persona_name(CSteamID)
Returns the name of another user. Same rules as GetFriendPersonaState() apply as to whether or not the user knowns the name of the other user note that on first joining a lobby, chat room or game server the local user will not known the name of the other users automatically; that information will arrive asyncronously.


PARAMS
* `CSteamID` [`friendId`] - 

RETURNS
* `String` [`name`] - Name of user


### friends_get_persona_name()
Returns the local players name - guaranteed to not be NULL. This is the same name as on the users community profile page. This is stored in UTF-8 format.


RETURNS
* `String` [`name`] - Name of user


### friends_get_persona_state()
Gets the status of the current user. Returned as EPersonaState.


RETURNS
* `Number` [`state`] - Status of user.


### friends_get_friend_count(iFriendFlags)
Friend iteration. Takes a set of EFriendFlags, and returns the number of users the client knows about who meet that criteria. Then GetFriendByIndex() can then be used to return the id&#x27;s of each of those users.


PARAMS
* `iFriendFlags` [`number`] - Set of friend flags to match friends against.

RETURNS
* `Number` [`count`] - Number of users matching search.


### friends_get_friend_by_index(iFriend,iFriendsFlags)
Returns the steamID of a user. The returned CSteamID can then be used by all the functions below to access details about the user.


PARAMS
* `iFriend` [`number`] - Is a index of range [0, GetFriendCount())
* `iFriendsFlags` [`number`] - Must be the same value as used in GetFriendCount()

RETURNS
* `Number` [`steamID`] - Steam id of the user


### friends_get_friend_persona_state(steamIDFriend)
Returns the current status of the specified user. This will only be known by the local user if steamIDFriend is in their friends list; on the same game server; in a chat room or lobby; or in a small group with the local user.


PARAMS
* `steamIDFriend` [`number`] - Id of friend

RETURNS
* `Number` [`state`] - State of friend


### friends_get_friend_steam_level(steamIDFriend)
Get friends steam level. 


PARAMS
* `steamIDFriend` [`number`] - Id of friend

RETURNS
* `Number` [`level`] - Steam level of friend


### friends_get_friend_relationship(steamIDFriend)
Returns a relationship to a user. 


PARAMS
* `steamIDFriend` [`number`] - Id of friend

RETURNS
* `Number` [`relationship`] - Relationship to the user.


### friends_activate_game_overlay_to_store(app_id,flag)
Activates game overlay to store page for app. 


PARAMS
* `app_id` [`number`] - 
* `flag` [`number`] - EOverlayToStoreFlag


### friends_activate_game_overlay_to_web_page(url,mode)
Activates game overlay web browser directly to the specified URL. Full address with protocol type is required, e.g. http://www.steamgames.com/


PARAMS
* `url` [`string`] - 
* `mode` [`number`] - EActivateGameOverlayToWebPageMode


### friends_set_rich_presence(key,value)
Sets a Rich Presence key/value for the current user. 


PARAMS
* `key` [`string`] - 
* `value` [`string`] - 

RETURNS
* `success` [`bool`] - True if the rich presence was set successfully, otherwise False.


### friends_clear_rich_presence()
Clears all of the current user&#x27;s Rich Presence key/values. 



### friends_invite_user_to_game(steamIDFriend,connect)
Invites a friend or clan member to the current game using a special invite string. If the target accepts the invite, a GameRichPresenceJoinRequested_t callback is posted containing the connect string.


PARAMS
* `steamIDFriend` [`number`] - Id of friend
* `connect` [`string`] - String

RETURNS
* `success` [`boolean`] - 


---

## steam_listener
*File: `steam/src/steam_listener.cpp`*




### set_listener(listener)
Set a listener. 
```
 steam.set_listener(function(self, event, data)if event == "GameOverlayActivated_t" then
print("Overlay is active:", data.m_bActive)
end
end)
```

PARAMS
* `listener` [`function`] - Listener function to call


---

## steam_matchmaking
*File: `steam/src/steam_matchmaking.cpp`*




### matchmaking_request_lobby_list()
Get a filtered list of relevant lobbies. Will return results as a LobbyMatchList_t event


RETURNS
* `Callback` [`string`] - Id


### matchmaking_get_lobby_by_index(index)
Gets the Steam ID of the lobby at the specified index. This should only be called after a LobbyMatchList_t call result is received LobbyMatchList_t.m_nLobbiesMatching


PARAMS
* `index` [`number`] - The index of the lobby to get the Steam ID of, from 0 to

RETURNS
* `Id` [`string`] - Of lobby


### matchmaking_join_lobby(lobby_id)
Joins an existing lobby. Will generate a LobbyEnter_t event


PARAMS
* `lobby_id` [`string`] - The Steam ID of the lobby to join.

RETURNS
* `Callback` [`string`] - Id


### matchmaking_leave_lobby(lobby_id)
Leave a lobby that the user is currently in Leave a lobby that the user is currently in; this will take effect immediately on the client side, other users in the lobby will be notified by a LobbyChatUpdate_t callback.


PARAMS
* `lobby_id` [`string`] - The lobby to leave


### matchmaking_get_lobby_owner(lobby_id)
Returns the current lobby owner. There always one lobby owner - if the current owner leaves, another user in the lobby will become the owner automatically. It is possible (but rare) to join a lobby just as the owner is leaving, thus entering a lobby with self as the owner. You must be a member of the lobby to access this.


PARAMS
* `lobby_id` [`string`] - The Steam ID of the lobby to get the owner of.

RETURNS
* `Id` [`string`] - Of owner


### matchmaking_get_num_lobby_members(lobby_id)
Gets the number of users in a lobby. This is used for iteration, after calling this then GetLobbyMemberByIndex can be used to get the Steam ID of each person in the lobby. Persona information for other lobby members (name, avatar, etc.) is automatically received and accessible via the ISteamFriends interface. The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.


PARAMS
* `lobby_id` [`string`] - The Steam ID of the lobby to get the owner of.

RETURNS
* `Number` [`number`] - Of users in the lobby


### matchmaking_get_lobby_member_by_index(lobby_id,index)
Gets the Steam ID of the lobby member at the given index. You must call matchmaking_get_num_lobby_members before calling this. The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.


PARAMS
* `lobby_id` [`string`] - 
* `index` [`number`] - 

RETURNS
* `Id` [`string`] - Of member


---

## steam_networking
*File: `steam/src/steam_networking.cpp`*




### networking_send_message_to_user(identity_remote,data,send_flags,remote_channel)
Send message 


PARAMS
* `identity_remote` [`string`] - 
* `data` [`string`] - 
* `send_flags` [`number`] - A bitmask of k_nSteamNetworkingSend_xxx options
* `remote_channel` [`number`] - A routing number you can use to help route message to different systems


### networking_receive_messages_on_channel(localChannel)
Receive message 


PARAMS
* `localChannel` [`number`] - 

RETURNS
* `message` [`table`] - 


### networking_accept_session_with_user(identity_remote)
Accept session Call this in response to a SteamNetworkingMessagesSessionRequest_t callback. pending or otherwise. If there is an existing active session, this function will return true, even if it is not pending.


PARAMS
* `identity_remote` [`string`] - 

RETURNS
* `result` [`boolean`] - Returns false if there is no session with the user


### networking_close_session_with_user(identity_remote)
Close sesssion Call this when you&#x27;re done talking to a user to immediately free up resources under-the-hood


PARAMS
* `identity_remote` [`string`] - 

RETURNS
* `result` [`boolean`] - 


### networking_close_channel_with_user(identity_remote,local_channel)
Close channel Call this when you&#x27;re done talking to a user on a specific channel.


PARAMS
* `identity_remote` [`string`] - 
* `local_channel` [`number`] - 

RETURNS
* `result` [`boolean`] - 


### networking_get_session_connection_info(identity_remote)
Get connection info Returns information about the latest state of a connection, if any, with the given peer.


PARAMS
* `identity_remote` [`string`] - 

RETURNS
* `info` [`table`] - Connection info (state, info, status)


---

## steam_remotestorage
*File: `steam/src/steam_remotestorage.cpp`*




### remote_storage_file_share(filename)
Share a file 


PARAMS
* `filename` [`string`] - Name of file to share

RETURNS
* `call` [`string`] - Id


### remote_storage_get_file_count()
Get number of uploaded files 


RETURNS
* `File` [`number`] - Count


### remote_storage_get_file_name_and_size()
Get file information 


RETURNS
* `File` [`number`] - Count


### remote_storage_get_quota()
Get storage quota 


RETURNS
* `Available` [`number`] - Bytes
* `Total` [`number`] - Bytes


### remote_storage_file_write(filename,data)
Creates a new file, writes the bytes to the file, and then closes the file. If the target file already exists, it is overwritten.


PARAMS
* `filename` [`string`] - The name of the file to write to.
* `data` [`string`] - 

RETURNS
* `Success` [`boolean`] - 


### remote_storage_file_read(filename)
Opens a binary file, reads the contents of the file into a byte array, And then closes the file.


PARAMS
* `filename` [`string`] - Name of the file to read from

RETURNS
* `data` [`string`] - 


---

## steam_user
*File: `steam/src/steam_user.cpp`*




### user_get_steam_id()
Returns the CSteamID of the account currently logged into the Steam client. A CSteamID is a unique identifier for an account, and used to differentiate users in all parts of the Steamworks API.


RETURNS
* `id` [`CSteamID`] - 


### user_get_player_steam_level()
Gets the Steam Level of the user, as shown on their profile. 


RETURNS
* `level` [`number`] - 


### user_get_game_badge_level()
Trading Card badges data access. If you only have one set of cards, the series will be 1. The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1).


RETURNS
* `series` [`number`] - 
* `foil` [`boolean`] - 


### user_logged_on()
Returns true if the Steam client current has a live connection to the Steam Servers.


RETURNS
* `logged_on` [`boolean`] - 


### user_is_behind_nat()
Returns true if this users looks like they are behind a NAT device. Only valid once the user has connected to steam .


RETURNS
* `behind_nat` [`boolean`] - 


### user_is_phone_verified()
Gets whether the users phone number is verified. 


RETURNS
* `verified` [`boolean`] - 


### user_is_phone_identifying()
Gets whether the users phone number is identifying. 


RETURNS
* `verifying` [`boolean`] - 


### user_is_phone_requiring_verification()
Gets whether the users phone number is awaiting (re)verification. 


RETURNS
* `verification` [`boolean`] - 


### user_is_two_factor_enabled()
Gets whether the user has two factor enabled on their account. 


RETURNS
* `enabled` [`boolean`] - 


### user_get_auth_session_ticket()
Get an authentication ticket. Retrieve an authentication ticket to be sent to the entity who wishes to authenticate you.


RETURNS
* `ticket` [`string`] - Or null
* `error` [`string`] - Or null


---

## steam_userstats
*File: `steam/src/steam_userstats.cpp`*

Provides functions for accessing and submitting stats, achievements, and Leaderboards.


### user_stats_get_stat_int(id)
Get user stat as an integer. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `Boolean` [`ok`] - 
* `Number` [`stat`] - The stat or nil


### user_stats_set_stat_int(id,stat)
Set user stat. Https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set

RETURNS
* `ok` [`boolean`] - 


### user_stats_get_stat_float(id)
Get user stat as a floating point number. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `ok` [`Boolean`] - 
* `stat` [`Number`] - The stat


### user_stats_set_stat_float(id,stat)
Set user stat. Https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set

RETURNS
* `ok` [`Boolean`] - 


### user_stats_request_global_stats(history_days)
Requests global stats data, which is available for stats marked as &quot;aggregated&quot;. This call is asynchronous, with the results returned in GlobalStatsReceived_t. nHistoryDays specifies how many days of day-by-day history to retrieve in addition to the overall totals. The limit is 60. https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalStats


PARAMS
* `history_days` [`number`] - 

RETURNS
* `ok` [`Boolean`] - 


### user_stats_store_stats()
Store the current data on the server. Will get a callback when set and one callback for every new achievement  If the callback has a result of k_EResultInvalidParam, one or more stats uploaded has been rejected, either because they broke constraints or were out of date. In this case the server sends back updated values. The stats should be re-iterated to keep in sync. https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats


RETURNS
* `ok` [`Boolean`] - 


### user_stats_reset_all_stats(achievements)
Reset stats. Https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats


PARAMS
* `achievements` [`boolean`] - True if achievements should be reset as well.

RETURNS
* `ok` [`Boolean`] - 


### user_stats_set_achievement(name)
Set achievement. Https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement


PARAMS
* `name` [`string`] - 

RETURNS
* `ok` [`Boolean`] - 


### user_stats_get_achievement(name)
Get achievement. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement


PARAMS
* `name` [`string`] - 

RETURNS
* `ok` [`Boolean`] - 
* `achieved` [`Boolean`] - 


### user_stats_clear_achievement(name)
Clear achievement. Https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement


PARAMS
* `name` [`string`] - 

RETURNS
* `ok` [`Boolean`] - 


### user_stats_get_num_achievements()
Used for iterating achievements. In general games should not need these functions because they should have a list of existing achievements compiled into them. https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumAchievements


RETURNS
* `num` [`Number`] - Number of achievements.


### user_stats_get_achievement_name(index)
Get achievement name iAchievement in [0,GetNumAchievements) Https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementName


PARAMS
* `index` [`number`] - 

RETURNS
* `name` [`String`] - 


### user_stats_get_achievement_display_attribute(name,key)
Get general attributes for an achievement. Accepts the following keys * &quot;name&quot; and &quot;desc&quot; for retrieving the localized achievement name and description (returned in UTF8) * &quot;hidden&quot; for retrieving if an achievement is hidden (returns &quot;0&quot; when not hidden, &quot;1&quot; when hidden) https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementDisplayAttribute


PARAMS
* `name` [`string`] - 
* `key` [`string`] - Either &quot;name&quot;, &quot;desc&quot; or &quot;hidden&quot;

RETURNS
* `attribute` [`String`] - 


### user_stats_get_achievement_achieved_percent()
Returns the percentage of users who have achieved the specified achievement. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAchievedPercent


RETURNS
* `ok` [`Boolean`] - 
* `percent` [`Number`] - 


### user_stats_find_or_create_leaderboard(leaderboard_name,eLeaderboardSortMethod,eLeaderboardDisplayType)
Gets a leaderboard by name, it will create it if it&#x27;s not yet created. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardFindResult_t. https://partner.steamgames.com/doc/api/ISteamUserStats#FindOrCreateLeaderboard


PARAMS
* `leaderboard_name` [`string`] - The name of the leaderboard to find or create.
* `eLeaderboardSortMethod` [`ELeaderboardSortMethod`] - The sort order of the new leaderboard if it&#x27;s created.
* `eLeaderboardDisplayType` [`ELeaderboardDisplayType`] - The display type (used by the Steam Community web site) of the new leaderboard if it&#x27;s created.


### user_stats_get_leaderboard_name(leaderboard)
Get the name of a leaderboard. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardName


PARAMS
* `leaderboard` [`string`] - 

RETURNS
* `name` [`string`] - 


### user_stats_get_leaderboard_entry_count(leaderboard)
Get the total number of entries in a leaderboard, as of the last request. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardEntryCount


PARAMS
* `leaderboard` [`string`] - 

RETURNS
* `count` [`number`] - 


### user_stats_get_leaderboard_sort_method(leaderboard)
Returns the sort method of the leaderboard Https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardSortMethod


PARAMS
* `leaderboard` [`string`] - 

RETURNS
* `sort` [`number`] - Order


### user_stats_get_leaderboard_display_type(leaderboard)
Returns the display type of a leaderboard handle. Https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardDisplayType


PARAMS
* `leaderboard` [`string`] - 

RETURNS
* `display` [`number`] - Type


### user_stats_download_leaderboard_entries(leaderboard,request,start,end)
Asks the Steam back-end for a set of rows in the leaderboard. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardScoresDownloaded_t. LeaderboardScoresDownloaded_t will contain a handle to pull the results from GetDownloadedLeaderboardEntries(). You can ask for more entries than exist, and it will return as many as do exist. * k_ELeaderboardDataRequestGlobal requests rows in the leaderboard from the full table, with nRangeStart &amp; nRangeEnd in the range [1, TotalEntries] * k_ELeaderboardDataRequestGlobalAroundUser requests rows around the current user, nRangeStart being negate e.g. DownloadLeaderboardEntries( hLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -3, 3 ) will return 7 rows, 3 before the user, 3 after * k_ELeaderboardDataRequestFriends requests all the rows for friends of the current user https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntries


PARAMS
* `leaderboard` [`string`] - 
* `request` [`ELeaderboardDataRequest`] - 
* `start` [`number`] - 
* `end` [`number`] - 


### user_stats_get_downloaded_leaderboard_entry(hSteamLeaderboardEntries,index)
Returns data about a single leaderboard entry Https://partner.steamgames.com/doc/api/ISteamUserStats#GetDownloadedLeaderboardEntry


PARAMS
* `hSteamLeaderboardEntries` [`string`] - Leaderboard entries handle
* `index` [`number`] - Which entry to get

RETURNS
* `ok` [`Boolean`] - 
* `entry` [`Table`] - The requested leaderboard entry.


### user_stats_upload_leaderboard_score(leaderboard,eLeaderboardUploadScoreMethod,nScore)
Uploads a user score to a specified leaderboard. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardScoreUploaded_t. https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore


PARAMS
* `leaderboard` [`string`] - 
* `eLeaderboardUploadScoreMethod` [`ELeaderboardUploadScoreMethod`] - 
* `nScore` [`number`] - 


### user_stats_attach_leadboard_ugc(leaderboard,ugc_handle)
Attaches a piece of user generated content the current user&#x27;s entry on a Leaderboard. https://partner.steamgames.com/doc/api/ISteamUserStats#AttachLeaderboardUGC


PARAMS
* `leaderboard` [`string`] - 
* `ugc_handle` [`string`] - 

RETURNS
* `Steam` [`string`] - API call id


---

## steam_utils
*File: `steam/src/steam_utils.cpp`*




### utils_get_app_id()
Returns the appID of the current process. 


RETURNS
* `app_id` [`Number`] - 


### utils_get_seconds_since_app_active()
Return the number of seconds since the user. 


RETURNS
* `seconds` [`Number`] - 


### utils_is_steam_running_on_steam_deck()
Returns true if currently running on the Steam Deck device. 


RETURNS
* `running_on_steamdeck` [`Boolean`] - 


### utils_get_image_size(image)
Get size of image 


PARAMS
* `image` [`number`] - Image handle

RETURNS
* `ok` [`Boolean`] - True if size of image was read successfully
* `width` [`Number`] - Image width or nil
* `height` [`Number`] - Image height or nil


### utils_get_image_rgba(image,size)
Get image in RGBA format. 


PARAMS
* `image` [`number`] - Image handle
* `size` [`number`] - Size of image

RETURNS
* `ok` [`Boolean`] - True if size of image was read successfully
* `Image` [`String`] - 


### utils_get_server_real_time()
Returns the Steam server time in Unix epoch format. (Number of seconds since Jan 1, 1970 UTC) 


RETURNS
* `Server` [`number`] - Time


### utils_show_floating_gamepad_text_input(mode,x,y,width,height)
Opens a floating keyboard over the game content and sends OS keyboard keys directly to the game. 


PARAMS
* `mode` [`number`] - EFloatingGamepadTextInputMode
* `x` [`number`] - Text field x position
* `y` [`number`] - Text field y position
* `width` [`number`] - Text field width
* `height` [`number`] - Text field height

RETURNS
* `result` [`bool`] - True if the floating keyboard was shown, otherwise, false.


### utils_show_gamepad_text_input(input_mode,line_input_mode,description,existing_text)
Activates the Big Picture text input dialog which only supports gamepad input. 


PARAMS
* `input_mode` [`number`] - EGamepadTextInputMode
* `line_input_mode` [`number`] - EGamepadTextInputLineMode
* `description` [`string`] - Sets the description that should inform the user what the input dialog is for
* `existing_text` [`string`] - Sets the preexisting text which the user can edit.

RETURNS
* `result` [`bool`] - True if the big picture overlay is running; otherwise, false


---

