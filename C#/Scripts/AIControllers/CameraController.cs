using UnityEngine;
using System.Collections;

[RequireComponent (typeof(Camera))]
public class CameraController : MonoBehaviour {

	public float interpolant = 0.1f;
	// Use this for initialization
	void Start () {
		//cameraComponent = GetComponent<Camera>();
		targetObject = GameObject.FindGameObjectWithTag ("Player");
	}
	
	// Update is called once per frame
	void Update () {
		//XXX: How to find the best size
		//cameraComponent.orthographicSize = (Screen.height / 100f) / 2f;

		if (targetObject) {
			Vector3 targetPosition = targetObject.transform.position;
			targetPosition.y = transform.position.y;
			transform.position = Vector3.Lerp (transform.position, targetPosition, interpolant);
		}
	}

	//private Camera cameraComponent = null;
	private GameObject targetObject = null;
}
