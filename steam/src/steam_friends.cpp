/** Interface to access information about individual users and interact with the
 * Steam Overlay.
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamFriends* g_SteamFriends;

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
	DM_LUA_STACK_CHECK(L, 1);
	EPersonaState e = g_SteamFriends->GetPersonaState();
	lua_pushnumber(L, e);
	return 1;
}


/*** Friend iteration.
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
	DM_LUA_STACK_CHECK(L, 1);
	int iFriendFlags = luaL_checknumber(L, 1);
	int count = g_SteamFriends->GetFriendCount(iFriendFlags);
	lua_pushnumber(L, count);
	return 1;
}


/*** Returns the steamID of a user.
 * The returned CSteamID can then be used by all the functions below to access
 * details about the user.
 * @name friends_get_friend_by_index
 * @number iFriend is a index of range [0, GetFriendCount())
 * @number iFriendsFlags must be the same value as used in GetFriendCount()
 * @treturn steamID Number Steam id of the user
 */
int SteamFriends_GetFriendByIndex(lua_State* L)
{
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
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	EPersonaState state = g_SteamFriends->GetFriendPersonaState(steamIDFriend);
	lua_pushnumber(L, state);
	return 1;
}


/** Get friends steam level.
 * @name friends_get_friend_steam_level
 * @number steamIDFriend Id of friend
 * @treturn level Number Steam level of friend
 */
int SteamFriends_GetFriendSteamLevel(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	int level = g_SteamFriends->GetFriendSteamLevel(steamIDFriend);
	lua_pushnumber(L, level);
	return 1;
}


/** Returns a relationship to a user.
 * @name friends_get_friend_relationship
 * @number steamIDFriend Id of friend
 * @treturn relationship Number Relationship to the user.
 */
int SteamFriends_GetFriendRelationship(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	EFriendRelationship relationship = g_SteamFriends->GetFriendRelationship(steamIDFriend);
	lua_pushnumber(L, relationship);
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
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID steamIDFriend = check_uint64(L, 1);
	int handle = g_SteamFriends->GetSmallFriendAvatar(steamIDFriend);
	lua_pushnumber(L, handle);
	return 1;
}


/** Activates game overlay to store page for app.
 * @name friends_activate_game_overlay_to_store
 * @number app_id
 * @number flag EOverlayToStoreFlag
 */
int SteamFriends_ActivateGameOverlayToStore(lua_State* L)
{
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


#endif
