using UnityEngine;

namespace AISandbox {
	
	public class SelectionUtility {

		// Returns the selected index based on the weights(probabilities)
		public static int RouletteSelect(double[] weights) {
			// calculate the total weight
			double weightSum = 0;
			for(int i = 0; i < weights.Length; i++) {
				weightSum += weights[i];
			}

			// get a random value
			double value = Random.value * weightSum;

			// locate the random value based on the weights
			for(int i = 0; i < weights.Length; i++) {		
				value -= weights[i];		
				if(value <= 0) return i;
			}

			// when rounding errors occur, we return the last item's index 
			return weights.Length - 1;
		}
	}
}
