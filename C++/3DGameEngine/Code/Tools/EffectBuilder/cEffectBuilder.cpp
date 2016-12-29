// Header Files
//=============

#include "cEffectBuilder.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include "../AssetBuildLibrary/UtilityFunctions.h"
#include "LoadLuaEffect.h"

// Inherited Implementation
//=========================

// Build
//======

bool eae6320::AssetBuild::cEffectBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	// Load Effect data using Lua
	EffectData effectData;
	{
		if (!LoadLuaEffectFromFile(m_path_source, effectData))
		{
			wereThereErrors = true;
			std::ostringstream errorMessage;
			errorMessage << "Failed to load \"" << m_path_source << "\" using lua: ";
			eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			goto OnExit;
		}

		// Write to file as binary data
		{

			std::ofstream outfile(m_path_target, std::ofstream::binary);

			outfile.write(reinterpret_cast<char*>(&(effectData.renderStateBits)), sizeof(uint8_t));

			std::string buildVertexShaderPath;
			std::string buildFragmentShaderPath;
			
			std::string errorMessage;

			// Handle error checking here
			ConvertSourceRelativePathToBuiltRelativePath(effectData.vertexShaderPath, "shaders", buildVertexShaderPath, &errorMessage);
			ConvertSourceRelativePathToBuiltRelativePath(effectData.fragmentShaderPath, "shaders", buildFragmentShaderPath, &errorMessage);

			// append data
			const std::string prefixPath = "data/";
			{
				buildVertexShaderPath = prefixPath + buildVertexShaderPath;
				buildFragmentShaderPath = prefixPath + buildFragmentShaderPath;
			}

			uint8_t vertexPathLen = static_cast<uint8_t>(buildVertexShaderPath.length());
			uint8_t fragmentPathLen = static_cast<uint8_t>(buildFragmentShaderPath.length());

			// Write length of vertex Path
			outfile.write(reinterpret_cast<char*>(&vertexPathLen), sizeof(uint8_t));

			// Write vertex Path with the null character
			outfile.write(reinterpret_cast<const char*>(buildVertexShaderPath.c_str()), vertexPathLen + 1);

			// Write fragment Path with the null character
			outfile.write(reinterpret_cast<const char*>(buildFragmentShaderPath.c_str()), fragmentPathLen + 1);

			outfile.close();
		}

		// Clean effect Data
		{
			if (!CleanLuaEffectData(effectData)) {
				wereThereErrors = true;
				std::ostringstream errorMessage;
				errorMessage << "Failed to Clean Lua Effect Data : ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				goto OnExit;
			}
		}
	}
OnExit:	
	return !wereThereErrors;
}