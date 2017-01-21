#include "Threading\NMutex.h"

#include <Windows.h>
#include <cassert>

namespace Illehc
{
	namespace Threading
	{
		Mutex :: Mutex(const char * i_pName)
		{
			m_Handle = CreateMutexA(NULL, FALSE, i_pName);
			assert(m_Handle != NULL);
		}

		Mutex :: ~Mutex()
		{
			BOOL result = CloseHandle(m_Handle);
			assert(result == TRUE);
		}

		bool Mutex :: TryLock(void)
		{
			DWORD result = WaitForSingleObject(m_Handle, 0);

			return result == WAIT_OBJECT_0;
		}

		void Mutex :: Lock(void)
		{
			DWORD result = WaitForSingleObject(m_Handle, INFINITE);
			assert(result == WAIT_OBJECT_0);
		}

		void Mutex :: Release(void)
		{
			BOOL result = ReleaseMutex(m_Handle);
			assert(result == TRUE);
		}
	} // namespace Threading
} // namespace Illehc