using UnityEngine;
using System.Collections;
using System.Linq;

namespace AISandbox {
	public class DNA {

		private static readonly int NO_OF_GENES = 120;

		public Vector2[] genes = null;

		public DNA() {
			
			genes = new Vector2[ NO_OF_GENES ];
			for ( int i = 0; i < genes.Length; i++ ) {
				genes[i].x = Random.Range ( -1.0f, 1.0f );
				genes[i].y = Random.Range ( -1.0f, 1.0f );
			}
		}

		public DNA (Vector2[] genes) {
			this.genes = new Vector2[ genes.Length ];
			// TODO: Replace with One line Clone using anonymous functions and Linq
			for ( int i = 0; i < genes.Length; i++ ) {
				this.genes[i].x = genes[i].x;
				this.genes[i].y = genes[i].y;
			}	
		}

		public DNA CrossOver(DNA partner) {
			Vector2[] childGenes = new Vector2[ genes.Length ];

			int crossOverIndex = (int)Random.Range ( 0, genes.Length - 1 );
			for (int i = 0; i < genes.Length; i++) {
				childGenes[i] = (i > crossOverIndex) ? genes[i] : partner.genes[i];
			}

			DNA childDNA = new DNA ( childGenes );
			return childDNA;
		}

		public void Mutate(float probability) {
			for(int i = 0; i < genes.Length; i++) {
				if ( Random.value < probability ) {
					genes[i].x = Random.Range ( -1.0f, 1.0f );
					genes[i].y = Random.Range ( -1.0f, 1.0f );
				}
			}
		}
	}
}
