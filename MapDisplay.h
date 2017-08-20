/**
* Robotics
*
* MapDisplay.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef MAPDISPLAY_H_
#define MAPDISPLAY_H_

#include "Grid.h"
#include "Particle.h"
#include <string>
#include <vector>
#include <queue>
#include <HamsterAPIClientCPP/Hamster.h>

#include "Coordinate.h"
using namespace std;
using namespace HamsterAPI;

class MapDisplay
{
private:
	Grid grid;
	int startRow;
	int startCol;
	int goalRow;
	int goalCol;
	vector<vector<bool> > occupationMap;
	int height;
	int width;
	vector<vector<int> > mapFromPlannedRoute;
	cv::Mat_<cv::Vec3b> routeCvMat;
	string plannedRoute;
	vector<Coordinate> waypoints;
	int numOfWaypoints;
	void InitMapWithRoute();
	void InitMapWithParticles(vector<Particle *> particles);
	void ColorPixelByRoute(int currentCellValue, int i, int j);
	void ColorPixelByParticles(int currentCellValue, int i, int j);

public:
	MapDisplay(Grid * grid, string plannedRoute, vector<Coordinate> * waypoints, int numOfWaypoints);
	Coordinate ConvertToHamsterCoordinate(Coordinate waypoint);
	void PrintWaypoints();
	void PrintRouteCvMat();
	void PrintRouteCvMat(vector<Particle *> particles);
	virtual ~MapDisplay();
};

#endif /* MAPDISPLAY_H_ */
