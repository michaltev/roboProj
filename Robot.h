/**
* Robotics
*
* Robot.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef ROBOT_H_
#define ROBOT_H_

#include "LocalizationManager.h"
#include <HamsterAPIClientCPP/Hamster.h>

#include "Coordinate.h"
using namespace HamsterAPI;

class Robot
{
private:
	Hamster * hamster;
	double hamsterStartX, hamsterStartY;
	LocalizationManager * localizationManager;
	double prevX, prevY, prevYaw, currX, currY, currYaw;
	int inflationRadius;
	double mapHeight, mapWidth;

public:
	Robot(Hamster * hamster, LocalizationManager * localizationManager, int inflationRadius, double mapHeight, double mapWidth);
	void Init(Coordinate startCoordinate);
	Coordinate GetCurrHamsterCoordinate();
	double GetDeltaX() const;
	double GetDeltaY() const;
	double GetDeltaYaw() const;
	void UpdateCoordinate();
	virtual ~Robot();
};

#endif /* ROBOT_H_ */
