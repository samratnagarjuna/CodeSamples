#include "Math\NVector3.h"

#include <cmath>
#include <cassert>

namespace Illehc
{
	const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 :: Vector3(void) :
		m_x(0.0f),
		m_y(0.0f),
		m_z(0.0f)
	{

	}

	Vector3 :: Vector3(float i_x, float i_y, float i_z) :
		m_x(i_x),
		m_y(i_y),
		m_z(i_z)
	{

	}

	Vector3 & Vector3 :: operator += (const Vector3 & i_other)
	{
		m_x += i_other.m_x;
		m_y += i_other.m_y;
		m_z += i_other.m_z;

		return *this;
	}

	Vector3 & Vector3 :: operator -= (const Vector3 & i_other)
	{
		m_x -= i_other.m_x;
		m_y -= i_other.m_y;
		m_z -= i_other.m_z;

		return *this;
	}

	Vector3 & Vector3 :: operator *= (const Vector3 & i_other)
	{
		m_x *= i_other.m_x;
		m_y *= i_other.m_y;
		m_z *= i_other.m_z;

		return *this;
	}

	Vector3 & Vector3 :: operator /= (const Vector3 & i_other)
	{
		m_x /= i_other.m_x;
		m_y /= i_other.m_y;
		m_z /= i_other.m_z;

		return *this;
	}

	bool Vector3 :: operator == (const Vector3 & i_other)
	{
		return (fabs(m_x - i_other.m_x) < 0.01f && fabs(m_y - i_other.m_y) < 0.01f && fabs(m_z - i_other.m_z) < 0.01f);
	}

	bool Vector3 :: operator != (const Vector3 & i_other)
	{
		return !(*this == i_other);
	}

	Vector3 Vector3::Normalized() const
	{
		float length = Length();
		assert(!IsZero(length));

		if (IsZero(length))
			return Zero;
		else
		{
			float inv_length = 1.0f / length;

			return *this * inv_length;
		}
	}
} // namespace Illehc