#pragma once

#ifndef __ENGINE__NVECTOR3__INL__H__
#define __ENGINE__NVECTOR3__INL__H__

#include <cmath>

namespace Illehc
{
	inline float Vector3 :: getX(void) const 
	{
		return m_x;
	}

	inline float Vector3 :: getY(void) const 
	{
		return m_y;
	}

	inline float Vector3 :: getZ(void) const 
	{
		return m_z;
	}

	inline void Vector3 :: setX(const float i_x) 
	{
		if (!isNaN(i_x)) m_x = i_x;
	}

	inline void Vector3 :: setY(const float i_y) 
	{
		if (!isNaN(i_y)) m_y = i_y;
	}

	inline void Vector3 :: setZ(const float i_z) 
	{
		if (!isNaN(i_z)) m_z = i_z;
	}

	inline bool Vector3 :: isNaN(const float i_val)
	{
		volatile float v = i_val;
		return v != v;
	}

	inline float Vector3 :: LengthSq() const
	{
		return (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
	}

	inline float Vector3::Length() const
	{
		return sqrt(LengthSq());
	}

	inline Vector3 operator + (const Vector3 & i_lhs, const Vector3 & i_rhs)
	{
		return Vector3(i_lhs.getX() + i_rhs.getX(), i_lhs.getY() + i_rhs.getY(), i_lhs.getZ() + i_rhs.getZ());
	}

	inline Vector3 operator - (const Vector3 & i_lhs, const Vector3 & i_rhs)
	{
		return Vector3(i_lhs.getX() - i_rhs.getX(), i_lhs.getY() - i_rhs.getY(), i_lhs.getZ() - i_rhs.getZ());
	}

	inline Vector3 operator * (const Vector3 & i_lhs, const Vector3 & i_rhs)
	{
		return Vector3(i_lhs.getX() * i_rhs.getX(), i_lhs.getY() * i_rhs.getY(), i_lhs.getZ() * i_rhs.getZ());
	}

	inline Vector3 operator / (const Vector3 & i_lhs, const Vector3 & i_rhs)
	{
		return Vector3(i_lhs.getX() / i_rhs.getX(), i_lhs.getY() / i_rhs.getY(), i_lhs.getZ() / i_rhs.getZ());
	}

	inline Vector3 operator * (const Vector3 & i_lhs, const float i_scalar)
	{
		return Vector3(i_lhs.getX() * i_scalar, i_lhs.getY() * i_scalar, i_lhs.getZ() * i_scalar);
	}

	inline Vector3 operator * (const float i_scalar, const Vector3 & i_rhs)
	{
		return Vector3(i_rhs.getX() * i_scalar, i_rhs.getY() * i_scalar, i_rhs.getZ() * i_scalar);
	}

	inline bool Vector3 :: AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff) const
	{
		return fabs(i_lhs - i_rhs) < i_maxDiff;
	}

	inline bool Vector3 :: IsZero(float i_val) const
	{
		return AreEqual_Eps(0.0f, i_val, .000000001f);
	}

	inline float dot(const Vector3 & i_lhs, const Vector3 & i_rhs)
	{
		return i_lhs.getX() * i_rhs.getX() + i_lhs.getY() * i_rhs.getY() + i_lhs.getZ() * i_rhs.getZ();
	}
} //namespace Illehc

#endif // !__ENGINE__NVECTOR3__INL__H__