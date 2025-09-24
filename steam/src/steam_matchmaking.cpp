/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamMatchmaking* g_SteamMatchmaking = 0;

int SteamMatchmaking_OnLobbyMatchList(lua_State* L, void* data)
{
	LobbyMatchList_t* s = (LobbyMatchList_t*)data;
	lua_pushstring(L, "LobbyMatchList_t");

	lua_newtable(L);
	table_push_uint64(L, "m_nLobbiesMatching", s->m_nLobbiesMatching);

	return 2;
}

int SteamMatchmaking_OnLobbyEnter(lua_State* L, void* data)
{
	LobbyEnter_t* s = (LobbyEnter_t*)data;
	lua_pushstring(L, "LobbyEnter_t");

	lua_newtable(L);
	table_push_uint64(L, "m_ulSteamIDLobby", s->m_ulSteamIDLobby);
	table_push_number(L, "m_rgfChatPermissions", s->m_rgfChatPermissions);
	table_push_boolean(L, "m_bLocked", s->m_bLocked);
	table_push_number(L, "m_EChatRoomEnterResponse", s->m_EChatRoomEnterResponse);

	return 2;
}

int SteamMatchmaking_OnLobbyCreated(lua_State* L, void* data)
{
	LobbyCreated_t* s = (LobbyCreated_t*)data;
	lua_pushstring(L, "LobbyCreated_t");

	lua_newtable(L);
	table_push_uint64(L, "m_ulSteamIDLobby", s->m_ulSteamIDLobby);
	table_push_number(L, "m_eResult", s->m_eResult);

	return 2;
}

int SteamMatchmaking_OnLobbyChatMsg(lua_State* L, void* data)
{
	LobbyChatMsg_t* s = (LobbyChatMsg_t*)data;
	lua_pushstring(L, "LobbyChatMsg_t");

	lua_newtable(L);
	table_push_uint64(L, "m_ulSteamIDLobby", s->m_ulSteamIDLobby);
	table_push_uint64(L, "m_ulSteamIDUser", s->m_ulSteamIDUser);
	table_push_number(L, "m_eChatEntryType", s->m_eChatEntryType);
	table_push_number(L, "m_iChatID", s->m_iChatID);

	return 2;
}

int SteamMatchmaking_OnLobbyDataUpdate(lua_State* L, void* data)
{
	LobbyDataUpdate_t* s = (LobbyDataUpdate_t*)data;
	lua_pushstring(L, "LobbyDataUpdate");

	lua_newtable(L);
	table_push_uint64(L, "m_ulSteamIDLobby", s->m_ulSteamIDLobby);
	table_push_uint64(L, "m_ulSteamIDMember", s->m_ulSteamIDMember);
	table_push_boolean(L, "m_bSuccess", s->m_bSuccess);

	return 2;
}

int SteamMatchmaking_OnLobbyChatUpdate(lua_State* L, void* data)
{
	LobbyChatUpdate_t* s = (LobbyChatUpdate_t*)data;
	lua_pushstring(L, "LobbyChatUpdate_t");

	lua_newtable(L);
	table_push_uint64(L, "m_ulSteamIDLobby", s->m_ulSteamIDLobby);
	table_push_uint64(L, "m_ulSteamIDUserChanged", s->m_ulSteamIDUserChanged);
	table_push_uint64(L, "m_ulSteamIDMakingChange", s->m_ulSteamIDMakingChange);
	table_push_number(L, "m_rgfChatMemberStateChange", s->m_rgfChatMemberStateChange);

	return 2;
}


int SteamMatchmaking_Init(lua_State* L)
{
	g_SteamMatchmaking = SteamMatchmaking();
	return 0;
}


/** Adds a string comparison filter to the next RequestLobbyList call.
 * @name matchmaking_add_request_lobby_list_string_filter
 * @string key
 * @number value
 * @number cmp
 */
int SteamMatchmaking_AddRequestLobbyListStringFilter(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	const char* key = luaL_checkstring(L, 1);
	const char* value = luaL_checkstring(L, 2);
	ELobbyComparison cmp = (ELobbyComparison)luaL_checknumber(L, 3);
	g_SteamMatchmaking->AddRequestLobbyListStringFilter(key, value, cmp);
	return 0;
}

