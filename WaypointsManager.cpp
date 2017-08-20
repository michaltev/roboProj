/**
* Robotics
*
* WaypointsManager.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "WaypointsManager.h"
#include "Globals.h"
#include <math.h>

#define NUMBER_OF_PARTICLES 1000
#define MAX_NUM_OF_WAYPOINTS 100

#define MAX_PIXEL_DIFF_BETWEEN_WAYPOINTS 10

int WayPointsManager::CalculateWaypoints(string plannedRoute, Coordinate startCoordinate, Coordinate goalCoordinate)
{
	int numOfWaypoints = 0;
	waypoints.resize(MAX_NUM_OF_WAYPOINTS);

	// Check if any route was found
	if (plannedRoute.length() <= 0)
	{
		return 0;
	}
	else
	{
		int movesInDirectionCounter = 0;
		char directionCharacter = plannedRoute.at(0);

		int currDirectionIndex = numericCharToInt(directionCharacter);
		int prevDirectionIndex = numericCharToInt(directionCharacter);

		int x = startCoordinate.x;
		int y = startCoordinate.y;

		// Run over all routes and create waypoints
		for (int i = 0; i < plannedRoute.length(); i++)
		{
			directionCharacter = plannedRoute.at(i);
			currDirectionIndex = numericCharToInt(directionCharacter);

			bool isSameDirection =
					currDirectionIndex == prevDirectionIndex &&
					movesInDirectionCounter <= MAX_PIXEL_DIFF_BETWEEN_WAYPOINTS;
			bool isNotWaypoint = movesInDirectionCounter == 0 || isSameDirection;

			if (isNotWaypoint)
			{
				movesInDirectionCounter++;
			}
			else
			{
				// Create a new waypoint
				waypoints.at(numOfWaypoints).x = x;
				waypoints.at(numOfWaypoints).y = y;

				prevDirectionIndex = currDirectionIndex;

				movesInDirectionCounter = 0;
				numOfWaypoints++;
			}

			x += dirX[currDirectionIndex];
			y += dirY[currDirectionIndex];
		}
	}

	waypoints.at(numOfWaypoints) = goalCoordinate;

	return numOfWaypoints;
}
