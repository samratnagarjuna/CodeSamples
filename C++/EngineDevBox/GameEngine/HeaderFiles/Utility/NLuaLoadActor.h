#pragma once

#ifndef __ENGINE__NLUALOADACTOR__H__
#define __ENGINE__NLUALOADACTOR__H__

#include "Entities\NActor.h"
#include "Threading\NMutex.h"
#include "Threading\NThreadedFileProcessor.h"

namespace Illehc
{
	class LuaLoadActor : public Threading::ThreadedFileProcessor::LoadTask 
	{
	public:
		LuaLoadActor(const char * i_pFilename);
		virtual void ProcessFileContents(uint8_t * i_pBuffer, uint32_t i_BufferBytes);
		Actor * GetActor(void);

	private:
		Threading::Mutex		m_CreateActorMutex;
		Actor *					m_pActor;
	};

} // namespace Illehc

#endif // !__ENGINE__NLUALOADACTOR__H__
