/*
This file loads a lua material file from table
*/

#ifndef EAE6320_CMATERIALBUILDER_LOAD_LUA_EFFECT_H
#define EAE6320_CMATERIALBUILDER_LOAD_LUA_EFFECT_H

// Header Files
//=============

#include "../../Engine/Graphics/Includes.h"

struct MaterialData
{
	char*						effectPath;
	char*						texturePath;
	sConstantBufferMaterialData constantData;
};

// Interface
//==========

namespace eae6320
{
	namespace AssetBuild
	{
		bool LoadLuaMaterialFromFile(const char * const i_path, MaterialData& o_materialData);
		bool CleanLuaMaterialData(MaterialData& io_materialData);
	}
}

#endif // !EAE6320_CMATERIALBUILDER_LOAD_LUA_EFFECT_H
