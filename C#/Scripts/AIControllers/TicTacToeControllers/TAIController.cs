using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class TAIController : TController
{
	private const int INT_MAX = 2147483647;
	private int depth = 3;

    public UnityEngine.UI.Text AIDebugText = null;

	private struct Move
	{
		public int i;
		public int j;

		public Move(int i, int j) { this.i = i; this.j = j; }
	};

    public override void Start()
    {
        base.Start();
    }

    public override void Update () {
		if (isActive)
		{
			NextMove();
		}
	}

	public void NextMove()
	{
		int size = _manager.Size;

		int[ , ] grid = new int[size, size];

		for ( int i = 0 ; i < size; i++ )
		{
			for ( int j = 0 ; j < size; j++ )
			{
				TNode.TNodeType type = _manager.GetNode(i,j).Type;

				if ( type == TNode.TNodeType.N ) grid[i,j] = -1;
				else if ( type == _type) grid[i,j] =  1;
				else grid[i, j] = 0;
			}
		}

		Move best_move;
		int score = Mini(grid, depth, out best_move);
		Debug.Log(best_move.i + " ," + best_move.j);

		AIDebugText.text = "Board Evaluation Score: " + score;

		TNode node = _manager.GetNode( best_move.i, best_move.j );

		node.Type = _type;
		_isTurnFinished = true;
	}

	private int GetGridScore(int[,] grid, int size, int val, int count)
	{
		int score = 0;

		// Rows and Columns Check

		for ( int i = 0; i < size; i++ )
		{
			for ( int j = 0 ; j <= size - count ; j++ )
			{
				bool row_scored = true;
				bool col_scored = true;

				for ( int k = 0; k < count; k++ )
				{
					if ( grid[i, j+k ] != val ) row_scored = false;
					if ( grid[j+k , i] != val ) col_scored = false;
				}

				if (row_scored)	score++;
				if (col_scored) score++;

			}
		}

		// Diagonal Checks

		for (int i = 0; i <=  size - count; i++ )
		{
			for ( int j = 0 ; j <= size - count ; j++ )
			{
				bool left_diag_scored = true;
				bool right_diag_scored = true;

				for( int k = 0 ; k < count; k++ )
				{
					if ( grid[ i+k, j+k ] != val ) left_diag_scored = false;
					if ( grid[ i+k, (size - 1) - (j+k) ] != val ) right_diag_scored = false;
				}

				if ( left_diag_scored ) score++;
				if ( right_diag_scored ) score++;
			}
		}

		return score;
	}

	private int Maxi ( int[,] grid, int depth, out Move best_move) 
	{
		int size = _manager.Size;

		best_move = new Move(-1, -1);

		if (depth == 0) return EvaluateGrid(grid);

		int max_score = -INT_MAX;

		for ( int i = 0 ; i < size; i++ )
		{
			for ( int j = 0 ; j < size; j++ )
			{
				if ( grid[i, j] == -1 )
				{
					// Take the move
					grid[i, j] = 1;

					Move move;
					int score = Mini(grid, depth - 1, out move);

					if ( score > max_score ) {
						max_score = score;
						best_move.i = i;
						best_move.j = j;
					}

					// Undo the move
					grid[i, j] = -1;
				}
			}
		}

		return max_score != -INT_MAX ? max_score : EvaluateGrid(grid);
	}

	private int Mini( int[,] grid, int depth, out Move best_move) 
	{
		int size = _manager.Size;

		best_move = new Move(-1, -1);

		if ( depth == 0 ) return -EvaluateGrid(grid);

		int min_score = INT_MAX;

		for ( int i = 0 ; i < size; i++ )
		{
			for ( int j = 0 ; j < size; j++ )
			{
				if ( grid[i, j] == -1 )
				{
					// Take the move
					grid[i, j] = 0;

					Move move;
					int score = Maxi(grid, depth - 1, out move);

					if ( score < min_score ) {
						min_score = score;
						best_move.i = i;
						best_move.j = j;
					}

					// Undo the move
					grid[i, j] = -1;
				}
			}
		}

		return min_score != INT_MAX ? min_score : -EvaluateGrid(grid);	
	}

	private int EvaluateGrid( int[,] grid )
	{
		int size = _manager.Size;
		int count = _manager.Count;

		int max_player_score = GetGridScore(grid, size, 1, count);
		int min_player_score = GetGridScore(grid, size, 0, count);

		return ( max_player_score - min_player_score);
	}
}
