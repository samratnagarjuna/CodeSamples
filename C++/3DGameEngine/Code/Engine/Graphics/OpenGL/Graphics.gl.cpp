// Header Files
//=============

#include "../Graphics.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include "Includes.h"
#include <string>
#include <sstream>
#include <list>
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"
#include "../../Time/Time.h"
#include "../../Windows/Functions.h"
#include "../../Windows/OpenGl.h"
#include "../../../External/OpenGlExtensions/OpenGlExtensions.h"
#include "../RenderInfo.h"
#include "../Includes.h"
#include "../cEffect.h"
#include "../cConstantBuffer.h"
#include "../Camera.h"

// Static Data Initialization
//===========================

namespace
{
	// The is the main window handle from Windows
	HWND s_renderingWindow = NULL;
	// These are Windows-specific interfaces
	HDC s_deviceContext = NULL;
	HGLRC s_openGlRenderingContext = NULL;
	GLuint s_samplerStateId = 0;

	std::list< eae6320::Graphics::RenderInfo* >* s_renderList;
	std::list< eae6320::Graphics::SpriteInfo* >* s_spriteList;

	// Reference to camera
	eae6320::Graphics::Camera* g_pCamera = NULL;


	// OpenGL encapsulates a matching vertex shader and fragment shader into what it calls a "program".

	// A vertex shader is a program that operates on vertices.
	// Its input comes from a C/C++ "draw call" and is:
	//	* Position
	//	* Any other data we want
	// Its output is:
	//	* Position
	//		(So that the graphics hardware knows which pixels to fill in for the triangle)
	//	* Any other data we want

	// The fragment shader is a program that operates on fragments
	// (or potential pixels).
	// Its input is:
	//	* The data that was output from the vertex shader,
	//		interpolated based on how close the fragment is
	//		to each vertex in the triangle.
	// Its output is:
	//	* The final color that the pixel should be
	// GLuint s_programId = 0;

	eae6320::Graphics::cConstantBuffer* g_pConstantFrameBuffer = NULL;
	eae6320::Graphics::cConstantBuffer* g_pConstantDrawBuffer = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateSamplerState();
	bool CreateConstantBuffer();
	bool CreateRenderingContext();
	bool EnableBackFaceCulling();
	bool EnableDepthBuffering();
	bool CreateVertexBuffer();
	bool LoadAndAllocateShaderProgram( const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage );

	// This helper struct exists to be able to dynamically allocate memory to get "log info"
	// which will automatically be freed when the struct goes out of scope
	struct sLogInfo
	{
		GLchar* memory;
		sLogInfo( const size_t i_size ) { memory = reinterpret_cast<GLchar*>( malloc( i_size ) ); }
		~sLogInfo() { if ( memory ) free( memory ); }
	};
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
		glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );

		// In addition to the color, "depth" and "stencil" can also be cleared
		glDepthMask(GL_TRUE);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);

		glClearDepth(1.0f);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);

		const GLbitfield clearColorAndDepth = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		glClear(clearColorAndDepth);
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );


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
		// Set the vertex and fragment shaders

		for (std::list<RenderInfo*>::iterator it = s_renderList->begin(); it != s_renderList->end(); it++)
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
	// In order to display it, the contents of the back buffer must be swapped with the "front buffer"
	// (which is what the user sees)
	{
		BOOL result = SwapBuffers( s_deviceContext );
		EAE6320_ASSERT( result != FALSE );
	}
}

// Initialization / Clean Up
//==========================

bool eae6320::Graphics::Initialize( const sInitializationParameters& i_initializationParameters )
{
	std::string errorMessage;

	s_renderingWindow = i_initializationParameters.mainWindow;

	// Load any required OpenGL extensions
	if ( !OpenGlExtensions::Load( &errorMessage ) )
	{
		EAE6320_ASSERTF( false, errorMessage.c_str() );
		Logging::OutputError( errorMessage.c_str() );
		return false;
	}

	// Create an OpenGL rendering context
	if ( !CreateRenderingContext() )
	{
		EAE6320_ASSERT( false );
		return false;
	}

	// Enable back face culling
	if (!EnableBackFaceCulling())
	{
		EAE6320_ASSERT(false);
		return false;
	}

	// Enable depth buffering
	if (!EnableDepthBuffering())
	{
		EAE6320_ASSERT(false);
		return false;
	}

	// Initialize the graphics objects
	if ( !CreateSamplerState() )
	{
		EAE6320_ASSERT( false );
		return false;
	}

	if ( !CreateVertexBuffer() )
	{
		EAE6320_ASSERT( false );
		return false;
	}

	if ( !CreateConstantBuffer() )
	{
		EAE6320_ASSERT( false );
		return false;
	}

	if (!cSprite::Initialize())
	{
		EAE6320_ASSERT(false);
		return false;
	}

	// Initialize Render List
	{
		s_renderList = new std::list< eae6320::Graphics::RenderInfo* >();
		s_spriteList = new std::list< eae6320::Graphics::SpriteInfo* >();
	}

	return true;
}

