/**
* Robotics
*
* PathPlanner.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "PathPlanner.h"
#include "Globals.h"

// Ctor
PathPlanner::PathPlanner(const Grid * grid)
{
	this->startRow=grid->GetGridStartCoordinate().y;
	this->startCol=grid->GetGridStartCoordinate().x;
	this->goalRow=grid->GetGridGoalCoordinate().y;
	this->goalCol=grid->GetGridGoalCoordinate().x;
	this->occupationMap=grid->GetInflatedMap();
	this->height=grid->GetGridHeight();
	this->width=grid->GetGridWidth();

	this->plannedRoute = CalculateAStarRoute();
}

string PathPlanner::CalculateAStarRoute()
{
	vector<vector<int> > closed_nodes;
	vector<vector<int> > open_nodes;
	vector<vector<int> > path_directions;

	static Node* nNodeA;
	static Node* nNodeB;
	static int smallerPQIndex;
	static int dirIndex, cellIndex, rowIndex, colIndex;
	static char c;
	smallerPQIndex = 0;

	closed_nodes.resize(height);
	open_nodes.resize(height);
	path_directions.resize(height);

	// Resizing the vectors
	for (int i = 0; i < height; i++)
	{
		(closed_nodes.at(i)).resize(width);
		(open_nodes.at(i)).resize(width);
		(path_directions.at(i)).resize(width);
	}

	// Initialize the cells in the maps
	for (rowIndex = 0; rowIndex < height; rowIndex++)
	{
		for (colIndex = 0; colIndex < width; colIndex++)
		{
			(closed_nodes.at(rowIndex)).at(colIndex) = 0;
			(open_nodes.at(rowIndex)).at(colIndex) = 0;
		}
	}

	// Create the start node and push into list of open nodes
	Coordinate startCoordinate = { .x = startCol, .y = startRow };
	nNodeA = new Node(startCoordinate, 0, 0);
	nNodeA->CalculatePriority(goalCol, goalRow);
	(openNodesQueues[smallerPQIndex]).push(*nNodeA);

	// Mark it on the open nodes map
	rowIndex = nNodeA->GetCoordinate().y;
	colIndex = nNodeA->GetCoordinate().x;
	(open_nodes.at(rowIndex)).at(colIndex) = nNodeA->GetPriority();

	// A* search
	while (!(openNodesQueues[smallerPQIndex]).empty())
	{
		// Get the current node with the highest priority from the list of open nodes
		Node highestPriorityNode = (openNodesQueues[smallerPQIndex]).top();
		nNodeA = new Node(highestPriorityNode.GetCoordinate(),
				highestPriorityNode.GetLevel(),
				highestPriorityNode.GetPriority());

		rowIndex = nNodeA->GetCoordinate().y;
		colIndex = nNodeA->GetCoordinate().x;

		// Remove the node from the open list
		(openNodesQueues[smallerPQIndex]).pop();
		(open_nodes.at(rowIndex)).at(colIndex) = 0;

		// Mark it on the closed nodes map
		(closed_nodes.at(rowIndex)).at(colIndex) = 1;

		// Check if we reached to goal coordinate
		bool isGoalCoordinateReached = rowIndex == goalRow && colIndex == goalCol;

		if (isGoalCoordinateReached)
		{
			// Generate the path from finish to start by following the directions
			string plannedRoute = "";

			while (rowIndex != startRow || colIndex != startCol)
			{
				cellIndex = (path_directions.at(rowIndex)).at(colIndex);
				c = '0' + (cellIndex + dirNum / 2) % dirNum;
				plannedRoute = c + plannedRoute;

				rowIndex += dirY[cellIndex];
				colIndex += dirX[cellIndex];
			}

			// Delete node and all left nodes
			delete nNodeA;

			while (!(openNodesQueues[smallerPQIndex]).empty())
			{
				(openNodesQueues[smallerPQIndex]).pop();
			}

			return plannedRoute;
		}

		// Generate all possible moves
		for (dirIndex = 0; dirIndex < dirNum; dirIndex++)
		{
			Coordinate coordinate = { .x = colIndex + dirX[dirIndex], .y = rowIndex + dirY[dirIndex] };

			bool isCoordinateInBounds =
					coordinate.x >= 0 && coordinate.x <= height - 1
					&& coordinate.y >= 0 && coordinate.y <= width - 1;
			bool isCellNotOccupied = (occupationMap.at(coordinate.y)).at(coordinate.x) != 1;
			bool isNodeNotClosed = (closed_nodes.at(coordinate.y)).at(coordinate.x) != 1;

			if (isCoordinateInBounds && isCellNotOccupied && isNodeNotClosed)
			{
				// Generate a child node
				nNodeB = new Node(coordinate, nNodeA->GetLevel(), nNodeA->GetPriority());
				nNodeB->CalculateLevel(dirIndex);
				nNodeB->CalculatePriority(goalCol, goalRow);

				// If the node isn't in the open list - add it
				bool isNodeNotOpened = (open_nodes.at(coordinate.y)).at(coordinate.x) == 0;

				if (isNodeNotOpened)
				{
					(open_nodes.at(coordinate.y)).at(coordinate.x) = nNodeB->GetPriority();
					(openNodesQueues[smallerPQIndex]).push(*nNodeB);

					// Mark its parent node direction
					(path_directions.at(coordinate.y)).at(coordinate.x) =
							(dirIndex + dirNum / 2) % dirNum;
				}
				else if ((open_nodes.at(coordinate.y)).at(coordinate.x) > nNodeB->GetPriority())
				{
					// Update the priority info
					(open_nodes.at(coordinate.y)).at(coordinate.x) = nNodeB->GetPriority();

					// Update the parent direction info
					(path_directions.at(coordinate.y)).at(coordinate.x) =
							(dirIndex + dirNum / 2) % dirNum;

					/* Replace the node by emptying one priority queue to the other one,
					   except the node to be replaced and the new node */
					while ((((Node) (openNodesQueues[smallerPQIndex].top())).GetCoordinate().x != coordinate.x) ||
						   (((Node) (openNodesQueues[smallerPQIndex].top())).GetCoordinate().y != coordinate.y))
					{
						Node topNode = (openNodesQueues[smallerPQIndex]).top();
						(openNodesQueues[1 - smallerPQIndex]).push(topNode);
						(openNodesQueues[smallerPQIndex]).pop();
					}

					// Remove the wanted node
					(openNodesQueues[smallerPQIndex]).pop();

					// Compare queues sizes
					int firstQueueSize =
							(openNodesQueues[smallerPQIndex]).size();
					int secondQueueSize =
							(openNodesQueues[1 - smallerPQIndex]).size();

					if (firstQueueSize > secondQueueSize)
					{
						smallerPQIndex = 1 - smallerPQIndex;
					}

					// Empty the larger size priority queue to the smaller one
					while (!openNodesQueues[smallerPQIndex].empty())
					{
						Node nodeToPush = openNodesQueues[smallerPQIndex].top();
						(openNodesQueues[1 - smallerPQIndex]).push(nodeToPush);
						(openNodesQueues[smallerPQIndex]).pop();
					}

					smallerPQIndex = 1 - smallerPQIndex;

					// Insert the better node instead
					(openNodesQueues[smallerPQIndex]).push(*nNodeB);
				}
				else
				{
					delete nNodeB;
				}
			}
		}

		delete nNodeA;
	}

	// No route found
	return "";
}

PathPlanner::~PathPlanner()
{}
