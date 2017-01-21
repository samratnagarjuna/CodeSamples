using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class Evade : MonoBehaviour {
	private Rigidbody m_rb;

	[SerializeField]
	private Rigidbody evadeRigidBody;

	[SerializeField]
	private float evadeDistance;

	[SerializeField]
	private float maxSpeed;
	[SerializeField]
	private float maxForce;

	void Start () {
		m_rb = GetComponent<Rigidbody>();
	}

	void Update () {

		if ( Vector3.Distance( 
			evadeRigidBody.transform.position, 
			transform.position) < evadeDistance ) {
			Vector3 steeringForce = SteeringUtility.Evade(
				transform.position, m_rb.velocity, 
				evadeRigidBody.transform.position, evadeRigidBody.velocity, 
				maxSpeed, maxForce);

			m_rb.AddForce( steeringForce );
		}
	}
}
