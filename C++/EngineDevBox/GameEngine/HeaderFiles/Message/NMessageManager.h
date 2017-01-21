#pragma once

#ifndef __ENGINE__NMESSAGEMANAGER__H__
#define __ENGINE__NMESSAGEMANAGER__H__

#include <map>
#include <list>

#include "Utility\NHashedString.h"
#include "Message\NIMessageHandler.h"

namespace Illehc
{
	namespace MessageSystem
	{
		void Init(void);
		void RegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler);
		void DeRegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler);
		void NotifyMessage(const HashedString & i_Message, void * i_Data);
		void ShutDown(void);

		class MessageManager
		{
		public:
			static MessageManager * Instance(void);
			void Init(void);
			void RegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler);
			void DeRegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler);
			void NotifyMessage(const HashedString & i_Message, void * i_Data);
			void ShutDown(void);
			~MessageManager(void);
		private:
			MessageManager(void);
			MessageManager(const MessageManager & i_other) {};
			MessageManager & operator = (const MessageManager & i_other) {};

			static MessageManager *										_instance;
			std::map < HashedString, std::list< IMessageHandler *>>		m_HandlerMap;
		};
	} // namespace MessageSystem
} // namespace Illehc
#endif // !__ENGINE__NMESSAGEMANAGER__H__
