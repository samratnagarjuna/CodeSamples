// Header Files
//=============

#include "../Graphics.h"

#include <cstddef>
#include <cstdint>
#include <D3D11.h>
#include <D3DX11async.h>
#include <D3DX11core.h>
#include <DXGI.h>
#include <list>
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"
#include "../../Time/Time.h"
#include "RenderContext.d3d.h"
#include "../cEffect.h"
#include "../RenderInfo.h"
#include "../Includes.h"
#include "../cConstantBuffer.h"
#include "../Camera.h"

// Static Data Initialization
//===========================

namespace
{
	// This is the main window handle from Windows
	HWND s_renderingWindow = NULL;
	// These are D3D interfaces
	ID3D11Device* s_direct3dDevice = NULL;
	IDXGISwapChain* s_swapChain = NULL;
	ID3D11DeviceContext* s_direct3dImmediateContext = NULL;
	ID3D11SamplerState* s_samplerState = NULL;
	ID3D11RenderTargetView* s_renderTargetView = NULL;
	ID3D11DepthStencilView* s_depthStencilView = NULL;

	// D3D has an "input layout" object that associates the layout of the struct above
	// with the input from a vertex shader
	// ID3D11InputLayout* s_vertexLayout = NULL;

	std::list< eae6320::Graphics::RenderInfo* >* s_renderList;
	std::list< eae6320::Graphics::SpriteInfo* >* s_spriteList;

	// Reference to camera
	eae6320::Graphics::Camera* g_pCamera = NULL;

	// The vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want
	// ID3D11VertexShader* s_vertexShader = NULL;
	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	// ID3D11PixelShader* s_fragmentShader = NULL;

	eae6320::Graphics::cConstantBuffer* g_pConstantFrameBuffer = NULL;
	eae6320::Graphics::cConstantBuffer* g_pConstantDrawBuffer = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateConstantBuffer();
	bool CreateSamplerState();
	bool CreateDevice( const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight );
	bool CreateView( const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight );
}

// Interface
//==========

// Render
//-------

void eae6320::Graphics::RenderFrame()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	{
		// Black is usually used
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		s_direct3dImmediateContext->ClearRenderTargetView( s_renderTargetView, clearColor );
	}

	// Clear stencil buffer
	{
		const float depthToClear = 1.0f;
		const uint8_t stencilToClear = 0; // Arbitrary until stencil is used
		s_direct3dImmediateContext->ClearDepthStencilView(s_depthStencilView, D3D11_CLEAR_DEPTH,
			depthToClear, stencilToClear);
	}

	// Update the constant buffer
	{
		sConstantBufferFrameData s_constantFrameBufferData;
		{
			s_constantFrameBufferData.g_transform_worldToCamera = g_pCamera->WorldToCamraTransform();
			s_constantFrameBufferData.g_transform_cameraToScreen = g_pCamera->CameraToScreenTransform();
			s_constantFrameBufferData.g_elapsedSecondCount_total = eae6320::Time::GetElapsedSecondCount_total();
		}
		g_pConstantFrameBuffer->Update(&s_constantFrameBufferData);
	}

	// Draw the geometry
	{		
		for (std::list<RenderInfo*>::iterator it = s_renderList->begin(); it != s_renderList->end() ; it++)
		{
			// Update Draw Buffer Data
			RenderInfo* renderInfo = *it;
			if (renderInfo != NULL)
			{
				sConstantBufferDrawData s_constantDrawBufferData;
				{
					Math::cMatrix_transformation transform_localToWorld(renderInfo->pGameObject->orientation, renderInfo->pGameObject->position);
					s_constantDrawBufferData.g_transform_localToWorld = transform_localToWorld;
				}
				g_pConstantDrawBuffer->Update(&s_constantDrawBufferData);

				// Bind the Material
				renderInfo->pMaterial->Bind();

				if (renderInfo->pMeshRenderer != NULL)
					renderInfo->pMeshRenderer->Render();
			}
		}

		// Clear all the renderinfo submitted for this frame
		s_renderList->clear();
	}

	// Draw the sprites
	{
		for (std::list<SpriteInfo*>::iterator it = s_spriteList->begin(); it != s_spriteList->end(); it++)
		{
			SpriteInfo* spriteInfo = *it;
			if (spriteInfo != NULL)
			{
				// Bind Material
				spriteInfo->pMaterial->Bind();
				spriteInfo->pSpriteRenderer->Draw();
			}
		}

		// Clear all the spriteInfo submitted for this frame
		s_spriteList->clear();
	}

	// Everything has been drawn to the "back buffer", which is just an image in memory.
	// In order to display it the contents of the back buffer must be "presented"
	// (to the front buffer)
	{
		const unsigned int swapImmediately = 0;
		const unsigned int presentNextFrame = 0;
		const HRESULT result = s_swapChain->Present( swapImmediately, presentNextFrame );
		EAE6320_ASSERT( SUCCEEDED( result ) );
	}
}

