#pragma once

#ifndef __ENGINE__NEVENT__H__
#define __ENGINE__NEVENT__H__

#include "Threading\NWaitableObject.h"

namespace Illehc
{
	namespace Threading
	{
		class ManualResetEvent : public WaitableObject
		{
		public:
			ManualResetEvent(const char * i_pName, bool i_bInitiallySignaled);
			~ManualResetEvent();

			void Reset(void);
			// We should add a wait timeout parameter but I'm in a hurry
			bool WaitFor(void);
			void Signal(void);
		};

		class AutoResetEvent : public WaitableObject
		{
		public:
			AutoResetEvent(const char * i_pName, bool i_bInitiallySignaled);
			~AutoResetEvent();

			// We should add a wait timeout parameter but I'm in a hurry
			bool WaitFor(void);
			void Signal(void);
		};
	} // namespace Threading
} // namespace Illehc

#endif // !__ENGINE__NEVENT__H__
