#include "Threading\NSemaphore.h"

#include <Windows.h>
#include <cassert>

namespace Illehc
{
	namespace Threading
	{
		Semaphore :: Semaphore(const char * i_pName, unsigned int i_CountMax, unsigned int i_CountInit)
		{
			assert(i_CountInit <= i_CountMax);

			m_Handle = CreateSemaphoreA(NULL, i_CountInit, i_CountMax, i_pName);
			assert(m_Handle != NULL);
		}

		Semaphore :: ~Semaphore()
		{
			BOOL result = CloseHandle(m_Handle);
			assert(result == TRUE);
		}

		void Semaphore :: Increment(unsigned int i_count)
		{
			BOOL result = ReleaseSemaphore(m_Handle, i_count, NULL);
			assert(result == TRUE);
		}

		void Semaphore :: WaitAndDecrement(void)
		{
			DWORD result = WaitForSingleObject(m_Handle, INFINITE);
			assert(result == WAIT_OBJECT_0);
		}

	} // namespace Threading
} // namespace Illehc