// Initialization / Clean Up
//--------------------------

bool eae6320::Graphics::Initialize( const sInitializationParameters& i_initializationParameters )
{
	bool wereThereErrors = false;

	s_renderingWindow = i_initializationParameters.mainWindow;
	eae6320::Platform::sDataFromFile compiledVertexShader;

	// Create an interface to a Direct3D device
	if ( !CreateDevice( i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight ) )
	{
		wereThereErrors = true;
		goto OnExit;
	}
	// Initialize the viewport of the device
	if ( !CreateView( i_initializationParameters.resolutionWidth, i_initializationParameters.resolutionHeight ) )
	{
		wereThereErrors = true;
		goto OnExit;
	}

	// Initialize the graphics objects
	if ( !CreateSamplerState() )
	{
		wereThereErrors = true;
		goto OnExit;
	}

	if ( !CreateConstantBuffer() )
	{
		wereThereErrors = true;
		goto OnExit;
	}

	if (!cSprite::Initialize())
	{
		wereThereErrors = true;
		goto OnExit;
	}
	
	// Initialize Render List
	{
		s_renderList = new std::list< eae6320::Graphics::RenderInfo* >();
		s_spriteList = new std::list< eae6320::Graphics::SpriteInfo* >();
	}
	
OnExit:

	// A vertex shader object is used to render the triangle.
	// The compiled vertex shader is the actual compiled code,
	// and once it has been used to create the vertex input layout
	// it can be freed.
	compiledVertexShader.Free();


	return !wereThereErrors;
}

