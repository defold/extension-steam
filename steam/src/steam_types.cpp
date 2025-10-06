#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"


/*****************************
* (u)int64 (as string)
******************************/
uint64 check_uint64(lua_State* L, int index)
{
	if(lua_isstring(L, index))
	{
		const char * s = luaL_checkstring(L, index);
		return strtoull(s, NULL, 10);
	}
	return 0;
}
int64 check_int64(lua_State* L, int index)
{
	if(lua_isstring(L, index))
	{
		const char * s = luaL_checkstring(L, index);
		return strtoll(s, NULL, 10);
	}
	return 0;
}
void check_uint64_array(lua_State* L, int index, uint64 * arr, unsigned int size)
{
	if(lua_isnil(L, index) || lua_isnone(L, index))
	{
		if(size > 0)
		{
			luaL_error(L, "Size > 0 but no table provided");
		}
		return;
	}
	if(!lua_istable(L, index))
	{
		luaL_error(L, "Not a table");
	}
	int table_size = lua_objlen(L, index);
	if(table_size > size)
	{
		table_size = size;
	}
	for(int i=1; i<=table_size; i++)
	{
		lua_pushinteger(L, i);
		lua_gettable(L, index);
		arr[i] = check_uint64(L, -1);
	}
}
void push_int64(lua_State* L, int64 n)
{
	char buf[22];
	snprintf(buf, sizeof(buf), "%lld", n);
	lua_pushstring(L, buf);
}
void push_int64_array(lua_State* L, int64 arr[], unsigned int size)
{
	lua_newtable(L);
	for(int i=1; i <= size; i++)
	{
		lua_pushinteger(L, i);
		push_int64(L, arr[i]);
		lua_settable(L, -3);
	}
}
void push_uint64(lua_State* L, uint64 n)
{
	char buf[22];
	snprintf(buf, sizeof(buf), "%llu", n);
	lua_pushstring(L, buf);
}
void push_uint64_array(lua_State* L, uint64 arr[], unsigned int size)
{
	lua_newtable(L);
	for(int i=1; i <= size; i++)
	{
		lua_pushinteger(L, i);
		push_uint64(L, arr[i]);
		lua_settable(L, -3);
	}
}



/*****************************
* CSteamID
******************************/
CSteamID check_CSteamID(lua_State* L, int index)
{
	uint64 v = check_uint64(L, index);
	return CSteamID(v);
}
void push_CSteamID(lua_State* L, CSteamID steamId)
{
	push_uint64(L, steamId.ConvertToUint64());
}
void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size)
{
	lua_newtable(L);
	for(int i=1; i <= size; i++) {
		lua_pushinteger(L, i);
		push_CSteamID(L, steamId[i]);
		lua_settable(L, -3);
	}
}


/*****************************
* PUSH table key value pairs
******************************/
void table_push_stringl(lua_State* L, const char* key, const char* value, size_t length)
{
	lua_pushstring(L, key);
	lua_pushlstring(L, value, length);
	lua_settable(L, -3);
}
void table_push_string(lua_State* L, const char* key, const char* value)
{
	lua_pushstring(L, key);
	lua_pushstring(L, value);
	lua_settable(L, -3);
}
void table_push_number(lua_State* L, const char* key, lua_Number value)
{
	lua_pushstring(L, key);
	lua_pushnumber(L, value);
	lua_settable(L, -3);
}
void table_push_integer(lua_State* L, const char* key, lua_Integer value)
{
	lua_pushstring(L, key);
	lua_pushinteger(L, value);
	lua_settable(L, -3);
}
void table_push_boolean(lua_State* L, const char* key, bool value)
{
	lua_pushstring(L, key);
	lua_pushboolean(L, value);
	lua_settable(L, -3);
}
void table_push_int64(lua_State* L, const char* key, uint64 value)
{
	lua_pushstring(L, key);
	push_int64(L, value);
	lua_settable(L, -3);
}
void table_push_uint64(lua_State* L, const char* key, uint64 value)
{
	lua_pushstring(L, key);
	push_uint64(L, value);
	lua_settable(L, -3);
}
void table_push_CSteamID(lua_State* L, const char* key, CSteamID value)
{
	lua_pushstring(L, key);
	push_CSteamID(L, value);
	lua_settable(L, -3);
}

void table_push_SteamNetConnectionInfo(lua_State* L, const char* key, SteamNetConnectionInfo_t info)
{
	lua_pushstring(L, key);
	lua_newtable(L);

	table_push_integer(L, "m_eState", info.m_eState);
	table_push_integer(L, "m_eEndReason", info.m_eEndReason);
	table_push_integer(L, "m_nFlags", info.m_nFlags);
	// TODO push more info

	lua_settable(L, -3);
}

void table_push_SteamNetConnectionRealTimeStatus(lua_State* L, const char* key, SteamNetConnectionRealTimeStatus_t status)
{
	lua_pushstring(L, key);
	lua_newtable(L);

	table_push_integer(L, "m_nPing", status.m_nPing);
	table_push_number(L, "m_flConnectionQualityLocal", status.m_flConnectionQualityLocal);
	table_push_number(L, "m_flConnectionQualityRemote", status.m_flConnectionQualityRemote);
	table_push_number(L, "m_flOutPacketsPerSec", status.m_flOutPacketsPerSec);
	table_push_number(L, "m_flOutBytesPerSec", status.m_flOutBytesPerSec);
	table_push_number(L, "m_flInPacketsPerSec", status.m_flInPacketsPerSec);
	table_push_number(L, "m_flInBytesPerSec", status.m_flInBytesPerSec);
	table_push_integer(L, "m_nSendRateBytesPerSecond", status.m_nSendRateBytesPerSecond);
	table_push_integer(L, "m_cbPendingUnreliable", status.m_cbPendingUnreliable);
	table_push_integer(L, "m_cbPendingReliable", status.m_cbPendingReliable);
	table_push_integer(L, "m_cbSentUnackedReliable", status.m_cbSentUnackedReliable);
	table_push_integer(L, "m_usecQueueTime", status.m_usecQueueTime);

	lua_settable(L, -3);
}



/*****************************
* SteamNetworkingMessage
******************************/
void push_SteamNetworkingMessage(lua_State* L, SteamNetworkingMessage_t* msg)
{
	lua_newtable(L);
	table_push_stringl(L, "m_pData", (const char*)msg->m_pData, msg->m_cbSize);
	table_push_number(L, "m_conn", msg->m_conn);
	table_push_CSteamID(L, "m_identityPeer", msg->m_identityPeer.GetSteamID());
	table_push_int64(L, "m_nConnUserData", msg->m_nConnUserData);
	table_push_integer(L, "m_usecTimeReceived", msg->m_usecTimeReceived);
	table_push_integer(L, "m_nMessageNumber", msg->m_nMessageNumber);
	table_push_integer(L, "m_nChannel", msg->m_nChannel);
	table_push_integer(L, "m_nFlags", msg->m_nFlags);
	table_push_int64(L, "m_nUserData", msg->m_nUserData);
}



#endif