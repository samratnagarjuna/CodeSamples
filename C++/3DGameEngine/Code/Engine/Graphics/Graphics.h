/*
	This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Header Files
//=============

#include "Configuration.h"
#include "RenderInfo.h"
#include "Camera.h"

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "../Windows/Includes.h"
	#if defined( EAE6320_PLATFORM_D3D )
		#include <D3D11.h>
	#elif defined (EAE6320_PLATFORM_GL)

	#endif
#endif

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		// Render
		//-------

		void LoadFrameData(RenderInfo* i_renderInfo);
		void LoadSpriteData(SpriteInfo* i_spriteInfo);
		void SetCamera(Camera* i_pCamera);
		void RenderFrame();

		// Initialization / Clean Up
		//--------------------------

		struct sInitializationParameters
		{
#if defined( EAE6320_PLATFORM_WINDOWS )
			HWND mainWindow;
	#if defined( EAE6320_PLATFORM_D3D )
			unsigned int resolutionWidth, resolutionHeight;
	#elif defined( EAE6320_PLATFORM_GL )
			HINSTANCE thisInstanceOfTheApplication;
	#endif
#endif
		};

		bool Initialize( const sInitializationParameters& i_initializationParameters );
		bool CleanUp();

	}
}

#endif	// EAE6320_GRAPHICS_H
