#define LIB_NAME "Steam"
#define MODULE_NAME "steam"

#ifndef DLIB_LOG_DOMAIN
#define DLIB_LOG_DOMAIN "steam"
#endif

#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "steam_api.h"

struct SteamBootstrap {
	SteamBootstrap() {
		SteamAPI_Init();
	}
} g_SteamBootstrap;

#define DM_STEAMWORKS_EXTENSION_STAT_TYPE_INT 0
#define DM_STEAMWORKS_EXTENSION_STAT_TYPE_FLOAT 1
#define DM_STEAMWORKS_EXTENSION_STAT_TYPE_AVERAGERATE 2

static ISteamApps *apps;
static ISteamFriends *friends;
static ISteamMatchmaking *matchmaking;
static ISteamMusic *music;
static ISteamUser *user;
static ISteamUserStats *user_stats;
static ISteamUtils *utils;
static ISteamRemoteStorage *remote_storage;
static ISteamInventory *inventory;
static ISteamUGC *ugc;
static ISteamParties *parties;
static ISteamInput *input;
static ISteamGameSearch *game_search;
static ISteamNetworking *networking;
static ISteamVideo *video;
static ISteamScreenshots *screenshots;
static ISteamMusicRemote *music_remote;

// /*****************************
// * PUSH numbers and other primitive types
// ******************************/
// static void push_double(lua_State* L, double n) {
// 	lua_pushnumber(L, n);
// }
// static void push_float(lua_State* L, float n) {
// 	lua_pushnumber(L, n);
// }
// static void push_float_array(lua_State* L, float arr[], unsigned int size) {
// 	lua_newtable(L);
// 	for(int i=1; i <= size; i++) {
// 		lua_pushnumber(L, i);
// 		push_float(L, arr[i]);
// 		lua_settable(L, -3);
// 	}
// }
// static void push_int(lua_State* L, int n) {
// 	lua_pushinteger(L, n);
// }
// static void push_int_array(lua_State* L, int arr[], unsigned int size) {
// 	lua_newtable(L);
// 	for(int i=1; i <= size; i++) {
// 		lua_pushnumber(L, i);
// 		push_int(L, arr[i]);
// 		lua_settable(L, -3);
// 	}
// }
// static void push_unsigned_int(lua_State* L, unsigned int n) {
// 	lua_pushinteger(L, n);
// }
// static void push_unsigned_int_array(lua_State* L, unsigned int arr[], unsigned int size) {
// 	lua_newtable(L);
// 	for(int i=1; i <= size; i++) {
// 		lua_pushnumber(L, i);
// 		push_unsigned_int(L, arr[i]);
// 		lua_settable(L, -3);
// 	}
// }
// static void push_bool(lua_State* L, bool b) {
// 	lua_pushboolean(L, b);
// }
// static void push_char_array(lua_State* L, char ca[], unsigned int size) {
// 	lua_pushstring(L, ca);
// }
// static void push_const_char_ptr(lua_State* L, const char * s) {
// 	lua_pushstring(L, s);
// }
// static void push_char_ptr(lua_State* L, char * s) {
// 	lua_pushstring(L, s);
// }
// static void push_void_ptr(lua_State* L, void * s) {
// 	lua_pushstring(L, (char *)s);
// }


// /*****************************
// * PUSH (u)int64 (to strings)
// ******************************/
// static void push_int64_t(lua_State* L, int64_t n) {
// 	char buf[22];
// 	snprintf(buf, sizeof(buf), "%lld", n);
// 	lua_pushstring(L, buf);
// }
// static void push_int64(lua_State* L, int64 n) {
// 	char buf[22];
// 	snprintf(buf, sizeof(buf), "%lld", n);
// 	lua_pushstring(L, buf);
// }
// static void push_int64_array(lua_State* L, int64 arr[], unsigned int size) {
// 	lua_newtable(L);
// 	for(int i=1; i <= size; i++) {
// 		lua_pushnumber(L, i);
// 		push_int64(L, arr[i]);
// 		lua_settable(L, -3);
// 	}
// }

// static void push_uint64(lua_State* L, uint64 n) {
// 	char buf[22];
// 	snprintf(buf, sizeof(buf), "%llu", n);
// 	lua_pushstring(L, buf);
// }
// static void push_uint64_array(lua_State* L, uint64 arr[], unsigned int size) {
// 	lua_newtable(L);
// 	for(int i=1; i <= size; i++) {
// 		lua_pushnumber(L, i);
// 		push_uint64(L, arr[i]);
// 		lua_settable(L, -3);
// 	}
// }


