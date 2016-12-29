/*
	This file loads a lua mesh file from table
*/

#ifndef EAE6320_CMESHBUILDER_LOAD_LUA_MESH_H
#define EAE6320_CMESHBUILDER_LOAD_LUA_MESH_H

// Header Files
//=============

#include <stdint.h>
#include "../../Engine/Graphics/Includes.h"

struct MeshData
{
	uint16_t			noOfVertices;
	sVertex*			vertices;

	uint16_t			noOfIndices;
	uint16_t*			indices;
};

// Interface
//==========
namespace eae6320
{
	namespace AssetBuild
	{
		// This functions allocates memory to the mesh data but does not clean
		// This user is responsible for cleaning up the data after use
		// TODO: Can we write another function which cleans up the data
		// and ask the user to call this function to clean
		bool LoadLuaMeshFromFile(const char * const i_path, MeshData& o_meshData);
		bool CleanLuaMeshData(MeshData& io_meshData);
	}
}


#endif // !EAE6320_CMESHBUILDER_LOAD_LUA_MESH_H