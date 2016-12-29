namespace AISandbox {
	public struct NeuronLayer {
		public int noOfNeurons;
		public Neuron[] neurons;

		public NeuronLayer(int noOfNeurons, int noOfInputsPerNeuron) {
			this.noOfNeurons = noOfNeurons;
			this.neurons = new Neuron[this.noOfNeurons];

			InitializeLayer(noOfInputsPerNeuron);
		}

		private void InitializeLayer(int noOfInputsPerNeuron) {
			for(int i = 0; i < noOfNeurons; i++) {
				neurons[i] = new Neuron(noOfInputsPerNeuron);
			}
		}
	}
}