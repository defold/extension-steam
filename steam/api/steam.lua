--[[
Steam API documentation.
--]]

---Functions and constants for interacting with Steamworks.
---@class defold_api.steam
steam = {}

-- Constants

---Enter dismisses the keyboard
---@type number
steam.EFloatingGamepadTextInputModeModeSingleLine = nil

---User needs to explicitly dismiss the keyboard
---@type number
steam.EFloatingGamepadTextInputModeModeMultipleLines = nil

---Keyboard is displayed in a special mode that makes it easier to enter emails
---@type number
steam.EFloatingGamepadTextInputModeModeEmail = nil

---Numeric keypad is shown
---@type number
steam.EFloatingGamepadTextInputModeModeNumeric = nil

---Normal text input
---@type number
steam.EGamepadTextInputModeNormal = nil

---Password text input
---@type number
steam.EGamepadTextInputModePassword = nil

---Single line text
---@type number
steam.EGamepadTextInputLineModeSingleLine = nil

---Multi line text
---@type number
steam.EGamepadTextInputLineModeMultipleLines = nil

-- Leaderboard constants
---Requests rows in the leaderboard from the full table.
---@type number
steam.ELeaderboardDataRequestGlobal = nil

---Requests rows in the leaderboard from rows around the user.
---@type number
steam.ELeaderboardDataRequestGlobalAroundUser = nil

---Requests all the rows for friends of the current user.
---@type number
steam.ELeaderboardDataRequestFriends = nil

---@type number
steam.ELeaderboardSortMethodNone = nil

---Top-score is lowest number.
---@type number
steam.ELeaderboardSortMethodAscending = nil

---Top-score is highest number.
---@type number
steam.ELeaderboardSortMethodDescending = nil

---@type number
steam.ELeaderboardUploadScoreMethodNone = nil

---Leaderboard will keep user's best score.
---@type number
steam.ELeaderboardUploadScoreMethodKeepBest = nil

---Leaderboard will always replace score with specified.
---@type number
steam.ELeaderboardUploadScoreMethodForceUpdate = nil

---@type number
steam.ELeaderboardDisplayTypeNone = nil

---Simple numerical score.
---@type number
steam.ELeaderboardDisplayTypeNumeric = nil

---The score represents a time, in seconds.
---@type number
steam.ELeaderboardDisplayTypeTimeSeconds = nil

---The score represents a time, in milliseconds.
---@type number
steam.ELeaderboardDisplayTypeTimeMilliSeconds = nil

-- Overlay / Store Flags
---Passed as parameter to the store.
---@type number
steam.EOverlayToStoreFlag_None = nil

---Passed as parameter to the store.
---@type number
steam.EOverlayToStoreFlag_AddToCart = nil

---Passed as parameter to the store.
---@type number
steam.EOverlayToStoreFlag_AddToCartAndShow = nil

---Passed as parameter to ActivateGameOverlayToWebPage.
---@type number
steam.EActivateGameOverlayToWebPageMode_Default = nil

---Passed as parameter to ActivateGameOverlayToWebPage.
---@type number
steam.EActivateGameOverlayToWebPageMode_Modal = nil

-- Persona States
---Friend is not currently logged on.
---@type number
steam.EPersonaStateOffline = nil

---Friend is logged on.
---@type number
steam.EPersonaStateOnline = nil

---User is on, but busy.
---@type number
steam.EPersonaStateBusy = nil

---Auto-away feature.
---@type number
steam.EPersonaStateAway = nil

---Auto-away for a long time.
---@type number
steam.EPersonaStateSnooze = nil

---Online, trading.
---@type number
steam.EPersonaStateLookingToTrade = nil

---Online, wanting to play.
---@type number
steam.EPersonaStateLookingToPlay = nil

---Online, but appears offline to friends. This status is never published to clients.
---@type number
steam.EPersonaStateInvisible = nil

-- Friend Flags
---@type number
steam.EFriendFlagNone = nil

---@type number
steam.EFriendFlagBlocked = nil

---@type number
steam.EFriendFlagFriendshipRequested = nil

---@type number
steam.EFriendFlagImmediate = nil

---@type number
steam.EFriendFlagClanMember = nil

---@type number
steam.EFriendFlagOnGameServer = nil

---@type number
steam.EFriendFlagRequestingFriendship = nil

---@type number
steam.EFriendFlagRequestingInfo = nil

---@type number
steam.EFriendFlagIgnored = nil

---@type number
steam.EFriendFlagIgnoredFriend = nil

---@type number
steam.EFriendFlagChatMember = nil

