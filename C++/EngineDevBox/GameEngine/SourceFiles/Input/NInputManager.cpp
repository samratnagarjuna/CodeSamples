#include "Input\NInputManager.h"

#include "GLib.h"

namespace Illehc
{
	namespace Input
	{
		void Init(void)
		{
			InputManager::Instance()->Init();
		}

		bool IsKeyDown(unsigned int i_VKeyID)
		{
			return InputManager::Instance()->IsKeyDown(i_VKeyID);
		}

		void ShutDown(void)
		{
			delete InputManager::Instance();
		}

		InputManager * InputManager::_instance = 0;
		bool InputManager::m_Keys[255];

		InputManager * InputManager :: Instance()
		{
			if (_instance == 0) {
				_instance = new InputManager();
			}

			return _instance;
		}

		InputManager :: InputManager(void)
		{

		}

		void InputManager :: Init(void)
		{
			GLib::SetKeyStateChangeCallback(InputKeyCallBack);
		}

		void InputManager :: InputKeyCallBack(unsigned int i_VKeyID, bool bWentDown)
		{
			m_Keys[i_VKeyID] = bWentDown;
		}

		bool InputManager :: IsKeyDown(unsigned int i_VKeyID)
		{
			return m_Keys[i_VKeyID];
		}

	} // namespace Input
} // namespace Illehc
