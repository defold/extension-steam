# API reference

* src
  * [steam_friends](#steam_friends)
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
* `Boolean` [`ok`] - 


### user_stats_get_stat_float(id)
Get user stat as a floating point number. 


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `Boolean` [`ok`] - 
* `Number` [`stat`] - The stat


### user_stats_set_stat_float(id,stat)
Set user stat. 


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set

RETURNS
* `Boolean` [`ok`] - 


### SteamUserStats_RequestCurrentStats()
Ask the server to send down this user&#x27;s data and achievements for this game. 


RETURNS
* `bool` [`ok`] - True if successful


### user_stats_get_downloaded_leaderboard_entry(leaderboard,index)
Returns data about a single leaderboard entry 


PARAMS
* `leaderboard` [`string`] - Leaderboard handle
* `index` [`number`] - Which entry to get

RETURNS
* `boolean` [`ok`] - 
* `table` [`entry`] - The requested leaderboard entry.


---

