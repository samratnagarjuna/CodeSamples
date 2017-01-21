#pragma once

#ifndef __ENGINE__NVECTOR4__H__
#define __ENGINE__NVECTOR4__H__

#include "Math\NVector3.h"

namespace Illehc
{
	class Vector4
	{
	public:
		inline Vector4(void);
		inline Vector4(float i_x, float i_y, float i_z, float i_w);
		inline Vector4(const Vector3 i_vector3, const float i_w);

		inline float getX(void) const;
		inline float getY(void) const;
		inline float getZ(void) const;
		inline float getW(void) const;

		inline void setX(const float i_x);
		inline void setY(const float i_y);
		inline void setZ(const float i_z);
		inline void setW(const float i_w);

		Vector4 & operator += (const Vector4 & i_other);
		Vector4 & operator -= (const Vector4 & i_other);
		Vector4 & operator *= (const Vector4 & i_other);
		Vector4 & operator /= (const Vector4 & i_other);
		bool operator == (const Vector4 & i_other);
		bool operator != (const Vector4 & i_other);

		static const Vector4 Zero;
	private:
		float m_x, m_y, m_z, m_w;
		inline bool isNaN(const float i_val);
	};

	inline Vector4 operator + (const Vector4 & i_lhs, const Vector4 & i_rhs);
	inline Vector4 operator - (const Vector4 & i_lhs, const Vector4 & i_rhs);
	inline Vector4 operator * (const Vector4 & i_lhs, const Vector4 & i_rhs);
	inline Vector4 operator / (const Vector4 & i_lhs, const Vector4 & i_rhs);
	inline Vector4 operator * (const Vector4 & i_lhs, const float i_scalar);
	inline Vector4 operator * (const float i_scalar, const Vector4 & i_lhs);

} // namespace Illehc

#include "NVector4-inl.h"

#endif // !__ENGINE__NVECTOR4__H__

