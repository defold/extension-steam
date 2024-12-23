#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_listener.h"

const uint32 MAX_INPUT_LENGTH = 1024;
const uint32 FOO = MAX_INPUT_LENGTH + 1;


int SteamUtils_OnFloatingGamepadTextInputDismissed(lua_State* L, void* data)
{
	FloatingGamepadTextInputDismissed_t* s = (FloatingGamepadTextInputDismissed_t*)data;
	lua_pushstring(L, "FloatingGamepadTextInputDismissed_t");
	return 1;
}

int SteamUtils_OnGamepadTextInputDismissed(lua_State* L, void* data)
{
	GamepadTextInputDismissed_t* s = (GamepadTextInputDismissed_t*)data;
	// The user canceled,
	if (!s->m_bSubmitted) {
		lua_pushnil(L);
		return 1;
	}

	const uint32 length = SteamUtils()->GetEnteredGamepadTextLength();
	char* szTextInput = (char*)malloc(length);
	bool success = SteamUtils()->GetEnteredGamepadTextInput( szTextInput, length );

	if ( !success ) {
		// Log an error. This should only ever happen if length is > MaxInputLength
		free(szTextInput);
		lua_pushnil(L);
		return 1;
	}

	lua_pushstring(L, "GamepadTextInputDismissed_t");

	lua_newtable(L);
	lua_pushstring(L, "m_szTextInput");
	lua_pushstring(L, szTextInput);
	lua_settable(L, -3);

	free(szTextInput);
	return 2;
}

class SteamUtilsCallbacks
{
	public:
		SteamUtilsCallbacks();
		STEAM_CALLBACK(SteamUtilsCallbacks, OnGamepadTextInputDismissed, GamepadTextInputDismissed_t, m_CallbackGamepadTextInputDismissed);
		STEAM_CALLBACK(SteamUtilsCallbacks, OnFloatingGamepadTextInputDismissed, FloatingGamepadTextInputDismissed_t, m_CallbackFloatingGamepadTextInputDismissed);
};
SteamUtilsCallbacks::SteamUtilsCallbacks() :
	m_CallbackGamepadTextInputDismissed(this, &SteamUtilsCallbacks::OnGamepadTextInputDismissed),
	m_CallbackFloatingGamepadTextInputDismissed(this, &SteamUtilsCallbacks::OnFloatingGamepadTextInputDismissed)
{}
void SteamUtilsCallbacks::OnGamepadTextInputDismissed(GamepadTextInputDismissed_t *s)
{
	SteamListener_Invoke(SteamUtils_OnGamepadTextInputDismissed, s);
}
void SteamUtilsCallbacks::OnFloatingGamepadTextInputDismissed(FloatingGamepadTextInputDismissed_t *s)
{
	SteamListener_Invoke(SteamUtils_OnFloatingGamepadTextInputDismissed, s);
}


static SteamUtilsCallbacks* g_SteamUtilsCallbacks = new SteamUtilsCallbacks();

static ISteamUtils* g_SteamUtils = 0;

extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char *pchDebugText)
{
	if (nSeverity == 0)
	{
		dmLogInfo("%s", pchDebugText);
	}
	else
	{
		dmLogWarning("%s", pchDebugText);
	}
}

int SteamUtils_Init(lua_State* L)
{
	DM_LUA_STACK_CHECK(L, 0);
	g_SteamUtils = SteamUtils();
	g_SteamUtils->SetWarningMessageHook(&SteamAPIDebugTextHook);
	return 0;
}

/** Returns the appID of the current process.
 * @name utils_get_app_id
 * @treturn Number app_id
 */
int SteamUtils_GetAppId(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	lua_pushnumber(L, g_SteamUtils->GetAppID());
	return 1;
}

/** Return the number of seconds since the user.
 * @name utils_get_seconds_since_app_active
 * @treturn Number seconds
 */
int SteamUtils_GetSecondsSinceAppActive(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushnumber(L, g_SteamUtils->GetSecondsSinceAppActive());
	return 1;
}

/** Returns true if currently running on the Steam Deck device.
 * @name utils_is_steam_running_on_steam_deck
 * @treturn Boolean running_on_steamdeck
 */
int SteamUtils_IsSteamRunningOnSteamDeck(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	lua_pushboolean(L, g_SteamUtils->IsSteamRunningOnSteamDeck());
	return 1;
}


