#include "NVector4.h"
#pragma once

#ifndef __ENGINE__NVECTOR4__INL__H__
#define __ENGINE__NVECTOR4__INL__H__

namespace Illehc
{
	inline Vector4::Vector4(void) :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f),
		m_w(0.0f)
	{

	}

	inline Vector4::Vector4(float i_x, float i_y, float i_z, float i_w) :
		m_x(i_x),
		m_y(i_y),
		m_z(i_z),
		m_w(i_w)
	{

	}

	inline Vector4 :: Vector4(const Vector3 i_vector3, const float i_w) :
		m_x ( i_vector3.getX() ), m_y ( i_vector3.getY() ), m_z (i_vector3.getZ()), m_w( i_w )
	{
		
	}


	inline float Vector4::getX(void) const
	{
		return m_x;
	}

	inline float Vector4::getY(void) const
	{
		return m_y;
	}

	inline float Vector4::getZ(void) const
	{
		return m_z;
	}

	inline float Vector4::getW(void) const
	{
		return m_w;
	}

	inline void Vector4::setX(const float i_x)
	{
		if (!isNaN(i_x)) m_x = i_x;
	}

	inline void Vector4::setY(const float i_y)
	{
		if (!isNaN(i_y)) m_y = i_y;
	}

	inline void Vector4::setZ(const float i_z)
	{
		if (!isNaN(i_z)) m_z = i_z;
	}

	inline void Vector4::setW(const float i_w)
	{
		if (!isNaN(i_w)) m_w = i_w;
	}

	inline bool Vector4::isNaN(const float i_val)
	{
		volatile float v = i_val;
		return v != v;
	}

	inline Vector4 operator + (const Vector4 & i_lhs, const Vector4 & i_rhs)
	{
		return Vector4(i_lhs.getX() + i_rhs.getX(), i_lhs.getY() + i_rhs.getY(), i_lhs.getZ() + i_rhs.getZ(), i_lhs.getW() + i_rhs.getW());
	}

	inline Vector4 operator - (const Vector4 & i_lhs, const Vector4 & i_rhs)
	{
		return Vector4(i_lhs.getX() - i_rhs.getX(), i_lhs.getY() - i_rhs.getY(), i_lhs.getZ() - i_rhs.getZ(), i_lhs.getW() - i_rhs.getW());
	}

	inline Vector4 operator * (const Vector4 & i_lhs, const Vector4 & i_rhs)
	{
		return Vector4(i_lhs.getX() * i_rhs.getX(), i_lhs.getY() * i_rhs.getY(), i_lhs.getZ() * i_rhs.getZ(), i_lhs.getW() * i_rhs.getW());
	}

	inline Vector4 operator / (const Vector4 & i_lhs, const Vector4 & i_rhs)
	{
		return Vector4(i_lhs.getX() / i_rhs.getX(), i_lhs.getY() / i_rhs.getY(), i_lhs.getZ() / i_rhs.getZ(), i_lhs.getW() / i_rhs.getW());
	}

	inline Vector4 operator * (const Vector4 & i_lhs, const float i_scalar)
	{
		return Vector4(i_lhs.getX() * i_scalar, i_lhs.getY() * i_scalar, i_lhs.getZ() * i_scalar, i_lhs.getW() * i_scalar);
	}

	inline Vector4 operator * (const float i_scalar, const Vector4 & i_rhs)
	{
		return Vector4(i_rhs.getX() * i_scalar, i_rhs.getY() * i_scalar, i_rhs.getZ() * i_scalar, i_rhs.getW() * i_scalar);
	}
} // namespace Illehc

#endif // !__ENGINE__NVECTOR4__H__

