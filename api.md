# API reference

* src
  * [steam_userstats](#steam_userstats)

# src

## steam_userstats
*File: `steam/src/steam_userstats.cpp`*




### user_stats_get_stat_int(id)
Get user stat as an integer. 


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `Number` [`stat`] - The stat or nil


### user_stats_set_stat_int(id,stat)
Set user stat. 


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set


### user_stats_get_stat_float(id)
Get user stat as a floating point number. 


PARAMS
* `id` [`string`] - Id of the stat to get

RETURNS
* `Number` [`stat`] - The stat


### user_stats_set_stat_float(id,stat)
Set user stat. 


PARAMS
* `id` [`string`] - Id of the stat to set
* `stat` [`number`] - Number to set


### None()
Ask the server to send down this user&#x27;s data and achievements for this game. 


RETURNS
* `bool` [`ok`] - True if successful


---

