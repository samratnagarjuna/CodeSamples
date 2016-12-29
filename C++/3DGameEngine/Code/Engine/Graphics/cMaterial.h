/*
This file defines the class for Material
*/

#ifndef EAE6320_CMATERIAL_H
#define EAE6320_CMATERIAL_H

//  Header Files
//==============

#include "cEffect.h"
#include "cTexture.h"
#include "cConstantBuffer.h"

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cMaterial
		{
		private:
			cEffect			m_effect;
			cTexture		m_texture;
			cConstantBuffer m_constantbuffer;
		public:
			
			bool Load(const char* const i_path);
			bool CleanUp();
			void Bind();
		};
	}
}

#endif // !EAE6320_CMATERIAL_H
