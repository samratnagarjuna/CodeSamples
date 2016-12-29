/*
	This file contains declaration of types used accross graphics
*/

#ifndef EAE6320_GRAPHICS_INCLUDES_H
#define EAE6320_GRAPHICS_INCLUDES_H

// Header Files
//=============

#include <stdint.h>

#include "Math/cMatrix_transformation.h"

namespace
{
	// This struct determines the layout of the geometric data that the CPU will send to the GPU
	struct sVertex
	{
		// POSITION
		// 3 floats == 12 bytes
		// Offset = 0
		float x, y, z;

		// COLOR
		// 4 uint8_t == 4 bytes
		// Offset = 12
		uint8_t r, g, b, a;

		// UV
		// 2 floats == 8 bytes
		// Offset = 16
		float u, v;
	};

	// This struct stores the information of a triangle vertices in the form of indices in a given mesh
	// This struct is currently only being used in
	struct sTriangleIndices
	{
		uint16_t indices[3];
	};

	// This struct determines the layout of the constant frame data that the CPU will send to the GPU
	struct sConstantBufferFrameData
	{
		eae6320::Math::cMatrix_transformation g_transform_worldToCamera;
		eae6320::Math::cMatrix_transformation g_transform_cameraToScreen;
		union
		{
			float g_elapsedSecondCount_total;
			float register0[4]; // padding that GPU expects
		};
	};

	// This struct determines the layout of the constant draw call data that the CPU will send to GPU
	struct sConstantBufferDrawData
	{
		eae6320::Math::cMatrix_transformation g_transform_localToWorld;
	};

	// This struct determines the layout of the material data that the CPU will send to GPU
	struct sConstantBufferMaterialData
	{
		struct
		{
			float r, g, b, a;
		} g_color;

		sConstantBufferMaterialData() { g_color.r = 1.0f, g_color.g = 1.0f, g_color.b = 1.0f, g_color.a = 1.0f; }
	};
}

#endif // !EAE6320_GRAPHICS_INCLUDES_H