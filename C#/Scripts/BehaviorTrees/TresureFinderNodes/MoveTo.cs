using System.Collections.Generic;
using UnityEngine;

namespace AISandbox
{
	public class MoveTo : Leaf {

		private static readonly string NAME = "MoveTo";

		private AStarController _controller = null;
		private GridNode _goalNode = null;
		private bool _reachedDestination = false;

		private bool _isGoalSet = false;
		private BehaviorTree.Result _lastResult;
		private BehaviorTree.Result _newResult;

		public MoveTo (AStarController controller, GridNode goalNode) : base() {
			_controller = controller;
			_goalNode = goalNode;
			_controller.ReachedGoalEvent += new AStarController.ReachedGoalEventHandler(OnReachedGoal);
		}
		public override void Init ( Dictionary<string, object> dataContext) {
			base.Init(dataContext);
			_isGoalSet = false;
			_reachedDestination = false;
			_lastResult = BehaviorTree.Result.FAILURE;
			_newResult = BehaviorTree.Result.RUNNING;
		}

		public override BehaviorTree.Result Process() {

			if ( _lastResult != _newResult) Log();

			_lastResult = _newResult;
			if ( _newResult == BehaviorTree.Result.RUNNING) {
				if ( !_isGoalSet )  {
					bool pathFound = _controller.SetGoalIfPathExists(_goalNode);
					if ( !pathFound) _newResult = BehaviorTree.Result.FAILURE;
					else _isGoalSet = true;
				}

				if (_reachedDestination == true) _newResult = BehaviorTree.Result.SUCCESS;
			}

			if (_lastResult != _newResult) Log();
			return _newResult;
		}

		public void OnReachedGoal(GridNode goalNode) {
			if (_goalNode == goalNode)	_reachedDestination = true;
		}

		private void Log() {
			if (_debug)
				BTLog.instance.Append(NAME + "(" + _goalNode + "): " + _newResult + "\n");
		}

	}

}
