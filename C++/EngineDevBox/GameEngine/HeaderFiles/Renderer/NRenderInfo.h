#pragma once

#ifndef __ENGINE__NRENDERINFO__H__
#define __ENGINE__NRENDERINFO__H__

#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"

namespace GLib
{
	namespace Sprites
	{
		struct Sprite;
	}
}

namespace Illehc
{
	namespace Renderer
	{
		typedef struct RenderInfo
		{
			SmartPtr<GameObject>		gameObject;
			GLib::Sprites::Sprite *		sprite;
		} RenderInfo;
	}
} // namespace Illehc
#endif // !__ENGINE__NRENDERINFO__H__
