#pragma once

#ifndef __ENGINE__NSCENEMANAGER__H__
#define __ENGINE__NSCENEMANAGER__H__

#include "Scene\NAbstractScene.h"

namespace Illehc
{
	namespace GameScene 
	{
		void Init(void);
		void ShutDown(void);
		void Update(const float i_dt);
		void LoadScene(void);
		void SetScene(AbstractScene * i_Scene);

		class SceneManager
		{
		public:
			static SceneManager * Instance(void);
			void Init(void);
			void SetScene(AbstractScene * i_Scene);
			void LoadScene(void);
			void ShutDown(void);
			void Update(const float i_dt);
			~SceneManager(void);

		private:
			SceneManager(void);
			SceneManager(const SceneManager & i_other) {};
			SceneManager & operator = (const SceneManager & i_other) {};

			static SceneManager *		_instance;
			AbstractScene *				m_CurrentScene;
			AbstractScene *				m_NewScene;
			bool						m_ChangeSceneRequested;
		};
	} // namespace GameScene
} // namespace Illehc

#endif // !__ENGINE__NSCENEMANAGER__H__

