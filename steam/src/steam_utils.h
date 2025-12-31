#ifndef DEFOLD_STEAMUTILS_H
#define DEFOLD_STEAMUTILS_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamUtils_OnGamepadTextInputDismissed(lua_State* L, void* data);
int SteamUtils_OnFloatingGamepadTextInputDismissed(lua_State* L, void* data);

int SteamUtils_Init(lua_State* L);
int SteamUtils_GetAppId(lua_State* L);
int SteamUtils_GetSecondsSinceAppActive(lua_State* L);
int SteamUtils_IsSteamRunningOnSteamDeck(lua_State* L);
int SteamUtils_IsSteamOverlayAvailable(lua_State* L);
int SteamUtils_GetImageSize(lua_State* L);
int SteamUtils_GetImageRGBA(lua_State* L);
int SteamUtils_GetServerRealTime(lua_State* L);
int SteamUtils_ShowFloatingGamepadTextInput(lua_State* L);
int SteamUtils_ShowGamepadTextInput(lua_State* L);

#endif

#endif
