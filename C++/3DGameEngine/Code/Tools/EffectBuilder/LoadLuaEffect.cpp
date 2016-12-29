// Header Files
//=============

#include "LoadLuaEffect.h"

#include <cstdlib>
#include <sstream>
#include <string.h>
#include "../../External/Lua/Includes.h"
#include "../AssetBuildLibrary/UtilityFunctions.h"
#include "../../Engine/Graphics/cRenderState.h"

// Helper Function Declarations
//=============================

namespace
{
	bool LoadLuaFile(const char * const i_path, EffectData& o_effectData);

	bool LoadTableValues(lua_State& io_luaState, EffectData& o_effectData);

	bool LoadTableValues_ShaderPaths(lua_State& io_luaState, EffectData& o_effectData);
	bool LoadTableValues_ShaderPaths_data(lua_State& io_luaState, EffectData& o_effectData);

	bool LoadTableValues_RenderStateBits(lua_State& io_luaState, EffectData& o_effectData);
	bool LoadTableValues_RenderStateBits_data(lua_State& io_luaState, EffectData& o_effectData);
}

bool eae6320::AssetBuild::LoadLuaEffectFromFile(const char* const i_path, EffectData& o_effectData)
{
	bool wereThereErrors = false;

	if (!LoadLuaFile(i_path, o_effectData))
	{
		wereThereErrors = true;
	}

	return !wereThereErrors;
}

bool eae6320::AssetBuild::CleanLuaEffectData(EffectData& io_effectData)
{
	if (io_effectData.vertexShaderPath != NULL) free(io_effectData.vertexShaderPath);
	if (io_effectData.fragmentShaderPath != NULL) free(io_effectData.fragmentShaderPath);

	return true;
}

// Helper Function Definitions
//============================

namespace
{
	bool LoadLuaFile(const char * const i_path, EffectData& o_effectData)
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
		// Load Table values
		if (!LoadTableValues(*luaState, o_effectData))
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

	bool LoadTableValues(lua_State& io_luaState, EffectData& o_effectData)
	{
		bool wereThereErrors = false;

		if (!LoadTableValues_ShaderPaths(io_luaState, o_effectData))
		{
			wereThereErrors = true;
			goto OnExit;
		}

		if (!LoadTableValues_RenderStateBits(io_luaState, o_effectData))
		{
			wereThereErrors = true;
			goto OnExit;
		}

OnExit:
		return !wereThereErrors;
	}

