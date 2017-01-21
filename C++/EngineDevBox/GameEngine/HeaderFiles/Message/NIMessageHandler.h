#pragma once

#ifndef __ENGINE__NIMESSAGEHANDLER__H__
#define __ENGINE__NIMESSAGEHANDLER__H__

#include "Utility\NHashedString.h"

namespace Illehc
{
	namespace MessageSystem
	{
		class IMessageHandler
		{
		public:
			virtual ~IMessageHandler() {};
			virtual void OnReceive(const HashedString & i_Message, void * i_Data) = 0;
		};
	} // namespace MessageSystem
}// namespace Illehc
#endif // !__ENGINE__NIMESSAGEHANDLER__H__

