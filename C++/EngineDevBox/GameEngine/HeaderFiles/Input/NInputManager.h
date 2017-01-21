#pragma once

#ifndef __ENGINE__NINPUTMANAGER__H__
#define __ENGINE__NINPUTMANAGER__H__

namespace Illehc
{
	namespace Input
	{
		void Init(void);
		bool IsKeyDown(unsigned int i_VKeyID);
		void ShutDown(void);

		class InputManager
		{
		public:
			static InputManager * Instance(void);
			void Init(void);
			bool IsKeyDown(unsigned int i_VKeyID);
			static void InputKeyCallBack(unsigned int i_VKeyID, bool bWentDown);

		private:
			InputManager(void);
			InputManager(const InputManager & i_other) {};
			InputManager & operator = (const InputManager & i_other) {};
			
			static InputManager *		_instance;
			static bool					m_Keys[255];

		};
	} // namespace Input
}
#endif // !__ENGINE__NINPUTMANAGER__H__
