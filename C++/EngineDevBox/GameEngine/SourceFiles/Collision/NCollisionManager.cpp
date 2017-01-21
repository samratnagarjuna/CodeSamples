#include "Collision\NCollisionManager.h"

#include "Math\NMatrix4.h"
#include "Utility\NConsolePrint.h"
#include "Physics\NPhysicsInfo.h"
#include <cmath>

namespace Illehc
{
	namespace Physics
	{
		CollisionManager * CollisionManager::_instance = 0;

		CollisionManager * CollisionManager::Instance(void)
		{
			if (_instance == 0) {
				_instance = new CollisionManager();
			}
			return _instance;
		}

		CollisionManager :: CollisionManager() : m_CollisionInfoListLock("CollisionInfoListLock")
		{

		}

		void CollisionManager :: Init(void)
		{

		}

		AABB * CollisionManager::AddAABBCollider(SmartPtr<GameObject> & i_GameObject, const Vector3 i_Center, const Vector3 i_Extents)
		{
			AABB * collider = new AABB;
			collider->gameObject = i_GameObject;
			collider->center = i_Center;
			collider->extents = i_Extents;

			m_CollisionInfoListLock.Lock();

			m_Colliders.push_back(collider);

			m_CollisionInfoListLock.Release();

			return collider;
		}

		void CollisionManager::RemoveCollider(AABB * i_Collider)
		{
			m_Colliders.remove(i_Collider);
			delete i_Collider;
		}

		Vector3 CollisionManager :: VelocityInTime(const AABB & i_Obj, const Vector3 i_Acceleration, const float i_dt)
		{
			const Vector3 acceleration = i_Acceleration * i_dt;
			const Vector3 velocity = i_Obj.gameObject->velocity;
			return (velocity + velocity + acceleration) * 0.5f;
		}

		bool CollisionManager::Check2DCollision(
			const AABB & i_Obj1AABB, const Vector3 i_Obj1Acceleration,
			const AABB & i_Obj2AABB, const Vector3 i_Obj2Acceleration,
			float i_dt,
			float & o_tCollision,
			Vector3 & o_nCollision)
		{
			GameObject & i_Obj1 = *(i_Obj1AABB.gameObject);
			GameObject & i_Obj2 = *(i_Obj2AABB.gameObject);

			Matrix4 Obj1ToWorld = Matrix4::CreateTranslationCV(i_Obj1.position) * i_Obj1.orientation.ToTransform();
			Matrix4 Obj2ToWorld = Matrix4::CreateTranslationCV(i_Obj2.position) * i_Obj2.orientation.ToTransform();

			Vector3 Obj1BBCenterInWorld = Obj1ToWorld.TransformPointCV(i_Obj1AABB.center);
			Vector3 Obj2BBCenterInWorld = Obj2ToWorld.TransformPointCV(i_Obj2AABB.center);

			float tEnter = 0.0f;
			float tExit = 1.0f;
			Vector3 normal = Vector3::Zero;

			// Obj2 vs Obj1
			Vector3 Obj2RelVel = i_Obj2.velocity - i_Obj1.velocity; // Modified
			//Vector3 Obj2RelVel = VelocityInTime(i_Obj2AABB, i_Obj2Acceleration, i_dt) - VelocityInTime(i_Obj1AABB, i_Obj1Acceleration, i_dt);
			Vector3 Obj2Travel = Obj2RelVel * i_dt;

			// Obj1 X Axis
			{
				Vector3 Obj1XAxisInWorld = i_Obj1.orientation.Col(0).Normalized();

				float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj1XAxisInWorld);
				float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj1XAxisInWorld);

				float Obj2ProjectedExtents =
					fabs(dot(Obj1XAxisInWorld, i_Obj2.orientation.Col(0).Normalized() * i_Obj2AABB.extents.getX())) +
					fabs(dot(Obj1XAxisInWorld, i_Obj2.orientation.Col(1).Normalized() * i_Obj2AABB.extents.getY())) +
					fabs(dot(Obj1XAxisInWorld, i_Obj2.orientation.Col(2).Normalized() * i_Obj2AABB.extents.getZ()));