---@type number
steam.EFriendFlagAll = nil

-- Networking
---@type number
steam.SteamNetworkingSend_Unreliable = nil

---@type number
steam.SteamNetworkingSend_NoNagle = nil

---@type number
steam.SteamNetworkingSend_UnreliableNoNagle = nil

---@type number
steam.SteamNetworkingSend_NoDelay = nil

---@type number
steam.SteamNetworkingSend_UnreliableNoDelay = nil

---@type number
steam.SteamNetworkingSend_Reliable = nil

---@type number
steam.SteamNetworkingSend_ReliableNoNagle = nil

---@type number
steam.SteamNetworkingSend_UseCurrentThread = nil

---@type number
steam.SteamNetworkingSend_AutoRestartBrokenSession = nil

-- Connection End Reasons
---@type number
steam.ESteamNetConnectionEnd_Invalid = nil

---@type number
steam.ESteamNetConnectionEnd_App_Generic = nil

---@type number
steam.ESteamNetConnectionEnd_AppException_Generic = nil

---@type number
steam.ESteamNetConnectionEnd_Local_OfflineMode = nil

---@type number
steam.ESteamNetConnectionEnd_Local_ManyRelayConnectivity = nil

---@type number
steam.ESteamNetConnectionEnd_Local_HostedServerPrimaryRelay = nil

---@type number
steam.ESteamNetConnectionEnd_Local_NetworkConfig = nil

---@type number
steam.ESteamNetConnectionEnd_Local_Rights = nil

---@type number
steam.ESteamNetConnectionEnd_Local_P2P_ICE_NoPublicAddresses = nil

---@type number
steam.ESteamNetConnectionEnd_Remote_Timeout = nil

---@type number
steam.ESteamNetConnectionEnd_Remote_BadCrypt = nil

---@type number
steam.ESteamNetConnectionEnd_Remote_BadCert = nil

---@type number
steam.ESteamNetConnectionEnd_Remote_BadProtocolVersion = nil

---@type number
steam.ESteamNetConnectionEnd_Remote_P2P_ICE_NoPublicAddresses = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_Generic = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_InternalError = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_Timeout = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_SteamConnectivity = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_NoRelaySessionsToClient = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_P2P_Rendezvous = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_P2P_NAT_Firewall = nil

---@type number
steam.ESteamNetConnectionEnd_Misc_PeerSentNoConnection = nil

-- Connection States
---@type number
steam.ESteamNetworkingConnectionState_None = nil

---@type number
steam.ESteamNetworkingConnectionState_Connecting = nil

---@type number
steam.ESteamNetworkingConnectionState_FindingRoute = nil

---@type number
steam.ESteamNetworkingConnectionState_Connected = nil

---@type number
steam.ESteamNetworkingConnectionState_ClosedByPeer = nil

---@type number
steam.ESteamNetworkingConnectionState_ProblemDetectedLocally = nil

-- Lobby Types
---ELobbyTypePrivate only way to join the lobby is to invite to someone else
---@type number
steam.ELobbyTypePrivate = nil

---ELobbyTypeFriendsOnly shows for friends or invitees, but not in lobby list
---@type number
steam.ELobbyTypeFriendsOnly = nil

---ELobbyTypePublic visible for friends and in lobby list
---@type number
steam.ELobbyTypePublic = nil

---ELobbyTypeInvisible returned by search, but not visible to other friends
---@type number
steam.ELobbyTypeInvisible = nil

---ELobbyTypePrivateUnique private, unique and does not delete when empty
---@type number
steam.ELobbyTypePrivateUnique = nil

-- Auth session responses
---EAuthSessionResponseOK Steam has verified the user is online, the ticket is valid and ticket has not been reused.
---@type number
steam.EAuthSessionResponseOK = nil

---EAuthSessionResponseUserNotConnectedToSteam The user in question is not connected to steam.
---@type number
steam.EAuthSessionResponseUserNotConnectedToSteam = nil

---EAuthSessionResponseNoLicenseOrExpired The user doesn't have a license for this App ID or the ticket has expired.
---@type number
steam.EAuthSessionResponseNoLicenseOrExpired = nil

---EAuthSessionResponseVACBanned The user is VAC banned for this game.
---@type number
steam.EAuthSessionResponseVACBanned = nil

---EAuthSessionResponseLoggedInElseWhere The user account has logged in elsewhere and the session containing the game instance has been disconnected.
---@type number
steam.EAuthSessionResponseLoggedInElseWhere = nil

