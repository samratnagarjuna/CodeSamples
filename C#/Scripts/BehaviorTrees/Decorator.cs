using System.Collections.Generic;

namespace AISandbox
{
	public abstract class Decorator : BehaviorTree.Node {
		protected BehaviorTree.Node _childNode = null;

		public void SetChildNode(BehaviorTree.Node node) {
			_childNode = node;
		}

		public override void Init( Dictionary<string, object> dataContext) {
			_childNode.Init(dataContext);
		}
	}
}