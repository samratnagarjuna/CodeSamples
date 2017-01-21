#include "Utility\NLuaUtility.h"

#include <cassert>

#include "Utility\NConsolePrint.h"

namespace Illehc
{
	uint8_t * LoadFile(const char * i_pFilename, size_t & o_sizeFile)
	{
		assert(i_pFilename != NULL);

		FILE * pFile = NULL;

		errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
		if (fopenError != 0)
			return NULL;

		assert(pFile != NULL);

		int FileIOError = fseek(pFile, 0, SEEK_END);
		assert(FileIOError == 0);

		long FileSize = ftell(pFile);
		assert(FileSize >= 0);

		FileIOError = fseek(pFile, 0, SEEK_SET);
		assert(FileIOError == 0);

		uint8_t * pBuffer = new uint8_t[FileSize];
		assert(pBuffer);

		size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
		assert(FileRead == FileSize);

		fclose(pFile);

		o_sizeFile = FileSize;

		return pBuffer;
	}

	int GetLuaTableEntries(lua_State * i_pState, int i_index)
	{
		assert(i_pState);
		assert(lua_gettop(i_pState) >= -i_index);
		assert(lua_type(i_pState, i_index) == LUA_TTABLE);

		lua_len(i_pState, i_index);
		assert(lua_type(i_pState, -1) == LUA_TNUMBER);

		lua_Integer table_entries = lua_tointeger(i_pState, -1);
		lua_pop(i_pState, 1);

		assert(table_entries <= INT_MAX);

		return static_cast<int>(table_entries);
	}

	size_t ReadIntArray(lua_State * i_pState, int i_index, int * o_pInts, size_t i_numInts)
	{
		assert(i_pState);
		assert(lua_gettop(i_pState) >= -i_index);
		assert(lua_type(i_pState, i_index) == LUA_TTABLE);

		size_t	index = 0;

		lua_pushnil(i_pState);

		while (lua_next(i_pState, i_index - 1) != 0)
		{
			if (lua_type(i_pState, -1) == LUA_TNUMBER)
			{
				lua_Integer lValue = lua_tointeger(i_pState, -1);
				assert(lValue <= INT_MAX);

				o_pInts[index] = lValue < INT_MAX ? int(lValue) : INT_MAX;
			}
			else
			{
				DEBUG_PRINT( "Expected a number at position %d while reading table as int array.\n", index );
			}

			lua_pop(i_pState, 1);

			if (++index == i_numInts)
			{
				lua_pop(i_pState, 1);
				break;
			}
		}

		return index;
	}

	size_t ReadFloatArray(lua_State * i_pState, int i_index, float * o_pFloats, size_t i_numFloats)
	{
		assert(i_pState);
		assert(lua_gettop(i_pState) >= -i_index);
		assert(lua_type(i_pState, i_index) == LUA_TTABLE);

		size_t				index = 0;

		lua_pushnil(i_pState);

		while (lua_next(i_pState, i_index - 1) != 0)
		{
			if (lua_type(i_pState, -1) == LUA_TNUMBER)
			{
				o_pFloats[index] = float(lua_tonumber(i_pState, -1));
			}
			else
			{
				DEBUG_PRINT( "Expected a number at position %d while reading table as float array.\n", index );
			}

			// pop the value now that we've retrieved it
			lua_pop(i_pState, 1);
			if (++index == i_numFloats)
			{
				lua_pop(i_pState, 1);
				break;
			}
		}
		return index;
	}

	size_t GetFloatArray(lua_State * i_pState, int i_indexTable, const char * i_pArrayKey, float * o_pFloats, size_t i_numFloats)
	{
		assert(i_pArrayKey);

		lua_pushstring(i_pState, i_pArrayKey);
		i_indexTable--;

		int type = lua_gettable(i_pState, i_indexTable);

		size_t itemsRead = 0;

		if (type == LUA_TTABLE)
			itemsRead = ReadFloatArray(i_pState, i_indexTable, o_pFloats, i_numFloats);
		else
		{
			// DEBUG_PRINT( "Expected Lua table for %s found value of type %d.\n", i_pArrayKey, type );
		}

		lua_pop(i_pState, 1);

		return itemsRead;
	}

	Vector3 GetVector3(lua_State * i_pState, int i_index)
	{
		assert(i_pState);
		assert(lua_gettop(i_pState) >= -i_index);
		assert(lua_type(i_pState, i_index) == LUA_TTABLE);

		const size_t		numFloats = 3;
		float				v[numFloats] = { 0 };

		size_t	readFloats = ReadFloatArray(i_pState, i_index, v, numFloats);

		return Vector3(v[0], v[1], v[2]);
	}
} // namespace Illehc