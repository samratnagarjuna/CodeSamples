#include "Physics\NPhysicsManager.h"

#include "Collision\NCollisionManager.h"
#include "Utility\NConsolePrint.h"
#include "Math\NVector3.h"

namespace Illehc
{
	namespace Physics
	{
		PhysicsManager * PhysicsManager::_instance = 0;

		void Init(void)
		{
			PhysicsManager::Instance()->Init();
			CollisionManager::Instance()->Init();
		}

		PhysicsManager * PhysicsManager::Instance(void)
		{
			if (_instance == 0) {
				_instance = new PhysicsManager();
			}
			return _instance;
		}

		PhysicsManager :: PhysicsManager() : m_PhysicsInfoListLock("PhysicsInfoListLock")
		{

		}

		void PhysicsManager::Init(void)
		{

		}

		PhysicsInfo * PhysicsManager::AddRigidBody(SmartPtr<GameObject> & i_GameObject, const float i_Mass, const float i_DragK)
		{
			PhysicsInfo * info = new PhysicsInfo;
			info->gameObject = i_GameObject;
			info->mass = i_Mass;
			info->dragK = i_DragK;
			info->netForce = Vector3::Zero;

			m_PhysicsInfoListLock.Lock();

			m_Infos.push_back(info);

			m_PhysicsInfoListLock.Release();

			return info;
		}

		void PhysicsManager :: Update(const float i_dt)
		{
			m_PhysicsInfoListLock.Lock();

			ApplyDrag(i_dt);

			float curdt = 0.0f;
			while (curdt < i_dt)
			{
				bool collisionFound = false;
				TimeNormalCollisionPair firstCollision;

				for (std::list< PhysicsInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
				{
					TimeNormalCollisionPair tmpCollision;
					tmpCollision.second.second.first = *it;
					
					if ( GetFirstCollidedObject(
						*it,
						i_dt - curdt,
						&(tmpCollision.second.second.second),
						tmpCollision.first,
						tmpCollision.second.first) 
						&& (!collisionFound || tmpCollision.first < firstCollision.first) )
					{
						collisionFound = true;
						firstCollision = tmpCollision;
					}
				}

				float step;
				step = collisionFound ? firstCollision.first : i_dt - curdt ;
				if (IsZero(step)) step = i_dt - curdt; // step == 0 ? when and why does this happen ?
				curdt += step;

				if (!IsZero(step))
					MoveGameObjects(step);

				if (collisionFound)
				{
					PhysicsInfo & obj1 = *firstCollision.second.second.first;
					PhysicsInfo & obj2 = *firstCollision.second.second.second;

					ResolveCollision(obj1, obj2, firstCollision.second.first);
				}
			}

			ClearNetForce(i_dt);

			m_PhysicsInfoListLock.Release();
		}

		void PhysicsManager :: ResolveCollision(PhysicsInfo& obj1, PhysicsInfo& obj2, Vector3 collisionNormal)
		{
			float m1 = obj1.mass;
			float m2 = obj2.mass;

			Vector3 v1i = obj1.gameObject->velocity;
			Vector3 v2i = obj2.gameObject->velocity;
			Vector3 v1f = (v1i * (m1 - m2) + 2 * m2 * v2i) * (1.0f / (m1 + m2));
			Vector3 v2f = (v2i * (m2 - m1) + 2 * m1 * v1i) * (1.0f / (m1 + m2));

			obj1.gameObject->velocity = v1f;
			obj2.gameObject->velocity = v2f;
		}

		bool PhysicsManager::GetFirstCollidedObject(
			PhysicsInfo * i_info, 
			const float i_dt, 
			PhysicsInfo ** i_other,
			float & o_tCollision, 
			Vector3 & o_nCollision)
		{
			float tCollision = 1.0f; // normalized
			Vector3 nCollision = Vector3::Zero;
			PhysicsInfo * other = NULL;
			for (std::list< PhysicsInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				if ( i_info != (*it) ) 
				{
					float o_t = 1.0f;
					Vector3 o_n = Vector3::Zero;
					if (CollisionManager::Instance()->Check2DCollision(
						*(i_info->collider), i_info->netForce * (1.0f / i_info->mass),
						*((*it)->collider), (*it)->netForce * ( 1.0f / (*it)->mass ),
						i_dt,
						o_t,
						o_n))
					{
						if (o_t < tCollision) 
						{
							tCollision = o_t;
							nCollision = o_n;
							other = *it;
						}
					}
				}
			}

			o_tCollision = tCollision * i_dt; // de-normalize
			o_nCollision = nCollision;
			*i_other = other;

			return other != NULL;
		}

		void PhysicsManager :: ApplyDrag(const float i_dt)
		{
			for (std::list< PhysicsInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				PhysicsInfo * info = *it;
				const Vector3 drag = -info->dragK * info->gameObject->velocity;
				info->netForce += drag;
			}
		}

		void PhysicsManager::MoveGameObjects(const float i_dt)
		{
			for (std::list< PhysicsInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				PhysicsInfo * info = *it;
				const Vector3 acceleration = info->netForce * (1.0f / info->mass);

				Vector3 oldVelocity = info->gameObject->velocity;
				info->gameObject->velocity += acceleration * i_dt;

				Vector3 position = info->gameObject->position;
				info->gameObject->position = position + (oldVelocity + info->gameObject->velocity) * 0.5f * i_dt;
			}
		}

		void PhysicsManager :: ClearNetForce(const float i_dt)
		{
			for (std::list< PhysicsInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				PhysicsInfo * info = *it;
				info->netForce = Vector3::Zero;
			}
		}

		void PhysicsManager :: RemoveRigidBody(PhysicsInfo * i_Info)
		{
			m_Infos.remove(i_Info);
			delete i_Info;
		}

		bool PhysicsManager::AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff) const
		{
			return fabs(i_lhs - i_rhs) < i_maxDiff;
		}

		bool PhysicsManager::IsZero(float i_val) const
		{
			return AreEqual_Eps(0.0f, i_val, .000000001f);
		}

		PhysicsManager::~PhysicsManager(void)
		{
			for (std::list< PhysicsInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				delete (*it);
			}

			m_Infos.clear();
		}

		void Update(const float i_dt)
		{
			PhysicsManager::Instance()->Update(i_dt);
		}

		void ShutDown(void)
		{
			delete PhysicsManager::Instance();
			delete CollisionManager::Instance();
		}

	} // namespace Physics
} // namespace Illehc