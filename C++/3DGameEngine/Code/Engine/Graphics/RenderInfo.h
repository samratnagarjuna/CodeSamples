/*
This file has the struct which holds information to render
*/

#ifndef EAE6320_GRAPHICS_RENDER_INFO_H
#define EAE6320_GRAPHICS_RENDER_INFO_H

// Header Files
//=============

#include "MeshRenderer.h"
#include "cSprite.h"
#include "cMaterial.h"
#include "../Entities/GameObject.h"
#include "../Utility/SmartPtr.h"

namespace eae6320
{
	namespace Graphics
	{
		struct RenderInfo
		{
			MeshRenderer*			pMeshRenderer;
			cMaterial*				pMaterial;
			SmartPtr<GameObject>	pGameObject;
		};

		struct SpriteInfo
		{
			cSprite*	pSpriteRenderer;
			cMaterial*	pMaterial;
		};
	}
}

#endif // !EAE6320_GRAPHICS_RENDER_INFO_H

