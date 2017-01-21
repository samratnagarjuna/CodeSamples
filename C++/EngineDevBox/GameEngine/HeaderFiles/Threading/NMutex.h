#pragma once

#ifndef __ENIGNE__NMUTEX__H__
#define __ENIGNE__NMUTEX__H__

#include "Threading\NWaitableObject.h"

namespace Illehc
{
	namespace Threading
	{
		class Mutex : public WaitableObject
		{
		public:
			Mutex(const char * i_pName);
			~Mutex();

			bool TryLock(void);
			void Lock(void);
			void Release(void);

		};
	} // namespace Threading
} // namespace Illehc

#endif // !__ENIGNE__NMUTEX__H__
