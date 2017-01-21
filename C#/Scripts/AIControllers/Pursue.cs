using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class Pursue : MonoBehaviour {
	private Rigidbody m_rb;

	[SerializeField]
	private Rigidbody pursueRigidBody;

	[SerializeField]
	private float maxSpeed;
	[SerializeField]
	private float maxForce;

	void Start () {
		m_rb = GetComponent<Rigidbody>();
	}

	void Update () {
		Vector3 steeringForce = SteeringUtility.Pursue(
			transform.position, m_rb.velocity, 
			pursueRigidBody.transform.position, pursueRigidBody.velocity, 
			maxSpeed, maxForce);

		m_rb.AddForce( steeringForce );
	}
}
