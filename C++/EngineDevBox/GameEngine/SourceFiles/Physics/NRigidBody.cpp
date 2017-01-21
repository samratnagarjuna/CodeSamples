#include "Physics\NRigidBody.h"
#include "Physics\NPhysicsManager.h"

namespace Illehc
{
	namespace Physics
	{
		
		RigidBody :: RigidBody(SmartPtr<GameObject>& i_GameObject, const float i_Mass, const float i_DragK)
			: m_Info(NULL)
		{
			m_Info = PhysicsManager::Instance()->AddRigidBody(i_GameObject, i_Mass, i_DragK);
		}

		void RigidBody :: ApplyForce(const Vector3 i_Force)
		{
			m_Info->netForce = i_Force;
		}

		void RigidBody::AddCollider(AABBCollider * i_Collider)
		{
			m_Info->collider = i_Collider->GetAABBCollider();
		}

		PhysicsInfo * RigidBody::GetPhysicsInfo()
		{
			return m_Info;
		}

		RigidBody :: ~RigidBody(void)
		{
			PhysicsManager::Instance()->RemoveRigidBody(m_Info);
		}

	}
}