/*
This file contains the function declarations for mesh
*/

#ifndef EAE6320_MESH_RENDERER_H
#define EAE6320_MESH_RENDERER_H

// Header Files
//=============
#include <stdint.h>
#include "../Graphics/Includes.h"

#if defined( EAE6320_PLATFORM_WINDOWS )

	#if defined( EAE6320_PLATFORM_D3D )
		#include <D3D11.h>
	#elif defined (EAE6320_PLATFORM_GL)
		#include "../Graphics/OpenGL/Includes.h"
	#endif
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class MeshRenderer
		{
		private:

#if defined( EAE6320_PLATFORM_WINDOWS )

	#if defined( EAE6320_PLATFORM_D3D )
			ID3D11Buffer* m_vertexBuffer = NULL;
			ID3D11Buffer* m_vertexIndexBuffer = NULL;
	#elif defined (EAE6320_PLATFORM_GL)
			// The vertex buffer holds the data for each vertex
			GLuint m_vertexArrayId = 0;
		#ifdef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
			// OpenGL debuggers don't seem to support freeing the vertex buffer
			// and letting the vertex array object hold a reference to it,
			// and so if debug info is enabled an explicit reference is held
			GLuint m_vertexBufferId = 0;
			GLuint m_vertexIndexBufferId = 0;
		#endif
	#endif
#endif
			uint16_t m_noOfVertices = 0;
			uint16_t m_noOfIndices = 0;

			bool Initialize(uint16_t i_noOfVertices, sVertex* i_pVertices, uint16_t i_noOfIndices, uint16_t* i_pIndices);

		public:
			// Initialization / Clean Up
			//--------------------------

			
			bool Load(const char * const i_path);
			void Render();
			bool CleanUp();
		};
	}
}

#endif // !EAE6320_MESH_RENDERER_H
