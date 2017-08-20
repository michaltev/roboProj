/**
* Robotics
*
* Grid.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "Grid.h"

// Ctor
Grid::Grid()
{
}

// Ctor
Grid::Grid(vector< vector<bool> > grid, double mapResolution,
		   double height, double width,
		   Coordinate start, Coordinate goal)
{
	this->gridInflated = grid;
	this->mapResolution = mapResolution;
	this->gridHeight = height;
	this->gridWidth = width;
	this->startCoordinate = start;
	this->goalCoordinate = goal;
}

int Grid::GetGridHeight() const
{
	return gridHeight;
}

int Grid::GetGridWidth() const
{
	return gridWidth;
}

vector< vector<bool> > Grid::GetInflatedMap() const
{
	return gridInflated;
}

double Grid::GetMapResolution() const
{
	return mapResolution;
}

Coordinate Grid::GetGridStartCoordinate() const
{
	return startCoordinate;
}

Coordinate Grid::GetGridGoalCoordinate() const
{
	return goalCoordinate;
}

Grid::~Grid()
{}