bool eae6320::Graphics::CleanUp()
{
	bool wereThereErrors = false;

	if ( s_openGlRenderingContext != NULL )
	{
		if (g_pConstantFrameBuffer)
		{
			g_pConstantFrameBuffer->CleanUp();
		}

		if (!cSprite::CleanUp())
		{
			EAE6320_ASSERT(false);
			wereThereErrors = true;
		}

		if (s_samplerStateId)
		{
			const GLsizei samplerStateCount = 1;
			glDeleteSamplers(samplerStateCount, &s_samplerStateId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to delete a sampler state: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
		}

		if ( wglMakeCurrent( s_deviceContext, NULL ) != FALSE )
		{
			if ( wglDeleteContext( s_openGlRenderingContext ) == FALSE )
			{
				wereThereErrors = true;
				const std::string windowsErrorMessage = Windows::GetLastSystemError();
				EAE6320_ASSERTF( false, windowsErrorMessage.c_str() );
				Logging::OutputError( "Windows failed to delete the OpenGL rendering context: %s", windowsErrorMessage.c_str() );
			}
		}
		else
		{
			wereThereErrors = true;
			const std::string windowsErrorMessage = Windows::GetLastSystemError();
			EAE6320_ASSERTF( false, windowsErrorMessage.c_str() );
			Logging::OutputError( "Windows failed to unset the current OpenGL rendering context: %s", windowsErrorMessage.c_str() );
		}
		s_openGlRenderingContext = NULL;
	}

	if ( s_deviceContext != NULL )
	{
		// The documentation says that this call isn't necessary when CS_OWNDC is used
		ReleaseDC( s_renderingWindow, s_deviceContext );
		s_deviceContext = NULL;
	}

	s_renderingWindow = NULL;

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

// Helper Function Declarations
//=============================

namespace
{
	bool CreateSamplerState()
	{
		// Create a sampler state object
		{
			const GLsizei samplerStateCount = 1;
			glGenSamplers(samplerStateCount, &s_samplerStateId);
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				if (s_samplerStateId != 0)
				{
					// Linear Filtering
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
					// If UVs go outside [0,1] wrap them around (so that textures can tile)
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_WRAP_S, GL_REPEAT);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
					glSamplerParameteri(s_samplerStateId, GL_TEXTURE_WRAP_T, GL_REPEAT);
					EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
				}
				else
				{
					EAE6320_ASSERT(false);
					eae6320::Logging::OutputError("OpenGL failed to create a sampler state");
					return false;
				}
			}
			else
			{
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to create a sampler state: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return false;
			}
		}
		// Bind the sampler state
		{
			// We will never be required to use more than one texture in an Effect in this class,
			// but it doesn't hurt to bind the state to a few extra texture units
			// just in case you decide to try using more
			const GLuint maxTextureUnitCountYouThinkYoullUse = 5;
			for (GLuint i = 0; i < maxTextureUnitCountYouThinkYoullUse; ++i)
			{
				glBindSampler(i, s_samplerStateId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to bind the sampler state to texture unit %u: %s",
						i, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return false;
				}
			}
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

		// Create constant buffer and initialize
		g_pConstantFrameBuffer = new eae6320::Graphics::cConstantBuffer();
		{
			if (!g_pConstantFrameBuffer->Initialize(
				eae6320::Graphics::cConstantBuffer::CONSTANT_FRAME_BUFFER,
				sizeof(sConstantBufferFrameData),
				&s_constantFrameBufferData))
			{
				EAE6320_ASSERT(false);
				eae6320::Logging::OutputError("OpenGL failed to create a constant frame buffer");
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
				eae6320::Logging::OutputError("OpenGL failed to create a constant draw buffer");
				return false;
			}
		}

		return true;
	}

	bool CreateRenderingContext()
	{
		// Get the device context
		{
			s_deviceContext = GetDC( s_renderingWindow );
			if ( s_deviceContext == NULL )
			{
				EAE6320_ASSERT( false );
				eae6320::Logging::OutputError( "Windows failed to get the device context" );
				return false;
			}
		}
		// Set the pixel format for the window
		// (This can only be done _once_ for a given window)
		{
			// Get the ID of the desired pixel format
			int pixelFormatId;
			{
				// Create a key/value list of attributes that the pixel format should have
				const int desiredAttributes[] =
				{
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
					WGL_COLOR_BITS_ARB, 24,
					WGL_RED_BITS_ARB, 8,
					WGL_GREEN_BITS_ARB, 8,
					WGL_BLUE_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_STENCIL_BITS_ARB, 8,
					// NULL terminator
					NULL
				};
				const float* const noFloatAttributes = NULL;
				const unsigned int onlyReturnBestMatch = 1;
				unsigned int returnedFormatCount;
				if ( wglChoosePixelFormatARB( s_deviceContext, desiredAttributes, noFloatAttributes, onlyReturnBestMatch,
					&pixelFormatId, &returnedFormatCount ) != FALSE )
				{
					if ( returnedFormatCount == 0 )
					{
						EAE6320_ASSERT( false );
						eae6320::Logging::OutputError( "Windows couldn't find a pixel format that satisfied the desired attributes" );
						return false;
					}
				}
				else
				{
					const std::string windowsErrorMessage = eae6320::Windows::GetLastSystemError();
					EAE6320_ASSERTF( false, windowsErrorMessage.c_str() );
					eae6320::Logging::OutputError( "Windows failed to choose the closest pixel format: %s", windowsErrorMessage.c_str() );
					return false;
				}
			}
			// Set it
			{
				PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };
				{
					// I think that the values of this struct are ignored
					// and unnecessary when using wglChoosePixelFormatARB() instead of ChoosePixelFormat(),
					// but the documentation is very unclear and so filling it in seems the safest bet
					pixelFormatDescriptor.nSize = sizeof( PIXELFORMATDESCRIPTOR );
					pixelFormatDescriptor.nVersion = 1;
					pixelFormatDescriptor.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
					pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
					pixelFormatDescriptor.cColorBits = 24;
					pixelFormatDescriptor.iLayerType = PFD_MAIN_PLANE;
					pixelFormatDescriptor.cDepthBits = 24;
					pixelFormatDescriptor.cStencilBits = 8;
				}
				if ( SetPixelFormat( s_deviceContext, pixelFormatId, &pixelFormatDescriptor ) == FALSE )
				{
					const std::string windowsErrorMessage = eae6320::Windows::GetLastSystemError();
					EAE6320_ASSERTF( false, windowsErrorMessage.c_str() );
					eae6320::Logging::OutputError( "Windows couldn't set the desired pixel format: %s", windowsErrorMessage.c_str() );
					return false;
				}
			}
		}
		// Create an OpenGL rendering context and make it current
		{
			// Create the context
			{
				// Create a key/value list of attributes that the context should have
				const int desiredAttributes[] =
				{
					// Request at least version 4.2
					WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
					WGL_CONTEXT_MINOR_VERSION_ARB, 2,
					// Request only "core" functionality and not "compatibility"
					// (i.e. only use modern features of version 4.2)
					WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
					WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
					// NULL terminator
					NULL
				};
				const HGLRC noSharedContexts = NULL;
				s_openGlRenderingContext = wglCreateContextAttribsARB( s_deviceContext, noSharedContexts, desiredAttributes );
				if ( s_openGlRenderingContext == NULL )
				{
					DWORD errorCode;
					const std::string windowsErrorMessage = eae6320::Windows::GetLastSystemError( &errorCode );
					std::ostringstream errorMessage;
					errorMessage << "Windows failed to create an OpenGL rendering context: ";
					if ( ( errorCode == ERROR_INVALID_VERSION_ARB )
						|| ( HRESULT_CODE( errorCode ) == ERROR_INVALID_VERSION_ARB ) )
					{
						errorMessage << "The requested version number is invalid";
					}
					else if ( ( errorCode == ERROR_INVALID_PROFILE_ARB )
						|| ( HRESULT_CODE( errorCode ) == ERROR_INVALID_PROFILE_ARB ) )
					{
						errorMessage << "The requested profile is invalid";
					}
					else
					{
						errorMessage << windowsErrorMessage;
					}
					EAE6320_ASSERTF( false, errorMessage.str().c_str() );
					eae6320::Logging::OutputError( errorMessage.str().c_str() );
						
					return false;
				}
			}
			// Set it as the rendering context of this thread
			if ( wglMakeCurrent( s_deviceContext, s_openGlRenderingContext ) == FALSE )
			{
				const std::string windowsErrorMessage = eae6320::Windows::GetLastSystemError();
				EAE6320_ASSERTF( false, windowsErrorMessage.c_str() );
				eae6320::Logging::OutputError( "Windows failed to set the current OpenGL rendering context: %s",
					windowsErrorMessage.c_str() );
				return false;
			}
		}

		return true;
	}

	bool CreateVertexBuffer()
	{
		bool wereThereErrors = false;
		return !wereThereErrors;
	}

	bool EnableBackFaceCulling()
	{
		bool wereThereErrors = false;
		glEnable(GL_CULL_FACE);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed enable back face culling: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
		}
		return !wereThereErrors;
	}

	bool EnableDepthBuffering()
	{
		bool wereThereErrors = false;
		glDepthFunc(GL_LESS);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set depth func: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

		glEnable(GL_DEPTH_TEST);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to enable depth test: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

		glDepthMask(GL_TRUE);
		{
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set depth mask: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

	OnExit:
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