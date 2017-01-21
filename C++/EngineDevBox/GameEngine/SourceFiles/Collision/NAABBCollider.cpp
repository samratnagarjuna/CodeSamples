#include "Collision\NAABBCollider.h"

#include "Collision\NCollisionManager.h"

namespace Illehc
{
	namespace Physics
	{
		AABBCollider::AABBCollider(SmartPtr<GameObject> & i_GameObject, Vector3 i_Center, Vector3 i_Extents)
			: m_AABB(NULL)
		{
			m_AABB = CollisionManager::Instance()->AddAABBCollider(i_GameObject, i_Center, i_Extents);
		}

		AABB * AABBCollider::GetAABBCollider()
		{
			return m_AABB;
		}

		AABBCollider :: ~AABBCollider(void)
		{
			CollisionManager::Instance()->RemoveCollider(m_AABB);
		}
	} // namespace Physics
} // namespace Illehc