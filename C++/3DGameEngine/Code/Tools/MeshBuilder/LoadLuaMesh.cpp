// Header Files
//=============

#include "LoadLuaMesh.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "../../Engine/Asserts/Asserts.h"
#include "../../External/Lua/Includes.h"
#include "../../Engine/Graphics/Includes.h"
#include "../../Engine/Utility/LuaUtility.h"
#include "../AssetBuildLibrary/UtilityFunctions.h"

// Helper Function Declarations
//=============================

namespace
{
	bool LoadTableValues(lua_State& io_luaState, MeshData& o_meshData);

	bool LoadTableValues_vertices(lua_State& io_luaState, MeshData& o_meshData);
	bool LoadTableValues_vertices_data(lua_State& io_luaState, MeshData& o_meshData);
	bool LoadTableValues_vertices_data_vertex(lua_State& io_luaState, sVertex& o_vertex);
	bool LoadTableValues_vertices_data_vertex_position(lua_State& io_luaState,sVertex& o_vertex);
	bool LoadTableValues_vertices_data_vertex_color(lua_State& io_luaState, sVertex& o_vertex);
	bool LoadTableValues_vertices_data_vertex_uv(lua_State& io_luaState, sVertex& o_vertex);

	bool LoadTableValues_indices(lua_State& io_luaState, MeshData& o_meshData);
	bool LoadTableValues_indices_data(lua_State& io_luaState, MeshData& o_meshData);
	bool LoadTableValues_indices_data_triangle(lua_State& io_luaState, sTriangleIndices& o_triangle);

	bool LoadLuaFile(const char * const i_path, MeshData& o_meshData);
}

bool eae6320::AssetBuild::LoadLuaMeshFromFile(const char * const i_path, MeshData& o_meshData)
{
	if (!LoadLuaFile(i_path, o_meshData))
	{
		return false;
	}

	return true;
}

bool eae6320::AssetBuild::CleanLuaMeshData(MeshData& io_meshData)
{
	if (io_meshData.indices != NULL) free(io_meshData.indices);
	if (io_meshData.vertices != NULL) free(io_meshData.vertices);

	return true;
}

// Helper Function Definitions
//============================

namespace
{
	bool LoadLuaFile(const char * const i_path, MeshData& o_meshData)
	{
		bool wereThereErrors = false;

		// Create a new Lua state
		lua_State* luaState = NULL;
		{
			luaState = luaL_newstate();
			if (!luaState)
			{
				wereThereErrors = true;
				std::ostringstream errorMessage;
				errorMessage << "Failed to create a new Lua state :";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				goto OnExit;
			}
		}

		// Load the asset file as a lua chunk
		{
			const int luaResult = luaL_loadfile(luaState, i_path);
			if (luaResult != LUA_OK)
			{
				wereThereErrors = true;
				std::ostringstream errorMessage;
				errorMessage << lua_tostring(luaState, -1) << " :";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// Execute the lua chunk, which should load the asset
		// into a table at the top of the stack
		{
			// Right now, the chunk is at index -1
			// (that's what luaL_loadfile() does)
			const int argumentCount = 0;
			const int returnValueCount = 1; // We expect an asset table to be returned
			const int noErrorHandler = 0;
			const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noErrorHandler);
			if (luaResult == LUA_OK)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					wereThereErrors = true;
					std::ostringstream errorMessage;
					errorMessage << "Asset files must return a table (instead of a "
						<< luaL_typename(luaState, -1) << "): ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::ostringstream errorMessage;
				errorMessage << lua_tostring(luaState, -1) << " :";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				// Pop the error message
				lua_pop(luaState, 1);
				goto OnExit;
			}
		}

		// If this code is reached the asset file was loaded successfully,
		// and its table is now at index -1

		// Load Table values
		if (!LoadTableValues(*luaState, o_meshData))
		{
			wereThereErrors = true;
		}

		// Pop the table
		lua_pop(luaState, 1);

	OnExit:

		if (luaState)
		{
			// If I haven't made any mistakes
			// there shouldn't be anything on the stack
			// regardless of any errors
			EAE6320_ASSERT(lua_gettop(luaState) == 0);

			lua_close(luaState);
			luaState = NULL;
		}

		return !wereThereErrors;
	}

