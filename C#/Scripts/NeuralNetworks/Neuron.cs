using UnityEngine;

namespace AISandbox{
	public struct Neuron {
		public int noOfInputs;
		public double[] weights;

		public Neuron(int noOfInputs) {
			// bias as additional input
			this.noOfInputs = noOfInputs + 1;
			this.weights = new double[this.noOfInputs];

			InitializeWeights();
		}

		private void InitializeWeights() {
			for(int i = 0; i < noOfInputs; i++) {
				weights[i] = Random.value - Random.value;
			}
		}
	}
}
