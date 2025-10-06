/**
 * @document
 * @namespace steam
 */

#if defined(DM_PLATFORM_OSX) || defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_LINUX)

#include <dmsdk/sdk.h>
#include "steam_api.h"
#include "steam_types.h"


int SteamRemoteStorage_OnRemoteStorageFileShareResult(lua_State* L, void* data)
{
	RemoteStorageFileShareResult_t* s = (RemoteStorageFileShareResult_t*)data;
	lua_pushstring(L, "RemoteStorageFileShareResult_t");

	lua_newtable(L);
	table_push_number(L, "m_eResult", s->m_eResult);
	table_push_uint64(L, "m_hFile", s->m_hFile);
	table_push_string(L, "m_rgchFilename", s->m_rgchFilename);

	return 2;
}


static ISteamRemoteStorage* g_SteamRemoteStorage = 0;

int SteamRemoteStorage_Init(lua_State* L)
{
	g_SteamRemoteStorage = SteamRemoteStorage();
	return 0;
}

/** Share a file
 * @name remote_storage_file_share
 * @string filename Name of file to share
 * @treturn string call id
 */
int SteamRemoteStorage_FileShare(lua_State* L)
{
	if (!g_SteamRemoteStorage) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* filename = luaL_checkstring(L, 1);
	SteamAPICall_t call = g_SteamRemoteStorage->FileShare(filename);
	push_uint64(L, call);
	return 1;
}

/** Get number of uploaded files
 * @name remote_storage_get_file_count
 * @treturn number File count
 */
int SteamRemoteStorage_GetFileCount(lua_State* L)
{
	if (!g_SteamRemoteStorage) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	int count = g_SteamRemoteStorage->GetFileCount();
	lua_pushinteger(L, count);
	return 1;
}

/** Get file information
 * @name remote_storage_get_file_name_and_size
 * @treturn number File count
 */
int SteamRemoteStorage_GetFileNameAndSize(lua_State* L)
{
	if (!g_SteamRemoteStorage) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	int index = luaL_checknumber(L, 1);
	int32 pnFileSizeInBytes;
	const char* name = g_SteamRemoteStorage->GetFileNameAndSize(index, &pnFileSizeInBytes);
	lua_pushstring(L, name);
	lua_pushinteger(L, pnFileSizeInBytes);
	return 2;
}

/** Get storage quota
 * @name remote_storage_get_quota
 * @treturn number Available bytes
 * @treturn number Total bytes
 */
int SteamRemoteStorage_GetQuota(lua_State* L)
{
	if (!g_SteamRemoteStorage) return 0;
	DM_LUA_STACK_CHECK(L, 2);
	uint64 pnTotalBytes;
	uint64 puAvailableBytes;
	bool ok = g_SteamRemoteStorage->GetQuota(&pnTotalBytes, &puAvailableBytes);
	if (!ok)
	{
		lua_pushnil(L);
		lua_pushnil(L);
	}
	else
	{
		lua_pushinteger(L, puAvailableBytes);
		lua_pushinteger(L, pnTotalBytes);
	}
	return 2;
}

/** Creates a new file, writes the bytes to the file, and then closes the file.
 * If the target file already exists, it is overwritten.
 * @name remote_storage_file_write
 * @string filename The name of the file to write to.
 * @string data
 * @treturn boolean Success
 */
int SteamRemoteStorage_FileWrite(lua_State* L)
{
	if (!g_SteamRemoteStorage) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* filename = luaL_checkstring(L, 1);
	size_t size;
	const char* data = luaL_checklstring(L, 2, &size);
	bool ok = g_SteamRemoteStorage->FileWrite(filename, data, size);
	lua_pushboolean(L, ok);
	return 1;
}

/** Opens a binary file, reads the contents of the file into a byte array,
 * and then closes the file.
 * @name remote_storage_file_read
 * @string filename Name of the file to read from
 * @treturn string data
 */
int SteamRemoteStorage_FileRead(lua_State* L)
{
	if (!g_SteamRemoteStorage) return 0;
	DM_LUA_STACK_CHECK(L, 1);
	const char* filename = luaL_checkstring(L, 1);
	size_t size = g_SteamRemoteStorage->GetFileSize(filename);
	uint8_t* data = (uint8_t*)malloc(size + 1);
	bool ok = g_SteamRemoteStorage->FileRead(filename, data, size);
	if (ok)
	{
		data[size] = 0;
		lua_pushstring(L, (char*)data);
	}
	else
	{
		lua_pushnil(L);
	}
	free(data);
	return 1;
}


#endif
