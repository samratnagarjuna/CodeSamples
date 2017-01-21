#pragma once

#ifndef __ENGINE__NMATRIX4__INL__H__
#define __ENGINE__NMATRIX4__INL__H__

#include <cassert>
#include <cmath>
#include "NMatrix4.h"

namespace Illehc
{
	inline bool Matrix4 :: IsNAN(const float i_val) const
	{
		volatile float v = i_val;
		return v != v;
	}

	inline bool Matrix4 :: IsEqual(const float i_lhv, const float i_rhv) const
	{
		return fabs(i_lhv - i_rhv) < 0.0001f;
	}

	inline bool Matrix4 :: IsZero(const float i_val) const
	{
		return IsEqual(i_val, 0.0f);
	}

	inline bool Matrix4 :: AreEqual_Eps(const float i_lhv, const float i_rhv, const float i_eps) const
	{
		return fabs(i_lhv - i_rhv) < i_eps;
	}

	// Allows us to use V = M * V; (i.e. column vector)
	inline Vector4 operator * (const Matrix4 & i_mtx, const Vector4 & i_vec)
	{
		return i_mtx.MultiplyRight(i_vec);
	}

	// Allows us to use V = M * V; (i.e. row vector)
	inline Vector4 operator * (const Vector4 & i_vec, const Matrix4 & i_mtx)
	{
		return i_mtx.MultiplyLeft(i_vec);
	}

	inline Vector3 Matrix4 :: TransformPointCV(const Vector3 & i_point) const
	{
		Vector4 Point = MultiplyRight(Vector4(i_point, 1.0f));
		assert(!IsZero(Point.getW()));

		float inv_w = 1.0f / Point.getW();
		return Vector3(Point.getX() * inv_w, Point.getY() * inv_w, Point.getZ() * inv_w);
	}

	inline Vector3 Matrix4 :: TransformPointRV(const Vector3 & i_point) const
	{
		Vector4 Point = MultiplyLeft(Vector4(i_point, 1.0f));
		assert(!IsZero(Point.getW()));

		float inv_w = 1.0f / Point.getW();
		return Vector3(Point.getX() * inv_w, Point.getY() * inv_w, Point.getZ() * inv_w);
	}

	inline Vector3 Matrix4 :: TransformNormalCV(const Vector3 & i_point) const
	{
		Vector4 Point = MultiplyRight(Vector4(i_point, 0.0f));
		return Vector3(Point.getX(), Point.getY(), Point.getZ());
	}

	inline Vector3 Matrix4 :: TransformNormalRV(const Vector3 & i_point) const
	{
		Vector4 Point = MultiplyLeft(Vector4(i_point, 0.0f));
		return Vector3(Point.getX(), Point.getY(), Point.getZ());
	}

	inline void Matrix4 :: Invert(void)
	{
		assert(!IsNAN(m_11));

		InverseSSE(*this);
	}

	inline Matrix4 Matrix4 :: GetInverse(void) const
	{
		assert(!IsNAN(m_11));

		Matrix4	Temporary;

		InverseSSE(Temporary);

		return Matrix4(Temporary);
	}

	inline void Matrix4 :: GetInverse(Matrix4 & o_out) const
	{
		InverseSSE(o_out);
	}

	inline Matrix4 Matrix4 :: operator * (const Matrix4 & i_other) const
	{
		assert(!IsNAN(m_11));

#if defined(_WIN32)
		// if the matrix we're multiplying against is aligned to a 16 byte address
		// we can use SSE instructions.
		if ((reinterpret_cast<uintptr_t>(&i_other) & (16 - 1)) == 0)
		{
			FastMatrix4 tResult;
			MultiplySSE(i_other, tResult);

			return Matrix4(tResult);
		}
#endif

		Matrix4 tResult;
		Multiply(i_other, tResult);

		return Matrix4(tResult);
	}

	inline Matrix4 Matrix4 :: CreateTranslationCV(const Vector3 & i_trans)
	{
		return CreateTranslationCV(i_trans.getX(), i_trans.getY(), i_trans.getZ());
	}

	inline Matrix4 Matrix4 :: CreateTranslationRV(const Vector3 & i_trans)
	{
		return CreateTranslationRV(i_trans.getX(), i_trans.getY(), i_trans.getZ());
	}
} // namespace Illehc

#endif // !__ENGINE__NMATRIX4__INL__H__
