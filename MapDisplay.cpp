/**
* Robotics
*
* MapDisplay.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "MapDisplay.h"

#include "Globals.h"

#define FREE_SPACE 0
#define OBSTACLE 1
#define START 2
#define PARTICLE 3
#define GOAL 4
#define TOP_PARTICLE 4
#define WAYPOINT 5
#define TOP_PARTICLES_NUM 5
#define BELIEF_LEVEL 0.5

MapDisplay::MapDisplay(Grid * grid, string plannedRoute, vector<Coordinate> * waypoints, int numOfWaypoints)
{
	this->startRow = grid->GetGridStartCoordinate().y;
	this->startCol = grid->GetGridStartCoordinate().x;
	this->goalRow = grid->GetGridGoalCoordinate().y;
	this->goalCol = grid->GetGridGoalCoordinate().x;
	this->occupationMap = grid->GetInflatedMap();
	this->height = grid->GetGridHeight();
	this->width = grid->GetGridWidth();
	this->plannedRoute = plannedRoute;
	this->waypoints = *waypoints;
	this->numOfWaypoints = numOfWaypoints;
}

// Convert coordinate from cv to hamster
Coordinate MapDisplay::ConvertToHamsterCoordinate(Coordinate waypoint)
{
	Coordinate hamsterCoordinate =
	{
		.x = (waypoint.x - startCol),
		.y = -(waypoint.y - startRow)
	};

	return hamsterCoordinate;
}

void MapDisplay::PrintRouteCvMat()
{
	InitMapWithRoute();

	//sleep(1);
	cv::namedWindow("OccupancyGrid-view-route");
	cv::imshow("OccupancyGrid-view-route", routeCvMat);
	cv::waitKey(1);
}

void MapDisplay::PrintRouteCvMat(vector<Particle *> particles)
{
	InitMapWithParticles(particles);

	sleep(1);
	cv::namedWindow("OccupancyGrid-view-particles");
	cv::imshow("OccupancyGrid-view-particles", routeCvMat);
	cv::waitKey(1);
}

void MapDisplay::InitMapWithRoute()
{
	Coordinate start = { .x = startCol, .y = startRow };

	mapFromPlannedRoute.resize(height);

	for (int i = 0; i < height; i++)
	{
		mapFromPlannedRoute.at(i).resize(width);
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(mapFromPlannedRoute.at(i)).at(j) = (occupationMap.at(i)).at(j);
		}
	}

	// Follow the route on the map
	if (plannedRoute.length() <= 0)
	{
		cout << "No route found";
		cout << endl;
	}
	else
	{
		int currDirectionIndex;
		int currWaypointIndex = 0;
		char directionCharacter;
		int chosenXDirection;
		int chosenYDirection;
		bool isWaypoint = false;

		unsigned int x = start.x;
		unsigned int y = start.y;

		(mapFromPlannedRoute.at(y)).at(x) = START;

		for (int i = 0; i < plannedRoute.length(); i++)
		{
			directionCharacter = plannedRoute.at(i);
			currDirectionIndex = numericCharToInt(directionCharacter);

			chosenXDirection = dirX[currDirectionIndex];
			chosenYDirection = dirY[currDirectionIndex];

			x += chosenXDirection;
			y += chosenYDirection;
		}

		(mapFromPlannedRoute.at(y)).at(x) = GOAL;

		// Iterate through all waypoints and mark them on the map
		for (int i = 0; i < numOfWaypoints; i++)
		{
			Coordinate currWaypoint = waypoints.at(i);

			int currCoordinate = (mapFromPlannedRoute.at(currWaypoint.y)).at(currWaypoint.x);

			if (currCoordinate != START && currCoordinate != GOAL)
			{
				(mapFromPlannedRoute.at(currWaypoint.y)).at(currWaypoint.x) = WAYPOINT;
			}
		}

		routeCvMat = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(0));

		// Initialize the map with the route
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int currentCellValue = (mapFromPlannedRoute.at(i)).at(j);
				ColorPixelByRoute(currentCellValue, i, j);
			}
		}
	}
}

void MapDisplay::InitMapWithParticles(vector<Particle *> particles)
{
	Coordinate start = { .x = startCol, .y = startRow };

	mapFromPlannedRoute.resize(height);

	for (int i = 0; i < height; i++)
	{
		mapFromPlannedRoute.at(i).resize(width);
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			(mapFromPlannedRoute.at(i)).at(j) = (occupationMap.at(i)).at(j);
		}
	}

	routeCvMat = cv::Mat(height, width, CV_8UC3, cv::Scalar::all(0));

	for (int i = 0; i < particles.size(); i++)
	{
		Particle * currParticle = particles.at(i);
		bool isTopParticle = i >= particles.size() - TOP_PARTICLES_NUM;

		bool isTopBeliefParticle = isTopParticle /*&& currParticle->belief > BELIEF_LEVEL*/;

		if (isTopBeliefParticle)
		{
			(mapFromPlannedRoute.at(currParticle->i)).at(currParticle->j) = TOP_PARTICLE;
		}
		else
		{
			(mapFromPlannedRoute.at(currParticle->i)).at(currParticle->j) = PARTICLE;
		}
	}

	// Initialize the map with the particles
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int currentCellValue = (mapFromPlannedRoute.at(i)).at(j);
			ColorPixelByParticles(currentCellValue, i, j);
		}
	}
}

void MapDisplay::ColorPixelByRoute(int currentCellValue, int i, int j)
{
	switch(currentCellValue)
	{
		case(0):
		{
			// Color in white
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 255;
			break;
		}
		case(OBSTACLE):
		{
			// Color in black
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 0;
			break;
		}
		case(START):
		{
			// Color in blue
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 0;
			break;
		}
		case(GOAL):
		{
			// Color in green
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 0;
			break;
		}
		case(WAYPOINT):
		{
			// Color in yellow
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 255;
			break;
		}
		default:	// unknown
		{
			// Color in gray
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 128;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 128;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 128;
			break;
		}
	}
}

void MapDisplay::ColorPixelByParticles(int currentCellValue, int i, int j)
{
	switch(currentCellValue)
	{
		case(FREE_SPACE):
		{
			// Color in white
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 255;
			break;
		}
		case(OBSTACLE):
		{
			// Color in black
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 0;
			break;
		}
		case(PARTICLE):
		{
			// Color in red
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 255;
			break;
		}
		case(TOP_PARTICLE):
		{
			// Color in green
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 0;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 255;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 0;
			break;
		}
		default:	// unknown
		{
			// Color in gray
			routeCvMat.at<cv::Vec3b>(i, j)[0] = 128;
			routeCvMat.at<cv::Vec3b>(i, j)[1] = 128;
			routeCvMat.at<cv::Vec3b>(i, j)[2] = 128;
			break;
		}
	}
}

MapDisplay::~MapDisplay()
{
	cv::destroyWindow("OccupancyGrid-view-route");
	cv::destroyWindow("OccupancyGrid-view-particles");
}