// /*****************************
// * PUSH CSteamID
// ******************************/
// static void push_CSteamID(lua_State* L, CSteamID steamId) {
// 	push_uint64(L, steamId.ConvertToUint64());
// }
// static void push_class_CSteamID(lua_State* L, CSteamID steamId) {
// 	push_uint64(L, steamId.ConvertToUint64());
// }
// static void push_CSteamID_array(lua_State* L, CSteamID steamId[], unsigned int size) {
// 	lua_newtable(L);
// 	for(int i=1; i <= size; i++) {
// 		lua_pushnumber(L, i);
// 		push_CSteamID(L, steamId[i]);
// 		lua_settable(L, -3);
// 	}
// }

// /*****************************
// * PUSH CGameID
// ******************************/
// static void push_CGameID(lua_State* L, CGameID gameId) {
// 	push_uint64(L, gameId.ToUint64());
// }

// /*****************************
// * PUSH structs
// ******************************/
// static void push_servernetadr_t(lua_State* L, servernetadr_t s) {
// 	lua_newtable(L);
// 	lua_pushstring(L, "m_usConnectionPort");
// 	push_uint16(L, s.GetConnectionPort());
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_usQueryPort");
// 	push_uint16(L, s.GetQueryPort());
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_unIP");
// 	push_uint32(L, s.GetIP());
// 	lua_settable(L, -3);
// }
// static void push_gameserveritem_t(lua_State* L, gameserveritem_t s) {
// 	lua_newtable(L);
// 	lua_pushstring(L, "m_NetAdr");
// 	push_servernetadr_t(L, s.m_NetAdr);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_nPing");
// 	push_int(L, s.m_nPing);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_bHadSuccessfulResponse");
// 	push_bool(L, s.m_bHadSuccessfulResponse);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_bDoNotRefresh");
// 	push_bool(L, s.m_bDoNotRefresh);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_szGameDir");
// 	push_char_array(L, s.m_szGameDir, 32);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_szMap");
// 	push_char_array(L, s.m_szMap, 32);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_szGameDescription");
// 	push_char_array(L, s.m_szGameDescription, 64);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_nAppID");
// 	push_uint32(L, s.m_nAppID);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_nPlayers");
// 	push_int(L, s.m_nPlayers);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_nMaxPlayers");
// 	push_int(L, s.m_nMaxPlayers);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_nBotPlayers");
// 	push_int(L, s.m_nBotPlayers);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_bPassword");
// 	push_bool(L, s.m_bPassword);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_bSecure");
// 	push_bool(L, s.m_bSecure);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_ulTimeLastPlayed");
// 	push_uint32(L, s.m_ulTimeLastPlayed);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_nServerVersion");
// 	push_int(L, s.m_nServerVersion);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_szServerName");
// 	lua_pushstring(L, s.GetName());
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_szGameTags");
// 	push_char_array(L, s.m_szGameTags, 128);
// 	lua_settable(L, -3);
// 	lua_pushstring(L, "m_steamID");
// 	push_CSteamID(L, s.m_steamID);
// 	lua_settable(L, -3);
// }
// static void push_SteamParamStringArray_t(lua_State* L, SteamParamStringArray_t s) {
// 	lua_newtable(L);
// 	const char *pStrings = *s.m_ppStrings;
// 	for(int i=1; i<=s.m_nNumStrings; i++) {
// 		lua_pushnumber(L, i);
// 		lua_pushstring(L, &pStrings[i]);
// 		lua_settable(L, -3);
// 	}
// }



