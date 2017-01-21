#pragma once

#ifndef __ENGINE__NVECTOR3__H__
#define __ENGINE__NVECTOR3__H__

namespace Illehc 
{
	class Vector3
	{
	public:
		Vector3(void);
		Vector3(float i_x, float i_y, float i_z);

		inline float getX(void) const;
		inline float getY(void) const;
		inline float getZ(void) const;

		inline void setX(const float i_x);
		inline void setY(const float i_y);
		inline void setZ(const float i_z);

		Vector3 & operator += (const Vector3 & i_other);
		Vector3 & operator -= (const Vector3 & i_other);
		Vector3 & operator *= (const Vector3 & i_other);
		Vector3 & operator /= (const Vector3 & i_other);
		bool operator == (const Vector3 & i_other);
		bool operator != (const Vector3 & i_other);
		Vector3 Normalized() const;
		inline float Length() const;
		inline float LengthSq() const;

		static const Vector3 Zero;

	private:
		float m_x, m_y, m_z;
		inline bool isNaN(const float i_val);
		inline bool IsZero(float i_val) const;
		inline bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff) const;

	};

	inline Vector3 operator + (const Vector3 & i_lhs, const Vector3 & i_rhs);
	inline Vector3 operator - (const Vector3 & i_lhs, const Vector3 & i_rhs);
	inline Vector3 operator * (const Vector3 & i_lhs, const Vector3 & i_rhs);
	inline Vector3 operator / (const Vector3 & i_lhs, const Vector3 & i_rhs);
	inline Vector3 operator * (const Vector3 & i_lhs, const float i_scalar);
	inline Vector3 operator * (const float i_scalar, const Vector3 & i_lhs);
	inline float dot(const Vector3 & i_lhs, const Vector3 & i_rhs);

} //namespace Illehc

#include "NVector3-inl.h"

#endif // __ENGINE__NVECTOR3__H__

