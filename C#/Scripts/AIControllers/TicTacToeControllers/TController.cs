using UnityEngine;
using System.Collections;

public abstract class TController : MonoBehaviour {

    protected TBoardManager _manager = null;

    protected bool _isTurnFinished = false;

	protected TNode.TNodeType _type = TNode.TNodeType.N;
	public TNode.TNodeType Type
	{
		get { return _type; }
		set { _type = value; }
	}

    private bool _isActive = false;
    public bool isActive {
        get { return _isActive; }
        set
        {
            _isActive = value;
            _isTurnFinished = false;
        }
    }
	// Use this for initialization
	public virtual void Start () {
        _manager = GameObject.FindObjectOfType<TBoardManager>();
        if (_manager == null) Debug.LogError("No Board Manager in the Scene");
	}

    // Update is called once per frame
    public abstract void Update();

    public bool isTurnFinished() { return _isTurnFinished; }
}
