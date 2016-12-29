using UnityEngine;
using System.Collections;

namespace AISandbox {
    [RequireComponent(typeof (IActor))]
    public class PlayerController : MonoBehaviour {
        private IActor _actor;

		private void Awake() {
            _actor = GetComponent<IActor>();
        }

        private void FixedUpdate() {
            // Read the inputs.
            float y_axis = Input.GetAxis("Vertical");
            float y2_axis = Input.GetAxis("AltV");

            // Pass all parameters to the character control script.
            _actor.SetInput( 0.0f, y_axis, 0.0f, y2_axis );

			// Debug
			Debug.Log(transform.up);
        }
    }
}