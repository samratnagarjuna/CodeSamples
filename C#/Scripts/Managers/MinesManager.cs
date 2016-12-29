using UnityEngine;
using System.Collections.Generic;
namespace AISandbox {
	public class MinesManager : MonoBehaviour {
		private static readonly float MINE_RANGE = 1.0f;

		[SerializeField]
		private int noOfMines;

		[SerializeField]
		private GameObject minePrefab = null;

		private List<Vector2> mines = null;
		public List<Vector2> Mines { get { return mines; } }

		private List<GameObject> mineObjects = null;

		// Can be changed to private?
		public int minesToBeSpawned { get; set; }

		private static MinesManager _instance = null;
		public static MinesManager Instance
		{
			get
			{
				if (_instance == null)
				{
					_instance = FindObjectOfType<MinesManager>();
				}
				return _instance;
			}
		}

		// Use this for initialization
		void Start () {
			InitializeMines();
		}
		
		// Update is called once per frame
		void Update () {
		
		}

		private void InitializeMines() {
			mineObjects = new List<GameObject>();
			mines = new List<Vector2>();
			SpawnMines( noOfMines );
			minesToBeSpawned = 0;
		}

		private void SpawnMines (int noOfMines) {
			Vector2 topLeftScreen = new Vector2(0, Screen.height);
			Vector2 topLeftWorld = Camera.main.ScreenToWorldPoint(topLeftScreen);
			Vector2 botRightScreen = new Vector2(Screen.width, 0);
			Vector2 botRightWorld = Camera.main.ScreenToWorldPoint(botRightScreen);

			for(int i = 0; i < noOfMines; i++ ) {
				Vector2 randomMinePosition = 
					new Vector2(
						UnityEngine.Random.Range(topLeftWorld.x, botRightWorld.x), 
						UnityEngine.Random.Range(botRightWorld.y, topLeftWorld.y));

				GameObject landMine = Instantiate<GameObject>( minePrefab );
				landMine.transform.position = randomMinePosition;
				landMine.gameObject.SetActive ( true );
				mineObjects.Add ( landMine );
				mines.Add ( randomMinePosition );
			}
		}

		void LateUpdate() {
			if ( minesToBeSpawned != 0 ) {
				SpawnMines ( minesToBeSpawned );
				minesToBeSpawned = 0;
			}
		}

		public void Reset() {
			foreach ( GameObject mineObject in mineObjects ) {
				Destroy( mineObject );
			}
			InitializeMines();
		}

		public int DetectAndRemoveMinesAt(Vector2 tankPosition) {
			
			int totalMines = mines.Count;
			mines.RemoveAll(position => AtSamePosition(position, tankPosition));
			int minesDetected = totalMines - mines.Count;

			List<GameObject> detectedMines = mineObjects.FindAll (mineObject => AtSamePosition( mineObject.transform.position, tankPosition ) );
			foreach ( GameObject mineObject in detectedMines ) {
				mineObjects.Remove( mineObject );
				Destroy( mineObject );
			}

			minesToBeSpawned += minesDetected;

			return minesDetected;
		}

		private bool AtSamePosition (Vector2 minePosition, Vector2 tankPosition) {
			return Vector2.Distance( minePosition, tankPosition ) < MINE_RANGE;
		}
	}
}