/** Adds a numerical comparison filter to the next RequestLobbyList call.
 * @name matchmaking_add_request_lobby_list_numerical_filter
 * @string key
 * @number value
 * @number cmp
 */
int SteamMatchmaking_AddRequestLobbyListNumericalFilter(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	const char* key = luaL_checkstring(L, 1);
	int value = (int)luaL_checknumber(L, 2);
	ELobbyComparison cmp = (ELobbyComparison)luaL_checknumber(L, 3);
	g_SteamMatchmaking->AddRequestLobbyListNumericalFilter(key, value, cmp);
	return 0;
}

/** Sorts the results closest to the specified value.
 * @name matchmaking_add_request_lobby_list_near_value_filter
 * @string key
 * @number value
 */
int SteamMatchmaking_AddRequestLobbyListNearValueFilter(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	const char* key = luaL_checkstring(L, 1);
	int value = (int)luaL_checknumber(L, 2);
	g_SteamMatchmaking->AddRequestLobbyListNearValueFilter(key, value);
	return 0;
}

/** Filters to only return lobbies with the specified number of open slots
 * available.
 * @name matchmaking_add_request_lobby_list_filter_slots_available
 * @number slots
 */
int SteamMatchmaking_AddRequestLobbyListFilterSlotsAvailable(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	int slots = (int)luaL_checknumber(L, 1);
	g_SteamMatchmaking->AddRequestLobbyListFilterSlotsAvailable(slots);
	return 0;
}

/** Sets the physical distance for which we should search for lobbies, this is
 * based on the users IP address and a IP location map on the Steam backed.
 * @name matchmaking_add_request_lobby_list_distance_filter
 * @number dist
 */
int SteamMatchmaking_AddRequestLobbyListDistanceFilter(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	ELobbyDistanceFilter dist = (ELobbyDistanceFilter)luaL_checknumber(L, 1);
	g_SteamMatchmaking->AddRequestLobbyListDistanceFilter(dist);
	return 0;
}

/** Sets the maximum number of lobbies to return. The lower the count the faster
 * it is to download the lobby results & details to the client.
 * @name matchmaking_add_request_lobby_list_result_count_filter
 * @number max_count
 */
int SteamMatchmaking_AddRequestLobbyListResultCountFilter(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	int maxCount = (int)luaL_checknumber(L, 1);
	g_SteamMatchmaking->AddRequestLobbyListResultCountFilter(maxCount);
	return 0;
}

/** Unused - Checks the player compatibility based on the frenemy system.
 * @name matchmaking_add_request_lobby_list_compatible_members_filter
 * @string steam_id
 */
int SteamMatchmaking_AddRequestLobbyListCompatibleMembersFilter(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	CSteamID steamID = check_CSteamID(L, 1);
	g_SteamMatchmaking->AddRequestLobbyListCompatibleMembersFilter(steamID);
	return 0;
}

/** Get a filtered list of relevant lobbies.
 * Will return results as a LobbyMatchList_t event
 * @name matchmaking_request_lobby_list
 * @treturn string Callback id 
 */
int SteamMatchmaking_RequestLobbyList(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	SteamAPICall_t call = g_SteamMatchmaking->RequestLobbyList();
	push_uint64(L, call);
	return 1;
}


/** Gets the Steam ID of the lobby at the specified index.
 * This should only be called after a LobbyMatchList_t call result is received
 * @name matchmaking_get_lobby_by_index
 * @number index The index of the lobby to get the Steam ID of, from 0 to
 * LobbyMatchList_t.m_nLobbiesMatching
 * @treturn string Id of lobby
 */
int SteamMatchmaking_GetLobbyByIndex(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int index = luaL_checknumber(L, 1);
	CSteamID id = g_SteamMatchmaking->GetLobbyByIndex(index);
	push_CSteamID(L, id);
	return 1;
}

