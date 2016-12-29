using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace AISandbox {
	[RequireComponent (typeof(IActor))]
	public class TankController : MonoBehaviour {

		private static readonly float EXEC_TIME = 0.25f;
		private static readonly float MINE_RANGE = 1.0f;

		public Individual individual = null;
		public List<Vector2> mines = null;

		private IActor actor = null;
		private Vector2 command = Vector2.zero;
		private int commandIndex = -1;

		private Vector3 initialPosition = Vector3.zero;
		private Quaternion initialRotation = Quaternion.identity;

        private float commandTime = 0.0f;

		private Coroutine executingCoroutine = null;

		void Start () {

			initialPosition = transform.position;
			initialRotation = transform.rotation;

			actor = GetComponent<IActor>();
            commandTime = EXEC_TIME;
		}

		void FixedUpdate () {
            commandTime -= Time.fixedDeltaTime;

            if ( commandTime < 0 )
            {
                commandTime = EXEC_TIME;
                commandIndex++;
                if ( commandIndex < individual.dna.genes.Length)
                    command = individual.dna.genes[commandIndex];
            }

            if ( commandIndex < individual.dna.genes.Length)
            {
                actor.SetInput(0.0f, command[0], 0.0f, command[1]);
            }
            else actor.SetInput(0.0f, 0.0f, 0.0f, 0.0f);

            int totalMines = mines.Count;
            mines.RemoveAll(position => IsOnMine(position));
            int minesDetected = totalMines - mines.Count;

            individual.fitness += minesDetected;
			
		}

		private IEnumerator ExecuteCommands() {
			commandIndex = 0;

			if ( individual == null ) Debug.LogError("Commands not assgined to controller");
			else {
				while ( commandIndex < individual.dna.genes.Length ) {
					command = individual.dna.genes[ commandIndex ];
					yield return new WaitForSeconds ( EXEC_TIME );
					commandIndex++;
				}
			}
			command = Vector2.zero;
			executingCoroutine = null;
		}

		public void Reset() {
			transform.position = initialPosition;
			transform.rotation = initialRotation;
		}

		private void Stop() {
			if (executingCoroutine != null) StopCoroutine(executingCoroutine);

		}

		private bool IsOnMine (Vector2 minePosition) {
			return Vector2.Distance( minePosition, transform.position ) < MINE_RANGE;
		}
	}
}

