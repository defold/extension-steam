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


int SteamMatchmaking_Init(lua_State* L)
{
	g_SteamMatchmaking = SteamMatchmaking();
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
	dmLogInfo("RequestLobbyList SteamAPICall_t %llu", call);
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
	dmLogInfo("Leave lobby");
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
#endif
