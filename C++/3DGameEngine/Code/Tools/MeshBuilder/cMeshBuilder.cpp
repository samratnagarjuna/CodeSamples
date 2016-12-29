// Header Files
//=============

#include "cMeshBuilder.h"

#include <sstream>
#include <fstream>

#include "../AssetBuildLibrary/UtilityFunctions.h"
#include "../../Engine/Platform/Platform.h"
#include "LoadLuaMesh.h"


// Inherited Implementation
//=========================

// Build
//------

bool eae6320::AssetBuild::cMeshBuilder::Build( const std::vector<std::string>& )
{
	bool wereThereErrors = false;

	// Load Mesh data using Lua
	MeshData meshData;
	{
		if (!LoadLuaMeshFromFile(m_path_source, meshData)) {
			wereThereErrors = true;
			std::ostringstream errorMessage;
			errorMessage << "Failed to load \"" << m_path_source << "\" using lua: ";
			eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			goto OnExit;
		}
	}

	// Write to to file as binary data
	{
		std::ofstream outfile(m_path_target, std::ofstream::binary);

		// Write no of vertices
		outfile.write(reinterpret_cast<char*>(&(meshData.noOfVertices)), sizeof(uint16_t));

		// Write no of indices
		outfile.write(reinterpret_cast<char*>(&(meshData.noOfIndices)), sizeof(uint16_t));

		// Write vertices
		outfile.write(reinterpret_cast<char*>(meshData.vertices), sizeof(sVertex) * meshData.noOfVertices);

		// Write indices
		outfile.write(reinterpret_cast<char*>(meshData.indices), sizeof(uint16_t) * meshData.noOfIndices);

		outfile.close();
	}

	// Clean Mesh Data
	{
		if (!CleanLuaMeshData(meshData)) {
			wereThereErrors = true;
			std::ostringstream errorMessage;
			errorMessage << "Failed to Clean Lua Mesh Data using lua: ";
			eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			goto OnExit;
		}		
	}

OnExit:
	return !wereThereErrors;
}
