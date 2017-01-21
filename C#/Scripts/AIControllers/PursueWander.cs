using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class PursueWander : MonoBehaviour {
	private Rigidbody m_rb;

	[SerializeField]
	private Rigidbody pursueRigidBody;

	[SerializeField]
	private float pursueDistance;

	[SerializeField]
	private float wanderRingDistance;
	[SerializeField]
	private float wanderRingRadius;
	[SerializeField]
	private float maxSpeed;
	[SerializeField]
	private float maxForce;

	void Start () {
		m_rb = GetComponent<Rigidbody>();
	}

	void FixedUpdate () {
		if ( Vector3.Distance( 
			pursueRigidBody.transform.position, 
			transform.position) < pursueDistance ) {
			Vector3 steeringForce = SteeringUtility.Pursue(
				transform.position, m_rb.velocity, 
				pursueRigidBody.transform.position, pursueRigidBody.velocity, 
				maxSpeed, maxForce);

			m_rb.AddForce( steeringForce );
		}
		else {
			Vector3 steeringForce = SteeringUtility.Wander(
				transform.position, m_rb.velocity,
				wanderRingDistance, wanderRingRadius, 
				maxSpeed, maxForce);

			m_rb.AddForce( steeringForce );
		}
	}
}
