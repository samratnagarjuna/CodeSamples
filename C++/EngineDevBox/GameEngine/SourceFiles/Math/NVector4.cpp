#include "Math\NVector4.h"

#include <cmath>

namespace Illehc
{
	const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	Vector4 & Vector4 :: operator += (const Vector4 & i_other)
	{
		m_x += i_other.m_x;
		m_y += i_other.m_y;
		m_z += i_other.m_z;
		m_w += i_other.m_w;

		return *this;
	}

	Vector4 & Vector4 :: operator -= (const Vector4 & i_other)
	{
		m_x -= i_other.m_x;
		m_y -= i_other.m_y;
		m_z -= i_other.m_z;
		m_w -= i_other.m_w;

		return *this;
	}

	Vector4 & Vector4 :: operator *= (const Vector4 & i_other)
	{
		m_x *= i_other.m_x;
		m_y *= i_other.m_y;
		m_z *= i_other.m_z;
		m_w *= i_other.m_z;

		return *this;
	}

	Vector4 & Vector4 :: operator /= (const Vector4 & i_other)
	{
		m_x /= i_other.m_x;
		m_y /= i_other.m_y;
		m_z /= i_other.m_z;
		m_w /= i_other.m_w;

		return *this;
	}

	bool Vector4 :: operator == (const Vector4 & i_other)
	{
		return (fabs(m_x - i_other.m_x) < 0.01f && fabs(m_y - i_other.m_y) < 0.01f && fabs(m_z - i_other.m_z) < 0.01f && fabs(m_w - i_other.m_w) < 0.01f);
	}

	bool Vector4 :: operator != (const Vector4 & i_other)
	{
		return !(*this == i_other);
	}
} // namespace Illehc