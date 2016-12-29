/*
This file contains the interface for a 3D camera
*/

#ifndef EAE6320_GRAPHICS_CAMERA_H
#define EAE6320_GRAPHICS_CAMERA_H

// Header Files
//=============

#include "./Math/cVector.h"
#include "./Math/cQuaternion.h"
#include "./Math/cMatrix_transformation.h"

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class Camera
		{
		public:
			Camera(
				Math::cVector i_position, 
				Math::cQuaternion i_orientation, 
				float i_feildOfView, float i_near, float i_far, 
				float i_aspectRatio);

			void Move(Math::cVector i_position);
			void Translate(Math::cVector i_relPosition);
			void Rotate(Math::cQuaternion i_orientation);
			Math::cMatrix_transformation WorldToCamraTransform();
			Math::cMatrix_transformation CameraToScreenTransform();

		private:
			Math::cVector		m_position;
			Math::cQuaternion	m_orientation;

			float				m_feildOfView;
			float				m_near;
			float				m_far;

			float				m_aspectRatio;
		};
	}
}



#endif // !EAE6320_GRAPHICS_CAMERA_H