---EAuthSessionResponseVACCheckTimedOut VAC has been unable to perform anti-cheat checks on this user.
---@type number
steam.EAuthSessionResponseVACCheckTimedOut = nil

---EAuthSessionResponseAuthTicketCanceled The ticket has been canceled by the issuer.
---@type number
steam.EAuthSessionResponseAuthTicketCanceled = nil

---EAuthSessionResponseAuthTicketInvalidAlreadyUsed This ticket has already been used, it is not valid.
---@type number
steam.EAuthSessionResponseAuthTicketInvalidAlreadyUsed = nil

---EAuthSessionResponseAuthTicketInvalid This ticket is not from a user instance currently connected to steam.
---@type number
steam.EAuthSessionResponseAuthTicketInvalid = nil

---EAuthSessionResponsePublisherIssuedBan The user is banned for this game. The ban came via the web api and not VAC.
---@type number
steam.EAuthSessionResponsePublisherIssuedBan = nil

---EAuthSessionResponseAuthTicketNetworkIdentityFailure The network identity in the ticket does not match the server authenticating the ticket.
---@type number
steam.EAuthSessionResponseAuthTicketNetworkIdentityFailure = nil


----------------------------------------------------------------
-- Core lifecycle
----------------------------------------------------------------

---Initialize Steamworks.
---@param appid number
---@return boolean success True if successful
---@return string message Error message if unsuccessful.
function steam.init(appid) end


---Update Steamworks. Call this from a script component.
function steam.update() end


---Restart Steamworks.
---@param appid number
function steam.restart(appid) end


---Finalize Steamworks.
function steam.final() end


----------------------------------------------------------------
-- Apps
----------------------------------------------------------------

---Takes AppID of DLC and checks if the user owns the DLC & if the DLC is installed.
---@param app_id number
---@return boolean installed
function steam.apps_is_dlc_installed(app_id) end


----------------------------------------------------------------
-- Friends
----------------------------------------------------------------

---Returns the local players name - guaranteed to not be NULL. This is the same name as on the users community profile page. This is stored in UTF-8 format.
---@return string name Name of user
function steam.friends_get_persona_name() end


---Gets the status of the current user. Returned as EPersonaState.
---@return number state Status of user.
function steam.friends_get_persona_state() end


---Friend iteration. Takes a set of EFriendFlags, and returns the number of users the client knows about who meet that criteria. Then GetFriendByIndex() can then be used to return the id's of each of those users.
---@param friend_flags number Set of friend flags to match friends against.
---@return number count Number of users matching search.
function steam.friends_get_friend_count(friend_flags) end


---Returns the steamID of a user. The returned CSteamID can then be used by all the functions below to access details about the user.
---@param index number Is a index of range [0, GetFriendCount())
---@param friend_flags number Must be the same value as used in GetFriendCount()
---@return string steam_id Steam id of the user
function steam.friends_get_friend_by_index(index, friend_flags) end


---Returns the name of another user. Same rules as GetFriendPersonaState() apply as to whether or not the user knowns the name of the other user note that on first joining a lobby, chat room or game server the local user will not known the name of the other users automatically; that information will arrive asyncronously.
---@param steam_id string
---@return string name Name of user
function steam.friends_get_friend_persona_name(steam_id) end


---Returns the current status of the specified user. This will only be known by the local user if steamIDFriend is in their friends list; on the same game server; in a chat room or lobby; or in a small group with the local user.
---@param steam_id string Id of friend
---@return number state State of friend
function steam.friends_get_friend_persona_state(steam_id) end


---Get friends steam level.
---@param steam_id string Id of friend
---@return number level Steam level of friend
function steam.friends_get_friend_steam_level(steam_id) end


---Returns a relationship to a user.
---@param steam_id string Id of friend
---@return number relationship Relationship to the user.
function steam.friends_get_friend_relationship(steam_id) end


---Gets a handle to the small (32*32px) avatar for the specified user. This is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
---@param steam_id string Id of friend
---@return number handle Image handle.
function steam.friends_get_small_friend_avatar(steam_id) end


---Gets a handle to the medium (64*64px) avatar for the specified user. This is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
---@param steam_id string Id of friend
---@return number handle Image handle.
function steam.friends_get_medium_friend_avatar(steam_id) end


---Gets a handle to the large (128*128px) avatar for the specified user. This is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
---@param steam_id string Id of friend
---@return number handle Image handle.
function steam.friends_get_large_friend_avatar(steam_id) end


---Activates game overlay to store page for app.
---@param app_id number
---@param flag number EOverlayToStoreFlag
function steam.friends_activate_game_overlay_to_store(app_id, flag) end


