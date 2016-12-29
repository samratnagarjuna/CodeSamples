// Header Files
//=============

#include "cShaderBuilder.h"

#include <sstream>
#include "../AssetBuildLibrary/UtilityFunctions.h"

// Inherited Implementation
//=========================

// Build
//------

bool eae6320::AssetBuild::cShaderBuilder::Build( const std::vector<std::string>& i_arguments )
{
	// Decide which kind of shader program to compile
	Graphics::ShaderTypes shaderType = Graphics::ShaderTypes::UnKnown;
	{
		if ( i_arguments.size() >= 1 )
		{
			const std::string& argument = i_arguments[0];
			if ( argument == "vertex" )
			{
				shaderType = Graphics::ShaderTypes::Vertex;
			}
			else if ( argument == "fragment" )
			{
				shaderType = Graphics::ShaderTypes::Fragment;
			}
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "\"" << argument << "\" is not a valid shader program type";
				OutputErrorMessage( errorMessage.str().c_str(), m_path_source );
				return false;
			}
		}
		else
		{
			OutputErrorMessage(
				"A Shader must be built with an argument defining which type of shader program (e.g. \"vertex\" or \"fragment\") to compile",
				m_path_source );
			return false;
		}
	}

	return Build( shaderType, i_arguments );
}
