#pragma once

#ifndef __ENGINE__NABSTRACTSCENE__H__
#define __ENGINE__NABSTRACTSCENE__H__

namespace Illehc
{
	namespace GameScene 
	{
		class AbstractScene
		{
		public:
			virtual void Init() = 0;
			virtual void Update(const float i_dt) = 0;
			virtual void ShutDown() = 0;
			virtual ~AbstractScene(void) {};
		};
	} // namespace GameScene
} // namespace Illehc

#endif // !__ENGINE__NABSTRACTSCENE__H__

