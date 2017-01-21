using UnityEngine;
using System.Collections;
using System;

public class TPlayerController : TController {

    // Use this for initialization
    public override void Start () {
        base.Start();
	}
	
	// Update is called once per frame
	public override void Update () {
        if (isActive)
        {
            if (Input.GetMouseButtonDown(0))
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Physics.Raycast(ray, out hit))
                {
                    if (hit.collider.CompareTag("TNode"))
                    {
                        TNode node = hit.collider.gameObject.GetComponent<TNode>();
						if (node.Type == TNode.TNodeType.N) {
							node.Type = _type;
                        	_isTurnFinished = true;
						}
                    }
                }
            }
        }
    }

}
