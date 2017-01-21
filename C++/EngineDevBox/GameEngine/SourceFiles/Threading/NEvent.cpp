#include "Threading\NEvent.h"

#include <Windows.h>
#include <cassert>

namespace Illehc
{
	namespace Threading
	{
		ManualResetEvent :: ManualResetEvent(const char * i_pName, bool i_bInitiallySignaled)
		{
			m_Handle = CreateEventA(NULL, TRUE, i_bInitiallySignaled, i_pName);
			assert(m_Handle != INVALID_HANDLE_VALUE);
		}

		ManualResetEvent :: ~ManualResetEvent()
		{
			CloseHandle(m_Handle);
		}

		void ManualResetEvent :: Reset(void)
		{
			ResetEvent(m_Handle);
		}

		bool ManualResetEvent :: WaitFor(void)
		{
			DWORD result = WaitForSingleObject(m_Handle, INFINITE);
			assert(result == WAIT_OBJECT_0);

			return true;
		}

		void ManualResetEvent :: Signal(void)
		{
			BOOL result = SetEvent(m_Handle);
			assert(result == TRUE);
		}

		AutoResetEvent :: AutoResetEvent(const char * i_pName, bool i_bInitiallySignaled)
		{
			m_Handle = CreateEventA(NULL, FALSE, i_bInitiallySignaled, i_pName);
			assert(m_Handle != INVALID_HANDLE_VALUE);
		}

		AutoResetEvent :: ~AutoResetEvent()
		{
			CloseHandle(m_Handle);
		}

		bool AutoResetEvent :: WaitFor(void)
		{
			DWORD result = WaitForSingleObject(m_Handle, INFINITE);
			assert(result == WAIT_OBJECT_0);

			return true;
		}

		void AutoResetEvent :: Signal(void)
		{
			BOOL result = SetEvent(m_Handle);
			assert(result == TRUE);
		}

	} // namespace Threading
} // namespace Illehc