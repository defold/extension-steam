#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamUser* g_SteamUser = 0;


int SteamUser_Init(lua_State* L)
{
	g_SteamUser = SteamUser();
	return 0;
}

/** Returns the CSteamID of the account currently logged into the Steam client.
 * A CSteamID is a unique identifier for an account, and used to differentiate
 * users in all parts of the Steamworks API.
 * @name user_get_steam_id
 * @treturn CSteamID id
 */
int SteamUser_GetSteamId(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	push_CSteamID(L, g_SteamUser->GetSteamID());
	return 1;
}

/** Gets the Steam Level of the user, as shown on their profile.
 * @name user_get_player_steam_level
 * @treturn number level
 */
int SteamUser_GetPlayerSteamLevel(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushnumber(L, g_SteamUser->GetPlayerSteamLevel());
	return 1;
}

/** Trading Card badges data access.
 * If you only have one set of cards, the series will be 1. The user has can
 * have two different badges for a series; the regular (max level 5) and the
 * foil (max level 1).
 * @name user_get_game_badge_level
 * @treturn number series
 * @treturn boolean foil
 */
int SteamUser_GetGameBadgeLevel(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int series = luaL_checknumber(L, 1);
	bool foil = lua_toboolean(L, 2);
	lua_pushnumber(L, g_SteamUser->GetGameBadgeLevel(series, foil));
	return 1;
}

/** Returns true if the Steam client current has a live connection to the Steam
 * servers.
 * @name user_logged_on
 * @treturn boolean logged_on
 */
int SteamUser_LoggedOn(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BLoggedOn());
	return 1;
}

/** Returns true if this users looks like they are behind a NAT device.
 * Only valid once the user has connected to steam .
 * @name user_is_behind_nat
 * @treturn boolean behind_nat
 */
int SteamUser_IsBehindNAT(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsBehindNAT());
	return 1;
}

/** Gets whether the users phone number is verified.
 * @name user_is_phone_verified
 * @treturn boolean verified
 */
int SteamUser_IsPhoneVerified(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsPhoneVerified());
	return 1;
}

/** Gets whether the users phone number is identifying.
 * @name user_is_phone_identifying
 * @treturn boolean verifying
 */
int SteamUser_IsPhoneIdentifying(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsPhoneIdentifying());
	return 1;
}

/** Gets whether the users phone number is awaiting (re)verification.
 * @name user_is_phone_requiring_verification
 * @treturn boolean verification
 */
int SteamUser_IsPhoneRequiringVerification(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsPhoneRequiringVerification());
	return 1;
}

/** Gets whether the user has two factor enabled on their account.
 * @name user_is_two_factor_enabled
 * @treturn boolean enabled
 */
int SteamUser_IsTwoFactorEnabled(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushboolean(L, g_SteamUser->BIsTwoFactorEnabled());
	return 1;
}

/** Retrieve a session ticket
 * @name user_get_auth_ticket_for_web_api
 * @treturn integer HAuthTicket
 */
int SteamUser_GetAuthTicketForWebApi(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char *pchIdentity = luaL_checkstring(L, 1);
	lua_pushinteger(L, g_SteamUser->GetAuthTicketForWebApi(pchIdentity));
	return 1;
}

#endif
