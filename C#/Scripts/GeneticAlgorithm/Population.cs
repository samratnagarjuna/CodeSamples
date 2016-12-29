using UnityEngine;
using System;
using System.Collections.Generic;

namespace AISandbox {
	public class Population {

		private float mutationRate 	= 0.0f;
		private int noOfIndividuals = 0;
		private int noOfMines = 0;

		public int generations { get; private set; }

		public Individual[] individuals = null;

		public Population( float mutationRate, int noOfIndividuals, int noOfMines) {
			this.mutationRate = mutationRate;
			this.noOfIndividuals = noOfIndividuals;
			this.noOfMines = noOfMines;

			generations = 0;

			individuals = new Individual[ noOfIndividuals ];
			for (int i = 0; i < noOfIndividuals; i++ ) {
				individuals[i] = new Individual(new DNA());	
			}
		}

		public void SelectAndReproduce() {


            // Pick elites
            Array.Sort(individuals);
            Array.Reverse(individuals);

            Individual[] nextGenIndividuals = new Individual[ noOfIndividuals ];

            // Pass on the elites
            if (noOfIndividuals > 0)
            {
                individuals[0].fitness = 0.0f;
                nextGenIndividuals[0] = individuals[0];
            }
            if (noOfIndividuals > 1)
            {
                individuals[1].fitness = 0.0f;
                nextGenIndividuals[1] = individuals[1];
            }

            // Generate weights for selection
            double[] weights = new double[individuals.Length];

            for (int i = 0; i < weights.Length; i++)
            {
                weights[i] = individuals[i].fitness;
            }

            for ( int i = 2; i < noOfIndividuals; i++ ) {
				int m = SelectionUtility.RouletteSelect( weights );
				int d = SelectionUtility.RouletteSelect( weights );

				Individual mom = individuals[m];
				Individual dad = individuals[d];

				DNA momGenes = mom.dna;
				DNA dadGenes = dad.dna;

				DNA child = momGenes.CrossOver( dadGenes );
				child.Mutate ( mutationRate );

				nextGenIndividuals[i] = new Individual(child);
			}

			individuals = nextGenIndividuals;
			generations++;
		}

		public bool IsTargetReached() {
			bool targetReached = false;
			double maxFitness = -Mathf.Infinity;
			foreach ( Individual individual in individuals ) {
				if ( maxFitness < individual.fitness ) maxFitness = individual.fitness;
				if ( individual.fitness == noOfMines ) { 
					targetReached = true;
					break;
				}
			}
			Debug.Log( maxFitness );	
			return targetReached;
		}
		
	}
}
