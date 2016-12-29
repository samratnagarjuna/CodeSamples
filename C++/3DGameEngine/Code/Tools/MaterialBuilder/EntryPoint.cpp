/*
The main() function is where the program starts execution
*/

// Header Files
//=============

#include "cMaterialBuilder.h"

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	return eae6320::AssetBuild::Build<eae6320::AssetBuild::cMaterialBuilder>(i_arguments, i_argumentCount);
}