#pragma once

#ifndef __ENGINE__NRENDERMANAGER__H__
#define __ENGINE__NRENDERMANAGER__H__

#include <Windows.h>
#include <list>

#include "NRenderInfo.h"

#include "GLib.h"

namespace Illehc
{
	namespace Renderer
	{
		void Init();
		void Draw(void);
		void ShutDown(void);

		class RenderManager
		{
		public:
			static RenderManager * Instance(void);
			void Init();
			void Draw(void);
			RenderInfo * AddRenderer(SmartPtr<GameObject> & i_GameObject, const char * const i_pFileName);
			void RemoveRenderer(RenderInfo * i_Info);
			~RenderManager();

		private:
			RenderManager(void);
			RenderManager(const RenderManager & i_other) {};
			RenderManager & operator = (const RenderManager & i_other) {};
			GLib::Sprites::Sprite * CreateSprite(const char * i_pFileName);
			void * LoadFile(const char * i_pFilename, size_t & o_sizeFile);

			static RenderManager *			_instance;
			std::list< RenderInfo * >		m_Infos;
			
		};
	} // namespce Renderer
} // namespace Illehc
#endif // !__ENGINE__NRENDERMANAGER__H__
