/**
* Robotics
*
* WaypointsManager.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef WAYPOINTSMANAGER_H_
#define WAYPOINTSMANAGER_H_

#include <string>
#include <vector>

#include "Coordinate.h"
using namespace std;

class WayPointsManager
{
public:
	vector<Coordinate> waypoints;
	int CalculateWaypoints(string plannedRoute, Coordinate startCoordinate, Coordinate goalCoordinate);
};

#endif /* WAYPOINTSMANAGER_H_ */
