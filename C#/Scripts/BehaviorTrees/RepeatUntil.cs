using UnityEngine;
using System.Collections;

namespace AISandbox 
{
	public class RepeatUntil : Decorator {

		private BehaviorTree.Result _expectedResult;

		public RepeatUntil(BehaviorTree.Result expectedResult) : base() {
			_expectedResult = expectedResult;
		}

		public override BehaviorTree.Result Process() {
			if ( _childNode.Process() == _expectedResult ) return BehaviorTree.Result.SUCCESS;
			else return BehaviorTree.Result.RUNNING;
		}
	}
}
