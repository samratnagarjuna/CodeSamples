using UnityEngine;
using System.Collections;

namespace AISandbox {
	public class SendMessage : Leaf {

		private string _message;

		public SendMessage(string message) : base(){
			_message = message;
			_debug = true;
		}

		public override BehaviorTree.Result Process ()
		{
			if (_debug)
				BTLog.instance.Append(_message + "\n");
			return BehaviorTree.Result.SUCCESS;
		}
	}

}