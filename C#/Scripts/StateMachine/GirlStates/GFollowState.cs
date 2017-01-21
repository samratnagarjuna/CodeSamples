using UnityEngine;
using System.Collections;
using AI;

public class GFollowState : StateMachine.State {

	private readonly string STATE_NAME = "GirlFollowState";
	public override string Name
	{
		get
		{
			return STATE_NAME;
		}
	}

	private GirlController controller;

	public GFollowState(GirlController controller) {
		this.controller = controller;
	}

	public override void FixedUpdate() {

		if ( controller.pursueRigidBody != null) {
			Vector3 steeringForce = SteeringUtility.Pursue(
				controller.transform.position, controller.rb.velocity, 
				controller.pursueRigidBody.transform.position, 
				controller.pursueRigidBody.velocity, 
				controller.behaviorParams.maxSpeed, 
				controller.behaviorParams.maxForce);

			controller.rb.AddForce( steeringForce );
		}
		else controller.stateMachine.ChangeState("GirlIdleState");
	}
}
