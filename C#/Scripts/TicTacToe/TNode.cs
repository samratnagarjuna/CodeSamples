using UnityEngine;
using System.Collections;

[RequireComponent (typeof(SpriteRenderer))]
public class TNode : MonoBehaviour {

    public int row  = -1;
    public int col = -1;

    public enum TNodeType { X, O, N };

    [SerializeField]
    private TNodeType _type;
    public TNodeType Type
    {
        get { return _type; }
        set {
            _type = value;

            switch(_type)
            {
                case TNodeType.N:
                    _renderer.sprite = _nSprite;
                    break;

                case TNodeType.X:
                    _renderer.sprite = _xSprite;
                    break;

                case TNodeType.O:
                    _renderer.sprite = _oSprite;
                    break;
            }
        }
    }

    [SerializeField]
    private Sprite _nSprite = null;

    [SerializeField]
    private Sprite _xSprite = null;

    [SerializeField]
    private Sprite _oSprite = null;

    private SpriteRenderer _renderer = null;

	// Use this for initialization
	void Start () {
        _renderer = GetComponent<SpriteRenderer>();
        _renderer.sprite = _nSprite;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
