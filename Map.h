/**
* Robotics
*
* Map.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef MAP_H_
#define MAP_H_

#include "Grid.h"
#include <unistd.h>
#include <HamsterAPIClientCPP/Hamster.h>
using namespace std;
using namespace HamsterAPI;

class Map
{
private:
	OccupancyGrid map;
	double mapResolutionInCm;	/* Each OccupancyGrid cell takes an area of mapResolutionInCm^2 centimeters */
	cv::Mat originalCvMat;
	cv::Mat inflatedCvMat;
	int mapWidth;
	int mapHeight;
	int robotSizeInCm;
	Coordinate startCoordinate;
	Coordinate goalCoordinate;
	void InitCvMatFromMap();
	vector< vector<bool> > InflateOccupationMap();
	void InitInflatedCvMat();
	bool DoesCellHaveOccupiedNeighbor(int rowIndex, int colIndex);

public:
	vector< vector<bool> > occupationMap;
	Grid grid;
	int inflationRadius;
	Map();
	Map(HamsterAPI::OccupancyGrid * map, int robotSizeInCm,
		Coordinate startCoordinate, Coordinate goalCoordinate,
		double mapHeight, double mapWidth);
	virtual ~Map();
};

#endif /* MAP_H_ */
