/**
* Robotics
*
* ConfigurationManager.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include "Coordinate.h"

class ConfigurationManager
{
private:
	double mapHeight, mapWidth;
public:
	ConfigurationManager();
	ConfigurationManager(double mapHeight, double mapWidth);
	Coordinate GetStartCoordinate();
	Coordinate GetGoalCoordinate();
	int GetRobotRadiusInCm();
};

#endif /* CONFIGURATIONMANAGER_H_ */
