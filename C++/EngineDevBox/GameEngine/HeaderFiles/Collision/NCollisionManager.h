#pragma once

#ifndef __ENGINE__NCOLLISIONMANAGER__H__
#define __ENGINE__NCOLLISIONMANAGER__H__

#include <list>

#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"
#include "Collision\NAABB.h"
#include "Threading\NMutex.h"

namespace Illehc
{
	namespace Physics
	{
		class CollisionManager
		{
		public:
			static CollisionManager * Instance(void);
			void Init(void);
			AABB * AddAABBCollider(SmartPtr<GameObject> & i_GameObject, const Vector3 i_Center, const Vector3 i_Extents);
			void RemoveCollider(AABB * i_Collider);
			bool Check2DCollision(const AABB & i_Obj1AABB, const Vector3 i_Obj1Acceleration,
								  const AABB & i_Obj2AABB, const Vector3 i_Obj2Acceleration,
								  float i_dt, 
								  float & o_tCollision,
								  Vector3 & o_nCollision);
			~CollisionManager(void);

		private:
			CollisionManager(void);
			CollisionManager(const CollisionManager & i_other) : m_CollisionInfoListLock("CollisionInfoListLock"){};
			CollisionManager & operator = (const CollisionManager & i_other) {};

			bool CheckSeparationAlongAxis(
				Vector3 & i_CollisionAxis, 
				const AABB & i_Obj1AABB, const Vector3 & i_Obj1BBCenterInWorld, 
				const AABB & i_Obj2AABB, const Vector3 & i_Obj2BBCenterInWorld, 
				float i_dt,
				float & i_tEnter, float & i_tExit);
			Vector3 VelocityInTime(const AABB & i_Obj, const Vector3 i_Acceleration, const float i_dt);
			bool DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float & o_tEnter, float & o_tExit);
			bool IsZero(const float i_val) const;
			bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff) const;
			void Swap(float & i_lhs, float & i_rhs);

			static CollisionManager *		_instance;
			std::list< AABB * >				m_Colliders;
			Threading::Mutex				m_CollisionInfoListLock;
		};
	} // namespace Physics
} // namespace Illehc

#endif // !__ENGINE__NCOLLISIONMANAGER__H__
