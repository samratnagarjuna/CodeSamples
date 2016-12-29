using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace AISandbox {
	public class NeuralGameManager : MonoBehaviour {

		private static NeuralGameManager _instance = null;
		public static NeuralGameManager Instance
		{
			get
			{
				if (_instance == null)
				{
					_instance = FindObjectOfType<NeuralGameManager>();
				}
				return _instance;
			}
		}

		public NTankController tankPrefab = null;

		// For Genetics
		public int noOfGenomes = 0;
		public float mutationRate = 0.0f;
		public float elitismRate = 0.0f;
		public float lifeTime = 0.0f;

		// For Neural Network
		public int noOfInputs = 0;
		public int noOfOutputs = 0;
		public int noOfHiddenLayers = 0;
		public int noOfNeuronsPerHiddenLayer = 0;

		public UnityEngine.UI.Text debugText = null;
		public UnityEngine.UI.Text timeText = null;
		public UnityEngine.UI.Text lastGenResText = null;
		public UnityEngine.UI.Text generationText = null;

		public NeuralNet neuralNet = null;
		private NeuralGenetic neuralGenetic = null;
		private List<NTankController> controllers = null;
		private float timeRemaining = 0.0f;

		// Use this for initialization
		void Start () {
			Init();
			InitializeNeuralNetwork();
			InitializeTanks(false);
			StartCoroutine(ExecuteGeneticAlgorithm());
		}
		
		// Update is called once per frame
		void Update () {

			if (Input.GetKeyDown(KeyCode.RightArrow)) {
				Time.timeScale++;
			}

			if (Input.GetKeyDown(KeyCode.LeftArrow)) {
				if (Time.timeScale > 1)
					Time.timeScale--;
			}

			int i = 0;
			debugText.text = "";
			foreach ( NTankController controller in controllers ) {
				debugText.text += "Tank " + i++ + ": " + controller.genome.fitness + "\n";
			}

			timeRemaining -= Time.deltaTime;
			if ( timeRemaining > 0.0f ) timeText.text = "Time Left: " + timeRemaining;
		}

		private IEnumerator ExecuteGeneticAlgorithm() {

			// Have interations based on no of generations
			while ( true ) {
				yield return new WaitForSeconds(lifeTime);

				neuralGenetic.SelectAndReproduce();

				DestroyTanks();
				Init();
				MinesManager.Instance.Reset();
				InitializeTanks(true);
				generationText.text = "Generation: " + neuralGenetic.generations;
				lastGenResText.text = "Last Gen Res " + "\n" + debugText.text;
			}
		}

		private void Init() {
			timeRemaining = lifeTime;
		}


		private void InitializeNeuralNetwork() {
			neuralNet = new NeuralNet (noOfInputs, noOfOutputs, noOfHiddenLayers, noOfNeuronsPerHiddenLayer);
			// Debug.Log( "No of weights: " + neuralNet.GetNumberOfWeights());

			neuralGenetic = new NeuralGenetic(mutationRate, elitismRate, noOfGenomes, neuralNet.GetNumberOfWeights());
		}

		private void InitializeTanks(bool markElites) {
			controllers = new List<NTankController>();

			int noOfElites = (int)(noOfGenomes * elitismRate);
			for( int i = 0; i < noOfGenomes; i++ ) {
				NTankController controller = Instantiate<NTankController>( tankPrefab );
				controller.genome = neuralGenetic.population[i];
				controller.gameObject.SetActive ( true );

				// TODO: remove this later
				if ( markElites && i < noOfElites ) controller.gameObject.GetComponent<TankActor>().IsElite = true;
				controllers.Add ( controller );
			}
		}

		private void DestroyTanks() {
			foreach( NTankController controller in controllers) {
				Destroy( controller.gameObject );
			}
			controllers.Clear();
		}
	}
}