---Activates game overlay web browser directly to the specified URL. Full address with protocol type is required, e.g. http://www.steamgames.com/
---@param url string
---@param mode number EActivateGameOverlayToWebPageMode
function steam.friends_activate_game_overlay_to_web_page(url, mode) end


---Sets a Rich Presence key/value for the current user.
---@param key string
---@param value string
---@return boolean success True if the rich presence was set successfully, otherwise False.
function steam.friends_set_rich_presence(key, value) end


---Clears all of the current user's Rich Presence key/values.
function steam.friends_clear_rich_presence() end


---Invites a friend or clan member to the current game using a special invite string. If the target accepts the invite, a GameRichPresenceJoinRequested_t callback is posted containing the connect string.
---@param steam_id string Id of friend
---@param connect string String
---@return boolean success
function steam.friends_invite_user_to_game(steam_id, connect) end


----------------------------------------------------------------
-- Matchmaking / Lobbies
----------------------------------------------------------------

---Adds a string comparison filter to the next RequestLobbyList call.
---@param key string
---@param value number
---@param cmp number
function steam.matchmaking_add_request_lobby_list_string_filter(key, value, cmp) end


---Adds a numerical comparison filter to the next RequestLobbyList call.
---@param key string
---@param value number
---@param cmp number
function steam.matchmaking_add_request_lobby_list_numerical_filter(key, value, cmp) end


---Sorts the results closest to the specified value.
---@param key string
---@param value number
function steam.matchmaking_add_request_lobby_list_near_value_filter(key, value) end


---Filters to only return lobbies with the specified number of open slots. Available.
---@param slots number
function steam.matchmaking_add_request_lobby_list_filter_slots_available(slots) end


---Sets the physical distance for which we should search for lobbies, this is. Based on the users IP address and a IP location map on the Steam backed.
---@param dist number
function steam.matchmaking_add_request_lobby_list_distance_filter(dist) end


---Sets the maximum number of lobbies to return. The lower the count the faster. It is to download the lobby results & details to the client.
---@param max_count number
function steam.matchmaking_add_request_lobby_list_result_count_filter(max_count) end


---Unused - Checks the player compatibility based on the frenemy system.
---@param steam_id string
function steam.matchmaking_add_request_lobby_list_compatible_members_filter(steam_id) end


---Get a filtered list of relevant lobbies. Will return results as a LobbyMatchList_t event
---@return string id Callback id
function steam.matchmaking_request_lobby_list() end


---Gets the Steam ID of the lobby at the specified index. This should only be called after a LobbyMatchList_t call result is received
---@param index number The index of the lobby to get the Steam ID of, from 0 to LobbyMatchList_t.m_nLobbiesMatching
---@return string id Id of lobby
function steam.matchmaking_get_lobby_by_index(index) end


---Create a new matchmaking lobby. Will generate a LobbyCreated_t, LobbyEnter_t and LobbyDataUpdate_t event
---@param lobby_type number The type and visibility of this lobby.
---@param max_members number The maximum number of players that can join this lobby.
---@return string id Callback id
function steam.matchmaking_create_lobby(lobby_type, max_members) end


---Joins an existing lobby. Will generate a LobbyEnter_t event
---@param lobby_id string The Steam ID of the lobby to join.
---@return string id Callback id
function steam.matchmaking_join_lobby(lobby_id) end


---Leave a lobby that the user is currently in. Leave a lobby that the user is currently in; this will take effect immediately on the client side, other users in the lobby will be notified by a LobbyChatUpdate_t callback.
---@param lobby_id string The lobby to leave
function steam.matchmaking_leave_lobby(lobby_id) end


---Returns the current lobby owner. There always one lobby owner - if the current owner leaves, another user in the lobby will become the owner automatically. It is possible (but rare) to join a lobby just as the owner is leaving, thus entering a lobby with self as the owner. You must be a member of the lobby to access this.
---@param lobby_id string The Steam ID of the lobby to get the owner of.
---@return string id Id of owner
function steam.matchmaking_get_lobby_owner(lobby_id) end


---Changes who the lobby owner is. This can only be set by the owner of the lobby. This will trigger a LobbyDataUpdate_t for all of the users in the lobby, each user should update their local state to reflect the new owner. This is typically accomplished by displaying a crown icon next to the owners name.
---@param lobby_id string The Steam ID of the lobby to get the owner of.
---@param new_owner string The new owner
function steam.matchmaking_set_lobby_owner(lobby_id, new_owner) end