/** Create a new matchmaking lobby.
 * Will generate a LobbyCreated_t, LobbyEnter_t and LobbyDataUpdate_t event
 * @name matchmaking_create_lobby
 * @number lobby_type The type and visibility of this lobby.
 * @number max_members The maximum number of players that can join this lobby.
 * @treturn string Callback id
 */
int SteamMatchmaking_CreateLobby(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	ELobbyType lobbyType = (ELobbyType)luaL_checknumber(L, 1);
	int maxMembers = luaL_checknumber(L, 1);
	SteamAPICall_t call = g_SteamMatchmaking->CreateLobby(lobbyType, maxMembers);
	push_uint64(L, call);
	return 1;
}

/** Joins an existing lobby.
 * Will generate a LobbyEnter_t event
 * @name matchmaking_join_lobby
 * @string lobby_id The Steam ID of the lobby to join.
 * @treturn string Callback id
 */
int SteamMatchmaking_JoinLobby(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	SteamAPICall_t call = g_SteamMatchmaking->JoinLobby(steamIDLobby);
	push_uint64(L, call);
	return 1;
}

/** Leave a lobby that the user is currently in
 * Leave a lobby that the user is currently in; this will take effect
 * immediately on the client side, other users in the lobby will be notified
 * by a LobbyChatUpdate_t callback.
 * @name matchmaking_leave_lobby
 * @string lobby_id The lobby to leave
 */
int SteamMatchmaking_LeaveLobby(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	g_SteamMatchmaking->LeaveLobby(steamIDLobby);
	return 0;
}


/** Returns the current lobby owner.
 * There always one lobby owner - if the current owner leaves, another user in
 * the lobby will become the owner automatically. It is possible (but rare) to
 * join a lobby just as the owner is leaving, thus entering a lobby with self
 * as the owner.
 * You must be a member of the lobby to access this.
 * @name matchmaking_get_lobby_owner
 * @string lobby_id The Steam ID of the lobby to get the owner of.
 * @treturn string Id of owner
 */
int SteamMatchmaking_GetLobbyOwner(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	CSteamID owner = g_SteamMatchmaking->GetLobbyOwner(steamIDLobby);
	push_CSteamID(L, owner);
	return 1;
}


/** Gets the number of users in a lobby.
 * This is used for iteration, after calling this then GetLobbyMemberByIndex can
 * be used to get the Steam ID of each person in the lobby. Persona information
 * for other lobby members (name, avatar, etc.) is automatically received and
 * accessible via the ISteamFriends interface.
 * The current user must be in the lobby to retrieve the Steam IDs of
 * other users in that lobby.
 * @name matchmaking_get_num_lobby_members
 * @string lobby_id The Steam ID of the lobby to get the owner of.
 * @treturn number Number of users in the lobby
 */
int SteamMatchmaking_GetNumLobbyMembers(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	int count = g_SteamMatchmaking->GetNumLobbyMembers(steamIDLobby);
	lua_pushnumber(L, count);
	return 1;
}


/** Gets the Steam ID of the lobby member at the given index.
 * You must call matchmaking_get_num_lobby_members before calling this.
 * The current user must be in the lobby to retrieve the Steam IDs of
 * other users in that lobby.
 * @name matchmaking_get_lobby_member_by_index
 * @string lobby_id
 * @number index
 * @treturn string Id of member
 */
int SteamMatchmaking_GetLobbyMemberByIndex(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	int index = luaL_checknumber(L, 2);
	CSteamID member = g_SteamMatchmaking->GetLobbyMemberByIndex(steamIDLobby, index);
	push_CSteamID(L, member);
	return 1;
}


/** Sets a key/value pair in the lobby metadata
 * @name matchmaking_set_lobby_data
 * @string lobby_id
 * @string key
 * @string data
 * @treturn boolean result
 */
int SteamMatchmaking_SetLobbyData(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	const char* key = luaL_checkstring(L, 2);
	const char* data = luaL_checkstring(L, 3);
	bool result = g_SteamMatchmaking->SetLobbyData(steamIDLobby, key, data);
	lua_pushboolean(L, result);
	return 1;
}


