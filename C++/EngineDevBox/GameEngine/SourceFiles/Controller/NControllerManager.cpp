#include "Controller\NControllerManager.h"

namespace Illehc
{
	namespace Controller
	{
		ControllerManager * ControllerManager::_instance = 0;

		ControllerManager * ControllerManager :: Instance(void)
		{
			if (_instance == 0) {
				_instance = new ControllerManager();
			}
			return _instance;
		}

		void ControllerManager :: Init(void)
		{

		}

		ControllerManager :: ControllerManager(void)
		{

		}

		void ControllerManager::RegisterController(const HashedString & i_ControllerName, IController * i_Controller)
		{
			m_ControllerMap[i_ControllerName] = i_Controller;
		}

		void ControllerManager::DeRegisterController(const HashedString & i_ControllerName, IController * i_Controller)
		{
			m_ControllerMap[i_ControllerName] = NULL;
		}

		IController * ControllerManager::GetController(const HashedString & i_ControllerName)
		{
			return m_ControllerMap[i_ControllerName];
		}

		void ControllerManager :: ShutDown(void)
		{
			m_ControllerMap.clear();
		}

		void Init(void)
		{
			ControllerManager::Instance()->Init();
		}

		void RegisterController(const HashedString & i_ControllerName, IController * i_Controller)
		{
			ControllerManager::Instance()->RegisterController(i_ControllerName, i_Controller);
		}

		void DeRegisterController(const HashedString & i_ControllerName, IController * i_Controller)
		{
			ControllerManager::Instance()->DeRegisterController(i_ControllerName, i_Controller);
		}

		IController * GetController(const HashedString & i_ControllerName)
		{
			return ControllerManager::Instance()->GetController(i_ControllerName);
		}

		void ShutDown(void)
		{
			ControllerManager::Instance()->ShutDown();
			delete ControllerManager::Instance();
		}

	} // namespace Controller
} // namespace Illehc