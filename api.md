# API reference

* src
  * [steam_friends](#steam_friends)
  * [steam_user](#steam_user)
  * [steam_userstats](#steam_userstats)

# src

## steam_friends
*File: `steam/src/steam_friends.cpp`*

Interface to access information about individual users and interact with the Steam Overlay.


### SteamFriends_GetFriendPersonaName()
Returns the name of another user. Same rules as GetFriendPersonaState() apply as to whether or not the user knowns the name of the other user note that on first joining a lobby, chat room or game server the local user will not known the name of the other users automatically; that information will arrive asyncronously.


RETURNS
* `String` [`name`] - Name of user


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


### SteamUserStats_RequestCurrentStats()
Ask the server to send down this user&#x27;s data and achievements for this game. 


RETURNS
* `ok` [`Boolean`] - True if successful


### user_stats_get_downloaded_leaderboard_entry(leaderboard,index)
Returns data about a single leaderboard entry 


PARAMS
* `leaderboard` [`string`] - Leaderboard handle
* `index` [`number`] - Which entry to get

RETURNS
* `ok` [`Boolean`] - 
* `entry` [`Table`] - The requested leaderboard entry.


---

