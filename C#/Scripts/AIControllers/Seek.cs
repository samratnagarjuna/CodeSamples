using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class Seek : MonoBehaviour {
	private Rigidbody m_rb;

	[SerializeField]
	private Rigidbody girlRigidBody;

	[SerializeField]
	private float maxSpeed;
	[SerializeField]
	private float maxForce;

	void Start () {
		m_rb = GetComponent<Rigidbody>();
	}

	void FixedUpdate () {
		Vector3 steeringForce = SteeringUtility.Seek(
			transform.position, m_rb.velocity, 
			girlRigidBody.transform.position, 
			maxSpeed, maxForce);

		m_rb.AddForce( steeringForce );
	}
}