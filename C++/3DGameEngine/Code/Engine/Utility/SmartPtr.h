#ifndef EAE6320_SMART_PTR_H
#define EAE6320_SMART_PTR_H

#include <iostream>

namespace eae6320
{
	template <class T>
	class SmartPtr
	{
	public:

		inline SmartPtr(T * i_pGameObject = NULL);
		inline SmartPtr(const SmartPtr<T> & i_other);
		inline T * operator->(void) const;
		inline T & operator*(void) const;
		inline SmartPtr<T> & operator = (const SmartPtr<T> & i_other);
		inline bool operator == (const SmartPtr<T> & i_other) const;
		inline bool operator != (const SmartPtr<T> & i_other) const;
		inline bool operator ! (void) const;
		inline ~SmartPtr(void);

	private:
		inline void ReleaseCurrentReference(void);
		inline void AcquireNewReference(const SmartPtr &i_other);

		size_t *		m_pRefCount;
		T *				m_pGameObject;
	};
} // namespace eae6320

#include "SmartPtr-inl.h"

#endif // !EAE6320_SMART_PTR_H