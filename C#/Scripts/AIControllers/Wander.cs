using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class Wander : MonoBehaviour {
	private Rigidbody m_rb;

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
		Vector3 steeringForce = SteeringUtility.Wander(
			transform.position, m_rb.velocity,
			wanderRingDistance, wanderRingRadius, 
			maxSpeed, maxForce);

		m_rb.AddForce( steeringForce );
	}
}
