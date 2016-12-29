//Header Files
//============

#include "LuaUtility.h"

#include <iostream>

bool ReadFloatArray(lua_State& io_luaState, int i_index, float* o_pFloats, size_t i_numFloats)
{
	bool wereThereErrors = false;

	// This tells lua_next() to return the first key
	lua_pushnil(&io_luaState);
	{
		const int currentIndexOfTable = i_index - 1;
		size_t index = 0;
		while (lua_next(&io_luaState, currentIndexOfTable) != 0)
		{
			if (lua_type(&io_luaState, -1) == LUA_TNUMBER)
			{
				o_pFloats[index] = float(lua_tonumber(&io_luaState, -1));
			}
			else
			{
				std::cerr << "The value for \"" << index << "\" must be a float "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
				wereThereErrors = true;
				// Pop current key value
				lua_pop(&io_luaState, 1);
				goto OnExit;
			}

			// Pop the current key value
			lua_pop(&io_luaState, 1);

			// Do not call lua_next if you reached the limit
			if (++index == i_numFloats) break;
		}
	}

OnExit:
	// Pop the left over key placed by lua_next()
	lua_pop(&io_luaState, 1);
	return !wereThereErrors;
}

bool ReadIntArray(lua_State& io_luaState, int i_index, int * o_pInts, size_t i_numInts)
{
	bool wereThereErrors = false;

	// This tells lua_next() to return the first key
	lua_pushnil(&io_luaState);
	{
		const int currentIndexOfTable = i_index - 1;
		size_t index = 0;
		while (lua_next(&io_luaState, currentIndexOfTable) != 0)
		{
			if (lua_type(&io_luaState, -1) == LUA_TNUMBER)
			{
				lua_Integer lValue = lua_tointeger(&io_luaState, -1);
				// Add an assert here to if the lValue is less than INT_MAX
				o_pInts[index] = lValue < INT_MAX ? int(lValue) : INT_MAX;
			}
			else
			{
				std::cerr << "The value for \"" << index << "\" must be a int "
					"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
				wereThereErrors = true;
				// Pop current key value
				lua_pop(&io_luaState, 1);
				goto OnExit;
			}

			// Pop the current key value
			lua_pop(&io_luaState, 1);

			// Do not call lua_next if you reached the limit
			if (++index == i_numInts) break;
		}
	}

OnExit:
	// Pop the left over key placed by lua_next()
	lua_pop(&io_luaState, 1);
	return !wereThereErrors;
}