// /*****************************
// * CHECK primitives
// ******************************/
// static lua_Number check_int(lua_State* L, int index) {
// 	if(lua_isnumber(L, index)) {
// 		return luaL_checknumber(L, index);
// 	}
// 	return 0;
// }
// static lua_Number check_bool(lua_State* L, int index) {
// 	if(lua_isnumber(L, index)) {
// 		return lua_toboolean(L, index);
// 	}
// 	return 0;
// }
// static lua_Number check_float(lua_State* L, int index) {
// 	if(lua_isnumber(L, index)) {
// 		return luaL_checknumber(L, index);
// 	}
// 	return 0;
// }
// static void check_float_array(lua_State* L, int index, float * arr, unsigned int size) {
// 	if(lua_isnil(L, index) || lua_isnone(L, index)) {
// 		if(size > 0) {
// 			luaL_error(L, "Size > 0 but no table provided");
// 		}
// 		return;
// 	}
// 	if(!lua_istable(L, index)) {
// 		luaL_error(L, "Not a table");
// 	}
// 	int table_size = lua_objlen(L, index);
// 	if(table_size > size) {
// 		table_size = size;
// 	}
// 	for(int i=1; i<=table_size; i++) {
// 		lua_pushnumber(L, i);
// 		lua_gettable(L, index);
// 		arr[i] = check_float(L, -1);
// 	}
// }
// static lua_Number check_double(lua_State* L, int index) {
// 	return luaL_checknumber(L, index);
// }


// /*****************************
// * CHECK strings
// ******************************/
// static const char * check_const_char_ptr(lua_State* L, int index) {
// 	return luaL_checkstring(L, index);
// }
// static char * check_char_ptr(lua_State* L, int index) {
// 	return (char*)luaL_checkstring(L, index);
// }
// static const void * check_const_void_ptr(lua_State* L, int index) {
// 	return (void*)luaL_checkstring(L, index);
// }
// static void * check_void_ptr(lua_State* L, int index) {
// 	return (void*)luaL_checkstring(L, index);
// }
// static void check_char_array(lua_State* L, int index, char* dest, unsigned int size) {
// 	const char * src = luaL_checkstring(L, index);
// 	strncpy(dest, src, size - 1);
// 	dest[size] = 0x0;
// }

// /*****************************
// * CHECK (u)int64 (from string)
// ******************************/
// static uint64 check_uint64(lua_State* L, int index) {
// 	if(lua_isstring(L, index)) {
// 		const char * s = luaL_checkstring(L, index);
// 		return strtoull(s, NULL, 10);
// 	}
// 	return 0;
// }
// static int64_t check_int64_t(lua_State* L, int index) {
// 	if(lua_isstring(L, index)) {
// 		const char * s = luaL_checkstring(L, index);
// 		return strtoll(s, NULL, 10);
// 	}
// 	return 0;
// }
// static int64 check_int64(lua_State* L, int index) {
// 	if(lua_isstring(L, index)) {
// 		const char * s = luaL_checkstring(L, index);
// 		return strtoll(s, NULL, 10);
// 	}
// 	return 0;
// }
// static void check_uint64_array(lua_State* L, int index, uint64 * arr, unsigned int size) {
// 	if(lua_isnil(L, index) || lua_isnone(L, index)) {
// 		if(size > 0) {
// 			luaL_error(L, "Size > 0 but no table provided");
// 		}
// 		return;
// 	}
// 	if(!lua_istable(L, index)) {
// 		luaL_error(L, "Not a table");
// 	}
// 	int table_size = lua_objlen(L, index);
// 	if(table_size > size) {
// 		table_size = size;
// 	}
// 	for(int i=1; i<=table_size; i++) {
// 		lua_pushnumber(L, i);
// 		lua_gettable(L, index);
// 		arr[i] = check_uint64(L, -1);
// 	}
// }

// /*****************************
// * CHECK LuaBuffer
// ******************************/
// static dmScript::LuaHBuffer * check_buffer(lua_State* L, int index) {
// 	return dmScript::CheckBuffer(L, index);
// }

// /*****************************
// * CHECK CSteamID
// ******************************/
// static CSteamID check_CSteamID(lua_State* L, int index) {
// 	uint64 id = check_uint64(L, index);
// 	CSteamID steamId = CSteamID(id);
// 	return steamId;
// }
// static CSteamID check_class_CSteamID(lua_State* L, int index) {
// 	return check_CSteamID(L, index);
// }
// static void check_CSteamID_array(lua_State* L, int index, CSteamID *ids, int size) {
// 	if(lua_isnil(L, index) || lua_isnone(L, index)) {
// 		if(size > 0) {
// 			luaL_error(L, "Size > 0 but no table provided");
// 		}
// 		return;
// 	}
// 	if(!lua_istable(L, index)) {
// 		luaL_error(L, "Not a table");
// 	}
// 	int table_size = lua_objlen(L, index);
// 	if(table_size > size) {
// 		table_size = size;
// 	}
// 	for(int i=1; i<=table_size; i++) {
// 		lua_pushnumber(L, i);
// 		lua_gettable(L, index);
// 		ids[i] = check_CSteamID(L, -1);
// 	}
// }