---Updates what type of lobby this is. This is also set when you create the lobby with CreateLobby. This can only be set by the owner of the lobby.
---@param lobby_id string The Steam ID of the lobby
---@param type number The lobby type
function steam.matchmaking_set_lobby_type(lobby_id, type) end


---Sets whether or not a lobby is joinable by other players. This always defaults to enabled for a new lobby. If joining is disabled, then no players can join, even if they are a friend or have been invited. Lobbies with joining disabled will not be returned from a lobby search.
---@param lobby_id string The Steam ID of the lobby
---@param joinable boolean Enable or disable allowing users to join this lobby?
---@return boolean result Success
function steam.matchmaking_set_lobby_joinable(lobby_id, joinable) end


---Set the maximum number of players that can join the lobby. This is also set when you create the lobby with CreateLobby. This can only be set by the owner of the lobby.
---@param lobby_id string The Steam ID of the lobby to set the member limit for.
---@param max_members number The maximum number of players allowed in this lobby. This can not be above 250.
---@return boolean result Success
function steam.matchmaking_set_lobby_member_limit(lobby_id, max_members) end


---The current limit on the # of users who can join the lobby.
---@param lobby_id string The Steam ID of the lobby to get the member limit of.
---@return number limit The current limit
function steam.matchmaking_get_lobby_member_limit(lobby_id) end


---Gets the number of users in a lobby. This is used for iteration, after calling this then GetLobbyMemberByIndex can be used to get the Steam ID of each person in the lobby. Persona information for other lobby members (name, avatar, etc.) is automatically received and accessible via the ISteamFriends interface. The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.
---@param lobby_id string The Steam ID of the lobby to get the owner of.
---@return number count Number of users in the lobby
function steam.matchmaking_get_num_lobby_members(lobby_id) end


---Gets the Steam ID of the lobby member at the given index. You must call matchmaking_get_num_lobby_members before calling this. The current user must be in the lobby to retrieve the Steam IDs of other users in that lobby.
---@param lobby_id string
---@param index number
---@return string id Id of member
function steam.matchmaking_get_lobby_member_by_index(lobby_id, index) end


---Sets a key/value pair in the lobby metadata.
---@param lobby_id string
---@param key string
---@param data string
---@return boolean result
function steam.matchmaking_set_lobby_data(lobby_id, key, data) end


---Sets per-user metadata for the local user.
---@param lobby_id string
---@param key string
---@param data string
function steam.matchmaking_set_lobby_member_data(lobby_id, key, data) end


---Get data associated with this lobby.
---@param lobby_id string
---@param key string
---@return string data Data
function steam.matchmaking_get_lobby_data(lobby_id, key) end


---Gets per-user metadata from another player in the specified lobby.
---@param lobby_id string
---@param user_id string
---@param key string
---@return string data Data
function steam.matchmaking_get_lobby_member_data(lobby_id, user_id, key) end


---Returns the number of metadata keys set on the specified lobby.
---@param lobby_id string
---@return number count Number of keys
function steam.matchmaking_get_lobby_data_count(lobby_id) end


---Returns a lobby metadata key/values pair by index.
---@param lobby_id string
---@param index number
---@return boolean success
---@return string key
---@return string value
function steam.matchmaking_get_lobby_data_by_index(lobby_id, index) end


---Broadcasts a chat message to the all the users in the lobby.
---@param lobby_id string
---@param body string
---@return boolean success
function steam.matchmaking_send_lobby_chat_message(lobby_id, body) end


---Get a chat message as specified in a LobbyChatMsg_t callback.
---@param lobby_id string
---@param index number
---@return string user
---@return string data
---@return number type
function steam.matchmaking_get_lobby_chat_entry(lobby_id, index) end


----------------------------------------------------------------
-- User / Stats / Achievements
----------------------------------------------------------------

---Returns the CSteamID of the account currently logged into the Steam client. A CSteamID is a unique identifier for an account, and used to differentiate users in all parts of the Steamworks API.
---@return string steam_id
function steam.user_get_steam_id() end


---Gets the Steam Level of the user, as shown on their profile.
---@return number level
function steam.user_get_player_steam_level() end


---Trading Card badges data access. If you only have one set of cards, the series will be 1. The user has can have two different badges for a series; the regular (max level 5) and the foil (max level 1).
---@return number series
---@return boolean foil
function steam.user_get_game_badge_level() end


---Returns true if the Steam client current has a live connection to the Steam. Servers.
---@return boolean logged_on
function steam.user_logged_on() end


