using System.Collections.Generic;

namespace AISandbox
{
	public abstract class Leaf : BehaviorTree.Node {

		protected Dictionary<string, object> _dataContext = null;
		protected bool _debug = false;

		public override void Init(Dictionary<string, object> dataContext) {
			_dataContext = dataContext;
		}
	}
}