// /*****************************
// * CHECK CGameID
// ******************************/
// static CGameID check_CGameID(lua_State* L, int index) {
// 	uint64 id = check_uint64(L, index);
// 	CGameID gameId = CGameID(id);
// 	return gameId;
// }

// /*****************************
// * CHECK structs
// ******************************/
// static servernetadr_t check_servernetadr_t(lua_State* L, int index) {
// 	servernetadr_t s;
// 	if(lua_isnil(L, index) || lua_isnone(L, index)) {
// 		s.SetConnectionPort(0);
// 		s.SetQueryPort(0);
// 		s.SetIP(0);
// 		return s;
// 	}
// 	if(!lua_istable(L, index)) {
// 		luaL_error(L, "Not a table");
// 	}
// 	lua_pushstring(L, "m_usConnectionPort");
// 	lua_gettable(L, index);
// 	s.SetConnectionPort(check_uint16(L, -1));
// 	lua_pushstring(L, "m_usQueryPort");
// 	lua_gettable(L, index);
// 	s.SetQueryPort(check_uint16(L, -1));
// 	lua_pushstring(L, "m_unIP");
// 	lua_gettable(L, index);
// 	s.SetIP(check_uint32(L, -1));
// 	return s;
// }
// static SteamParamStringArray_t check_SteamParamStringArray_t(lua_State* L, int index) {
// 	SteamParamStringArray_t s;
// 	if(lua_isnil(L, index) || lua_isnone(L, index)) {
// 		s.m_nNumStrings = 0;
// 		s.m_ppStrings = 0;
// 		return s;
// 	}
// 	if(!lua_istable(L, index)) {
// 		luaL_error(L, "Not a table");
// 	}
// 	const int table_size = lua_objlen(L, index);
// 	const char *pStrings[table_size];

// 	s.m_nNumStrings = table_size;
// 	s.m_ppStrings = pStrings;
// 	for(int i=1; i<=table_size; i++) {
// 		lua_pushnumber(L, i);
// 		lua_gettable(L, index);
// 		pStrings[i] = luaL_checkstring(L, -1);
// 	}
// 	return s;
// }




static dmScript::LuaCallbackInfo* steamworksListener;

class SteamCallbackWrapper {
	private:
		int64 m_iAppID;
	public:
		SteamCallbackWrapper();

		// //STEAM_CALLBACK(SteamCallbackWrapper, OnGameOverlayActivated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);
		// {{#callbacks}}
		// STEAM_CALLBACK(SteamCallbackWrapper, On{{callback}}, {{callback}}, m_Callback{{callback}});
		// {{/callbacks}}


		// {{#structs}}{{#callresult}}
		// CCallResult<SteamCallbackWrapper, {{struct}}> m_CallResult{{struct}};
		// void TrackSteamAPICall{{struct}}(SteamAPICall_t steamAPICall) {
		// 	m_CallResult{{struct}}.Set(steamAPICall, this, &SteamCallbackWrapper::On{{struct}});
		// }
		// void On{{struct}}({{struct}} *pResult, bool bIOFailure) {
		// 	dmLogInfo("SteamCallbackWrapper::On{{struct}}\n");
		// 	lua_State* L = steamworksListener.m_L;
		// 	if (!L) {
		// 		dmLogInfo("no lua state\n");
		// 		return;
		// 	}
		// 	int top = lua_gettop(L);
		// 	lua_pushlistener(L, steamworksListener);

		// 	lua_pushstring(L, "{{struct}}");
		// 	push_{{struct}}(L, *pResult);
		// 	int ret = lua_pcall(L, 3, LUA_MULTRET, 0);
		// 	if (ret != 0) {
		// 		dmLogInfo("SteamCallbackWrapper::On{{struct}} error: %s\n", lua_tostring(L, -1));
		// 		lua_pop(L, 1);
		// 	}
		// 	assert(top == lua_gettop(L));
		// }
		// {{/callresult}}{{/structs}}
};

