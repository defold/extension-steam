/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamFriends* g_SteamFriends = 0;

int SteamFriends_OnGameRichPresenceJoinRequested(lua_State* L, void* data)
{
	GameRichPresenceJoinRequested_t* s = (GameRichPresenceJoinRequested_t*)data;
	lua_pushstring(L, "GameRichPresenceJoinRequested_t");

	lua_newtable(L);
	lua_pushstring(L, "m_steamIDFriend");
	push_CSteamID(L, s->m_steamIDFriend);
	lua_settable(L, -3);
	lua_pushstring(L, "m_rgchConnect");
	lua_pushstring(L, s->m_rgchConnect);
	lua_settable(L, -3);

	return 2;
}

int SteamFriends_OnFriendRichPresenceUpdate(lua_State* L, void* data)
{
	FriendRichPresenceUpdate_t* s = (FriendRichPresenceUpdate_t*)data;
	lua_pushstring(L, "FriendRichPresenceUpdate_t");

	lua_newtable(L);
	table_push_CSteamID(L, "m_steamIDFriend", s->m_steamIDFriend);
	table_push_integer(L, "m_nAppID", s->m_nAppID);

	return 2;
}

int SteamFriends_OnPersonaStateChange(lua_State* L, void* data)
{
	PersonaStateChange_t* s = (PersonaStateChange_t*)data;
	lua_pushstring(L, "PersonaStateChange_t");

	lua_newtable(L);
	table_push_uint64(L, "m_ulSteamID", s->m_ulSteamID);
	table_push_integer(L, "m_nChangeFlags", s->m_nChangeFlags);

	return 2;
}

int SteamFriends_Init(lua_State* L)
{
	g_SteamFriends = SteamFriends();
	return 0;
}

/** Returns the name of another user.
 * Same rules as GetFriendPersonaState() apply as to whether or not the user
 * knowns the name of the other user note that on first joining a lobby, chat
 * room or game server the local user will not known the name of the other users
 * automatically; that information will arrive asyncronously.
 * @name friends_get_friend_persona_name
 * @tparam friendId CSteamID
 * @treturn name String Name of user
 */
int SteamFriends_GetFriendPersonaName(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	const char* name = g_SteamFriends->GetFriendPersonaName(steamIDFriend);
	lua_pushstring(L, name);
	return 1;
}


/** Returns the local players name - guaranteed to not be NULL.
 * This is the same name as on the users community profile page.
 * This is stored in UTF-8 format.
 * @name friends_get_persona_name
 * @treturn name String Name of user
 */
int SteamFriends_GetPersonaName(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* name = g_SteamFriends->GetPersonaName();
	lua_pushstring(L, name);
	return 1;
}

/** Gets the status of the current user.
 * Returned as EPersonaState.
 * @name friends_get_persona_state
 * @treturn state Number Status of user.
 */
int SteamFriends_GetPersonaState(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	EPersonaState e = g_SteamFriends->GetPersonaState();
	lua_pushinteger(L, e);
	return 1;
}


/** Friend iteration.
 * Takes a set of EFriendFlags, and returns the number of users the client
 * knows about who meet that criteria.
 * Then GetFriendByIndex() can then be used to return the id's of each of those
 * users.
 * @name friends_get_friend_count
 * @number iFriendFlags Set of friend flags to match friends against.
 * @treturn count Number Number of users matching search.
 */
 int SteamFriends_GetFriendCount(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int iFriendFlags = luaL_checknumber(L, 1);
	int count = g_SteamFriends->GetFriendCount(iFriendFlags);
	lua_pushinteger(L, count);
	return 1;
}


/** Returns the steamID of a user.
 * The returned CSteamID can then be used by all the functions below to access
 * details about the user.
 * @name friends_get_friend_by_index
 * @number iFriend is a index of range [0, GetFriendCount())
 * @number iFriendsFlags must be the same value as used in GetFriendCount()
 * @treturn steamID Number Steam id of the user
 */
int SteamFriends_GetFriendByIndex(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int iFriend = luaL_checknumber(L, 1);
	int iFriendFlags = luaL_checknumber(L, 2);
	CSteamID friendId = g_SteamFriends->GetFriendByIndex(iFriend, iFriendFlags);
	push_CSteamID(L, friendId);
	return 1;
}


/** Returns the current status of the specified user.
 * This will only be known by the local user if steamIDFriend is in their
 * friends list; on the same game server; in a chat room or lobby; or in a small
 * group with the local user.
 * @name friends_get_friend_persona_state
 * @number steamIDFriend Id of friend
 * @treturn state Number State of friend
 */
