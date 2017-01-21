#pragma once

#ifndef __ENGINE__NRENDERER__H__
#define __ENGINE__NRENDERER__H__

#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"
#include "Renderer\NRenderInfo.h"
#include "Math\NVector3.h"

namespace Illehc
{
	namespace Renderer
	{
		class Renderer
		{
		public:
			Renderer(SmartPtr<GameObject> & i_GameObject, const char * const i_pFileName);
			~Renderer(void);

		private:
			RenderInfo * m_Info;
		};
	} // namespace Renderer
}// namespace Illehc

#endif // !__ENGINE__NRENDERER__H__
