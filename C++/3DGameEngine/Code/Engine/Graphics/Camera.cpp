// Header Files
//=============

#include "Camera.h"

// Constructor
eae6320::Graphics::Camera::Camera(
	Math::cVector i_position,
	Math::cQuaternion i_orientation,
	float i_feildOfView, float i_near, float i_far,
	float i_aspectRatio) :
	m_position(i_position),
	m_orientation(i_orientation),
	m_feildOfView(i_feildOfView), m_near(i_near), m_far(i_far),
	m_aspectRatio(i_aspectRatio)
{

}

void eae6320::Graphics::Camera::Move(Math::cVector i_position)
{
	m_position = i_position;
}

void eae6320::Graphics::Camera::Translate(Math::cVector i_relPosition)
{
	m_position += i_relPosition;
}

void eae6320::Graphics::Camera::Rotate(Math::cQuaternion i_orientation)
{
	m_orientation = i_orientation;
}

eae6320::Math::cMatrix_transformation eae6320::Graphics::Camera::WorldToCamraTransform()
{
	return Math::cMatrix_transformation::CreateWorldToCameraTransform(m_orientation, m_position);
}

eae6320::Math::cMatrix_transformation eae6320::Graphics::Camera::CameraToScreenTransform()
{
	return Math::cMatrix_transformation::CreateCameraToScreenTransform_perspectiveProjection(
		m_feildOfView, m_aspectRatio, m_near, m_far);
}

