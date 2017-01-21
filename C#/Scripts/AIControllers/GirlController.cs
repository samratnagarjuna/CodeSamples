using UnityEngine;
using System.Collections;
using AI;

[RequireComponent (typeof(Rigidbody))]
public class GirlController : MonoBehaviour {

	[System.Serializable]
	public struct GirlBehaviorParams {
		public float wanderRingDistance;
		public float wanderRingRadius;
		public float maxSpeed;
		public float maxForce;
	}

	private Rigidbody m_pursueRigidBody;
	public Rigidbody pursueRigidBody { get { return m_pursueRigidBody; } }

	private Rigidbody m_rb;
	public Rigidbody rb { get { return m_rb; } }

	[SerializeField]
	private GirlBehaviorParams m_behaviorParams;
	public GirlBehaviorParams behaviorParams {
		get { return m_behaviorParams; }
	}

	private StateMachine m_stateMachine;
	public StateMachine stateMachine { get { return m_stateMachine; } }

	private GIdleState m_idleState;
	private GFollowState m_followState;

	void Awake() {
		m_rb = GetComponent<Rigidbody>();
		InitializeStateMachine();
	}

	private void InitializeStateMachine() {
		m_stateMachine = new StateMachine();
		if ( !m_stateMachine.Initialize() ) {
			Debug.LogError("Unable to initialize StateMachine");
		}

		m_idleState = new GIdleState(this);
		m_stateMachine.AddState(m_idleState);

		m_followState = new GFollowState(this);
		m_stateMachine.AddState(m_followState);
	}

	void Start () {
		m_stateMachine.ChangeState(m_idleState);
	}

	void Update () {
		m_stateMachine.Update();
	}

	void FixedUpdate() {
		m_stateMachine.FixedUpdate();
	}

	void OnTriggerEnter(Collider other) {
		if ( other.tag.CompareTo("Cheater") == 0 && m_pursueRigidBody == null) {
			m_pursueRigidBody = other.gameObject.GetComponent<Rigidbody>();
			stateMachine.ChangeState(m_followState);
		}
	}

	void OnTriggerExit(Collider other) {
		if ( other.tag.CompareTo("Cheater") == 0 && m_pursueRigidBody != null) {
			if (m_pursueRigidBody == other.gameObject.GetComponent<Rigidbody>()) {
				m_pursueRigidBody = null;
				stateMachine.ChangeState(m_idleState);
			}
		}
	}
}