---Returns true if this users looks like they are behind a NAT device. Only valid once the user has connected to steam .
---@return boolean behind_nat
function steam.user_is_behind_nat() end


---Gets whether the users phone number is verified.
---@return boolean verified
function steam.user_is_phone_verified() end


---Gets whether the users phone number is identifying.
---@return boolean verifying
function steam.user_is_phone_identifying() end


---Gets whether the users phone number is awaiting (re)verification.
---@return boolean verification
function steam.user_is_phone_requiring_verification() end


---Gets whether the user has two factor enabled on their account.
---@return boolean enabled
function steam.user_is_two_factor_enabled() end


---Get an authentication ticket. Retrieve an authentication ticket to be sent to the entity who wishes to authenticate you.
---@return string ticket Auth ticket or null
---@return number handle Ticket handle or null
---@return string error Error message or null
function steam.user_get_auth_session_ticket() end


---Validate an authentication ticket. Authenticate the ticket from the entity Steam ID to be sure it is valid and isn't reused. Note that identity is not confirmed until the callback ValidateAuthTicketResponse_t is received and the return value in that callback is checked for success.
---@param ticket string The auth ticket to validate
---@param steam_id string The entity's Steam ID that sent this ticket.
---@return number result
function steam.user_begin_auth_session(ticket, steam_id) end


---Cancels an auth ticket. Cancels an auth ticket received from GetAuthSessionTicket or GetAuthTicketForWebApi. This should be called when no longer playing with the specified entity.
---@param ticket number The active auth ticket to cancel.
function steam.user_cancel_auth_ticket(ticket) end


---Ends an auth session. Ends an auth session that was started with BeginAuthSession. This should be called when no longer playing with the specified entity.
---@param steam_id string The entity to end the active auth session with.
function steam.user_end_auth_session(steam_id) end


---Get an authentication ticket for web API. Request an authentication ticket suitable to authenticated in a web backend. Will trigger a GetTicketForWebApiResponse_t callback when the ticket is ready.
---@param identity string Optional identity string to associate with the ticket
---@return number hAuthTicket The handle of the requested ticket
---@return string error Error message or null
function steam.user_get_auth_ticket_for_web_api(identity) end


---Get user stat as an integer. https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat
---@param id string Id of the stat to get
---@return boolean ok
---@return number stat The stat or nil
function steam.user_stats_get_stat_int(id) end


---Set user stat. https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat
---@param id string Id of the stat to set
---@param stat number Number to set
---@return boolean ok
function steam.user_stats_set_stat_int(id, stat) end


---Get user stat as a floating point number. https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat
---@param id string Id of the stat to get
---@return boolean ok
---@return number stat The stat
function steam.user_stats_get_stat_float(id) end


---Set user stat. https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat
---@param id string Id of the stat to set
---@param stat number Number to set
---@return boolean ok
function steam.user_stats_set_stat_float(id, stat) end


---Requests global stats data, which is available for stats marked as "aggregated". This call is asynchronous, with the results returned in GlobalStatsReceived_t. nHistoryDays specifies how many days of day-by-day history to retrieve in addition to the overall totals. The limit is 60. https://partner.steamgames.com/doc/api/ISteamUserStats#RequestGlobalStats
---@param history_days number
---@return boolean ok
function steam.user_stats_request_global_stats(history_days) end


---Store the current data on the server. Will get a callback when set and one callback for every new achievement  If the callback has a result of k_EResultInvalidParam, one or more stats uploaded has been rejected, either because they broke constraints or were out of date. In this case the server sends back updated values. The stats should be re-iterated to keep in sync. https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats
---@return boolean ok
function steam.user_stats_store_stats() end


---Reset stats. https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats
---@param achievements boolean True if achievements should be reset as well.
---@return boolean ok
function steam.user_stats_reset_all_stats(achievements) end


---Set achievement. https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement
---@param name string
---@return boolean ok
function steam.user_stats_set_achievement(name) end


---Get achievement. https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement
---@param name string
---@return boolean ok
---@return boolean achieved
function steam.user_stats_get_achievement(name) end


---Clear achievement. https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement
---@param name string
---@return boolean ok
function steam.user_stats_clear_achievement(name) end


---Used for iterating achievements. In general games should not need these functions because they should have a list of existing achievements compiled into them. https://partner.steamgames.com/doc/api/ISteamUserStats#GetNumAchievements
---@return number num Number of achievements.
function steam.user_stats_get_num_achievements() end


---Get achievement name iAchievement in [0,GetNumAchievements). https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementName
---@param index number
---@return string name
function steam.user_stats_get_achievement_name(index) end


