using UnityEngine;
using System.Collections.Generic;

public class FlockingUtility {

	public static Vector3 Alignment(
		Vector3 sourceVelocity,	List<Rigidbody> neighbours,
		float maxSpeed, float maxForce) {

		Vector3 desired = Vector3.zero;
		foreach ( Rigidbody neighbour in neighbours ) {
			desired += neighbour.velocity;
		}

		if ( neighbours.Count > 0 ) {
			desired /= neighbours.Count;
		}

		desired = desired.normalized * maxSpeed;

		Vector3 steering = desired - sourceVelocity;
		steering = Vector3.ClampMagnitude( steering, maxForce );

		return steering;
	}

	public static Vector3 Seperation(
		Vector3 sourcePosition, Vector3 sourceVelocity, 
		List<Rigidbody> neighbours,
		float desiredSeperation,
		float maxSpeed, float maxForce) {

		Vector3 desired = Vector3.zero;
		foreach ( Rigidbody neighbour in neighbours ) {
			Vector3 direction = sourcePosition - neighbour.transform.position;
			float distance = direction.magnitude;

			if ( distance > 0 && distance < desiredSeperation ) {
				desired += direction.normalized / distance;
			}
		}

		if ( neighbours.Count > 0 ) {
			desired /= neighbours.Count;
		}

		desired = desired.normalized * maxSpeed;

		Vector3 steering = desired - sourceVelocity;
		steering = Vector3.ClampMagnitude( steering, maxForce );

		return steering;
	}

	public static Vector3 Cohesion(
		Vector3 sourcePosition, Vector3 sourceVelocity,
		List<Rigidbody> neighbours,
		float maxSpeed, float maxForce) {

		Vector3 desired = Vector3.zero;
		foreach ( Rigidbody neighbour in neighbours ) {
			desired += neighbour.transform.position;
		}

		if ( neighbours.Count > 0 ) {
			desired /= neighbours.Count;
		}

		desired -= sourcePosition;

		desired = desired.normalized * maxSpeed;

		Vector3 steering = desired - sourceVelocity;
		steering = Vector3.ClampMagnitude( steering, maxForce );

		return steering;
	}
}
