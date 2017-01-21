using UnityEngine;
using System.Collections.Generic;

namespace AI {
	public class StateMachine {
		public abstract class State	{
			public abstract string Name { get; }
			public virtual void Enter() { }
			public virtual void Update() { }
			public virtual void FixedUpdate() { }
			public virtual void Exit() { }
		}

		private State currentState = null;
		private Dictionary<string, State> statesMap = null;

		public delegate void OnStateChangeEvent(State state);
		public event OnStateChangeEvent onStateChangeEventHandler;

		public bool Initialize() {
			statesMap = new Dictionary<string, State>();
			return true;
		}

		public bool Clear() {
			statesMap.Clear();
			currentState = null;
			return true;
		}

		public bool ClearState() {
			currentState = null;
			return true;
		}

		public bool AddState(State state) {

			string name = state.Name;
			if (statesMap.ContainsKey(name)) return false;

			statesMap[name] = state;
			return true;
		}

		public bool RemoveState(State state) {
			string name = state.Name;
			return RemoveState(name);
		}

		public bool RemoveState(string name) {
			if (!statesMap.ContainsKey(name)) return false;

			statesMap.Remove(name);
			return true;
		}

		public bool ChangeState(State state) {
			string name = state.Name;
			return ChangeState(name);
		}

		public bool ChangeState(string name) {
			if (!statesMap.ContainsKey(name)) return false;

			if (currentState != null) currentState.Exit();
			currentState = statesMap[name];
			currentState.Enter();

			if (onStateChangeEventHandler != null) 
				onStateChangeEventHandler(currentState);
			return true;
		}

		public void Update() {
			if (currentState != null) currentState.Update();
		}

		public void FixedUpdate() {
			if (currentState != null) currentState.FixedUpdate();
		}

		public string GetCurrentStateName()	{
			if (currentState != null) return currentState.Name;
			else return "";
		}
	}
}
