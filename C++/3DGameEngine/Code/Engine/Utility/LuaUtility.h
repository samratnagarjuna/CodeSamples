// This file has utility functions for Lua parsing

//Header Files
//============

#ifndef EAE6320_UTILITY_LUA_UTILITY
#define EAE6320_UTILITY_LUA_UTILITY

#include "../../External/Lua/Includes.h"

// Interface
//==========

bool ReadFloatArray(lua_State& io_luaState, int i_index, float* o_pFloats, size_t i_numFloats);
bool ReadIntArray(lua_State& io_luaState, int i_index, int * o_pInts, size_t i_numInts);

#endif // !EAE6320_GRAPHICS_LUA_UTILITY