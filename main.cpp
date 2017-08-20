/**
* Robotics
*
* main.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "math.h"
#include "ConfigurationManager.h"
#include "PathPlanner.h"
#include "Map.h"
#include "MapDisplay.h"
#include "Navigator.h"
#include "WaypointsManager.h"
#include "Robot.h"

int main()
{
	try
	{
		Hamster * hamster = new HamsterAPI::Hamster(1);

		sleep(3);
		OccupancyGrid occupancyGrid = hamster->getSLAMMap();

		sleep(1);
		double mapHeight = occupancyGrid.getHeight();
		double mapWidth = occupancyGrid.getWidth();
		double mapResolution = occupancyGrid.getResolution();

		ConfigurationManager configurationManager(mapHeight, mapWidth);
		Coordinate startCoordinate = configurationManager.GetStartCoordinate();
		Coordinate goalCoordinate = configurationManager.GetGoalCoordinate();
		int robotSize = configurationManager.GetRobotRadiusInCm();

		Map map = Map(&occupancyGrid, robotSize, startCoordinate, goalCoordinate, mapHeight, mapWidth);
		Grid grid = map.grid;

		LocalizationManager localizationManager(hamster, occupancyGrid, mapHeight, mapWidth, mapResolution);
		Robot robot(hamster, &localizationManager, map.inflationRadius, mapHeight, mapWidth);

		PathPlanner pathPlanner = PathPlanner(&grid);
		string plannedRoute = pathPlanner.plannedRoute;

		WayPointsManager waypointsManager;

		int numOfWaypoints = waypointsManager.CalculateWaypoints(plannedRoute, startCoordinate, goalCoordinate);
		vector<Coordinate> waypoints = waypointsManager.waypoints;

		// Create and display the map
		MapDisplay mapDisplay = MapDisplay(&grid, plannedRoute, &waypoints, numOfWaypoints);

		Navigator navigator(hamster, &robot, &mapDisplay);

		robot.Init(startCoordinate);

		Coordinate currCoordinate;
		int waypointIndex = 0;
		Coordinate currWaypoint, hamsterWaypoint;
		double deltaX = 0, deltaY = 0, deltaYaw = 0;

		while (hamster->isConnected())
		{
			try
			{
				while (waypointIndex < numOfWaypoints)
				{
					currCoordinate = robot.GetCurrHamsterCoordinate();
					currWaypoint = waypoints.at(waypointIndex);
					hamsterWaypoint = mapDisplay.ConvertToHamsterCoordinate(currWaypoint);

					double distanceFromWaypoint =
						sqrt(pow(currCoordinate.x - hamsterWaypoint.x, 2) +
							 pow(currCoordinate.y - hamsterWaypoint.y, 2));

					bool isWaypointReached = distanceFromWaypoint <= DISTANCE_FROM_WAYPOINT_TOLERANCE;

					if (!isWaypointReached)
					{
						navigator.NavigateToWaypoint(&hamsterWaypoint);
					}
					else
					{
						cout << endl <<
							"Reached waypoint (" << hamsterWaypoint.x << ", " << hamsterWaypoint.y << ")" << endl;
					}

					waypointIndex++;

					/*robot.UpdateCoordinate();

					deltaX = robot.GetDeltaX();
					deltaY = robot.GetDeltaY();
					deltaYaw = robot.GetDeltaYaw();

					cout << "Real values:" << " deltaX : " << deltaX << " deltaY: " << deltaY << " deltaYaw : " << deltaYaw << endl;

					localizationManager.UpdateParticles(deltaX, deltaY, deltaYaw);
					MapDisplay.PrintRouteCvMat(localizationManager.GetParticles());
					localizationManager.PrintParticles();*/
				}

				navigator.Stop();
				cout << "Hamster has reached its destination!" << endl;

				return 0;
			}
			catch (const HamsterAPI::HamsterError & message_error)
			{
				HamsterAPI::Log::i("Client", message_error.what());
			}
		}
	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Client", connection_error.what());
	}

	return 0;
}
