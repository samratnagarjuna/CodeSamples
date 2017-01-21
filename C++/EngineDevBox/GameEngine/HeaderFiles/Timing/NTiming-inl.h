#include "NTiming.h"
#pragma once

#ifndef __ENGINE__NTIMING__INL__H__
#define __ENGINE__NTIMING__INL__H__

namespace Illehc
{
	namespace Timing
	{
		Time * Time :: Instance(void) {
			if (_instance == 0) {
				_instance = new Time();
			}
			return _instance;
		}

		inline void Time :: Init(void)
		{
			m_Frequency = GetFrequency();
			m_LastFrameStartTick = GetCounter();
		}

		inline LARGE_INTEGER Time :: GetFrequency(void)
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			return frequency;
		}

		inline LARGE_INTEGER Time :: GetCounter(void)
		{
			LARGE_INTEGER ticks;
			QueryPerformanceCounter(&ticks);
			return ticks;
		}

		inline void Init(void)
		{
			Time * timer = Time::Instance();
			timer->Init();
		}

		float TimeSinceLastFrame()
		{
			return Time::Instance()->TimeSinceLastFrame();
		}

		inline void ShutDown(void)
		{
			Time * timer = Time::Instance();
			delete timer;
		}
	}
}
#endif // !__ENGINE__NTIMING__INL__H__
