/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamNetworkingMessages* g_SteamNetworking = 0;

static const int MAX_STEAM_NETWORKING_MESSAGES = 1;
static SteamNetworkingMessage_t* g_SteamNetworkingMessage = 0;

int SteamNetworking_Init(lua_State* L)
{
	g_SteamNetworking = SteamNetworkingMessages();
	g_SteamNetworkingMessage = (SteamNetworkingMessage_t*)malloc(MAX_STEAM_NETWORKING_MESSAGES * sizeof(SteamNetworkingMessage_t));
	return 0;
}


int SteamNetworking_OnSteamNetworkingMessagesSessionFailed(lua_State* L, void* data)
{
	SteamNetworkingMessagesSessionFailed_t* s = (SteamNetworkingMessagesSessionFailed_t*)data;
	lua_pushstring(L, "SteamNetworkingMessagesSessionFailed_t");

	lua_newtable(L);
	table_push_SteamNetConnectionInfo(L, "m_Info", s->m_info);

	return 2;
}

int SteamNetworking_OnSteamNetworkingMessagesSessionRequest(lua_State* L, void* data)
{
	SteamNetworkingMessagesSessionRequest_t* s = (SteamNetworkingMessagesSessionRequest_t*)data;
	lua_pushstring(L, "SteamNetworkingMessagesSessionRequest_t");

	lua_newtable(L);
	table_push_CSteamID(L, "m_identityRemote", s->m_identityRemote.GetSteamID());

	return 2;
}

/** Send message
 * 
 * @name networking_send_message_to_user
 * @string identity_remote
 * @string data
 * @number send_flags A bitmask of k_nSteamNetworkingSend_xxx options
 * @number remote_channel A routing number you can use to help route message to different systems
 */
int SteamNetworking_SendMessageToUser(lua_State* L)
{
	if (!g_SteamNetworking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID id = check_CSteamID(L, 1);
	SteamNetworkingIdentity identityRemote;
	identityRemote.SetSteamID(id);
	size_t cubData = 0;
	const char* pubData = (const char*)luaL_checklstring(L, 2, &cubData);
	int sendFlags = luaL_checknumber(L, 3);
	int remoteChannel = luaL_checknumber(L, 4);
	EResult result = g_SteamNetworking->SendMessageToUser(identityRemote, pubData, (uint32)cubData, sendFlags, remoteChannel);
	lua_pushinteger(L, result);
	return 1;
}

/** Receive message
 * 
 * @name networking_receive_messages_on_channel
 * @number localChannel
 * @treturn table message
 */
int SteamNetworking_ReceiveMessagesOnChannel(lua_State* L)
{
	// int nLocalChannel, SteamNetworkingMessage_t **ppOutMessages, int nMaxMessages
	if (!g_SteamNetworking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int localChannel = luaL_checknumber(L, 1);
	const int nMaxMessages = MAX_STEAM_NETWORKING_MESSAGES;
	SteamNetworkingMessage_t** out = &g_SteamNetworkingMessage;
	int count = g_SteamNetworking->ReceiveMessagesOnChannel(localChannel, out, nMaxMessages);
	if (count == 0)
	{
		lua_pushnil(L);
		return 1;
	}
	SteamNetworkingMessage_t* message = &g_SteamNetworkingMessage[0];
	push_SteamNetworkingMessage(L, message);
	message->Release();
	return 1;
}

/** Accept session
 * Call this in response to a SteamNetworkingMessagesSessionRequest_t callback.
 * @name networking_accept_session_with_user
 * @string identity_remote
 * @treturn boolean result Returns false if there is no session with the user
 * pending or otherwise. If there is an existing active session, this function
 * will return true, even if it is not pending.
 */
int SteamNetworking_AcceptSessionWithUser(lua_State* L)
{
	// const SteamNetworkingIdentity &identityRemote
	if (!g_SteamNetworking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID id = check_CSteamID(L, 1);
	SteamNetworkingIdentity identityRemote;
	identityRemote.SetSteamID(id);
	bool result = g_SteamNetworking->AcceptSessionWithUser(identityRemote);
	lua_pushboolean(L, result);
	return 1;
}

/** Close sesssion
 * Call this when you're done talking to a user to immediately free up
 * resources under-the-hood
 * @name networking_close_session_with_user
 * @string identity_remote
 * @treturn boolean result
 */
int SteamNetworking_CloseSessionWithUser(lua_State* L)
{
	if (!g_SteamNetworking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID id = check_CSteamID(L, 1);
	SteamNetworkingIdentity identityRemote;
	identityRemote.SetSteamID(id);
	bool result = g_SteamNetworking->CloseSessionWithUser(identityRemote);
	lua_pushboolean(L, result);
	return 1;
}

/** Close channel
 * Call this when you're done talking to a user on a specific channel.
 * @name networking_close_channel_with_user
 * @string identity_remote
 * @number local_channel
 * @treturn boolean result
 */
int SteamNetworking_CloseChannelWithUser(lua_State* L)
{
	if (!g_SteamNetworking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID id = check_CSteamID(L, 1);
	SteamNetworkingIdentity identityRemote;
	identityRemote.SetSteamID(id);
	int localChannel = luaL_checknumber(L, 2);
	bool result = g_SteamNetworking->CloseChannelWithUser(identityRemote, localChannel);
	lua_pushboolean(L, result);
	return 1;
}

/** Get connection info
 * Returns information about the latest state of a connection, if any, with the
 * given peer.
 * @name networking_get_session_connection_info
 * @string identity_remote
 * @treturn table info Connection info (state, info, status)
 */
int SteamNetworking_GetSessionConnectionInfo(lua_State* L)
{
	if (!g_SteamNetworking) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	CSteamID id = check_CSteamID(L, 1);
	SteamNetworkingIdentity identityRemote;
	identityRemote.SetSteamID(id);

	SteamNetConnectionInfo_t connectionInfo;
	SteamNetConnectionRealTimeStatus_t quickStatus;

	ESteamNetworkingConnectionState state = g_SteamNetworking->GetSessionConnectionInfo(identityRemote, &connectionInfo, &quickStatus);

	lua_newtable(L);
	table_push_integer(L, "state", state);
	table_push_SteamNetConnectionInfo(L, "info", connectionInfo);
	table_push_SteamNetConnectionRealTimeStatus(L, "status", quickStatus);
	return 1;
}

#endif
