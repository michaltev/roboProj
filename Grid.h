/**
* Robotics
*
* Grid.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef GRID_H_
#define GRID_H_

#include <vector>

#include "Coordinate.h"
using namespace std;

class Grid
{
private:
	double mapResolution;
	vector<vector<bool> > gridInflated;
	int gridWidth;
	int gridHeight;
	Coordinate startCoordinate;
	Coordinate goalCoordinate;

public:
	Grid();
	Grid(vector< vector<bool> > grid, double mapResolution, double height, double width, Coordinate start, Coordinate goal);
	int GetGridHeight() const;
	int GetGridWidth() const;
	vector< vector<bool> > GetInflatedMap() const;
	double GetMapResolution() const;
	Coordinate GetGridStartCoordinate() const;
	Coordinate GetGridGoalCoordinate() const;
	virtual ~Grid();
};

#endif /* GRID_H_ */