	bool LoadTableValues(lua_State& io_luaState, MeshData& o_meshData)
	{
		if (!LoadTableValues_vertices(io_luaState, o_meshData))
		{
			return false;
		}

		if (!LoadTableValues_indices(io_luaState, o_meshData))
		{
			return false;
		}

		return true;
	}

	bool LoadTableValues_vertices(lua_State& io_luaState, MeshData& o_meshData)
	{
		bool wereThereErrors = false;

		// Right now the asset table is at -1
		const char* const key = "vertices";
		lua_pushstring( &io_luaState, key);
		{
			// Right now the asset table is at -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			// If we reach this code we know that a value exists
			// but we don't know if it's the right type or not
			if (lua_istable(&io_luaState, -1))
			{
				// Load all vertex data
				if (!LoadTableValues_vertices_data(io_luaState, o_meshData))
				{
					wereThereErrors = true;
					goto OnExit;
				}
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:
		// Pop the verticies table
		lua_pop(&io_luaState, 1);
		
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_data(lua_State& io_luaState, MeshData& o_meshData)
	{
		bool wereThereErrors = false;

		// We know that that vertices table exist at -1
		const int noOfVertices = luaL_len( &io_luaState, -1);
		o_meshData.noOfVertices = noOfVertices;

		// Create space to load vertex data
		sVertex* vertexData = (sVertex*)malloc(noOfVertices * sizeof(sVertex));

		// Iterate on each vertex data
		for (int i = 1; i <= noOfVertices; i++)
		{
			lua_pushinteger(&io_luaState, i);
			// The vertex table is now at index -2
			{
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);

				if (lua_isnil(&io_luaState, -1))
				{
					std::ostringstream errorMessage;
					errorMessage << "No value for \"" << i << "\" was found in the vertices table: ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}

				if (lua_istable(&io_luaState, -1))
				{
					//Load i'th vertex data
					if (!LoadTableValues_vertices_data_vertex(io_luaState, vertexData[i - 1]))
					{
						wereThereErrors = true;
						goto OnExit;
					}
				}
				else
				{
					std::ostringstream errorMessage;
					errorMessage << "The value for \"" << i << "\" must be a table "
						"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}
			}
			//Pop the key value
			lua_pop(&io_luaState, 1);
		}

	OnExit:
		// Pop the current key value
		if (wereThereErrors) lua_pop(&io_luaState, 1);
		else o_meshData.vertices = vertexData;
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_data_vertex(lua_State& io_luaState, sVertex& o_vertex)
	{
		bool wereThereErrors = false;

		// We know that the vertex table is at -1
		const char* const position_key = "position";
		lua_pushstring(&io_luaState, position_key);
		{
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << position_key << "\" was found in the vertex table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_istable(&io_luaState, -1))
			{
				if (!LoadTableValues_vertices_data_vertex_position(io_luaState, o_vertex))
				{
					wereThereErrors = true;
					goto OnExit;
				}
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << position_key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

		}

		// Pop position table
		lua_pop(&io_luaState, 1);

		// We know that the vertex table is at -1
		const char* const color_key = "color";
		lua_pushstring(&io_luaState, color_key);
		{
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << color_key << "\" was found in the vertex table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_istable(&io_luaState, -1))
			{
				if (!LoadTableValues_vertices_data_vertex_color(io_luaState, o_vertex))
				{
					wereThereErrors = true;
					goto OnExit;
				}
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << color_key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

		// Pop color table
		lua_pop(&io_luaState, 1);

		// We know that the vertex table is at -1
		const char* const uv_key = "texture_coordinates";
		lua_pushstring(&io_luaState, uv_key);
		{
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << uv_key << "\" was found in the vertex table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_istable(&io_luaState, -1))
			{
				if (!LoadTableValues_vertices_data_vertex_uv(io_luaState, o_vertex))
				{
					wereThereErrors = true;
					goto OnExit;
				}
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << color_key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:
		// Pop position or color or uv table
		lua_pop(&io_luaState, 1);

		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_data_vertex_position(lua_State& io_luaState, sVertex& o_vertex)
	{
		bool wereThereErrors = false;
		float position[3] = { 0.0f };
		// We know that the position table is at -1
		if (!ReadFloatArray(io_luaState, -1, position, 3))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		o_vertex.x = position[0];
		o_vertex.y = position[1];
		o_vertex.z = position[2];
		
	OnExit:
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_data_vertex_color(lua_State& io_luaState, sVertex& o_vertex)
	{
		bool wereThereErrors = false;
		float color[4] = { 0.0f };
		// We know that the position table is at -1
		if (!ReadFloatArray(io_luaState, -1, color, 4))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		o_vertex.r = static_cast<uint8_t>(color[0] * 255.0f);
		o_vertex.g = static_cast<uint8_t>(color[1] * 255.0f);
		o_vertex.b = static_cast<uint8_t>(color[2] * 255.0f);
		o_vertex.a = static_cast<uint8_t>(color[3] * 255.0f);

	OnExit:
		return !wereThereErrors;
	}

	bool LoadTableValues_vertices_data_vertex_uv(lua_State& io_luaState, sVertex& o_vertex)
	{
		bool wereThereErrors = false;
		float uv[2] = { 0.0f };
		// We Know that the texture_coordinate table is at -1
		if (!ReadFloatArray(io_luaState, -1, uv, 2))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		o_vertex.u = uv[0];
#if defined( EAE6320_PLATFORM_D3D )
		o_vertex.v = 1 - uv[1];
#elif defined( EAE6320_PLATFORM_GL )
		o_vertex.v = uv[1];
#endif

	OnExit:
		return !wereThereErrors;
	}

	bool LoadTableValues_indices(lua_State& io_luaState, MeshData& o_meshData)
	{
		bool wereThereErrors = false; 

		//Right now the asset table is at index -1
		const char* const key = "indices";
		lua_pushstring(&io_luaState, key);
		{
			const int currentIndexOfTable = -2;
			lua_gettable( &io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the asset table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_istable(&io_luaState, -1))
			{
				// Load all vertex data
				if (!LoadTableValues_indices_data(io_luaState, o_meshData))
				{
					wereThereErrors = true;
					goto OnExit;
				}
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a table "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:

		// Pop indices table
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;

	}

	bool LoadTableValues_indices_data(lua_State& io_luaState, MeshData& o_meshData)
	{
		bool wereThereErrors = false;

		// We know that that indices table exist at -1
		const int noOfTriangles = luaL_len(&io_luaState, -1);
		const int noOfVerticesPerTriangle = 3;
		o_meshData.noOfIndices = noOfTriangles * noOfVerticesPerTriangle;
		uint16_t* indices = (uint16_t*)malloc(o_meshData.noOfIndices * sizeof(uint16_t));

		for (int i = 1; i <= noOfTriangles; i++)
		{
			lua_pushinteger(&io_luaState, i);
			{
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);

				if (lua_isnil(&io_luaState, -1))
				{
					std::ostringstream errorMessage;
					errorMessage << "No value for \"" << i << "\" was found in the indices table: ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}

				if (lua_istable(&io_luaState, -1))
				{
					//Load i'th triangle data
					sTriangleIndices triangle;
					if (!LoadTableValues_indices_data_triangle(io_luaState, triangle))
					{
						wereThereErrors = true;
						goto OnExit;
					}
					else
					{
						for (int j = 0; j < noOfVerticesPerTriangle; j++)
						{
							indices[(i - 1) * noOfVerticesPerTriangle + j] = triangle.indices[j];
						}
					}
				}
				else
				{
					std::ostringstream errorMessage;
					errorMessage << "The value for \"" << i << "\" must be a table "
						"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}
			}
			// Pop Key value table
			lua_pop(&io_luaState, 1);
		}

	OnExit:

		// Pop the current key value
		if (wereThereErrors) lua_pop(&io_luaState, 1);
		else o_meshData.indices = indices;
		return !wereThereErrors;
	}

	bool LoadTableValues_indices_data_triangle(lua_State& io_luaState, sTriangleIndices& o_triangle)
	{
		bool wereThereErrors = false;

		const unsigned int noOfVerticesPerTriangle = 3;
		int indices[noOfVerticesPerTriangle] = { 0 };
		// We know that the position table is at -1
		if (!ReadIntArray(io_luaState, -1, indices, noOfVerticesPerTriangle))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		for (int i = 0 ; i < noOfVerticesPerTriangle; i++)
			o_triangle.indices[i] = static_cast<uint16_t>(indices[i]);

	OnExit:

		return !wereThereErrors;
	}
}