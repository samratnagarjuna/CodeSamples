#include "Renderer\NRenderer.h"

#include "Renderer\NRenderManager.h"

namespace Illehc
{
	namespace Renderer
	{
		Renderer :: Renderer(SmartPtr<GameObject>& i_GameObject, const char * const i_pFileName)
		{
			m_Info = RenderManager::Instance()->AddRenderer(i_GameObject, i_pFileName);
		}

		Renderer :: ~Renderer(void)
		{
			RenderManager::Instance()->RemoveRenderer(m_Info);
		}
	} // namespace Renderer
}// namespace Illehc