/*
This file defines the class for Effect
*/

#ifndef EAE6320_CEFFECT_H
#define EAE6320_CEFFECT_H

// Header Files
//=============

#if defined( EAE6320_PLATFORM_WINDOWS )
	#if defined( EAE6320_PLATFORM_D3D )
		#include <D3D11.h>
	#elif defined (EAE6320_PLATFORM_GL)
		#include "../Graphics/OpenGL/Includes.h"
	#endif
#endif

#include "cRenderState.h"

// Class Declarations
//===================

namespace eae6320
{
	namespace Graphics
	{
		class cEffect
		{
		private:
#if defined( EAE6320_PLATFORM_WINDOWS )

	#if defined (EAE6320_PLATFORM_D3D)
			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader*	m_fragmentShader;
			ID3D11InputLayout*	m_vertexLayout;
	#elif defined (EAE6320_PLATFORM_GL)
			GLuint m_programId;
	#endif
			cRenderState m_renderState;
#endif

		public:
			cEffect();
			bool Load(const char* i_effectPath);
			bool Load(const char* i_vertexShaderPath, const char* i_fragmentShaderPath);
			void Bind();
			bool CleanUp();
		};
	}
}

#endif // !EAE6320_CEFFECT_H