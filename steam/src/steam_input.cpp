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

	int count = g_SteamInput->GetConnectedControllers(g_InputHandles);

	dmLogInfo("count %d", count);

	return 0;
}

#endif