	bool LoadTableValues_ShaderPaths(lua_State& io_luaState, EffectData& o_effectData)
	{
		bool wereThereErrors = false;

		// Right now the effect table is at -1
		const char* const key = "shaderPaths";
		lua_pushstring(&io_luaState, key);
		{
			// Right now the effect table is at -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
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
				if (!LoadTableValues_ShaderPaths_data(io_luaState, o_effectData))
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
		// Pop the shaderPaths table
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_ShaderPaths_data(lua_State& io_luaState, EffectData& o_effectData)
	{
		bool wereThereErrors = false;
		// We know that the shaderPaths table exits at -1

		const char* const vertexKey = "vertex";
		lua_pushstring(&io_luaState, vertexKey);
		{
			// Right now the shaderPaths table is at index -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << vertexKey << "\" was found in the shaderPaths table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_isstring(&io_luaState, -1))
			{
				const char* vertexShaderPath = lua_tostring(&io_luaState, -1);
				o_effectData.vertexShaderPath = _strdup(vertexShaderPath);
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << vertexKey << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			// Pop vertex value
			lua_pop(&io_luaState, 1);
		}

		const char* const fragmentKey = "fragment";
		lua_pushstring(&io_luaState, fragmentKey);
		{
			// Right now the shaderPaths table is at index -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);

			if (lua_isnil(&io_luaState, -1))
			{
				std::ostringstream errorMessage;
				errorMessage << "No value for \"" << fragmentKey << "\" was found in the shaderPaths table: ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}

			if (lua_isstring(&io_luaState, -1))
			{
				const char* fragmentShaderPath = lua_tostring(&io_luaState, -1);
				o_effectData.fragmentShaderPath = _strdup(fragmentShaderPath);
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "The value for \"" << fragmentKey << "\" must be a string "
					"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
				eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
				wereThereErrors = true;
				goto OnExit;
			}
		}

	OnExit:
		// Pop vertex/fragment key value
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}

	bool LoadTableValues_RenderStateBits(lua_State& io_luaState, EffectData& o_effectData)
	{
		bool wereThereErrors = false;
		
		o_effectData.renderStateBits = 0;
		// Assign default values
		{
			eae6320::Graphics::RenderStates::DisableAlphaTransparency(o_effectData.renderStateBits);
			eae6320::Graphics::RenderStates::EnableDepthBuffering(o_effectData.renderStateBits);
			eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides(o_effectData.renderStateBits);
		}

		// Right now the effect table is at -1
		const char* const key = "renderStates";
		lua_pushstring(&io_luaState, key);
		{
			// Right now the effect table is at -2
			const int currentIndexOfTable = -2;
			lua_gettable(&io_luaState, currentIndexOfTable);
			if (lua_isnil(&io_luaState, -1))
			{
				// Go with default values
				goto OnExit;
			}

			// If we reach this code we know that a value exists
			// but we don't know if it's the right type or not
			if (lua_istable(&io_luaState, -1))
			{
				// Load all path data
				if (!LoadTableValues_RenderStateBits_data(io_luaState, o_effectData))
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
		// Pop the renderStates table
		lua_pop(&io_luaState, 1);	
		return !wereThereErrors;
	}

	bool LoadTableValues_RenderStateBits_data(lua_State& io_luaState, EffectData& o_effectData)
	{
		bool wereThereErrors = false;

		// Alpha Transparency
		{
			// Right now the renderStates table is at -1
			const char* const key = "alphaTransparency";
			lua_pushstring(&io_luaState, key);
			{
				// Right now the renderStates table is at -2
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);

				if (lua_isboolean(&io_luaState, -1))
				{
					int alphaTransparency = lua_toboolean(&io_luaState, -1);
					if (alphaTransparency) eae6320::Graphics::RenderStates::EnableAlphaTransparency(o_effectData.renderStateBits);
				}
				else
				{
					std::ostringstream errorMessage;
					errorMessage << "The value for \"" << key << "\" must be a boolean "
						"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}
			}

			// Pop alphaTransparency value
			lua_pop(&io_luaState, 1);
		}
		
		// Depth Buffering
		{
			// Right now the renderStates table is at -1
			const char* const key = "depthBuffering";
			lua_pushstring(&io_luaState, key);
			{
				// Right now the renderStates table is at -2
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);

				if (lua_isboolean(&io_luaState, -1))
				{
					int depthBuffering = lua_toboolean(&io_luaState, -1);
					if (!depthBuffering) eae6320::Graphics::RenderStates::DisableDepthBuffering(o_effectData.renderStateBits);
				}
				else
				{
					std::ostringstream errorMessage;
					errorMessage << "The value for \"" << key << "\" must be a boolean "
						"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}
			}

			// Pop depthBuffering value
			lua_pop(&io_luaState, 1);
		}

		// Draw Both Triangle Sides
		{
			// Right now the renderStates table is at -1
			const char* const key = "drawBothTriangleSides";
			lua_pushstring(&io_luaState, key);
			{
				// Right now the renderStates table is at -2
				const int currentIndexOfTable = -2;
				lua_gettable(&io_luaState, currentIndexOfTable);

				if (lua_isboolean(&io_luaState, -1))
				{
					int drawBothTriangleSides = lua_toboolean(&io_luaState, -1);
					if (drawBothTriangleSides) eae6320::Graphics::RenderStates::EnableDrawingBothTriangleSides(o_effectData.renderStateBits);
				}
				else
				{
					std::ostringstream errorMessage;
					errorMessage << "The value for \"" << key << "\" must be a boolean "
						"(instead of a " << luaL_typename(&io_luaState, -1) << "): ";
					eae6320::AssetBuild::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
					wereThereErrors = true;
					goto OnExit;
				}
			}
		}

	OnExit:

		// pop key/value
		lua_pop(&io_luaState, 1);
		return !wereThereErrors;
	}
}

