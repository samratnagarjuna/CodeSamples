using UnityEngine;
using System.Collections.Generic;

namespace AISandbox {
	[RequireComponent (typeof(IActor))]
	public class NTankController : MonoBehaviour {

		private IActor actor = null;

		private Vector3 initialPosition = Vector3.zero;
		private Quaternion initialRotation = Quaternion.identity;

		public Genome genome = null;

		private double screenDiagonalDistance = 0.0;

		void Start () {
			initialPosition = transform.position;
			initialRotation = transform.rotation;

			actor = GetComponent<IActor>();


			Vector2 topLeftScreen = new Vector2(0, Screen.height);
			Vector3 topLeftWorld = Camera.main.ScreenToWorldPoint(topLeftScreen);
			Vector2 botRightScreen = new Vector2(Screen.width, 0);
			Vector3 botRightWorld = Camera.main.ScreenToWorldPoint(botRightScreen);

			screenDiagonalDistance = Vector3.Distance( topLeftWorld, botRightWorld );

		}
			
		void FixedUpdate () {
			Vector2 tankPosition = transform.position;

			int minesDetected = MinesManager.Instance.DetectAndRemoveMinesAt (tankPosition);
			genome.fitness += minesDetected;

			List<double> inputs = new List<double>();

			float nearestMineDistance = Mathf.Infinity;
			Vector2 nearestMinePosition = tankPosition;
			foreach ( Vector2 minePosition in MinesManager.Instance.Mines) {
				float mineDistance = Vector2.Distance( minePosition, tankPosition);
				if (  mineDistance < nearestMineDistance) {
					nearestMineDistance = mineDistance;
					nearestMinePosition = minePosition;
				}
			}

			Vector2 mineDirection = nearestMinePosition - tankPosition;
			float angle = AngleBetweenVector2( transform.up , mineDirection );
			double normalizedAngle = NormalizeAngle ( angle );
			inputs.Add ( normalizedAngle );

			inputs.Add ( nearestMineDistance / screenDiagonalDistance);
			// Debug.Log ( "Inputs: " + inputs[0] + "," + inputs[1] );

			NeuralGameManager.Instance.neuralNet.PutWeights(ref genome.genes);
			List<double> outputs = NeuralGameManager.Instance.neuralNet.Evaluate(inputs);

			// Debug.Log ( "Outputs: " + outputs[0] + "," + outputs[1] );
			actor.SetInput ( 0.0f, (float)outputs[0], 0.0f, (float)outputs[1] );
		}

		private float AngleBetweenVector2(Vector2 vec1, Vector2 vec2) {
			Vector2 difference = vec2 - vec1;
			float sign = (vec2.y < vec1.y)? -1.0f : 1.0f;
			return Vector2.Angle(Vector2.right, difference) * sign;
		}

		private double NormalizeAngle ( float angle ) {
			return angle / 180.0;
		}

		public void Reset() {
			transform.position = initialPosition;
			transform.rotation = initialRotation;
		}
	}
}
