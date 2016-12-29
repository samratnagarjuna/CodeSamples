// Header Files
//=============

#include "RenderContext.d3d.h"

// Static Data Initialization
//===========================

eae6320::Graphics::RenderContext* eae6320::Graphics::RenderContext::m_Instance = 0;


// Interface
//==========

eae6320::Graphics::RenderContext::RenderContext()
{

}

eae6320::Graphics::RenderContext::~RenderContext()
{
	
}

eae6320::Graphics::RenderContext* eae6320::Graphics::RenderContext::Instance() 
{
	if (m_Instance == 0)
	{
		m_Instance = new RenderContext();
	}

	return m_Instance;
}

ID3D11Device * eae6320::Graphics::RenderContext::GetDirect3DDevice()
{
	return m_direct3dDevice;
}

void eae6320::Graphics::RenderContext::SetDirect3DDevice(ID3D11Device * i_direct3dDevice)
{
	m_direct3dDevice = i_direct3dDevice;
}

ID3D11DeviceContext * eae6320::Graphics::RenderContext::GetDirect3DImmediateContext()
{
	return m_direct3dImmediateContext;
}

void eae6320::Graphics::RenderContext::SetDirect3DImmediateContext(ID3D11DeviceContext * i_direct3dImmediateContext)
{
	m_direct3dImmediateContext = i_direct3dImmediateContext;
}





