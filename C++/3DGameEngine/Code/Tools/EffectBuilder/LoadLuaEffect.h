/*
	This file loads a lua effect file from table
*/

#ifndef EAE6320_CEFFECTBUILDER_LOAD_LUA_EFFECT_H
#define EAE6320_CEFFECTBUILDER_LOAD_LUA_EFFECT_H


// Header Files
//=============

#include <stdint.h>

struct EffectData
{
	char* vertexShaderPath;
	char* fragmentShaderPath;
	uint8_t renderStateBits;
};


// Interface
//==========

namespace eae6320 
{
	namespace AssetBuild
	{
		bool LoadLuaEffectFromFile(const char * const i_path, EffectData& o_effectData);
		bool CleanLuaEffectData(EffectData& io_effectData);
	}
}

#endif // !EAE6320_CEFFECTBUILDER_LOAD_LUA_EFFECT_H


