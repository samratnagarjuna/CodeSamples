#include "Scene\NSceneManager.h"
#include "Scene\NAbstractScene.h"

#include <iostream>
#include <cassert>

#include "NEngine.h"

namespace Illehc
{
	namespace GameScene
	{
		SceneManager * SceneManager::_instance = 0;

		SceneManager * SceneManager :: Instance(void)
		{
			if (_instance == 0) {
				_instance = new SceneManager();
			}

			return _instance;
		}

		SceneManager :: SceneManager()
		{
			m_CurrentScene = NULL;
			m_NewScene = NULL;
		}

		void SceneManager :: Init(void)
		{
			m_ChangeSceneRequested = false;
		}

		void SceneManager :: SetScene(AbstractScene * i_Scene)
		{
			assert(i_Scene);

			//m_NewScene = i_Scene;
			//m_ChangeSceneRequested = true;

			if (m_CurrentScene != NULL)
			{
				m_CurrentScene->ShutDown();
				GameEngine::Engine::Instance()->DeleteAllActors();
			}

			m_CurrentScene = i_Scene;
			m_CurrentScene->Init();
		}

		void SceneManager::LoadScene(void)
		{
			if (m_ChangeSceneRequested)
			{
				if (m_CurrentScene != NULL)
				{
					m_CurrentScene->ShutDown();
					GameEngine::Engine::Instance()->DeleteAllActors();
					delete m_CurrentScene;
				}

				m_CurrentScene = m_NewScene;
				m_NewScene = NULL;

				m_CurrentScene->Init();
				m_ChangeSceneRequested = false;
			}
		}

		void SceneManager :: Update(const float i_dt)
		{
			if (m_CurrentScene != NULL)
				m_CurrentScene->Update(i_dt);
		}

		void SceneManager :: ShutDown(void)
		{
			/*if (m_CurrentScene != NULL)
			{
				m_CurrentScene->ShutDown();
				GameEngine::Engine::Instance()->DeleteAllActors();
				delete m_CurrentScene;
			}*/
		}

		SceneManager :: ~SceneManager(void)
		{
			
		}

		void Init(void)
		{
			SceneManager::Instance()->Init();
		}

		void Update(const float i_dt)
		{
			SceneManager::Instance()->Update(i_dt);
		}

		void LoadScene(void)
		{
			SceneManager::Instance()->LoadScene();
		}

		void ShutDown(void)
		{
			SceneManager::Instance()->ShutDown();
			delete SceneManager::Instance();
		}

		void SetScene(AbstractScene * i_Scene)
		{
			SceneManager::Instance()->SetScene(i_Scene);
		}
	}
}