/** Get size of image
 * @name utils_get_image_size
 * @number image Image handle
 * @treturn Boolean ok True if size of image was read successfully
 * @treturn Number width Image width or nil
 * @treturn Number height Image height or nil
 */
int SteamUtils_GetImageSize(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 3);

	int iImage = luaL_checknumber(L, 1);
	uint32 pnWidth = 0;
	uint32 pnHeight = 0;

	bool ok = g_SteamUtils->GetImageSize(iImage, &pnWidth, &pnHeight);
	lua_pushboolean(L, ok);
	if (ok)
	{
		lua_pushnumber(L, pnWidth);
		lua_pushnumber(L, pnWidth);
	}
	else
	{
		lua_pushnil(L);
		lua_pushnil(L);
	}
	return 3;
}

/** Get image in RGBA format.
 * @name utils_get_image_rgba
 * @number image Image handle
 * @number size Size of image
 * @treturn Boolean ok True if size of image was read successfully
 * @treturn String Image
 */
int SteamUtils_GetImageRGBA(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	int iImage = luaL_checknumber(L, 1);
	int imageSize = luaL_checknumber(L, 2);
	int nDestBufferSize = sizeof(uint8) * 4 * imageSize;
	uint8* pubDest = (uint8*)malloc(nDestBufferSize);
	bool ok = g_SteamUtils->GetImageRGBA(iImage, pubDest, nDestBufferSize);
	lua_pushboolean(L, ok);
	if (ok)
	{
		lua_pushlstring(L, (char*)pubDest, nDestBufferSize);
	}
	else
	{
		lua_pushnil(L);
	}
	free(pubDest);
	return 2;
}


/** Returns the Steam server time in Unix epoch format. (Number of seconds since Jan 1, 1970 UTC)
 * @name utils_get_server_real_time
 * @treturn number Server time
 */
int SteamUtils_GetServerRealTime(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	uint32 time = g_SteamUtils->GetServerRealTime();
	lua_pushnumber(L, time);
	return 1;
}



/** Opens a floating keyboard over the game content and sends OS keyboard keys directly to the game.
 * @name utils_show_floating_gamepad_text_input
 * @number mode EFloatingGamepadTextInputMode
 * @number x Text field x position
 * @number y Text field y position
 * @number width Text field width
 * @number height Text field height
 * @treturn bool result True if the floating keyboard was shown, otherwise, false.
 */
int SteamUtils_ShowFloatingGamepadTextInput(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	EFloatingGamepadTextInputMode eKeyboardMode = (EFloatingGamepadTextInputMode)luaL_checknumber(L, 1);
	int nTextFieldXPosition = luaL_checknumber(L, 2);
	int nTextFieldYPosition = luaL_checknumber(L, 3);
	int nTextFieldWidth = luaL_checknumber(L, 4);
	int nTextFieldHeight = luaL_checknumber(L, 5);

	bool was_shown = g_SteamUtils->ShowFloatingGamepadTextInput(eKeyboardMode, nTextFieldXPosition, nTextFieldYPosition, nTextFieldWidth, nTextFieldHeight);
	lua_pushboolean(L, was_shown);
	return 1;
}


/** Activates the Big Picture text input dialog which only supports gamepad input.
 * @name utils_show_gamepad_text_input
 * @number input_mode EGamepadTextInputMode
 * @number line_input_mode EGamepadTextInputLineMode
 * @string description Sets the description that should inform the user what the input dialog is for
 * @string existing_text Sets the preexisting text which the user can edit.
 * @treturn bool result True if the big picture overlay is running; otherwise, false
 */
int SteamUtils_ShowGamepadTextInput(lua_State* L)
{
	if (!g_SteamUtils) return 0;
	DM_LUA_STACK_CHECK(L, 1);

	EGamepadTextInputMode eInputMode = (EGamepadTextInputMode)luaL_checknumber(L, 1);
	EGamepadTextInputLineMode eLineInputMode = (EGamepadTextInputLineMode)luaL_checknumber(L, 2);
	const char *pchDescription = luaL_checkstring(L, 3);
	uint32 unCharMax = MAX_INPUT_LENGTH;
	const char *pchExistingText = luaL_checkstring(L, 4);

	bool big_picture = g_SteamUtils->ShowGamepadTextInput(eInputMode, eLineInputMode, pchDescription, unCharMax, pchExistingText);
	lua_pushboolean(L, big_picture);
	return 1;
}

#endif