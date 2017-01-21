using System.Collections.Generic;

namespace AISandbox
{
	public abstract class Composite : BehaviorTree.Node 
	{
		protected List<BehaviorTree.Node> _nodes = null;

		public Composite() : base() {
			_nodes = new List<BehaviorTree.Node>();
		}

		public void AddNode(BehaviorTree.Node node) {
			_nodes.Add (node);
		}

		public override void Init( Dictionary<string, object> dataContext) {
			foreach ( BehaviorTree.Node node in _nodes ) {
				node.Init(dataContext);
			}
		}
	}
}
