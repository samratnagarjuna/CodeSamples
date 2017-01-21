using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace AISandbox {
    public class Pathfinding : MonoBehaviour {
        public Grid grid;
        private GridNode startNode = null;
        private GridNode endNode = null;

        private void Start() {
            // Create and center the grid
            grid.Create( 30, 31 );
            Vector2 gridSize = grid.size;
            Vector2 gridPos = new Vector2(gridSize.x * -0.5f, gridSize.y * 0.5f);
            grid.transform.position = gridPos;
        }

		public void FindPath() {
			grid.SetButtonsState(false);
			startNode = grid.getStartNode();
			endNode = grid.getEndNode();
			List<GridNode> pathNodes;
			bool pathFound = FindPathAStar(startNode, endNode, out pathNodes);

			if (pathFound) {
				foreach ( GridNode node in pathNodes ) {
					node.Type = GridNode.GridType.GRASS;
				}
			}
		}


		// A* Path finding Algorithm
		public bool FindPathAStar(GridNode startNode, GridNode endNode, out List<GridNode> pathNodes)
		{
			bool pathFound = false;
			pathNodes = null;
			if ( startNode != null && endNode != null )
			{
                // Create a list to hold closed nodes
                List<GridNode> closedNodes = new List<GridNode>();

                // Create a list to hold open nodes
                List<GridNode> openNodes = new List<GridNode>();

                // Create a priority queue to store open nodes along with their f value ( f = g + h )
                PriorityQueue<double, GridNode> frontier = new PriorityQueue<double, GridNode>();

                // Create dict to store where the node came from
                Dictionary<GridNode, GridNode> cameFrom = new Dictionary<GridNode, GridNode>();

                // Create dict to store the g value of nodes
                Dictionary<GridNode, double> gScore = new Dictionary<GridNode, double>();
                gScore[startNode] = 0.0f;

                // Create dict to store the g value of nodes
                Dictionary<GridNode, double> fScore = new Dictionary<GridNode, double>();
                fScore[startNode] = gScore[startNode] + Heuristic(startNode, endNode);

                // Push the start node into the queue
                openNodes.Add(startNode);
                frontier.Enqueue(fScore[startNode], startNode);

                // While there are nodes to be processed in the open list
                while ( openNodes.Count > 0 )
                {
                    GridNode currentNode = frontier.DequeueValue();

                    if (currentNode.IsAtSamePosition(endNode))
                    {
						ReconstructPath(cameFrom, currentNode, out pathNodes);
						pathFound = true;
                        break;
                    }

                    openNodes.Remove(currentNode);
                    closedNodes.Add(currentNode);

                    foreach (GridNode neighbour in currentNode.GetNeighbors() )
                    {
                        // If already processed then continue
                        if (closedNodes.Contains(neighbour)) continue;

                        // Calculate current g score for neighbour
                        double tentative_gScore = gScore[currentNode] + Distance(currentNode, neighbour) * neighbour.weight;

                        // if neighbour node not in open nodes list
                        if (!openNodes.Contains(neighbour))
                        {
                            openNodes.Add(neighbour);
                        }
                        // if neighbour in open nodes list but has a lesser gScore
                        else if ( gScore.ContainsKey(neighbour) && tentative_gScore >= gScore[neighbour] )
                        {
                            continue;
                        }

                        cameFrom[neighbour] = currentNode;
                        gScore[neighbour] = tentative_gScore;
                        fScore[neighbour] = gScore[neighbour] + Heuristic(neighbour, endNode);
                        frontier.Enqueue(fScore[neighbour], neighbour);
                    }
                }
            }
			return pathFound;
		}

		private double Heuristic (GridNode n1, GridNode n2) 
		{
			return Mathf.Abs(n1.row - n2.row) + Mathf.Abs(n1.column - n2.column);
		}

		private double Distance (GridNode n1, GridNode n2)
		{
			return Mathf.Abs(n1.row - n2.row) + Mathf.Abs(n1.column - n2.column);
		}

		private void ReconstructPath(Dictionary<GridNode, GridNode> cameFrom, GridNode current, out List<GridNode> pathNodes)
		{
			pathNodes = new List<GridNode>();
			pathNodes.Add (current);
			while (cameFrom.ContainsKey (current))
			{
				current = cameFrom[current];
				pathNodes.Add(current);
            }
			pathNodes.Reverse();
		}
    }
}