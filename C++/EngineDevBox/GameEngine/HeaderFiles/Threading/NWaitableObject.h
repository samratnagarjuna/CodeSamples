#pragma once

#ifndef __ENGINE__NWAITABLEOBJECT__H__
#define __ENGINE__NWAITABLEOBJECT__H__

#include <Windows.h>

namespace Illehc
{
	namespace Threading
	{
		class WaitableObject
		{
		public:
			WaitableObject() :
				m_Handle(INVALID_HANDLE_VALUE)
			{

			}

			HANDLE GetHandle(void) { return m_Handle; }

		protected:
			HANDLE					m_Handle;

		};
	} // namespace Threading
} // namespace Illehc

#endif // !__ENGINE__NWAITABLEOBJECT__H__
