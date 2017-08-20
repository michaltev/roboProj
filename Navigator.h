/**
* Robotics
*
* Navigator.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include "Robot.h"
#include "HamsterAPIClientCPP/Hamster.h"
#include <vector>

#include "Coordinate.h"
#include "MapDisplay.h"
using namespace std;
using namespace HamsterAPI;

#define DISTANCE_FROM_WAYPOINT_TOLERANCE 5

class Navigator
{
private:
	HamsterAPI::Hamster * hamster;
	Robot * robot;
	Coordinate currCoordinate;
	Coordinate prevCoordinate;
	Coordinate * waypoint;
	double distanceFromWaypoint, prevDistanceFromWaypoint;
	double currYaw, destYaw, currDeltaYaw;
	double turnSpeed, moveSpeed;
	string chosenDirectionName;
	clock_t navigationStartTime;
	float wheelsAngle;
	bool coordinateChanged;
	MapDisplay * mapDisplay;

	void TurnToWaypoint();
	void MoveToWaypoint();
	double GetAdjustedYaw(double yawToAdjust) const;
	void RecalculateTurningDirection();
	void RecalculateDistanceFromWaypoint();
	void CalculateTurnSpeedByDeltaYaw();
	void CalculateSpeedByDistanceFromWaypoint();
	void PrintBeforeTurning();
	void PrintAfterTurning();
	void PrintAfterMoving();
	void PrintAfterWaypointIsReached();

public:
	Navigator(HamsterAPI::Hamster * hamster, Robot * robot, MapDisplay * MapDisplay);
	void NavigateToWaypoint(Coordinate * waypoint);
	void Stop() ;
	virtual ~Navigator();
};

#endif /* NAVIGATOR_H_ */
