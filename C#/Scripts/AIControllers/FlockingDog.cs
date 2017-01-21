using UnityEngine;
using System.Collections.Generic;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class FlockingDog : MonoBehaviour {

	private Rigidbody m_rb;
	private List<Rigidbody> m_flock;

	[System.Serializable]
	private struct FlockingParams {
		public float alignmentWeight;
		public float seperatoinWeight;
		public float seperationDistance;
		public float cohesionWeight;
	}

	[SerializeField]
	private FlockingParams flockingParams;
	[SerializeField]
	private float maxSpeed;
	[SerializeField]
	private float maxForce;

	void Start () {
		m_rb = GetComponent<Rigidbody>();
		m_flock = new List<Rigidbody>();
		Init();
	}

	private void Init() {
		Vector3 randomForce = Random.onUnitSphere * maxForce;
		randomForce.y = 0;
		m_rb.AddForce ( randomForce );
	}

	void FixedUpdate () {
		Vector3 alignemntSteering = FlockingUtility.Alignment(m_rb.velocity, m_flock,
			maxSpeed, maxForce);
		
		Vector3 seperationSteering = FlockingUtility.Seperation(
			transform.position, m_rb.velocity, 
			m_flock, flockingParams.seperationDistance,
			maxSpeed, maxForce);

		Vector3 cohesionSteering = FlockingUtility.Cohesion(
			transform.position, m_rb.velocity,
			m_flock, maxSpeed, maxForce);

		Vector3 steering = alignemntSteering * flockingParams.alignmentWeight +
			seperationSteering * flockingParams.seperatoinWeight +
			cohesionSteering * flockingParams.cohesionWeight;
		m_rb.AddForce(steering);
	}

	void OnTriggerEnter(Collider other) {
		if ( other.tag.CompareTo("Dog") == 0) {
			m_flock.Add (other.gameObject.GetComponent<Rigidbody>());
		}
	}

	void OnTriggerExit(Collider other) {
		if ( other.tag.CompareTo("Dog") == 0) {
			m_flock.Remove (other.gameObject.GetComponent<Rigidbody>());
		}
	}
}
