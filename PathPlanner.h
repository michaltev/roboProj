/**
* Robotics
*
* PathPlanner.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include "Grid.h"
#include "Node.h"
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct NodePriorityComparer
{
    bool operator()(const Node& nodeA, const Node& nodeB)
    {
    	return nodeA.GetPriority() > nodeB.GetPriority();
    }
};

// List of open nodes
static priority_queue<Node, vector<Node>, NodePriorityComparer> openNodesQueues[2];

class PathPlanner
{
private:
	Grid grid;
	int startRow;
	int startCol;
	int goalRow;
	int goalCol;
	vector< vector<bool> > occupationMap;
	int height;
	int width;

public:
	string plannedRoute;
	PathPlanner(const Grid * grid);
	void PrintRouteCvMat();
	string CalculateAStarRoute();
	virtual ~PathPlanner();
};

#endif /* PATHPLANNER_H_ */
