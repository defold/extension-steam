/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"

static ISteamInput*		g_SteamInput = 0;
static InputHandle_t*	g_InputHandles = 0;

int SteamInput_Init(lua_State* L)
{
	g_InputHandles = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
	g_SteamInput = SteamInput();
	const bool bExplicitlyCallRunFrame = false;
	g_SteamInput->Init(bExplicitlyCallRunFrame);
	g_SteamInput->EnableDeviceCallbacks();
	return 0;
}

void SteamInput_Update()
{
	if (!g_SteamInput) return;
	g_SteamInput->RunFrame();
}

void SteamInput_Shutdown()
{
	if (!g_SteamInput) return;
	g_SteamInput->Shutdown();
	delete g_InputHandles;
	g_InputHandles = 0;
}

int SteamInput_GetConnectedControllers(lua_State* L)
{
	if (!g_SteamInput) return 0;

	DM_LUA_STACK_CHECK(L, 1);
	int count = g_SteamInput->GetConnectedControllers(g_InputHandles);

	lua_newtable(L);
	for (int i = 0; i < count; i++)
	{
		lua_pushnumber(L, i + 1);
		push_uint64(L, g_InputHandles[i]);
		lua_settable(L, -3);
	}

	return 1;
}

int SteamInput_GetDigitalActionHandle(lua_State* L)
{
	if (!g_SteamInput) return 0;

	DM_LUA_STACK_CHECK(L, 1);
	const char* pszActionName = luaL_checkstring(L, 1);
	InputDigitalActionHandle_t handle = g_SteamInput->GetDigitalActionHandle(pszActionName);
	push_uint64(L, handle);
	return 1;
}
int SteamInput_GetDigitalActionData(lua_State* L)
{
	if (!g_SteamInput) return 0;

	DM_LUA_STACK_CHECK(L, 1);
	InputHandle_t inputHandle = check_uint64(L, 1);
	InputDigitalActionHandle_t digitalActionHandle = check_uint64(L, 2);
	InputDigitalActionData_t data = g_SteamInput->GetDigitalActionData(inputHandle, digitalActionHandle);

	lua_newtable(L);
	lua_pushstring(L, "state");
	lua_pushboolean(L, data.bState);
	lua_settable(L, -3);

	lua_pushstring(L, "active");
	lua_pushboolean(L, data.bActive);
	lua_settable(L, -3);

	return 1;
}
int SteamInput_GetAnalogActionHandle(lua_State* L)
{
	if (!g_SteamInput) return 0;

	DM_LUA_STACK_CHECK(L, 1);
	const char* pszActionName = luaL_checkstring(L, 1);
	InputAnalogActionHandle_t handle = g_SteamInput->GetAnalogActionHandle(pszActionName);
	push_uint64(L, handle);
	return 1;
}
int SteamInput_GetAnalogActionData(lua_State* L)
{
	if (!g_SteamInput) return 0;

	DM_LUA_STACK_CHECK(L, 1);
	InputHandle_t inputHandle = check_uint64(L, 1);
	InputAnalogActionHandle_t digitalActionHandle = check_uint64(L, 2);
	InputAnalogActionData_t data = g_SteamInput->GetAnalogActionData(inputHandle, digitalActionHandle);

	lua_newtable(L);
	lua_pushstring(L, "mode");
	lua_pushinteger(L, data.eMode);
	lua_settable(L, -3);

	lua_pushstring(L, "x");
	lua_pushnumber(L, data.x);
	lua_settable(L, -3);

	lua_pushstring(L, "y");
	lua_pushnumber(L, data.y);
	lua_settable(L, -3);

	lua_pushstring(L, "active");
	lua_pushboolean(L, data.bActive);
	lua_settable(L, -3);

	return 1;
}


#endif
