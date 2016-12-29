/*
	This class exposes render variables required by the mesh and graphics for direct 3d
*/

#ifndef EAE6320__GRAPHICS_RENDER_CONTEXT_D3D_H
#define EAE6320__GRAPHICS_RENDER_CONTEXT_D3D_H

#include <D3D11.h>

// Interface
//==========

//TODO

namespace eae6320
{
	namespace Graphics
	{
		class RenderContext
		{

		public:
			static RenderContext* Instance();

			ID3D11Device* GetDirect3DDevice();
			void SetDirect3DDevice(ID3D11Device* i_direct3dDevice);

			ID3D11DeviceContext* GetDirect3DImmediateContext();
			void SetDirect3DImmediateContext(ID3D11DeviceContext* i_direct3dImmediateContext);

			~RenderContext();

		private:
			static RenderContext* m_Instance;

			ID3D11Device* m_direct3dDevice = NULL;
			ID3D11DeviceContext* m_direct3dImmediateContext = NULL;

			RenderContext();
			
		};
	}
}

#endif // !EAE6320_GRAPHICS_RENDER_CONTEXT_D3D_H

