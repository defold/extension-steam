/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"


static dmScript::LuaCallbackInfo* g_SteamListener = 0;

void SteamListener_Invoke(int (*fn)(lua_State*, const void*), const void* data)
{
	if (!g_SteamListener)
	{
		dmLogWarning("Steam callback is not set");
		return;
	}

	if (!dmScript::IsCallbackValid(g_SteamListener))
	{
		dmLogWarning("Steam callback is not valid");
		g_SteamListener = 0;
		return;
	}

	lua_State* L = dmScript::GetCallbackLuaContext(g_SteamListener);
    DM_LUA_STACK_CHECK(L, 0)

	if (!dmScript::SetupCallback(g_SteamListener))
	{
		dmLogWarning("Steam callback setup failed");
		dmScript::DestroyCallback(g_SteamListener);
		g_SteamListener = 0;
		return;
	}

	int num = (*fn)(L, data);
	int ret = dmScript::PCall(L, 1 + num, 0);
	dmScript::TeardownCallback(g_SteamListener);
}

static int SteamListener_GenericCallbackArgs(lua_State* L, const void* data) {
    lua_pushstring(L, (const char*)data);
	lua_newtable(L);
	table_push_boolean(L, "not_implemented_yet", true);
    return 2;
}

void SteamListener_InvokeGeneric(const char* id)
{
    SteamListener_Invoke(&SteamListener_GenericCallbackArgs, id);
}

void SteamListener_Destroy()
{
	if (g_SteamListener)
	{
		dmScript::DestroyCallback(g_SteamListener);
		g_SteamListener = 0;
	}
}

/** Set a listener.
 * @name set_listener
 * @function listener Listener function to call
 * @usage steam.set_listener(function(self, event, data)
 *    if event == "GameOverlayActivated_t" then
 *        print("Overlay is active:", data.m_bActive)
 *    end
 * end)
 */
int SteamListener_Set(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	if (g_SteamListener)
	{
		dmScript::DestroyCallback(g_SteamListener);
		g_SteamListener = 0;
	}
	if (lua_isfunction(L, 1))
	{
		g_SteamListener = dmScript::CreateCallback(L, 1);
	}
	return 0;
}


#endif
