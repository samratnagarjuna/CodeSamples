using UnityEngine;
using System.Collections;
using System;

namespace AISandbox {
	public class Individual : IComparable<Individual> {
		public DNA dna = null;
		public double fitness = 0.0f;

		public Individual(DNA dna) {
			this.dna = dna;
		}

        public int CompareTo(Individual other)
        {
            return fitness.CompareTo(other.fitness);
        }

    }
}
