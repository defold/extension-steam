#ifndef DEFOLD_STEAMNETWORKING_H
#define DEFOLD_STEAMNETWORKING_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamNetworking_OnSteamNetworkingMessagesSessionFailed(lua_State* L, void* data);
int SteamNetworking_OnSteamNetworkingMessagesSessionRequest(lua_State* L, void* data);

int SteamNetworking_Init(lua_State* L);
int SteamNetworking_SendMessageToUser(lua_State* L);
int SteamNetworking_ReceiveMessagesOnChannel(lua_State* L);
int SteamNetworking_AcceptSessionWithUser(lua_State* L);
int SteamNetworking_CloseSessionWithUser(lua_State* L);
int SteamNetworking_CloseChannelWithUser(lua_State* L);
int SteamNetworking_GetSessionConnectionInfo(lua_State* L);

#endif

#endif
