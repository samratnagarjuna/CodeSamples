using UnityEngine;
using System.Collections;

namespace AISandbox {
	public class SetVariableTo : Leaf {
		private static readonly string NAME = "SetVariableTo";

		private string _key = null;
		private object _value = null;

		private BehaviorTree.Result _lastResult;
		private BehaviorTree.Result _newResult;

		public SetVariableTo(string key, object value) : base() {
			_key = key;
			_value = value;
		}

		public override void Init (System.Collections.Generic.Dictionary<string, object> dataContext)
		{
			base.Init (dataContext);
			InitLog();
		}

		public override BehaviorTree.Result Process() {
			if (_lastResult != _newResult) Log();

			_lastResult = _newResult;

			_dataContext[_key] = _value;
			_newResult = BehaviorTree.Result.SUCCESS;

			if (_lastResult != _newResult) Log();

			return _newResult;
		}

		private void InitLog() {
			_lastResult = BehaviorTree.Result.FAILURE;
			_newResult = BehaviorTree.Result.RUNNING;
		}

		private void Log() {
			if (_debug)
				BTLog.instance.Append(NAME + "(" + _key + "," + _value + "): " + _newResult + "\n");
		}
	}
}
