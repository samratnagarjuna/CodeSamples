#pragma once

#ifndef __ENGINE__NPHYSICSMANAGER__H__
#define __ENGINE__NPHYSICSMANAGER__H__

#include <list>
#include <utility>

#include "NPhysicsInfo.h"
#include "Utility\NSmartPtr.h"
#include "Entities\NGameObject.h"
#include "Threading\NMutex.h"

namespace Illehc
{
	namespace Physics
	{
		void Init(void);
		void Update(const float i_dt);
		void ShutDown(void);

		typedef std::pair < float, std::pair<Vector3, std::pair< PhysicsInfo *, PhysicsInfo *> > > TimeNormalCollisionPair;

		class PhysicsManager
		{

		public:
			static PhysicsManager * Instance(void);
			void Init(void);
			void Update(const float i_dt);
			PhysicsInfo * AddRigidBody(SmartPtr<GameObject> & i_GameObject, const float i_Mass, const float i_DragK);
			void RemoveRigidBody(PhysicsInfo * i_Info);
			~PhysicsManager(void);

		private:
			PhysicsManager(void);
			PhysicsManager(const PhysicsManager & i_other) : m_PhysicsInfoListLock("PhysicsInfoListLock"){};
			PhysicsManager & operator = (const PhysicsManager & i_other) {};

			bool GetFirstCollidedObject(PhysicsInfo * i_info, 
										const float i_dt, 
										PhysicsInfo ** i_other, 
										float & o_tCollision, 
										Vector3 & o_nCollision);
			void ResolveCollision(PhysicsInfo & obj1, PhysicsInfo & obj22, Vector3 collisionNormal);
			void MoveGameObjects(const float i_dt);
			void ApplyDrag(const float i_dt);
			void ClearNetForce(const float i_dt);
			bool IsZero(float i_val) const;
			bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff) const;
			static PhysicsManager *			_instance;
			std::list< PhysicsInfo * >		m_Infos;
			Threading::Mutex				m_PhysicsInfoListLock;
		};
	} // namespace Physics
} // namespace Illehc

#endif // !__ENGINE__NPHYSICSMANAGER__H__
