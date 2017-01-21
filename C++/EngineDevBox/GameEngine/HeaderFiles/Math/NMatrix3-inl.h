#pragma once

#ifndef __ENGINE__NMATRIX3__INL__H__
#define __ENGINE__NMATRIX3__INL__H__

#include <cassert>

namespace Illehc
{
	inline bool Matrix3 :: IsNAN(const float i_val) const
	{
		volatile float v = i_val;
		return v != v;
	}

	// Allows us to use V = V * M; (i.e. row vector)
	inline Vector3 operator * (const Matrix3 & i_mtx, const Vector3 & i_vec)
	{
		return i_mtx.MultiplyRight(i_vec);
	}

	// Allows us to use V = M * V; (i.e. column vector)
	inline Vector3 operator * (const Vector3 & i_vec, const Matrix3 & i_mtx)
	{
		return i_mtx.MultiplyLeft(i_vec);
	}

	inline Matrix3 Matrix3::operator * (const Matrix3 & i_other) const
	{
		assert(!IsNAN(m_11));

		Matrix3 tResult;
		Multiply(i_other, tResult);

		return Matrix3(tResult);
	}
} // namespace Illehc

#endif // !__ENGINE__NMATRIX3__INL__H__

