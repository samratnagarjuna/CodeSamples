// Header Files
//=============

#include "cMaterialBuilder.h"

#include <sstream>
#include <fstream>
#include <cstring>
#include "../AssetBuildLibrary/UtilityFunctions.h"
#include "LoadLuaMaterial.h"

// Inherited Implementation
//=========================

// Build
//======

bool eae6320::AssetBuild::cMaterialBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	MaterialData materialData;
	{
		if (!LoadLuaMaterialFromFile(m_path_source, materialData))
		{
			wereThereErrors = true;
			std::ostringstream errorMessage;
			errorMessage << "Failed to load \"" << m_path_source << "\" using lua: ";
			eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			goto OnExit;
		}
	}


	// Write to file as binary data
	{
		std::string buildEffectPath;
		std::string buildTexturePath;

		std::string errorMessage;

		// Handle error checking here
		ConvertSourceRelativePathToBuiltRelativePath(materialData.effectPath, "effects", buildEffectPath, &errorMessage);
		ConvertSourceRelativePathToBuiltRelativePath(materialData.texturePath, "textures", buildTexturePath, &errorMessage);

		// append data
		const std::string prefixPath = "data/";
		{
			buildEffectPath = prefixPath + buildEffectPath;
			buildTexturePath = prefixPath + buildTexturePath;
		}

		std::ofstream outfile(m_path_target, std::ofstream::binary);

		uint8_t effectPathLen = static_cast<uint8_t>(buildEffectPath.length());

		// Write constant buffer struct first
		outfile.write(reinterpret_cast<char*>(&materialData.constantData), sizeof(sConstantBufferMaterialData));

		// Write length of effect Path
		outfile.write(reinterpret_cast<char*>(&effectPathLen), sizeof(uint8_t));

		// Write path to the effect file
		outfile.write(reinterpret_cast<const char*>(buildEffectPath.c_str()), buildEffectPath.length() + 1);

		// Write path to the texture file
		outfile.write(reinterpret_cast<const char*>(buildTexturePath.c_str()), buildTexturePath.length() + 1);

		outfile.close();
	}

	// Clean material Data
	{
		if (!CleanLuaMaterialData(materialData)) {
			wereThereErrors = true;
			std::ostringstream errorMessage;
			errorMessage << "Failed to Clean Lua Material Data : ";
			eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			goto OnExit;
		}
	}

OnExit:
	return !wereThereErrors;
}