int SteamFriends_GetFriendPersonaState(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	EPersonaState state = g_SteamFriends->GetFriendPersonaState(steamIDFriend);
	lua_pushinteger(L, state);
	return 1;
}


/** Get friends steam level.
 * @name friends_get_friend_steam_level
 * @number steamIDFriend Id of friend
 * @treturn level Number Steam level of friend
 */
int SteamFriends_GetFriendSteamLevel(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	int level = g_SteamFriends->GetFriendSteamLevel(steamIDFriend);
	lua_pushinteger(L, level);
	return 1;
}


/** Returns a relationship to a user.
 * @name friends_get_friend_relationship
 * @number steamIDFriend Id of friend
 * @treturn relationship Number Relationship to the user.
 */
int SteamFriends_GetFriendRelationship(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	EFriendRelationship relationship = g_SteamFriends->GetFriendRelationship(steamIDFriend);
	lua_pushinteger(L, relationship);
	return 1;
}


/*** Gets the small (32x32) avatar of the current user.
 * This is a handle to be used in IClientUtils::GetImageRGBA(), or 0 if none set
 * @name friends_get_small_friend_avatar
 * @number steamIDFriend Id of friend
 * @treturn handle Number Image handle.
 */
int SteamFriends_GetSmallFriendAvatar(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	int handle = g_SteamFriends->GetSmallFriendAvatar(steamIDFriend);
	lua_pushinteger(L, handle);
	return 1;
}

int SteamFriends_GetMediumFriendAvatar(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	int handle = g_SteamFriends->GetMediumFriendAvatar(steamIDFriend);
	lua_pushinteger(L, handle);
	return 1;
}

int SteamFriends_GetLargeFriendAvatar(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	int handle = g_SteamFriends->GetLargeFriendAvatar(steamIDFriend);
	lua_pushinteger(L, handle);
	return 1;
}

/** Activates game overlay to store page for app.
 * @name friends_activate_game_overlay_to_store
 * @number app_id
 * @number flag EOverlayToStoreFlag
 */
int SteamFriends_ActivateGameOverlayToStore(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	AppId_t nAppID = luaL_checknumber(L, 1);
	EOverlayToStoreFlag eFlag;
	if (lua_isnil(L, 2)) {
		eFlag = k_EOverlayToStoreFlag_None;
	}
	else {
		eFlag = (EOverlayToStoreFlag)luaL_checknumber(L, 2);
	}
	g_SteamFriends->ActivateGameOverlayToStore(nAppID, eFlag);
	return 0;
}


/** Activates game overlay web browser directly to the specified URL.
 * Full address with protocol type is required, e.g. http://www.steamgames.com/
 * @name friends_activate_game_overlay_to_web_page
 * @string url
 * @number mode EActivateGameOverlayToWebPageMode
 */
int SteamFriends_ActivateGameOverlayToWebPage(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	const char *pchURL = luaL_checkstring(L, 1);
	EActivateGameOverlayToWebPageMode eMode;
	if (lua_isnil(L, 2)) {
		eMode = k_EActivateGameOverlayToWebPageMode_Default;
	}
	else {
		eMode = (EActivateGameOverlayToWebPageMode)luaL_checknumber(L, 2);
	}
	g_SteamFriends->ActivateGameOverlayToWebPage(pchURL, eMode);
	return 0;
}


/** Sets a Rich Presence key/value for the current user.
 * @name friends_set_rich_presence
 * @string key
 * @string value
 * @treturn bool success True if the rich presence was set successfully, otherwise False.
 */
int SteamFriends_SetRichPresence(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char *pchKey = luaL_checkstring(L, 1);
	const char *pchValue = luaL_checkstring(L, 2);
	bool success = g_SteamFriends->SetRichPresence(pchKey, pchValue);
	lua_pushboolean(L, success);
	return 1;
}


/** Clears all of the current user's Rich Presence key/values.
 * @name friends_clear_rich_presence
 */
int SteamFriends_ClearRichPresence(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 0);
	g_SteamFriends->ClearRichPresence();
	return 0;
}

/** Invites a friend or clan member to the current game using a special invite string.
 * If the target accepts the invite, a GameRichPresenceJoinRequested_t callback is posted containing the connect string.
 * @name friends_invite_user_to_game
 * @number steamIDFriend Id of friend
 * @string connect string
 * @treturn boolean success
 */
int SteamFriends_InviteUserToGame(lua_State* L)
{
	if (!g_SteamFriends) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	const char *pchConnectString = luaL_checkstring(L, 2);
	bool success = g_SteamFriends->InviteUserToGame(steamIDFriend, pchConnectString);
	lua_pushboolean(L, success);
	return 1;
}

#endif
