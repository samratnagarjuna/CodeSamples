/*
This file contains the class to create constant buffer
*/

#ifndef EAE6320_C_CONSTANT_BUFFER_H
#define EAE6320_C_CONSTANT_BUFFER_H

// Header Files
//=============

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
		class cConstantBuffer
		{
		public:
			enum ConstantBufferType
			{
				CONSTANT_FRAME_BUFFER,
				CONSTANT_DRAW_BUFFER,
				CONSTANT_MATERIAL_BUFFER
			};

			bool Initialize(ConstantBufferType i_bufferType, size_t i_bufferSize, void* i_pBufferData = NULL);
			void Update(void* i_pConstantBufferData);
			bool Bind();
			bool CleanUp();

		private:
#if defined( EAE6320_PLATFORM_WINDOWS )
	#if defined( EAE6320_PLATFORM_D3D )
			ID3D11Buffer * m_pConstantBuffer = NULL;
	#elif defined (EAE6320_PLATFORM_GL)
			GLuint 	m_constantBufferId = 0;
	#endif
#endif
			// This struct holds mapping of buffer in shader code
			struct ConstantBufferRegisterID
			{
				unsigned int frameRegisterId;
				unsigned int drawRegisterId;
				unsigned int materialRegisterId;
			};

			static const ConstantBufferRegisterID s_registerIds;

			ConstantBufferType	m_bufferType;
			size_t				m_bufferSize;
			void*				m_pBufferData;

			bool CreateConstantBuffer();
			bool BindConstantBuffer();
			unsigned int GetBufferRegisterID();
		
		};
	}
}

#endif // !EAE6320_C_CONSTANT_BUFFER_H