---Get general attributes for an achievement. Accepts the following keys * "name" and "desc" for retrieving the localized achievement name and description (returned in UTF8) * "hidden" for retrieving if an achievement is hidden (returns "0" when not hidden, "1" when hidden) https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementDisplayAttribute
---@param name string
---@param key string Either "name", "desc" or "hidden"
---@return string attribute
function steam.user_stats_get_achievement_display_attribute(name, key) end


---Returns the percentage of users who have achieved the specified achievement. https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievementAchievedPercent
---@return boolean ok
---@return number percent
function steam.user_stats_get_achievement_achieved_percent() end


---Find a leaderboard. Will return leaderboard asynchronously. https://partner.steamgames.com/doc/api/ISteamUserStats#FindLeaderboard
---@param name string
function steam.user_stats_find_leaderboard(name) end


---Gets a leaderboard by name, it will create it if it's not yet created. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardFindResult_t. https://partner.steamgames.com/doc/api/ISteamUserStats#FindOrCreateLeaderboard
---@param leaderboard_name string The name of the leaderboard to find or create.
---@param eLeaderboardSortMethod number The sort order of the new leaderboard if it's created.
---@param eLeaderboardDisplayType number The display type (used by the Steam Community web site) of the new leaderboard if it's created.
function steam.user_stats_find_or_create_leaderboard(leaderboard_name, eLeaderboardSortMethod, eLeaderboardDisplayType) end


---Get the name of a leaderboard. https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardName
---@param leaderboard string
---@return string name
function steam.user_stats_get_leaderboard_name(leaderboard) end


---Get the total number of entries in a leaderboard, as of the last request. https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardEntryCount
---@param leaderboard string
---@return number count
function steam.user_stats_get_leaderboard_entry_count(leaderboard) end


---Returns the sort method of the leaderboard. https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardSortMethod
---@param leaderboard string
---@return number sort_order
function steam.user_stats_get_leaderboard_sort_method(leaderboard) end


---Returns the display type of a leaderboard handle. https://partner.steamgames.com/doc/api/ISteamUserStats#GetLeaderboardDisplayType
---@param leaderboard string
---@return number display_type
function steam.user_stats_get_leaderboard_display_type(leaderboard) end


---Asks the Steam back-end for a set of rows in the leaderboard. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardScoresDownloaded_t. LeaderboardScoresDownloaded_t will contain a handle to pull the results from GetDownloadedLeaderboardEntries(). You can ask for more entries than exist, and it will return as many as do exist. * k_ELeaderboardDataRequestGlobal requests rows in the leaderboard from the full table, with nRangeStart & nRangeEnd in the range [1, TotalEntries] * k_ELeaderboardDataRequestGlobalAroundUser requests rows around the current user, nRangeStart being negate e.g. DownloadLeaderboardEntries( hLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -3, 3 ) will return 7 rows, 3 before the user, 3 after * k_ELeaderboardDataRequestFriends requests all the rows for friends of the current user https://partner.steamgames.com/doc/api/ISteamUserStats#DownloadLeaderboardEntries
---@param leaderboard string
---@param request number
---@param start number
---@param end_ number
function steam.user_stats_download_leaderboard_entries(leaderboard, request, start, end_) end


---Returns data about a single leaderboard entry. https://partner.steamgames.com/doc/api/ISteamUserStats#GetDownloadedLeaderboardEntry
---@param hSteamLeaderboardEntries string Leaderboard entries handle
---@param index number Which entry to get
---@return boolean ok
---@return table entry The requested leaderboard entry.
function steam.user_stats_get_downloaded_leaderboard_entry(hSteamLeaderboardEntries, index) end


---Uploads a user score to a specified leaderboard. This call is asynchronous, with the result returned in a listener callback with event set to LeaderboardScoreUploaded_t. https://partner.steamgames.com/doc/api/ISteamUserStats#UploadLeaderboardScore
---@param leaderboard string
---@param eLeaderboardUploadScoreMethod number
---@param nScore number
function steam.user_stats_upload_leaderboard_score(leaderboard, eLeaderboardUploadScoreMethod, nScore) end


---Attaches a piece of user generated content the current user's entry on a. Leaderboard. https://partner.steamgames.com/doc/api/ISteamUserStats#AttachLeaderboardUGC
---@param leaderboard string
---@param ugc_handle string
---@return string Steam Steam API call id
function steam.user_stats_attach_leadboard_ugc(leaderboard, ugc_handle) end


----------------------------------------------------------------
-- Remote Storage
----------------------------------------------------------------

