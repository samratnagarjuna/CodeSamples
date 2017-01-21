#pragma once

#ifndef __ENGINE__NLUAUTILITY__H__
#define __ENGINE__NLUAUTILITY__H__

#include "lua.hpp"
#include "Math\NVector3.h"
#include <stdint.h>

namespace Illehc
{

	uint8_t * LoadFile(const char * i_pFilename, size_t & o_sizeFile);
	int GetLuaTableEntries(lua_State * i_pState, int i_index);
	size_t ReadIntArray(lua_State * i_pState, int i_index, int * o_pInts, size_t i_numInts);
	size_t ReadFloatArray(lua_State * i_pState, int i_index, float * o_pFloats, size_t i_numFloats);
	size_t GetFloatArray(lua_State * i_pState, int i_indexTable, const char * i_pArrayKey, float * o_pFloats, size_t i_numFloats);
	Vector3 GetVector3(lua_State * i_pState, int i_index);
} // namespace Illehc

#endif // !__ENGINE__NLUAUTILITY__H__

