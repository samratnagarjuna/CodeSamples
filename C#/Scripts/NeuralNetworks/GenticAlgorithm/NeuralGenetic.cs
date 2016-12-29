using UnityEngine;
using System.Collections.Generic;

namespace AISandbox {
	public class NeuralGenetic {

		private float mutationRate = 0.0f;
		private float elitismRate = 0.0f;
		private int noOfGenomes = 0;
		private int noOfWeights = 0;

		public int generations { get; private set; }

		public List<Genome> population = null;

		public NeuralGenetic(float mutationRate, float elitismRate, int noOfGenomes, int noOfWeights) {
			this.mutationRate = mutationRate;
			this.elitismRate = elitismRate;
			this.noOfGenomes = noOfGenomes;
			this.noOfWeights = noOfWeights;

			generations = 0;

			InitializePopulation();
		}

		private void InitializePopulation() {
			population = new List<Genome>();
			for(int i = 0; i < noOfGenomes; i++) {
				population.Add(new Genome(noOfWeights));
			}
		}

		public void SelectAndReproduce() {
			
			population.Sort();
			population.Reverse();

			List<Genome> newPopulation = new List<Genome>();

			int noOfElitesToCopy = (int)(noOfGenomes * elitismRate);

			for(int i = 0; i < noOfElitesToCopy; i++) {
				newPopulation.Add (new Genome(population[i].genes, 0.0));
			}

			// Generate weights for selection
			double[] weights = new double[noOfGenomes];

			for (int i = 0; i < weights.Length; i++)
			{
				weights[i] = population[i].fitness;
			}

			int noOfGenomesLeft = noOfGenomes - noOfElitesToCopy;
			for(int i = 0; i < noOfGenomesLeft; i++) {
				int m = SelectionUtility.RouletteSelect( weights );
				int d = SelectionUtility.RouletteSelect( weights );

				Genome mom = population[m];
				Genome dad = population[d];

				Genome child = mom.CrossOver(dad);
				child.Mutate(mutationRate);

				newPopulation.Add(child);
			}

			population = newPopulation;
			generations++;
		}
	}
}
