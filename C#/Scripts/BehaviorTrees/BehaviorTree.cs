using System.Collections.Generic;

namespace AISandbox 
{
	public class BehaviorTree {
		public enum Result {
			FAILURE,
			SUCCESS,
			RUNNING
		}

		public abstract class Node {
			public virtual void Init( Dictionary<string, object> dataContext ) { }
			public abstract Result Process();
		}
			
		private Node _rootNode;

		public BehaviorTree( Node rootNode ) {
			_rootNode = rootNode;
		}

		public void Init(Dictionary<string, object> dataContext) {
			_rootNode.Init( dataContext );
		}

		public Result Process() {
			return _rootNode.Process();
		}
	}
}