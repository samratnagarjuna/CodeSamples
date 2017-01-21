using UnityEngine;
using System.Collections;

namespace AI {
	public class SteeringUtility {

		public static Vector3 Arrive(
			Vector3 sourcePosition, Vector3 sourceVelocity,
			Vector3 targetPosition,
			float maxSpeed, float maxForce, float dampingDistance) {

			Vector3 desired = targetPosition - sourcePosition;
			float distance = desired.magnitude;
			float speed = Mathf.Lerp( 0.0f, maxSpeed, distance / dampingDistance);
			desired = desired.normalized * speed;

			Vector3 steering = desired - sourceVelocity;
			steering = Vector3.ClampMagnitude( steering, maxForce );

			return steering;
			
		}

		public static Vector3 Seek(
			Vector3 sourcePosition, Vector3 sourceVelocity,
			Vector3 targetPosition,
			float maxSpeed, float maxForce) {

			Vector3 desired = targetPosition - sourcePosition;
			desired = desired.normalized * maxSpeed;

			Vector3 steering = desired - sourceVelocity;
			steering = Vector3.ClampMagnitude( steering, maxForce );

			return steering;
		}

		public static Vector3 Flee(
			Vector3 sourcePosition, Vector3 sourceVelocity,
			Vector3 targetPosition,
			float maxSpeed, float maxForce) {

			Vector3 desired = sourcePosition - targetPosition;
			desired = desired.normalized * maxSpeed;

			Vector3 steering = desired - sourceVelocity;
			steering = Vector3.ClampMagnitude( steering, maxForce );

			return steering;
		}

		public static Vector3 Pursue(
			Vector3 sourcePosition, Vector3 sourceVelocity,
			Vector3 targetPosition, Vector3 targetVelocity,
			float maxSpeed, float maxForce) {

			float distance = Vector3.Distance( sourcePosition, targetPosition );
			float time = distance / maxSpeed;

			Vector3 pursueTargetPosition = 
				targetPosition + targetVelocity.normalized * time;

			return Seek(sourcePosition, sourceVelocity,
				pursueTargetPosition,
				maxSpeed, maxForce);
		}

		public static Vector3 Evade(
			Vector3 sourcePosition, Vector3 sourceVelocity,
			Vector3 targetPosition, Vector3 targetVelocity,
			float maxSpeed, float maxForce) {

			float distance = Vector3.Distance( sourcePosition, targetPosition );
			float time = distance / maxSpeed;

			Vector3 evadeTargetPosition = 
				targetPosition + targetVelocity.normalized * time;

			return Flee(sourcePosition, sourceVelocity,
				evadeTargetPosition,
				maxSpeed, maxForce);
		}

		public static Vector3 Wander(
			Vector3 sourcePosition, Vector3 sourceVelocity,
			float wanderRingDistance, float wanderRingRadius,
			float maxSpeed, float maxForce) {

			Vector3 centerOfWanderRing = 
				sourcePosition + sourceVelocity.normalized * wanderRingDistance;
			Vector2 randomUnitCircle = 
				Random.insideUnitCircle.normalized * wanderRingRadius;
			Vector3 targetPosition = 
				centerOfWanderRing + 
				new Vector3(randomUnitCircle.x, 0.0f, randomUnitCircle.y);

			return Seek(sourcePosition, sourceVelocity, 
				targetPosition, 
				maxSpeed, maxForce);
		}
	}
}