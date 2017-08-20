/**
* Robotics
*
* Node.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "Node.h"
#include <math.h>
using namespace std;

Node::Node()
{
	Coordinate coordinate = { .x = 0, .y = 0 };
	this->currCoordinate = coordinate;
	this->level = 0;
	this->priority = 0;
}

Node::Node(Coordinate currCoordinate, int level, int priority)
{
	this->currCoordinate = currCoordinate;
	this->level = level;
	this->priority = priority;
}

Coordinate Node::GetCoordinate() const
{
	return currCoordinate;
}

int Node::GetLevel() const
{
	return level;
}

int Node::GetPriority() const
{
	return priority;
}

void Node::CalculatePriority(const int & xDest, const int & yDest)
{
	priority = level + GetHeuristicEstimate(xDest, yDest) * 10;
}

// Set the level according to the next move
void Node::CalculateLevel(const int & direction)
{
	if (direction % 2 == 0)
	{
		level += 10;
	}
	else
	{
		level += 14;
	}
}

// Heuristic estimation function for the remaining distance to the goal
const int & Node::GetHeuristicEstimate(const int & xDest, const int & yDest) const
{
	static int xd, yd, distance;
	xd = xDest - currCoordinate.x;
	yd = yDest - currCoordinate.y;

	distance = static_cast<int>(sqrt(xd * xd + yd * yd));

	return (distance);
}

Node::~Node() {}
