#pragma once

#ifndef __ENGINE__NRIGIDBODY__H__
#define __ENGINE__NRIGIDBODY__H__

#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"
#include "Physics\NPhysicsInfo.h"
#include "Collision\NAABBCollider.h"
#include "Math\NVector3.h"

namespace Illehc
{
	namespace Physics
	{
		class RigidBody
		{
		public:
			RigidBody(SmartPtr<GameObject> & i_GameObject, const float i_Mass, const float i_DragK);
			void ApplyForce(const Vector3 i_Force);
			void AddCollider(AABBCollider * i_Collider);
			PhysicsInfo * GetPhysicsInfo();
			~RigidBody(void);
		private:
			Physics::PhysicsInfo *		m_Info;
		};
	} // namespace Illehc
} // namespace Illehc

#endif // !__ENGINE__NRIGIDBODY__H__

