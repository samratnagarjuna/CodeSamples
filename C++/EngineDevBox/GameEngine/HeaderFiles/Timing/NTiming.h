#pragma once

#ifndef __ENGINE__NTIMING__H__
#define __ENGINE__NTIMING__H__

#include <Windows.h>

#define DESIRED_FPS				60.0f
#define DESIRED_FRAMETIME_MS	(1000.0f / DESIRED_FPS)
#define MAX_FRAMETIME_MS		(2 * DESIRED_FRAMETIME_MS)

#define CLAMP_FRAMETIME

namespace Illehc
{
	namespace Timing 
	{
		inline void Init(void);
		inline float TimeSinceLastFrame();
		inline void ShutDown(void);

		class Time
		{
		public:
			inline static Time * Instance(void);
			void Init(void);
			float TimeSinceLastFrame(void);

		protected:
			inline Time(void) {};

		private:

			Time(Time const & i_other) {};
			Time & operator = (const Time & i_other) {};
			
			void  CalculateLastFrameTime(void);
			inline LARGE_INTEGER GetCounter(void);
			inline LARGE_INTEGER GetFrequency(void);

			static Time *		_instance;
			float				m_LastFrameTime_ms;
			LARGE_INTEGER		m_LastFrameStartTick;
			LARGE_INTEGER		m_Frequency;
		};
	} //namespace Timing
} // namespace Illehc

#include "NTiming-inl.h"

#endif // !__ENGINE__NTIMING__H__
