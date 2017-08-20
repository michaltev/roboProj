/**
* Robotics
*
* ConfigurationManager.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "ConfigurationManager.h"
#include "Globals.h"

// Ctor
ConfigurationManager::ConfigurationManager(double mapHeight, double mapWidth)
{
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;
}


Coordinate ConfigurationManager::GetStartCoordinate()
{
	Coordinate startCoordinate =
	{
		.x = X_START,
		.y = Y_START,
		.yaw = YAW_START
	};

	return startCoordinate;
}

Coordinate ConfigurationManager::GetGoalCoordinate()
{
	Coordinate goalCoordinate =
	{
		.x = X_GOAL,
		.y = Y_GOAL
	};

	return goalCoordinate;
}

int ConfigurationManager::GetRobotRadiusInCm()
{
	return ROBOT_SIZE_IN_CM;
}
