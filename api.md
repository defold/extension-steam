# API reference

* src
  * [steam](#steam)
  * [steam_apps](#steam_apps)
  * [steam_friends](#steam_friends)
  * [steam_listener](#steam_listener)
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


### finalize()
Finalize Steamworks. 



### ELeaderboardDataRequestGlobal [`ELeaderboardDataRequestGlobal`]
Requests rows in the leaderboard from the full table 



### ELeaderboardDataRequestGlobalAroundUser [`ELeaderboardDataRequestGlobalAroundUser`]
Requests rows in the leaderboard from rows around the user 



### ELeaderboardDataRequestFriends [`ELeaderboardDataRequestFriends`]
Requests all the rows for friends of the current user 



### EOverlayToStoreFlag_None [`EOverlayToStoreFlag_None`]
Passed as parameter to the store 



### EOverlayToStoreFlag_AddToCart [`EOverlayToStoreFlag_AddToCart`]
Passed as parameter to the store 



### EOverlayToStoreFlag_AddToCartAndShow [`EOverlayToStoreFlag_AddToCartAndShow`]
Passed as parameter to the store 



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


### friends_activate_game_overlay_to_store(app_id,flag)
Activates game overlay to store page for app. 


PARAMS
* `app_id` [`number`] - 
* `flag` [`number`] - EOverlayToStoreFlag


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


---

## steam_userstats
*File: `steam/src/steam_userstats.cpp`*

Provides functions for accessing and submitting stats, achievements, and Leaderboards.


### user_stats_get_stat_int(id)
Get user stat as an integer. 


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `Boolean` [`ok`] - 
* `Number` [`stat`] - The stat or nil


### user_stats_set_stat_int(id,stat)
Set user stat. 


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set

RETURNS
* `ok` [`boolean`] - 


### user_stats_get_stat_float(id)
Get user stat as a floating point number. 


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `ok` [`Boolean`] - 
* `stat` [`Number`] - The stat


### user_stats_set_stat_float(id,stat)
Set user stat. 


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set

RETURNS
* `ok` [`Boolean`] - 


### user_stats_request_current_stats()
Ask the server to send down this user&#x27;s data and achievements for this game. 


RETURNS
* `ok` [`Boolean`] - True if successful


### user_stats_request_global_stats(history_days)
Requests global stats data, which is available for stats marked as &quot;aggregated&quot;. This call is asynchronous, with the results returned in GlobalStatsReceived_t. nHistoryDays specifies how many days of day-by-day history to retrieve in addition to the overall totals. The limit is 60.


PARAMS
* `history_days` [`number`] - 

RETURNS
* `ok` [`Boolean`] - 


### user_stats_store_stats()
Store the current data on the server. Will get a callback when set and one callback for every new achievement  If the callback has a result of k_EResultInvalidParam, one or more stats uploaded has been rejected, either because they broke constraints or were out of date. In this case the server sends back updated values. The stats should be re-iterated to keep in sync.


RETURNS
* `ok` [`Boolean`] - 


### user_stats_reset_all_stats(achievements)
Reset stats. 


PARAMS
* `achievements` [`boolean`] - True if achievements should be reset as well.

RETURNS
* `ok` [`Boolean`] - 


### user_stats_set_achievement(name)
Set achievement. 


PARAMS
* `name` [`string`] - 

RETURNS
* `ok` [`Boolean`] - 


### user_stats_get_achievement(name)
Get achievement. 


PARAMS
* `name` [`string`] - 

RETURNS
* `ok` [`Boolean`] - 
* `achieved` [`Boolean`] - 


### user_stats_clear_achievement(name)
Clear achievement. 


PARAMS
* `name` [`string`] - 

RETURNS
* `ok` [`Boolean`] - 


### user_stats_get_num_achievements()
Used for iterating achievements. In general games should not need these functions because they should have a list of existing achievements compiled into them.


RETURNS
* `num` [`Number`] - Number of achievements.


### user_stats_get_achievement_name(index)
Get achievement name iAchievement in [0,GetNumAchievements) 


PARAMS
* `index` [`number`] - 

RETURNS
* `name` [`String`] - 


### user_stats_get_achievement_display_attribute(name,key)
Get general attributes for an achievement. Accepts the following keys * &quot;name&quot; and &quot;desc&quot; for retrieving the localized achievement name and description (returned in UTF8) * &quot;hidden&quot; for retrieving if an achievement is hidden (returns &quot;0&quot; when not hidden, &quot;1&quot; when hidden)


PARAMS
* `name` [`string`] - 
* `key` [`string`] - Either &quot;name&quot;, &quot;desc&quot; or &quot;hidden&quot;

RETURNS
* `attribute` [`String`] - 


### user_stats_get_achievement_achieved_percent()
Returns the percentage of users who have achieved the specified achievement. 


RETURNS
* `ok` [`Boolean`] - 
* `percent` [`Number`] - 


### user_stats_download_leaderboard_entries(leaderboard,request,start,end)
Asks the Steam back-end for a set of rows in the leaderboard. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardScoresDownloaded_t. LeaderboardScoresDownloaded_t will contain a handle to pull the results from GetDownloadedLeaderboardEntries(). You can ask for more entries than exist, and it will return as many as do exist. * k_ELeaderboardDataRequestGlobal requests rows in the leaderboard from the full table, with nRangeStart &amp; nRangeEnd in the range [1, TotalEntries] * k_ELeaderboardDataRequestGlobalAroundUser requests rows around the current user, nRangeStart being negate e.g. DownloadLeaderboardEntries( hLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -3, 3 ) will return 7 rows, 3 before the user, 3 after * k_ELeaderboardDataRequestFriends requests all the rows for friends of the current user


PARAMS
* `leaderboard` [`string`] - 
* `request` [`ELeaderboardDataRequest`] - 
* `start` [`number`] - 
* `end` [`number`] - 

RETURNS
* `handle` [`string`] - 


### user_stats_get_downloaded_leaderboard_entry(leaderboard,index)
Returns data about a single leaderboard entry 


PARAMS
* `leaderboard` [`string`] - Leaderboard handle
* `index` [`number`] - Which entry to get

RETURNS
* `ok` [`Boolean`] - 
* `entry` [`Table`] - The requested leaderboard entry.


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


---

