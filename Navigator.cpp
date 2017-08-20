/**
* Robotics
*
* Navigator.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "Navigator.h"
#include "Globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

#define MAP_ANGLE -30
#define MAX_MOVE_SPEED 0.4
#define MIN_TURN_SPEED 0.1
#define MAX_TURN_SPEED 0.2
#define YAW_TOLERANCE 1.05
#define NAVIGATION_TIMEOUT_IN_SECONDS 15
#define TURN_ANGLE 45.0
#define leftTurnAngle()		TURN_ANGLE
#define rightTurnAngle()	-TURN_ANGLE
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

Navigator::Navigator(HamsterAPI::Hamster * hamster, Robot * robot, MapDisplay * mapDisplay)
{
	this->hamster = hamster;
	this->robot = robot;
	this->mapDisplay = mapDisplay;
}

void Navigator::Stop()
{
	hamster->sendSpeed(0.0, 0.0);
}

void Navigator::NavigateToWaypoint(Coordinate * waypoint)
{
	this->waypoint = waypoint;

	double destYawInRad = atan2((waypoint->y - currCoordinate.y),(waypoint->x - currCoordinate.x));
	double destYawInDegrees = radiansToDegrees(destYawInRad) + MAP_ANGLE;
	destYaw = GetAdjustedYaw(destYawInDegrees);

	RecalculateDistanceFromWaypoint();
	RecalculateTurningDirection();

	coordinateChanged = true;
	bool isWaypointReached = false;

	navigationStartTime = clock();

	while (!isWaypointReached)
	{
		currCoordinate = robot->GetCurrHamsterCoordinate();
		currYaw = currCoordinate.yaw;
		currDeltaYaw = fabs(destYaw - currYaw);

		if (currDeltaYaw > YAW_TOLERANCE)
		{
			bool isStuck = ((clock() - navigationStartTime) / CLOCKS_PER_SEC) >= NAVIGATION_TIMEOUT_IN_SECONDS;

			if (!isStuck)
			{
				// Keep turning in the chosen direction
				RecalculateTurningDirection();
				TurnToWaypoint();
			}
			else
			{
				cout << "Moving Backwards" << endl;

				// Move backwards trying to avoid the obstacle that the robot got stuck in
				for (int i = 0; i < 1000000; i++)
				{
					hamster->sendSpeed(-0.1, 0.0);
				}

				navigationStartTime = clock();
			}
		}
		else
		{
			// Keep moving in the chosen direction
			cout << "Reached destination yaw, moving forward towards waypoint" << endl;
			MoveToWaypoint();
		}

		RecalculateDistanceFromWaypoint();
		mapDisplay->PrintRouteCvMat();
		isWaypointReached = distanceFromWaypoint <= DISTANCE_FROM_WAYPOINT_TOLERANCE;
	}

	PrintAfterWaypointIsReached();
	Stop();

	return;
}

void Navigator::TurnToWaypoint()
{
	CalculateTurnSpeedByDeltaYaw();
	hamster->sendSpeed(turnSpeed, wheelsAngle);

	prevCoordinate = currCoordinate;
	currCoordinate = robot->GetCurrHamsterCoordinate();
	currYaw = currCoordinate.yaw;

	coordinateChanged =
		prevCoordinate.x != currCoordinate.x &&
		prevCoordinate.y != currCoordinate.y &&
		prevCoordinate.yaw != currCoordinate.yaw;

	if (coordinateChanged)
	{
		chosenDirectionName = wheelsAngle > 0 ? "Left" : "Right";
		PrintAfterTurning();
	}
}

void Navigator::MoveToWaypoint()
{
	CalculateSpeedByDistanceFromWaypoint();
	hamster->sendSpeed(moveSpeed, 0.0);

	currCoordinate = robot->GetCurrHamsterCoordinate();
	prevDistanceFromWaypoint = distanceFromWaypoint;
	RecalculateDistanceFromWaypoint();

	coordinateChanged = prevDistanceFromWaypoint != distanceFromWaypoint;

	if (coordinateChanged)
	{
		chosenDirectionName = "Forward";
		PrintAfterMoving();
	}
}

void Navigator::RecalculateTurningDirection()
{
	currCoordinate = robot->GetCurrHamsterCoordinate();
	currYaw = currCoordinate.yaw;
	currDeltaYaw = fabs(destYaw - currYaw);

	if (currYaw > destYaw)
	{
		if (360 - currYaw + destYaw < currYaw - destYaw)
		{
			wheelsAngle = leftTurnAngle();
		}
		else
		{
			wheelsAngle = rightTurnAngle();
		}
	}
	else
	{
		if (360 - destYaw + currYaw < destYaw - currYaw)
		{
			wheelsAngle = rightTurnAngle();
		}
		else
		{
			wheelsAngle = leftTurnAngle();
		}
	}
}

double Navigator::GetAdjustedYaw(double yawToAdjust) const
{
	if (yawToAdjust < 0)
	{
		return yawToAdjust + 360;
	}

	if (yawToAdjust > 360)
	{
		return yawToAdjust - 360;
	}
}

void Navigator::RecalculateDistanceFromWaypoint()
{
	currCoordinate = robot->GetCurrHamsterCoordinate();

	distanceFromWaypoint =
		sqrt(pow(currCoordinate.x - waypoint->x, 2) +
			 pow(currCoordinate.y - waypoint->y, 2));
}

// Calculate the turn speed in order not to miss the destination yaw
void Navigator::CalculateTurnSpeedByDeltaYaw()
{
	int numOfSpeedClasses = 5;
	double diffConst = 2 * ((double)(MAX_TURN_SPEED - MIN_TURN_SPEED) / (numOfSpeedClasses - 1));

	double classSize = (double)360.0 / numOfSpeedClasses;

	double divisionResult = (double)currDeltaYaw / classSize;

	// Varies from (0) to (numOfSpeedClasses - 1)
	int speedClassIndex = floor(divisionResult);

	if (speedClassIndex > ((int)(numOfSpeedClasses / 2)))
	{
		turnSpeed = MIN_TURN_SPEED + (numOfSpeedClasses - speedClassIndex) * diffConst;
	}
	else
	{
		turnSpeed = MIN_TURN_SPEED + speedClassIndex * diffConst;
	}
}

// Calculate the move speed as it approached the waypoint in order not to miss it
void Navigator::CalculateSpeedByDistanceFromWaypoint()
{
	if (distanceFromWaypoint > 5 * DISTANCE_FROM_WAYPOINT_TOLERANCE)
	{
		moveSpeed = MAX_MOVE_SPEED;
	}

	moveSpeed = (double)distanceFromWaypoint / 30;
}

void Navigator::PrintBeforeTurning()
{
	cout << "Preparing to turn..." << endl <<
		"current coordinate: " <<
		"x = " << currCoordinate.x <<
		", y = " << currCoordinate.y <<
		", yaw = " << currYaw << endl <<
		"current waypoint: " <<
		"x = " << waypoint->x <<
		", y = " << waypoint->y << endl <<
		"destYaw: " << destYaw << endl;
}

void Navigator::PrintAfterTurning()
{
	cout << "Turned " << chosenDirectionName << " to: " <<
		"x = " << currCoordinate.x <<
		", y = " << currCoordinate.y <<
		", yaw = " << currYaw <<
		", deltaYaw = " << currDeltaYaw <<
		" (turnSpeed: " << turnSpeed << ")" << endl;
}

void Navigator::PrintAfterMoving()
{
	cout << "Moved " << chosenDirectionName << " to: " <<
		"x = " << currCoordinate.x <<
		", y = " << currCoordinate.y <<
		", yaw = " << currYaw <<
		", distanceFromWaypoint =  " << distanceFromWaypoint <<
		" (moveSpeed: " << moveSpeed << ")" << endl;
}

void Navigator::PrintAfterWaypointIsReached()
{
	cout << endl <<
		"Reached waypoint (" << waypoint->x << ", " << waypoint->y << ")" << endl <<
		"current coordinate: " <<
		"x = " << currCoordinate.x <<
		", y = " << currCoordinate.y <<
		", yaw = " << currCoordinate.yaw << endl << endl;
}

Navigator::~Navigator() {}