bool eae6320::Graphics::CleanUp()
{
	bool wereThereErrors = false;

	if ( s_direct3dDevice )
	{
		if (g_pConstantFrameBuffer)
		{
			g_pConstantFrameBuffer->CleanUp();
		}

		if (g_pConstantDrawBuffer)
		{
			g_pConstantDrawBuffer->CleanUp();
		}

		if ( s_renderTargetView )
		{
			s_renderTargetView->Release();
			s_renderTargetView = NULL;
		}

		if (s_depthStencilView)
		{
			s_depthStencilView->Release();
			s_depthStencilView = NULL;
		}

		if (s_samplerState)
		{
			s_samplerState->Release();
			s_samplerState = NULL;
		}

		s_direct3dDevice->Release();
		s_direct3dDevice = NULL;

		if (!cSprite::CleanUp())
		{
			EAE6320_ASSERT(false);
			wereThereErrors = true;
		}
	}
	if ( s_direct3dImmediateContext )
	{
		s_direct3dImmediateContext->Release();
		s_direct3dImmediateContext = NULL;
	}
	if ( s_swapChain )
	{
		s_swapChain->Release();
		s_swapChain = NULL;
	}

	s_renderingWindow = NULL;

	delete RenderContext::Instance();

	delete g_pConstantFrameBuffer;
	g_pConstantFrameBuffer = NULL;

	delete g_pConstantDrawBuffer;
	g_pConstantDrawBuffer = NULL;

	s_renderList->clear();
	delete s_renderList;

	s_spriteList->clear();
	delete s_spriteList;

	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateSamplerState()
	{
		{
			D3D11_SAMPLER_DESC samplerStateDescription;
			{
				// Linear filtering
				samplerStateDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				// If UVs go outside [0,1] wrap them around (so that textures can tile)
				samplerStateDescription.AddressU = samplerStateDescription.AddressV
					= samplerStateDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				// Default values
				samplerStateDescription.MipLODBias = 0.0f;
				samplerStateDescription.MaxAnisotropy = 1;
				samplerStateDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
				samplerStateDescription.BorderColor[0] = samplerStateDescription.BorderColor[1]
					= samplerStateDescription.BorderColor[2] = samplerStateDescription.BorderColor[3] = 1.0f;
				samplerStateDescription.MinLOD = -FLT_MAX;
				samplerStateDescription.MaxLOD = FLT_MAX;
			}
			const HRESULT result = s_direct3dDevice->CreateSamplerState(&samplerStateDescription, &s_samplerState);
			if (FAILED(result))
			{
				EAE6320_ASSERT(false);
				eae6320::Logging::OutputError("Direct3D failed to create a sampler state with HRESULT %#010x", result);
				return false;
			}
		}
		// Bind the sampler state
		{
			const unsigned int samplerStateRegister = 0; // This must match the SamplerState register defined in the shader
			const unsigned int samplerStateCount = 1;
			s_direct3dImmediateContext->PSSetSamplers(samplerStateRegister, samplerStateCount, &s_samplerState);
		}

		return true;
	}

	bool CreateConstantBuffer()
	{
		// Create constant buffer frame data
		sConstantBufferFrameData s_constantFrameBufferData;
		{
			s_constantFrameBufferData.g_transform_worldToCamera = eae6320::Math::cMatrix_transformation();
			s_constantFrameBufferData.g_transform_cameraToScreen = eae6320::Math::cMatrix_transformation();
			s_constantFrameBufferData.g_elapsedSecondCount_total = eae6320::Time::GetElapsedSecondCount_total();
		}

		// Create constant frame buffer and initialize
		g_pConstantFrameBuffer = new eae6320::Graphics::cConstantBuffer();
		{
			if (!g_pConstantFrameBuffer->Initialize(
				eae6320::Graphics::cConstantBuffer::CONSTANT_FRAME_BUFFER,
				sizeof(sConstantBufferFrameData),
				&s_constantFrameBufferData))
			{
				EAE6320_ASSERT(false);
				eae6320::Logging::OutputError( "Direct3D failed to create a constant frame buffer");
				return false;
			}
		}

		// Create constant buffer draw data
		sConstantBufferDrawData s_constantDrawBufferData;
		{
			s_constantDrawBufferData.g_transform_localToWorld = eae6320::Math::cMatrix_transformation();
		}

		// Create constant draw buffer and initialize
		g_pConstantDrawBuffer = new eae6320::Graphics::cConstantBuffer();
		{
			if (!g_pConstantDrawBuffer->Initialize(
				eae6320::Graphics::cConstantBuffer::CONSTANT_DRAW_BUFFER,
				sizeof(sConstantBufferDrawData),
				&s_constantDrawBufferData))
			{
				EAE6320_ASSERT(false);
				eae6320::Logging::OutputError("Direct3D failed to create a constant draw buffer");
				return false;
			}
		}

		return true;
	}

	bool CreateDevice( const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight )
	{
		IDXGIAdapter* const useDefaultAdapter = NULL;
		const D3D_DRIVER_TYPE useHardwareRendering = D3D_DRIVER_TYPE_HARDWARE;
		const HMODULE dontUseSoftwareRendering = NULL;
		unsigned int flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
		{
#ifdef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
			flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		}
		D3D_FEATURE_LEVEL* const useDefaultFeatureLevels = NULL;
		const unsigned int requestedFeatureLevelCount = 0;
		const unsigned int sdkVersion = D3D11_SDK_VERSION;
		DXGI_SWAP_CHAIN_DESC swapChainDescription = { 0 };
		{
			{
				DXGI_MODE_DESC& bufferDescription = swapChainDescription.BufferDesc;

				bufferDescription.Width = i_resolutionWidth;
				bufferDescription.Height = i_resolutionHeight;
				{
					DXGI_RATIONAL& refreshRate = bufferDescription.RefreshRate;

					refreshRate.Numerator = 0;	// Refresh as fast as possible
					refreshRate.Denominator = 1;
				}
				bufferDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				bufferDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				bufferDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			}
			{
				DXGI_SAMPLE_DESC& multiSamplingDescription = swapChainDescription.SampleDesc;

				multiSamplingDescription.Count = 1;
				multiSamplingDescription.Quality = 0;	// Anti-aliasing is disabled
			}
			swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDescription.BufferCount = 1;
			swapChainDescription.OutputWindow = s_renderingWindow;
			swapChainDescription.Windowed = TRUE;
			swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDescription.Flags = 0;
		}
		D3D_FEATURE_LEVEL highestSupportedFeatureLevel;
		const HRESULT result = D3D11CreateDeviceAndSwapChain( useDefaultAdapter, useHardwareRendering, dontUseSoftwareRendering,
			flags, useDefaultFeatureLevels, requestedFeatureLevelCount, sdkVersion, &swapChainDescription,
			&s_swapChain, &s_direct3dDevice, &highestSupportedFeatureLevel, &s_direct3dImmediateContext );

		eae6320::Graphics::RenderContext::Instance()->SetDirect3DDevice(s_direct3dDevice);
		eae6320::Graphics::RenderContext::Instance()->SetDirect3DImmediateContext(s_direct3dImmediateContext);

		if ( SUCCEEDED( result ) )
		{
			return true;
		}
		else
		{
			EAE6320_ASSERT( false );
			eae6320::Logging::OutputError( "Direct3D failed to create a Direct3D11 device with HRESULT %#010x", result );
			return false;
		}
	}

	bool CreateView( const unsigned int i_resolutionWidth, const unsigned int i_resolutionHeight )
	{
		bool wereThereErrors = false;

		// Create and bind the render target view
		// (the back buffer was already created by the call to D3D11CreateDeviceAndSwapChain(),
		// but we need a "view" of it to use as a "render target",
		// meaning a texture that the GPU can render to)
		ID3D11Texture2D* backBuffer = NULL;
		ID3D11Texture2D* depthBuffer = NULL;
		{
			// Get the back buffer from the swap chain
			HRESULT result;
			{
				const unsigned int bufferIndex = 0;	// This must be 0 since the swap chain is discarded
				result = s_swapChain->GetBuffer(bufferIndex, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
				if (FAILED(result))
				{
					EAE6320_ASSERT(false);
					eae6320::Logging::OutputError("Direct3D failed to get the back buffer from the swap chain with HRESULT %#010x", result);
					goto OnExit;
				}
			}
			// Create the view
			{
				const D3D11_RENDER_TARGET_VIEW_DESC* const accessAllSubResources = NULL;
				result = s_direct3dDevice->CreateRenderTargetView(backBuffer, accessAllSubResources, &s_renderTargetView);
				if (FAILED(result))
				{
					EAE6320_ASSERT(false);
					eae6320::Logging::OutputError("Direct3D failed to create the render target view with HRESULT %#010x", result);
					goto OnExit;
				}
			}
		}
		// Create a depth / stencil buffer and a view of it
		{
			// Unlike the back buffer no depth/stencil buffer exists until and unless we create it
			{
				D3D11_TEXTURE2D_DESC textureDescription = { 0 };
				{
					textureDescription.Width = i_resolutionWidth;
					textureDescription.Height = i_resolutionHeight;
					textureDescription.MipLevels = 1; // A depth buffer has no MIP maps
					textureDescription.ArraySize = 1;
					textureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 24 bits for depth and 8 bits for stencil
					{
						DXGI_SAMPLE_DESC& sampleDescription = textureDescription.SampleDesc;
						sampleDescription.Count = 1; // No multisampling
						sampleDescription.Quality = 0; // Doesn't matter when Count is 1
					}
					textureDescription.Usage = D3D11_USAGE_DEFAULT; // Allows the GPU to write to it
					textureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
					textureDescription.CPUAccessFlags = 0; // CPU doesn't need access
					textureDescription.MiscFlags = 0;
				}
				// The GPU renders to the depth/stencil buffer and so there is no initial data
				// (like there would be with a traditional texture loaded from disk)
				const D3D11_SUBRESOURCE_DATA* const noInitialData = NULL;
				const HRESULT result = s_direct3dDevice->CreateTexture2D(&textureDescription, noInitialData, &depthBuffer);
				if (FAILED(result))
				{
					EAE6320_ASSERT(false);
					eae6320::Logging::OutputError("Direct3D failed to create the depth buffer resource with HRESULT %#010x", result);
					goto OnExit;
				}
			}
			// Create the view
			{
				const D3D11_DEPTH_STENCIL_VIEW_DESC* const noSubResources = NULL;
				const HRESULT result = s_direct3dDevice->CreateDepthStencilView(depthBuffer, noSubResources, &s_depthStencilView);
				if (FAILED(result))
				{
					EAE6320_ASSERT(false);
					eae6320::Logging::OutputError("Direct3D failed to create the depth stencil view with HRESULT %#010x", result);
					goto OnExit;
				}
			}
		}

		// Bind the views
		{
			const unsigned int renderTargetCount = 1;
			s_direct3dImmediateContext->OMSetRenderTargets(renderTargetCount, &s_renderTargetView, s_depthStencilView);
		}

		// Specify that the entire render target should be visible
		{
			D3D11_VIEWPORT viewPort = { 0 };
			viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;
			viewPort.Width = static_cast<float>( i_resolutionWidth );
			viewPort.Height = static_cast<float>( i_resolutionHeight );
			viewPort.MinDepth = 0.0f;
			viewPort.MaxDepth = 1.0f;
			const unsigned int viewPortCount = 1;
			s_direct3dImmediateContext->RSSetViewports( viewPortCount, &viewPort );
		}

	OnExit:

		if ( backBuffer )
		{
			backBuffer->Release();
			backBuffer = NULL;
		}

		if (depthBuffer)
		{
			depthBuffer->Release();
			depthBuffer = NULL;
		}

		return !wereThereErrors;
	}
}

void eae6320::Graphics::LoadFrameData(RenderInfo* i_renderInfo)
{
	s_renderList->push_back(i_renderInfo);
}

void eae6320::Graphics::SetCamera(Camera* i_pCamera)
{
	g_pCamera = i_pCamera;
}

void eae6320::Graphics::LoadSpriteData(SpriteInfo* i_spriteInfo)
{
	s_spriteList->push_back(i_spriteInfo);
}