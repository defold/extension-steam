/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamUser* g_SteamUser = 0;
static HAuthTicket g_AuthSessionTicket = 0;

#define MAX_TICKET_SIZE 4096

int SteamUser_OnMicroTxnAuthorizationResponse(lua_State* L, void* data)
{
	MicroTxnAuthorizationResponse_t* s = (MicroTxnAuthorizationResponse_t*)data;
	lua_pushstring(L, "MicroTxnAuthorizationResponse_t");

	lua_newtable(L);
	table_push_integer(L, "m_unAppID", s->m_unAppID);
	table_push_uint64(L, "m_ulOrderID", s->m_ulOrderID);
	table_push_integer(L, "m_bAuthorized", s->m_bAuthorized);

	return 2;
}

int SteamUser_OnGetTicketForWebApiResponse(lua_State * L, void* data)
{
	GetTicketForWebApiResponse_t* s = (GetTicketForWebApiResponse_t*)data;
	lua_pushstring(L, "GetTicketForWebApiResponse_t");

	lua_newtable(L);
	table_push_integer(L, "m_hAuthTicket", s->m_hAuthTicket);
	table_push_integer(L, "m_eResult", s->m_eResult);
	table_push_stringl(L, "m_rgubTicket", (const char*)s->m_rgubTicket, (size_t)s->m_cubTicket);

	return 2;
}

int SteamUser_OnValidateAuthTicketResponse(lua_State * L, void* data)
{
	ValidateAuthTicketResponse_t* s = (ValidateAuthTicketResponse_t*)data;
	lua_pushstring(L, "ValidateAuthTicketResponse_t");

	lua_newtable(L);
	table_push_CSteamID(L, "m_SteamID", s->m_SteamID);
	table_push_integer(L, "m_eAuthSessionResponse", s->m_eAuthSessionResponse);
	table_push_CSteamID(L, "m_OwnerSteamID", s->m_OwnerSteamID);

	return 2;
}

int SteamUser_OnGetAuthSessionTicketResponse(lua_State* L, void* data)
{
	GetAuthSessionTicketResponse_t* s = (GetAuthSessionTicketResponse_t*)data;
	lua_pushstring(L, "GetAuthSessionTicketResponse_t");

	lua_newtable(L);
	table_push_integer(L, "m_hAuthTicket", s->m_hAuthTicket);
	table_push_integer(L, "m_eResult", s->m_eResult);

	return 2;
}

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
	lua_pushinteger(L, g_SteamUser->GetPlayerSteamLevel());
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
	lua_pushinteger(L, g_SteamUser->GetGameBadgeLevel(series, foil));
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


/** Get an authentication ticket.
 * Retrieve an authentication ticket to be sent to the entity who wishes to
 * authenticate you.
 * @name user_get_auth_session_ticket
 * @treturn string ticket Auth ticket or null
 * @treturn string handle Ticket handle or null
 * @treturn string error Error message or null
 */
int SteamUser_GetAuthSessionTicket(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 3);

	if (g_AuthSessionTicket)
	{
		g_SteamUser->CancelAuthTicket(g_AuthSessionTicket);
		g_AuthSessionTicket = 0;
	}

	char pTicket[MAX_TICKET_SIZE];
	uint32 pcbTicket;
	HAuthTicket ticket = g_SteamUser->GetAuthSessionTicket(pTicket, MAX_TICKET_SIZE, &pcbTicket, 0x0);
	if (ticket == k_HAuthTicketInvalid)
	{
		lua_pushnil(L);
		lua_pushnil(L);
		lua_pushstring(L, "k_HAuthTicketInvalid");
		return 3;
	}

	g_AuthSessionTicket = ticket;
	lua_pushlstring(L, pTicket, pcbTicket);
	lua_pushnumber(L, ticket);
	lua_pushnil(L);
	return 3;
}


/** Validate an authentication ticket.
 * Authenticate the ticket from the entity Steam ID to be sure it is valid and
 * isn't reused. Note that identity is not confirmed until the callback
 * ValidateAuthTicketResponse_t is received and the return value in that
 * callback is checked for success.
 * @name user_begin_auth_session
 * @string ticket The auth ticket to validate
 * @string steamId The entity's Steam ID that sent this ticket.
 * @treturn number result
 */
int SteamUser_BeginAuthSession(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	size_t cbAuthTicket = 0;
	const void* pAuthTicket = (const void*)luaL_checklstring(L, 1, &cbAuthTicket);
	CSteamID steamID = check_uint64(L, 2);

	EBeginAuthSessionResult result = g_SteamUser->BeginAuthSession(pAuthTicket, cbAuthTicket, steamID);
	lua_pushinteger(L, result);
	return 1;
}

/** Cancels an auth ticket.
 * Cancels an auth ticket received from GetAuthSessionTicket or
 * GetAuthTicketForWebApi. This should be called when no longer playing with the
 * specified entity.
 * @name cancel_auth_ticket
 * @number ticket The active auth ticket to cancel.
 */
int SteamUser_CancelAuthTicket(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 0);

	HAuthTicket hAuthTicket = luaL_checknumber(L, 1);

	g_SteamUser->CancelAuthTicket(hAuthTicket);

	return 0;
}

/** Ends an auth session.
 * Ends an auth session that was started with BeginAuthSession. This should be
 * called when no longer playing with the specified entity.
 * @name end_auth_session
 * @string steamId The entity to end the active auth session with.
 */
int SteamUser_EndAuthSession(lua_State* L)
{
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 0);

	CSteamID steamID = check_uint64(L, 1);

	g_SteamUser->EndAuthSession(steamID);

	return 0;
}

/** Get an authentication ticket for web API.
 * Request an authentication ticket suitable to authenticated in a web backend. Will
 * trigger a GetTicketForWebApiResponse_t callback when the ticket is ready.
 * @name user_get_auth_ticket_for_web_api
 * @string identity Optional identity string to associate with the ticket
 * @treturn number hAuthTicket The handle of the requested ticket
 * @treturn string error Error message or null
 */
int SteamUser_GetAuthTicketForWebAPI(lua_State* L) {
	if (!g_SteamUser) return 0;
	DM_LUA_STACK_CHECK(L, 2);

	const char* pchIdentity = nullptr;
	if (lua_gettop(L) >= 1)
	{
		pchIdentity = luaL_checkstring(L, 1);
	}

	HAuthTicket ticket = g_SteamUser->GetAuthTicketForWebApi(pchIdentity);
	if (ticket == k_HAuthTicketInvalid)
	{
		lua_pushnil(L);
		lua_pushstring(L, "k_HAuthTicketInvalid");
		return 2;
	}

	lua_pushnumber(L, ticket);
	lua_pushnil(L);
	return 2;
}

#endif
