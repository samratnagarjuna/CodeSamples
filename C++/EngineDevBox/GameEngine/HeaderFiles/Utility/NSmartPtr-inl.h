#pragma once

#ifndef __ENGINE__NSMARTPTR__INL__H__
#define __ENGINE__NSMARTPTR__INL__H__

#include <cassert> // Change after adding Debug Module

namespace Illehc
{
	template <class T>
	inline SmartPtr<T> :: SmartPtr(T * i_pGameObject) : m_pGameObject(i_pGameObject), m_pRefCount(NULL)
	{
		if (m_pGameObject != NULL)
		{
			m_pRefCount = new size_t;
			*m_pRefCount = 1;
		}
	}

	template <class T>
	inline T * SmartPtr<T> :: operator -> (void) const
	{
		assert( m_pGameObject != NULL );
		return m_pGameObject;
	}

	template <class T>
	inline T & SmartPtr<T> :: operator * (void) const
	{
		assert( m_pGameObject != NULL );
		return *m_pGameObject;
	}

	template <class T>
	inline SmartPtr<T> :: SmartPtr(const SmartPtr<T> & i_other) : m_pGameObject(i_other.m_pGameObject), m_pRefCount(i_other.m_pRefCount)
	{
		if (m_pRefCount != NULL)
			(*m_pRefCount)++;
	}

	template <class T>
	inline void SmartPtr<T> :: ReleaseCurrentReference()
	{
		if (m_pRefCount != NULL)
		{
			assert(*m_pRefCount != 0);
			if (--(*m_pRefCount) == 0)
			{
				delete m_pGameObject;
				delete m_pRefCount;
			}
		}
	}

	template <class T>
	inline void SmartPtr<T> :: AcquireNewReference(const SmartPtr<T> &i_other)
	{
		m_pRefCount = i_other.m_pRefCount;
		m_pGameObject = i_other.m_pGameObject;
	}

	template <class T>
	inline SmartPtr<T>& SmartPtr<T> :: operator = (const SmartPtr<T> & i_other)
	{
		if (*this != i_other)
		{
			ReleaseCurrentReference();
			AcquireNewReference(i_other);
			if (m_pRefCount != NULL)
				(*m_pRefCount)++;
		}
		return *this;
	}

	template <class T>
	inline bool SmartPtr<T> :: operator == (const SmartPtr<T> & i_other) const
	{
		return m_pGameObject == i_other.m_pGameObject;
	}

	template <class T>
	inline bool SmartPtr<T> :: operator != (const SmartPtr<T> & i_other) const
	{
		return m_pGameObject != i_other.m_pGameObject;
	}

	template <class T>
	inline bool SmartPtr<T> :: operator ! (void) const
	{
		return !m_pGameObject;
	}

	template <class T>
	inline SmartPtr<T> :: ~SmartPtr(void)
	{
		ReleaseCurrentReference();
	}
} // namespace Illehc

#endif // !__ENGINE__NSMARTPTR__INL__H__