				float Obj1ExpandedExtents = i_Obj1AABB.extents.getX() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1XAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

				//DEBUG_PRINT("Found Overlap along Obj1 X Axis from %f to %f\n", axisEnter, axisExit);

				if (axisEnter > tEnter) 
				{
					tEnter = axisEnter;
					normal = Obj1XAxisInWorld;
					//DEBUG_PRINT("Nobj1X = (%f, %f, %f)\n", normal.getX(), normal.getY(), normal.getZ());
				}
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj1 Y Axis
			{
				Vector3 Obj1YAxisInWorld = i_Obj1.orientation.Col(1).Normalized();

				float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj1YAxisInWorld);
				float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj1YAxisInWorld);

				float Obj2ProjectedExtents =
					fabs(dot(Obj1YAxisInWorld, i_Obj2.orientation.Col(0).Normalized() * i_Obj2AABB.extents.getX())) +
					fabs(dot(Obj1YAxisInWorld, i_Obj2.orientation.Col(1).Normalized() * i_Obj2AABB.extents.getY())) +
					fabs(dot(Obj1YAxisInWorld, i_Obj2.orientation.Col(2).Normalized() * i_Obj2AABB.extents.getZ()));

				float Obj1ExpandedExtents = i_Obj1AABB.extents.getY() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1YAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

				//DEBUG_PRINT("Found Overlap along Obj1 Y Axis from %f to %f\n", axisEnter, axisExit);

