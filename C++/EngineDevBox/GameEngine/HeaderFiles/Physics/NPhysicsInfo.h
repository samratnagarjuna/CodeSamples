#pragma once

#ifndef __ENGINE__NPHYSICSINFO__H__
#define __ENGINE__NPHYSICSINFO__H__

#include "Entities\NGameObject.h"
#include "Utility\NSmartPtr.h"
#include "Math\NVector3.h"
#include "Collision\NAABB.h"

namespace Illehc
{
	namespace Physics
	{
		typedef struct PhysicsInfo
		{
			SmartPtr<GameObject>	gameObject;
			float					mass;
			float					dragK;
			Vector3					netForce;
			AABB *					collider;
		} PhysicsInfo;
	} // namespace Physics
} // namespace Illehc

#endif // !__ENGINE__NPHYSICSINFO__H__

