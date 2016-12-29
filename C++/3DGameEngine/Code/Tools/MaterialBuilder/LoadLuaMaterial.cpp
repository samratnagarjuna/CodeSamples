// Header Files
//=============

#include "LoadLuaMaterial.h"

#include <cstdlib>
#include <sstream>
#include <string.h>
#include "../../External/Lua/Includes.h"
#include "../../Engine/Utility/LuaUtility.h"
#include "../AssetBuildLibrary/UtilityFunctions.h"

// Helper Function Declarations
//=============================

namespace
{
	bool LoadLuaFile(const char * const i_path, MaterialData& o_materialData);

	bool LoadTableValues(lua_State& io_luaState, MaterialData& o_materialData);

	bool LoadTableValues_EffectPath(lua_State& io_luaState, MaterialData& o_materialData);

	bool LoadTableValues_Parameters(lua_State& io_luaState, MaterialData& o_materialData);
	bool LoadTableValues_Parameters_Color(lua_State& io_luaState, MaterialData& o_materialData);
	bool LoadTableValues_Parameters_TexturePath(lua_State& io_luaState, MaterialData& o_materialData);
}

bool eae6320::AssetBuild::LoadLuaMaterialFromFile(const char* const i_path, MaterialData& o_materialData)
{
	bool wereThereErrors = false;

	if (!LoadLuaFile(i_path, o_materialData))
	{
		wereThereErrors = true;
	}

	return !wereThereErrors;
}

bool eae6320::AssetBuild::CleanLuaMaterialData(MaterialData& io_materialData)
{
	if (io_materialData.effectPath != NULL) free(io_materialData.effectPath);
	if (io_materialData.texturePath != NULL) free(io_materialData.texturePath);

	return true;
}

// Helper Function Definitions
//============================

namespace
{
	bool LoadLuaFile(const char * const i_path, MaterialData& o_materialData)
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
		if (!LoadTableValues(*luaState, o_materialData))
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
			if (!(lua_gettop(luaState) == 0))
			{
				wereThereErrors = true;
				std::ostringstream errorMessage;
				errorMessage << "Stack not empty after processing: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
			}

			lua_close(luaState);
			luaState = NULL;
		}

		return !wereThereErrors;
	}

	bool LoadTableValues(lua_State& io_luaState, MaterialData& o_MaterialData)
	{
		bool wereThereErrors = false;

		if (!LoadTableValues_EffectPath(io_luaState, o_MaterialData))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		if (!LoadTableValues_Parameters(io_luaState, o_MaterialData))
		{
			wereThereErrors = true;
			goto OnExit;
		}

	OnExit:
		return !wereThereErrors;
	}

	bool LoadTableValues_EffectPath(lua_State& io_luaState, MaterialData& o_materialData)
	{
		bool wereThereErrors = false;

		// Right now the table is at index -1
		const char* const key = "effectPath";
		lua_pushstring(&io_luaState, key);
		{
			// Right now the table is at index -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_isstring(&io_luaState, -1))
			{
				const char* effectPath = lua_tostring(&io_luaState, -1);
				o_materialData.effectPath = _strdup(effectPath);
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}
	OnExit:
		// Pop key value
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_Parameters(lua_State& io_luaState, MaterialData& o_materialData)
	{
		bool wereThereErrors = false;

		// Right now the table is at index -1
		const char* const key = "parameters";
		lua_pushstring(&io_luaState, key);
		{
			// Right now the table is at index -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				// TODO: assign default parameters here
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the effect table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			// If we reach this code we know that a value exists
			// but we don't know if it's the right type or not
			if (lua_istable(&io_luaState, -1))
			{
				// Load all path data
				if (!LoadTableValues_Parameters_Color(io_luaState, o_materialData))
				{
					wereThereErrors = true;
					goto OnExit;
				}

				if (!LoadTableValues_Parameters_TexturePath(io_luaState, o_materialData))
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
		// Pop the parameters table
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_Parameters_Color(lua_State& io_luaState, MaterialData& o_materialData)
	{
		bool wereThereErrors = false;

		//Right now the parameters table is at index -1
		const char* const key = "color";
		lua_pushstring(&io_luaState, key);
		{
			// Right now the parameters table is at index -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << key << "\" was found in the parameters table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_istable(&io_luaState, -1))
			{
				float color[4] = { 1.0f };

				// We Know that the color table is at -1
				if (!ReadFloatArray(io_luaState, -1, color, 4))
				{
					wereThereErrors = true;
					std::ostringstream errorMessage;
					errorMessage << "Error in loading \"" << key << "\" from color table: ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					goto OnExit;
				}

				o_materialData.constantData.g_color.r = static_cast<float>(color[0]);
				o_materialData.constantData.g_color.g = static_cast<float>(color[1]);
				o_materialData.constantData.g_color.b = static_cast<float>(color[2]);
				o_materialData.constantData.g_color.a = static_cast<float>(color[3]);
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
		// Pop color key value
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_Parameters_TexturePath(lua_State& io_luaState, MaterialData& o_materialData)
	{
		bool wereThereErrors = false;

		//Right now the parameters table is at index -1
		const char* const key = "texturePath";
		lua_pushstring(&io_luaState, key);
		{
			// Right now the parameters table is at index -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				const char* const defaultTexturePath = "Images/default.tga";
				o_materialData.texturePath = _strdup(defaultTexturePath);
			}
			else if (lua_isstring(&io_luaState, -1))
			{
				const char* texturePath = lua_tostring(&io_luaState, -1);
				o_materialData.texturePath = _strdup(texturePath);
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << key << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:
		// Pop parameters key value
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;;
	}
}