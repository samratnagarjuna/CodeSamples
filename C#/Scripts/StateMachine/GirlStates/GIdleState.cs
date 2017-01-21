using UnityEngine;
using System.Collections;
using AI;

public class GIdleState : StateMachine.State {

	private readonly string STATE_NAME = "GirlIdleState";
	public override string Name
	{
		get
		{
			return STATE_NAME;
		}
	}

	private GirlController controller;

	public GIdleState(GirlController controller) {
		this.controller = controller;
	}

	public override void FixedUpdate() {
		
		Vector3 steeringForce = SteeringUtility.Wander(
			controller.transform.position, controller.rb.velocity,
			controller.behaviorParams.wanderRingDistance, 
			controller.behaviorParams.wanderRingRadius, 
			controller.behaviorParams.maxSpeed, 
			controller.behaviorParams.maxForce);

		controller.rb.AddForce( steeringForce );
	}
}
