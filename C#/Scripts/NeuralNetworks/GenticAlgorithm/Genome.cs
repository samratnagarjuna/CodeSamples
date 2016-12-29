using UnityEngine;
using System;
using System.Collections.Generic;

namespace AISandbox {
	public class Genome : IComparable<Genome>{

		private static readonly double maxPerturbation = 0.3;

		public List<double> genes;
		public double fitness;

		public Genome(int noOfGenes) {
			this.fitness = 0.0;
			this.genes = new List<double>();

			for(int i = 0; i < noOfGenes; i++) {
				genes.Add(UnityEngine.Random.value - UnityEngine.Random.value);
			}
		}

		public Genome(List<double> genes, double fitness ) {
			this.fitness = fitness;
			this.genes = new List<double>();

			foreach( double gene in genes ) {
				this.genes.Add(gene);
			}
		}

		public int CompareTo(Genome other) {
			return fitness.CompareTo(other.fitness);
		}

		public Genome CrossOver(Genome partner) {
			List<double> childGenes = new List<double>();

			if ( this == partner ) {
				return new Genome( genes, 0.0);
			}

			int crossOverIndex = (int)UnityEngine.Random.Range(0, genes.Count - 1);
			for(int i = 0 ; i < genes.Count; i++) {
				childGenes.Add ( ( i > crossOverIndex ) ? genes[i] : partner.genes[i] );
			}

			return new Genome(childGenes, 0.0);
		}

		public void Mutate(float probability) {
			for(int i = 0; i < genes.Count; i++) {
				if ( UnityEngine.Random.value < probability ) {
					genes[i] += UnityEngine.Random.value * maxPerturbation;
				}
			}
		}
	}
}
