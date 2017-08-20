/**
* Robotics
*
* Node.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef NODE_H_
#define NODE_H_

#include "Coordinate.h"

class Node
{
private:
	Coordinate currCoordinate;
	int level;
	int priority;

public:
	Node();
	Node(Coordinate currCoordinate, int level, int priority);
	Coordinate GetCoordinate() const;
	int GetLevel() const;
	int GetPriority() const;
	void CalculatePriority(const int & xDest, const int & yDest);
	void CalculateLevel(const int & direction);
	const int & GetHeuristicEstimate(const int & xDest, const int & yDest) const;
	virtual ~Node();
};


#endif /* NODE_H_ */
