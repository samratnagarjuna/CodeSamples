using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class PlayerController : MonoBehaviour {

	private Rigidbody m_rb;
	private float m_y;

	private readonly int m_touchIndex = 0;
	private readonly float m_hitDistance = 100;

	private Vector3 m_target;

	[SerializeField]
	private float maxSpeed;
	[SerializeField]
	private float maxForce;
	[SerializeField]
	private float dampingDistance;

	[SerializeField]
	private float killingDistance;

	[SerializeField]
	private Transform girlTransform;

	void Start () {
		m_rb = GetComponent<Rigidbody>();
		m_target = transform.position;
		m_y = transform.position.y;
	}

	void Update () {
		if ( Input.GetMouseButtonDown ( m_touchIndex ) ) {
			RaycastHit hit;
			Ray ray = Camera.main.ScreenPointToRay (Input.mousePosition);

			if ( Physics.Raycast (ray, out hit, m_hitDistance) ) {
				
				if (hit.transform.gameObject.tag == "Plane") {
					m_target = hit.point;
					m_target.y = m_y;
				}
				else if (
					(hit.transform.gameObject.tag == "Cheater" ||
						hit.transform.gameObject.tag == "Enemy" ) &&
					Vector3.Distance(hit.transform.position, 
						girlTransform.transform.position) < killingDistance) {
					hit.transform.gameObject
						.GetComponent<Destroyer>().DestroyEntity();
				}
			}
		}
	}

	void FixedUpdate() {
		Vector3 steeringForce = SteeringUtility.Arrive( 
			transform.position, m_rb.velocity,
			m_target,
			maxSpeed, maxForce, dampingDistance);

		m_rb.AddForce ( steeringForce );
	}
}
