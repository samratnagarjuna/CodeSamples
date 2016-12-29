using UnityEngine;
using System;
using System.Collections.Generic;

namespace AISandbox {	
	public class NeuralNet {

		private static readonly double biasInput = -1.0;
		private static readonly double activationResponse = 1.0;
		private int noOfInputs;
		private int noOfOutputs;
		private int noOfHiddenLayers;
		private int noOfNeuronsPerHiddenLayer;

		private NeuronLayer[] neuralLayers;

		public NeuralNet(int noOfInputs, int noOfOutputs, 
			int noOfHiddenLayers, int noOfNeuronsPerHiddenLayer) {

			this.noOfInputs = noOfInputs;
			this.noOfOutputs = noOfOutputs;
			this.noOfHiddenLayers = noOfHiddenLayers;
			this.noOfNeuronsPerHiddenLayer = noOfNeuronsPerHiddenLayer;

			neuralLayers = new NeuronLayer[noOfHiddenLayers + 1];
			CreateNeuralNet();
		}

		public void CreateNeuralNet() {
			if ( noOfHiddenLayers > 0 ) {

				// Creating first hidden layer
				neuralLayers[0] = new NeuronLayer(noOfNeuronsPerHiddenLayer, noOfInputs);

				for(int i = 1; i < noOfHiddenLayers ; i++) {
					neuralLayers[i] = new NeuronLayer(noOfNeuronsPerHiddenLayer, noOfNeuronsPerHiddenLayer);
				}

				// Creating output layer
				neuralLayers[ neuralLayers.Length - 1] = new NeuronLayer(noOfOutputs, noOfNeuronsPerHiddenLayer);
			}
			else {
				// Creating output layer
				neuralLayers[ neuralLayers.Length - 1] = new NeuronLayer(noOfOutputs, noOfInputs);
			}
		}

		public List<double> GetWeights() {
			List<double> weights = new List<double>();

			// For each hidden layer and the output layer
			for(int i = 0; i < noOfHiddenLayers + 1; i++) {
				// For each neuron
				for(int j = 0 ; j < neuralLayers[i].noOfNeurons; j++) {
					// For each weight
					for( int k = 0; k < neuralLayers[i].neurons[j].noOfInputs; k++ ) {
						weights.Add(neuralLayers[i].neurons[j].weights[k]);
					}
				}
			}

			return weights;
		}

		public void PutWeights(ref List<double> weights) {
			int weightIndex = 0;

			// For each hidden layer and the output layer
			for(int i = 0; i < noOfHiddenLayers + 1; i++) {
				// For each neuron
				for(int j = 0 ; j < neuralLayers[i].noOfNeurons; j++) {
					// For each weight
					for( int k = 0; k < neuralLayers[i].neurons[j].noOfInputs; k++ ) {
						neuralLayers[i].neurons[j].weights[k] = weights[weightIndex++];
					}
				}
			}
		}

		public int GetNumberOfWeights() {
			int noOfWeights = 0;

			// For each hidden layer and the output layer
			for(int i = 0; i < noOfHiddenLayers + 1; i++) {
				// For each neuron
				for(int j = 0 ; j < neuralLayers[i].noOfNeurons; j++) {
					// For each weight
					for( int k = 0; k < neuralLayers[i].neurons[j].noOfInputs; k++ ) {
						noOfWeights++;
					}
				}
			}

			return noOfWeights;
		}

		public List<double> Evaluate(List<double> inputs) {
			
			// Output from each layer
			List<double> outputs = new List<double>();

			if ( inputs.Count != this.noOfInputs ) {
				Debug.LogError("No of inputs doesn't match");
			}

			// For each hidden layer and the out layer
			for(int i = 0; i < noOfHiddenLayers + 1; i++) {

				// Clear output list
				outputs.Clear();

				// For each neuron in the layer
				for( int j = 0; j < neuralLayers[i].noOfNeurons; j++) {
					double netInput = 0.0;

					// includes the bias input
					int noOfInputs = neuralLayers[i].neurons[j].noOfInputs;

					// For each input to the neuron except the bias
					for(int k = 0; k < noOfInputs - 1; k++) {
						netInput += neuralLayers[i].neurons[j].weights[k] * inputs[k];
					}

					// Adding the bias input
					netInput += neuralLayers[i].neurons[j].weights[noOfInputs - 1] * biasInput;
					// Debug.Log( netInput );

					// Feed the net input to the activation function and add to outputs
					outputs.Add(ActivationFunction(netInput, activationResponse));
				}

				// Pass the outputs as inputs to the next layer
				inputs.Clear();
				inputs.AddRange(outputs);
			}

			return outputs;
		}

		private static double ActivationFunction(double netInput, double activationResponse) {
			double exp = Math.Exp( -2.0 * netInput / activationResponse );
			return (2.0 / ( 1 + exp ) - 1.0);
		}
	}
}
