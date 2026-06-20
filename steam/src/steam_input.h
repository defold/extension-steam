#ifndef DEFOLD_STEAMINPUT_H
#define DEFOLD_STEAMINPUT_H

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>

int SteamInput_Init(lua_State* L);
void SteamInput_Update();
void SteamInput_Shutdown();

/**
 * Enumerate currently connected Steam Input enabled devices.
 * @name input_get_connected_controllers
 * @treturn table Table with input handles
 */
int SteamInput_GetConnectedControllers(lua_State* L);

/**
 * Invokes the Steam overlay and brings up the binding screen.
 * @name input_show_binding_panel
 * @param string input_handle
 * @treturn boolean Success
 */
int SteamInput_ShowBindingPanel(lua_State* L);

/**
 * Lookup the handle for a digital action. Best to do this once on startup, and
 * store the handles for all future API calls.
 * @name input_get_digital_action_handle
 * @string action_name
 * @treturn string action_handle
 */
int SteamInput_GetDigitalActionHandle(lua_State* L);

/**
 * Get the current state of the supplied digital game action.
 * @name input_get_digital_action_data
 * @string input_handle
 * @string action_handle
 * @treturn table Table with input data (active, state)
 */
int SteamInput_GetDigitalActionData(lua_State* L);

/**
 * Lookup the handle for an analog action. Best to do this once on startup, and
 * store the handles for all future API calls.
 * @name input_get_analog_action_handle
 * @string action_name
 * @treturn string action_handle
 */
int SteamInput_GetAnalogActionHandle(lua_State* L);

/**
 * Get the current state of the supplied analog game action.
 * @name input_get_analog_action_data
 * @string input_handle
 * @string action_handle
 * @treturn table Table with input data (active, mode, x, y)
 */
int SteamInput_GetAnalogActionData(lua_State* L);

#endif

#endif