// ctor
SteamCallbackWrapper::SteamCallbackWrapper() :

	// {{#callbacks}}
	// m_Callback{{callback}}(this, &SteamCallbackWrapper::On{{callback}}),
	// {{/callbacks}}
	m_iAppID( 0 )
{
	//m_iAppID = SteamUtils()->GetAppID();
}

// {{#callbacks}}
// void SteamCallbackWrapper::On{{callback}}({{callback}} *pCallback) {
// 	dmLogInfo("SteamCallbackWrapper::On{{callback}}\n");
// 	lua_State* L = steamworksListener.m_L;
// 	if (!L) {
// 		// no listener set
// 		return;
// 	}
// 	int top = lua_gettop(L);
// 	lua_pushlistener(L, steamworksListener);
// 	lua_pushstring(L, "{{callback}}");
// 	push_{{callback}}(L, *pCallback);
// 	int ret = lua_pcall(L, 3, LUA_MULTRET, 0);
// 	if (ret != 0) {
// 		dmLogInfo("SteamCallbackWrapper::On{{callback}} error: %s\n", lua_tostring(L, -1));
// 		lua_pop(L, 1);
// 	}
// 	assert(top == lua_gettop(L));

// }
// {{/callbacks}}


static SteamCallbackWrapper *steamCallbackWrapper = new SteamCallbackWrapper();





extern "C" void __cdecl SteamAPIDebugTextHook( int nSeverity, const char *pchDebugText )
{
	if (nSeverity == 0) {
		dmLogInfo("%s", pchDebugText);
	}
	else {
		dmLogWarning("%s", pchDebugText);
	}
}

static int Init(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	dmLogInfo("Init");
	if(!SteamAPI_Init()) {
		luaL_error(L, "Error initialising SteamAPI");
	}
	if (!SteamAPI_IsSteamRunning()) {
		luaL_error(L, "Steam is not running");
	}
	apps = SteamApps();
	friends = SteamFriends();
	inventory = SteamInventory();
	matchmaking = SteamMatchmaking();
	music = SteamMusic();
	remote_storage = SteamRemoteStorage();
	user = SteamUser();
	utils = SteamUtils();
	utils->SetWarningMessageHook(&SteamAPIDebugTextHook);
	user_stats = SteamUserStats();
	ugc = SteamUGC();
	parties = SteamParties();
	input = SteamInput();
	game_search = SteamGameSearch();
	networking = SteamNetworking();
	video = SteamVideo();
	screenshots = SteamScreenshots();
	music_remote = SteamMusicRemote();
	return 0;
}

static int Update(lua_State* L) {
	SteamAPI_RunCallbacks();
	return 0;
}

static int Restart(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);
	uint32_t appid = (uint32_t)luaL_checknumber(L, 1);
	int result = SteamAPI_RestartAppIfNecessary(appid);
	lua_pushboolean(L, result);
	return 1;
}

static int Final(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);
	SteamAPI_Shutdown();
	return 0;
}

static int SetListener(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 0);

	steamworksListener = dmScript::CreateCallback(L, 1);
	return 0;
}

static int UtilsGetAppId(lua_State* L) {
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushnumber(L, utils->GetAppID());
	return 1;
}

static const luaL_reg Module_methods[] = {
	{ "init", Init },
	{ "restart", Restart },
	{ "update", Update },
	{ "final", Final },
	{ "set_listener", SetListener },
	{ "utils_get_app_id", UtilsGetAppId },
	{ 0, 0 }
};

static void LuaInit(lua_State* L) {
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);

	#define SETCONSTANT(name, val) \
	lua_pushnumber(L, (lua_Number) val); \
	lua_setfield(L, -2, #name);

	// SETCONSTANT({{name}}, {{value}});
	#undef SETCONSTANT

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeSteam(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeSteam(dmExtension::Params* params) {
	LuaInit(params->m_L);
	dmLogInfo("Registered %s Extension", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeSteam(dmExtension::AppParams* params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeSteam(dmExtension::Params* params) {
	return dmExtension::RESULT_OK;
}

#else

static dmExtension::Result AppInitializeSteam(dmExtension::AppParams* params)
{
    dmLogWarning("Registered %s (null) Extension", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeSteam(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeSteam(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeSteam(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

#endif

DM_DECLARE_EXTENSION(steam, LIB_NAME, AppInitializeSteam, AppFinalizeSteam, InitializeSteam, 0, 0, FinalizeSteam)
