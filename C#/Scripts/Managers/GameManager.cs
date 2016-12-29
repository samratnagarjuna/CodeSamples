using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace AISandbox {
	public class GameManager : MonoBehaviour {

		public TankController tankPrefab = null;
		public GameObject minePrefab = null;
		public UnityEngine.UI.Text debugText = null;
		public UnityEngine.UI.Text timeText = null;
		public UnityEngine.UI.Text lastGenResText = null;

		public int noOfMines = 0;
		public int noOfIndividuals = 0;
		public float mutationRate = 0.0f;
		public float lifeTime = 0.0f;

		private Population population = null;
		private List<TankController> controllers = null;
		private List<Vector2> mines = null;
		private float timeRemaining = 0.0f;

		void Awake() {
			Time.timeScale = 2.0f;
		}

		// Use this for initialization
		void Start () {
			Init();
			InitializePoplation();
			InitializeMines();
			InitializeTanks();

			StartCoroutine(ExecuteGeneticAlgorithm());
		}
		
		// Update is called once per frame
		void Update () {
			int i = 0;
			debugText.text = "";
			foreach ( TankController controller in controllers ) {
				debugText.text += "Tank " + i++ + ": " + controller.individual.fitness + "\n";
			}

			timeRemaining -= Time.deltaTime;
			if ( timeRemaining > 0.0f ) timeText.text = "Time Left: " + timeRemaining;
		}

		private IEnumerator ExecuteGeneticAlgorithm() {

			// Have interations based on no of generations
			while ( true ) {
				yield return new WaitForSeconds(lifeTime);

				if ( population.IsTargetReached() ) {
					// Target Reached
					Debug.Log( "Target Reached");
				}
				else {
					population.SelectAndReproduce();

					DestroyTanks();
					Init();
					InitializeTanks();
					MinesManager.Instance.Reset();
				}

				lastGenResText.text = "Last Gen Res " + "\n" + debugText.text;

			}
		}

		private void Init() {
			timeRemaining = lifeTime;
		}

		private void InitializePoplation() {
			population = new Population( mutationRate, noOfIndividuals, noOfMines );
		}

		private void InitializeMines() {
			Vector2 topLeftScreen = new Vector2(0, Screen.height);
			Vector2 topLeftWorld = Camera.main.ScreenToWorldPoint(topLeftScreen);
			Vector2 botRightScreen = new Vector2(Screen.width, 0);
			Vector2 botRightWorld = Camera.main.ScreenToWorldPoint(botRightScreen);

			mines = new List<Vector2>();
			for( int i = 0; i < noOfMines; i++ ) {
				Vector2 randomMinePosition = 
					new Vector2(
						UnityEngine.Random.Range(topLeftWorld.x, botRightWorld.x), 
						UnityEngine.Random.Range(botRightWorld.y, topLeftWorld.y));

				GameObject landMine = Instantiate<GameObject>( minePrefab );
				landMine.transform.position = randomMinePosition;
				landMine.gameObject.SetActive ( true );

				mines.Add ( randomMinePosition );
			}
		}

		private void InitializeTanks() {
			controllers = new List<TankController>();
			for( int i = 0; i < noOfIndividuals; i++ ) {
				TankController controller = Instantiate<TankController>( tankPrefab );
				controller.individual = population.individuals[i];
				controller.mines = new List<Vector2> (mines.Select( position => new Vector2(position.x, position.y)));
				controller.gameObject.SetActive ( true );

				controllers.Add ( controller );
			}
		}

		private void DestroyTanks() {
			foreach( TankController controller in controllers) {
				Destroy( controller.gameObject );
			}
		}

	}
}