/** Sets per-user metadata for the local user.
 * @name matchmaking_set_lobby_member_data
 * @string lobby_id
 * @string key
 * @string data
 */
int SteamMatchmaking_SetLobbyMemberData(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	const char* key = luaL_checkstring(L, 2);
	const char* data = luaL_checkstring(L, 3);
	g_SteamMatchmaking->SetLobbyMemberData(steamIDLobby, key, data);
	return 0;
}


/** Get data associated with this lobby.
 * @name matchmaking_get_lobby_data
 * @string lobby_id
 * @string key
 * @treturn string Data
 */
int SteamMatchmaking_GetLobbyData(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	const char* key = luaL_checkstring(L, 2);
	const char* data = g_SteamMatchmaking->GetLobbyData(steamIDLobby, key);
	lua_pushstring(L, data);
	return 1;
}


/** Gets per-user metadata from another player in the specified lobby.
 * @name matchmaking_get_lobby_member_data
 * @string lobby_id
 * @string user_id
 * @string key
 * @treturn string Data
 */
int SteamMatchmaking_GetLobbyMemberData(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	CSteamID steamIDUser = check_CSteamID(L, 2);
	const char* key = luaL_checkstring(L, 3);
	const char* data = g_SteamMatchmaking->GetLobbyMemberData(steamIDLobby, steamIDUser, key);
	lua_pushstring(L, data);
	return 1;
}

/** Returns the number of metadata keys set on the specified lobby
 * @name matchmaking_get_lobby_data_count
 * @string lobby_id
 * @treturn number Number of keys
 */
int SteamMatchmaking_GetLobbyDataCount(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	int count = g_SteamMatchmaking->GetLobbyDataCount(steamIDLobby);
	lua_pushnumber(L, count);
	return 1;
}


/** Returns a lobby metadata key/values pair by index
 * @name matchmaking_get_lobby_data_by_index
 * @string lobby_id
 * @number index
 * @treturn boolean success
 * @treturn string key
 * @treturn string value
 */
int SteamMatchmaking_GetLobbyDataByIndex(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 3);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	int index = luaL_checknumber(L, 2);

	char key[256];
	char* keyp = key;
	char value[4096];
	bool ok = g_SteamMatchmaking->GetLobbyDataByIndex(steamIDLobby, index, key, 256, value, 4096);
	if (!ok)
	{
		lua_pushboolean(L, 0);
		lua_pushnil(L);
		lua_pushnil(L);
		return 3;
	}
	lua_pushboolean(L, 1);
	lua_pushstring(L, key);
	lua_pushstring(L,value);
	return 3;
}

/** Broadcasts a chat message to the all the users in the lobby
 * @name matchmaking_send_lobby_chat_message
 * @string lobby_id
 * @string body
 * @treturn boolean success
 */
int SteamMatchmaking_SendLobbyChatMsg(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	size_t l;
	const char* body = luaL_checklstring(L, 2, &l);
	bool ok = g_SteamMatchmaking->SendLobbyChatMsg(steamIDLobby, body, l);
	lua_pushboolean(L, ok);
	return 1;
}

/** Get a chat message as specified in a LobbyChatMsg_t callback
 * @name matchmaking_get_lobby_chat_entry
 * @string lobby_id
 * @number index
 * @treturn data
 * @treturn type
 */
int SteamMatchmaking_GetLobbyChatEntry(lua_State* L)
{
	if (!g_SteamMatchmaking) return 0;
	DM_LUA_STACK_CHECK(L, 3);
	CSteamID steamIDLobby = check_CSteamID(L, 1);
	int index = luaL_checknumber(L, 2);

	CSteamID steamIDUser;
	char data[4096];
	EChatEntryType peChatEntryType;
	int count = g_SteamMatchmaking->GetLobbyChatEntry(steamIDLobby, index, &steamIDUser, data, 4096, &peChatEntryType);
	push_CSteamID(L, steamIDUser);
	lua_pushlstring(L, data, count);
	lua_pushnumber(L, peChatEntryType);
	return 3;
}



#endif