---Share a file.
---@param filename string Name of file to share
---@return string id
function steam.remote_storage_file_share(filename) end


---Get number of uploaded files.
---@return number count File count
function steam.remote_storage_get_file_count() end


---Get file information.
---@return number count File count
function steam.remote_storage_get_file_name_and_size() end


---Get storage quota.
---@return number available Available bytes
---@return number total Total bytes
function steam.remote_storage_get_quota() end


---Creates a new file, writes the bytes to the file, and then closes the file. If the target file already exists, it is overwritten.
---@param filename string The name of the file to write to.
---@param data string
---@return boolean result Success
function steam.remote_storage_file_write(filename, data) end


---Opens a binary file, reads the contents of the file into a byte array,. And then closes the file.
---@param filename string Name of the file to read from
---@return string data
function steam.remote_storage_file_read(filename) end


---@param filename string
---@return boolean
function steam.remote_storage_file_delete(filename) end


----------------------------------------------------------------
-- Networking
----------------------------------------------------------------

---Send message.
---@param identity_remote string
---@param data string
---@param send_flags number A bitmask of k_nSteamNetworkingSend_xxx options
---@param remote_channel number A routing number you can use to help route message to different systems
function steam.networking_send_message_to_user(identity_remote, data, send_flags, remote_channel) end


---Receive message.
---@param localChannel number
---@return table message
function steam.networking_receive_messages_on_channel(localChannel) end


---Accept session. Call this in response to a SteamNetworkingMessagesSessionRequest_t callback.
---@param identity_remote string
---@return boolean result Returns false if there is no session with the user pending or otherwise. If there is an existing active session, this function will return true, even if it is not pending.
function steam.networking_accept_session_with_user(identity_remote) end


---Close sesssion. Call this when you're done talking to a user to immediately free up resources under-the-hood
---@param identity_remote string
---@return boolean result
function steam.networking_close_session_with_user(identity_remote) end


---Close channel. Call this when you're done talking to a user on a specific channel.
---@param identity_remote string
---@param local_channel number
---@return boolean result
function steam.networking_close_channel_with_user(identity_remote, local_channel) end


---Get connection info. Returns information about the latest state of a connection, if any, with the given peer.
---@param identity_remote string
---@return table info Connection info (state, info, status)
function steam.networking_get_session_connection_info(identity_remote) end


----------------------------------------------------------------
-- Utilities
----------------------------------------------------------------

---Returns the appID of the current process.
---@return number app_id
function steam.utils_get_app_id() end


---Return the number of seconds since the user.
---@return number seconds
function steam.utils_get_seconds_since_app_active() end


---Returns true if currently running on the Steam Deck device.
---@return boolean running_on_steamdeck
function steam.utils_is_steam_running_on_steam_deck() end


---Returns true if the Steam Overlay is running and the user can access it.
---@return boolean steam_overlay_available
function steam.utils_is_steam_overlay_available() end


---Get size of image.
---@param image number Image handle
---@return boolean ok True if size of image was read successfully
---@return number width Image width or nil
---@return number height Image height or nil
function steam.utils_get_image_size(image) end


---Get image in RGBA format.
---@param image number Image handle
---@param size number Size of image
---@return boolean ok True if size of image was read successfully
---@return string Image
function steam.utils_get_image_rgba(image, size) end


---Returns the Steam server time in Unix epoch format. (Number of seconds since Jan 1, 1970 UTC).
---@return number Server Time
function steam.utils_get_server_real_time() end


---Opens a floating keyboard over the game content and sends OS keyboard keys directly to the game.
---@param mode number EFloatingGamepadTextInputMode
---@param x number Text field x position
---@param y number Text field y position
---@param width number Text field width
---@param height number Text field height
---@return boolean result True if the floating keyboard was shown, otherwise, false.
function steam.utils_show_floating_gamepad_text_input(mode, x, y, width, height) end


---Activates the Big Picture text input dialog which only supports gamepad input.
---@param input_mode number EGamepadTextInputMode
---@param line_input_mode number EGamepadTextInputLineMode
---@param description string Sets the description that should inform the user what the input dialog is for
---@param existing_text string Sets the preexisting text which the user can edit.
---@return boolean result True if the big picture overlay is running; otherwise, false
function steam.utils_show_gamepad_text_input(input_mode, line_input_mode, description, existing_text) end


----------------------------------------------------------------
-- Listener
----------------------------------------------------------------

---Set a listener.
---@param listener function Listener function to call
function steam.set_listener(listener) end


return steam
