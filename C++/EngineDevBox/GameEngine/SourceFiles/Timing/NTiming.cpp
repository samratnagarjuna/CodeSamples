#include "Timing\NTiming.h"

namespace Illehc 
{
	namespace Timing
	{
		Time * Time::_instance = 0;

		void Time :: CalculateLastFrameTime(void)
		{
			LARGE_INTEGER currentTick = GetCounter();
			LARGE_INTEGER elapsedTicks;
			elapsedTicks.QuadPart = (currentTick.QuadPart - m_LastFrameStartTick.QuadPart);


			elapsedTicks.QuadPart *= 1000;
			m_LastFrameTime_ms = elapsedTicks.QuadPart * (1.0f / m_Frequency.QuadPart);

			m_LastFrameStartTick = currentTick;
		}

		float Time :: TimeSinceLastFrame(void)
		{
			CalculateLastFrameTime();
#if defined (CONSTANT_FRAMETIME)
			return DESIRED_FRAMETIME_MS;
#elif defined (CLAMP_FRAMETIME)
			if (m_LastFrameTime_ms > MAX_FRAMETIME_MS)
				return MAX_FRAMETIME_MS;
			else
				return m_LastFrameTime_ms;
#else
			return m_lastFrameTime_ms;
#endif
		}

	} // namespace Timing
} // namespace Illehc

