using System.Collections.Generic;

namespace AISandbox
{
	public class Sequence : Composite {

		private int _processingNodeIndex = -1;
		private BehaviorTree.Result _lastResult;

		public Sequence() : base() {
			
		}

		public override void Init (Dictionary<string, object> dataContext)
		{
			base.Init (dataContext);
			_lastResult = BehaviorTree.Result.SUCCESS;
			_processingNodeIndex = 0;
		}
			
		public override BehaviorTree.Result Process() {

			if ( _lastResult == BehaviorTree.Result.FAILURE ) return _lastResult;

			while ( !AreAllNodesProcessed() && (_lastResult = _nodes[_processingNodeIndex].Process()) == BehaviorTree.Result.SUCCESS) {
				_processingNodeIndex++;
			}

			return _lastResult;
		}

		private bool AreAllNodesProcessed() {
			return (_processingNodeIndex == _nodes.Count);
		}
	}
}