				if (axisEnter > tEnter)
				{
					tEnter = axisEnter;
					normal = Obj1YAxisInWorld;
					//DEBUG_PRINT("Nobj1Y = (%f, %f, %f)\n", normal.getX(), normal.getY(), normal.getZ());
				}
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj1 Z Axis
			{
				Vector3 Obj1ZAxisInWorld = i_Obj1.orientation.Col(2).Normalized();

				float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj1ZAxisInWorld);
				float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj1ZAxisInWorld);

				float Obj2ProjectedExtents =
					fabs(dot(Obj1ZAxisInWorld, i_Obj2.orientation.Col(0).Normalized() * i_Obj2AABB.extents.getX())) +
					fabs(dot(Obj1ZAxisInWorld, i_Obj2.orientation.Col(1).Normalized() * i_Obj2AABB.extents.getY())) +
					fabs(dot(Obj1ZAxisInWorld, i_Obj2.orientation.Col(2).Normalized() * i_Obj2AABB.extents.getZ()));

				float Obj1ExpandedExtents = i_Obj1AABB.extents.getZ() + Obj2ProjectedExtents;

				float Obj2TravelAlongAxis = dot(Obj2Travel, Obj1ZAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj1BBCenterOnAxis, Obj1ExpandedExtents, Obj2BBCenterOnAxis, Obj2TravelAlongAxis, axisEnter, axisExit))
					return false;

				//DEBUG_PRINT("Found Overlap along Obj1 Z Axis from %f to %f\n", axisEnter, axisExit);

				if (axisEnter > tEnter)
				{
					tEnter = axisEnter;
					normal = Obj1ZAxisInWorld;
					//DEBUG_PRINT("Nobj1Z = (%f, %f, %f)\n", normal.getX(), normal.getY(), normal.getZ());
				}
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj1 vs Obj2
			Vector3 Obj1RelVel = i_Obj1.velocity - i_Obj2.velocity; // modified
			//Vector3 Obj1RelVel = VelocityInTime(i_Obj1AABB, i_Obj1Acceleration, i_dt) - VelocityInTime(i_Obj2AABB, i_Obj2Acceleration, i_dt);
			Vector3 Obj1Travel = Obj1RelVel * i_dt;

			// Obj2 X Axis
			{
				Vector3 Obj2XAxisInWorld = i_Obj2.orientation.Col(0).Normalized();

				float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj2XAxisInWorld);
				float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj2XAxisInWorld);

				float Obj1ProjectedExtents =
					fabs(dot(Obj2XAxisInWorld, i_Obj1.orientation.Col(0).Normalized() * i_Obj1AABB.extents.getX())) +
					fabs(dot(Obj2XAxisInWorld, i_Obj1.orientation.Col(1).Normalized() * i_Obj1AABB.extents.getY())) +
					fabs(dot(Obj2XAxisInWorld, i_Obj1.orientation.Col(2).Normalized() * i_Obj1AABB.extents.getZ()));

				float Obj2ExpandedExtents = i_Obj2AABB.extents.getX() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2XAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				//DEBUG_PRINT("Found Overlap along Obj2 X Axis from %f to %f\n", axisEnter, axisExit);

				if (axisEnter > tEnter)
				{
					tEnter = axisEnter;
					normal = Obj2XAxisInWorld;
					//DEBUG_PRINT("Nobj2X = (%f, %f, %f)\n", normal.getX(), normal.getY(), normal.getZ());
				}
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj2 Y Axis
			{
				Vector3 Obj2YAxisInWorld = i_Obj2.orientation.Col(1).Normalized();

				float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj2YAxisInWorld);
				float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj2YAxisInWorld);

				float Obj1ProjectedExtents =
					fabs(dot(Obj2YAxisInWorld, i_Obj1.orientation.Col(0).Normalized() * i_Obj1AABB.extents.getX())) +
					fabs(dot(Obj2YAxisInWorld, i_Obj1.orientation.Col(1).Normalized() * i_Obj1AABB.extents.getY())) +
					fabs(dot(Obj2YAxisInWorld, i_Obj1.orientation.Col(2).Normalized() * i_Obj1AABB.extents.getZ()));

				float Obj2ExpandedExtents = i_Obj2AABB.extents.getY() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2YAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				//DEBUG_PRINT("Found Overlap along Obj2 Y Axis from %f to %f\n", axisEnter, axisExit);

				if (axisEnter > tEnter)
				{
					tEnter = axisEnter;
					normal = Obj2YAxisInWorld;
					//DEBUG_PRINT("Nobj2Y = (%f, %f, %f)\n", normal.getX(), normal.getY(), normal.getZ());
				}
				if (axisExit < tExit)
					tExit = axisExit;
			}

			// Obj2 Z Axis
			{
				Vector3 Obj2ZAxisInWorld = i_Obj2.orientation.Col(2).Normalized();

				float Obj2BBCenterOnAxis = dot(Obj2BBCenterInWorld, Obj2ZAxisInWorld);
				float Obj1BBCenterOnAxis = dot(Obj1BBCenterInWorld, Obj2ZAxisInWorld);

				float Obj1ProjectedExtents =
					fabs(dot(Obj2ZAxisInWorld, i_Obj1.orientation.Col(0).Normalized() * i_Obj1AABB.extents.getX())) +
					fabs(dot(Obj2ZAxisInWorld, i_Obj1.orientation.Col(1).Normalized() * i_Obj1AABB.extents.getY())) +
					fabs(dot(Obj2ZAxisInWorld, i_Obj1.orientation.Col(2).Normalized() * i_Obj1AABB.extents.getZ()));

				float Obj2ExpandedExtents = i_Obj2AABB.extents.getZ() + Obj1ProjectedExtents;

				float Obj1TravelAlongAxis = dot(Obj1Travel, Obj2ZAxisInWorld);

				float axisEnter = 0.0f;
				float axisExit = 1.0f;

				if (!DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, axisEnter, axisExit))
					return false;

				//DEBUG_PRINT("Found Overlap along Obj2 Z Axis from %f to %f\n", axisEnter, axisExit);

				if (axisEnter > tEnter)
				{
					tEnter = axisEnter;
					normal = Obj2ZAxisInWorld;
					//DEBUG_PRINT("Nobj2Z = (%f, %f, %f)\n", normal.getX(), normal.getY(), normal.getZ());
				}
				if (axisExit < tExit)
					tExit = axisExit;
			}

			o_tCollision = tEnter;
			o_nCollision = normal;

			return tEnter < tExit;
		}

		bool CollisionManager :: CheckSeparationAlongAxis(
			Vector3 & i_CollisionAxis,
			const AABB & i_Obj1AABB, const Vector3 & i_Obj1BBCenterInWorld,
			const AABB & i_Obj2AABB, const Vector3 & i_Obj2BBCenterInWorld,
			float i_dt,
			float & i_tEnter, float & i_tExit)
		{
			GameObject & i_Obj1 = *(i_Obj1AABB.gameObject);
			GameObject & i_Obj2 = *(i_Obj2AABB.gameObject);

			float Obj1BBCenterOnAxis = dot(i_Obj1BBCenterInWorld, i_CollisionAxis);
			float Obj2BBCenterOnAxis = dot(i_Obj2BBCenterInWorld, i_CollisionAxis);

			float Obj1ProjectedExtents =
				fabs(dot(i_CollisionAxis, i_Obj1.orientation.Col(0).Normalized() * i_Obj1AABB.extents.getX())) +
				fabs(dot(i_CollisionAxis, i_Obj1.orientation.Col(1).Normalized() * i_Obj1AABB.extents.getY())) +
				fabs(dot(i_CollisionAxis, i_Obj1.orientation.Col(2).Normalized() * i_Obj1AABB.extents.getZ()));

			float Obj2ProjectedExtents =
				fabs(dot(i_CollisionAxis, i_Obj2.orientation.Col(0).Normalized() * i_Obj2AABB.extents.getX())) +
				fabs(dot(i_CollisionAxis, i_Obj2.orientation.Col(1).Normalized() * i_Obj2AABB.extents.getY())) +
				fabs(dot(i_CollisionAxis, i_Obj2.orientation.Col(2).Normalized() * i_Obj2AABB.extents.getZ()));

			float Obj2ExpandedExtents = Obj2ProjectedExtents + Obj1ProjectedExtents;

			float Obj1VelOnAxis = dot(i_Obj1.velocity, i_CollisionAxis);
			float Obj2VelOnAxis = dot(i_Obj2.velocity, i_CollisionAxis);

			float Obj1RelVel = Obj1VelOnAxis - Obj2VelOnAxis;

			float Obj1TravelAlongAxis = Obj1RelVel * i_dt;

			i_tEnter = 0.0f;
			i_tExit = 1.0f;

			return !DetectCrossTimes(Obj2BBCenterOnAxis, Obj2ExpandedExtents, Obj1BBCenterOnAxis, Obj1TravelAlongAxis, i_tEnter, i_tExit);
		}

		bool CollisionManager :: DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float & o_tEnter, float & o_tExit)
		{
			assert(i_Extent > 0.0f);

			float i_Start = i_Center - i_Extent;
			float i_End = i_Center + i_Extent;

			if (IsZero(i_Travel))
			{
				if ((i_Point < i_Start) || (i_Point > i_End))
					return false;
				else
				{
					o_tEnter = 0.0f;
					o_tExit = 1.0f;
					return true;
				}
			}
			o_tEnter = (i_Start - i_Point) / i_Travel;
			o_tExit = (i_End - i_Point) / i_Travel;

			if (o_tEnter > o_tExit)
				Swap(o_tEnter, o_tExit);

			return !((o_tEnter >= 1.0f) || (o_tExit <= 0.0f));
		}

		inline bool CollisionManager :: AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff) const
		{
			return fabs(i_lhs - i_rhs) < i_maxDiff;
		}

		inline bool CollisionManager :: IsZero(float i_val) const
		{
			return AreEqual_Eps(0.0f, i_val, .000000001f);
		}

		void CollisionManager :: Swap(float & i_lhs, float & i_rhs)
		{
			float tmp = i_lhs;
			i_lhs = i_rhs;
			i_rhs = tmp;
		}

		CollisionManager :: ~CollisionManager(void)
		{
			for (std::list< AABB * >::iterator it = m_Colliders.begin(); it != m_Colliders.end(); it++)
			{
				delete (*it);
			}

			m_Colliders.clear();
		}
	} // namespace Physics
} // namespace Illehc