#pragma once

#ifndef __ENGINE__NAABB__H__
#define __ENGINE__NAABB__H__

#include "Entities\NGameObject.h"
#include "Utility\NSmartPtr.h"
#include "Math\NVector3.h"

namespace Illehc
{
	namespace Physics
	{
		typedef struct AABB
		{
			SmartPtr<GameObject>	gameObject;
			Vector3					center;
			Vector3					extents;
		} AABB;
	} // namespace Physics
} // namespace Illehc

#endif // !__ENGINE__NAABB__H__
