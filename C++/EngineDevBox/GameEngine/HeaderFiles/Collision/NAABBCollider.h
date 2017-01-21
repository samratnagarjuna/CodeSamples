#pragma once

#ifndef __ENGINE__NAABBCOLLIDER__H__
#define __ENGINE__NAABBCOLLIDER__H__

#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"
#include "Collision\NAABB.h"

namespace Illehc
{
	namespace Physics
	{
		class RigidBody; // forward declare
		class AABBCollider
		{
		public:
			AABBCollider(SmartPtr<GameObject> & i_GameObject, Vector3 i_Center, Vector3 i_Extents);
			AABB * GetAABBCollider();
			~AABBCollider(void);
		private:
			AABB *			m_AABB;
		};
	} // namespace Physics
}// namespace Illehc
#endif // !__ENGINE__NAABBCOLLIDER__H__

