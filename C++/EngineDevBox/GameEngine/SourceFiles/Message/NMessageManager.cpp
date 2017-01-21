#include "Message\NMessageManager.h"

namespace Illehc
{
	namespace MessageSystem
	{
		MessageManager * MessageManager::_instance = 0;

		MessageManager * MessageManager::Instance(void)
		{
			if (_instance == 0) {
				_instance = new MessageManager();
			}
			return _instance;
		}

		void MessageManager :: Init(void)
		{

		}

		MessageManager :: MessageManager(void)
		{

		}

		void MessageManager :: RegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler)
		{
			m_HandlerMap[i_Message].push_back(i_Handler);
		}

		void MessageManager :: DeRegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler)
		{
			m_HandlerMap[i_Message].remove(i_Handler);
		}

		void MessageManager :: NotifyMessage(const HashedString & i_Message, void * i_Data)
		{
			std::list<IMessageHandler *> handlers = m_HandlerMap[i_Message];
			for (std::list< IMessageHandler * >::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				(*it)->OnReceive(i_Message, i_Data);
			}
		}

		void MessageManager :: ShutDown(void)
		{
			m_HandlerMap.clear();
		}

		MessageManager :: ~MessageManager(void)
		{

		}

		void Init(void)
		{
			MessageManager::Instance()->Init();
		}

		void RegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler)
		{
			MessageManager::Instance()->RegisterMessageHandler(i_Message, i_Handler);
		}

		void DeRegisterMessageHandler(const HashedString & i_Message, IMessageHandler * i_Handler)
		{
			MessageManager::Instance()->DeRegisterMessageHandler(i_Message, i_Handler);
		}

		void NotifyMessage(const HashedString & i_Message, void * i_Data)
		{
			MessageManager::Instance()->NotifyMessage(i_Message, i_Data);
		}

		void ShutDown(void)
		{
			MessageManager::Instance()->ShutDown();
			delete MessageManager::Instance();
		}

	} // namespace MessageSystem
} // namespace Illehc