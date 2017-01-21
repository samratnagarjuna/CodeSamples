#pragma once

#ifndef __ENGINE__NCONTROLLERMANAGER__H__
#define __ENGINE__NCONTROLLERMANAGER__H__

#include <map>

#include "Utility\NHashedString.h"
#include "Entities\NIController.h"

namespace Illehc
{
	namespace Controller
	{
		void Init(void);
		void RegisterController(const HashedString & i_ControllerName, IController * i_Controller);
		void DeRegisterController(const HashedString & i_ControllerName, IController * i_Controller);
		IController * GetController(const HashedString & i_ControllerName);
		void ShutDown(void);

		class ControllerManager
		{
		public:
			static ControllerManager * Instance(void);
			void Init(void);
			void RegisterController(const HashedString & i_ControllerName, IController * i_Controller);
			void DeRegisterController(const HashedString & i_ControllerName, IController * i_Controller);
			IController * GetController(const HashedString & i_ControllerName);
			void ShutDown(void);
		private:
			ControllerManager(void);
			ControllerManager(const ControllerManager & i_other) {};
			ControllerManager & operator = (const ControllerManager & i_other) {};

			static ControllerManager *					_instance;
			std::map < HashedString, IController *>		m_ControllerMap;
		};
	}
} // namespace Illehc

#endif // !__ENGINE__NCONTROLLERMANAGER__H__

