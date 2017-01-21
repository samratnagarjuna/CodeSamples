#pragma once

#ifndef __ENGINE__NSEMAPHORE__H__
#define __ENGINE__NSEMAPHORE__H__

#include "Threading\NWaitableObject.h"

namespace Illehc
{
	namespace Threading
	{
		class Semaphore : public WaitableObject
		{
		public:
			Semaphore(const char * i_pName, unsigned int i_CountMax, unsigned int i_CountInit);
			~Semaphore();

			void Increment(unsigned int i_count = 1);
			void WaitAndDecrement(void);
		};
	} // namespace Threading
} // namespace Illehc

#endif // !__ENGINE__NSEMAPHORE__H__
