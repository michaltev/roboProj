/**
* Robotics
*
* Robot.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "Robot.h"

Robot::Robot(
	Hamster * hamster, LocalizationManager * localizationManager, int inflationRadius,
	double mapHeight, double mapWidth)
{
	this->hamster = hamster;
	this->localizationManager = localizationManager;
	this->inflationRadius = inflationRadius;
	this->mapHeight = mapHeight;
	this->mapWidth = mapWidth;

	this->prevX = 0;
	this->prevY = 0;
	this->prevYaw = 0;
	this->currX = 0;
	this->currY = 0;
	this->currYaw = 0;
}

void Robot::Init(Coordinate startCoordinate)
{
	hamsterStartX = startCoordinate.x - (mapWidth / 2);
	hamsterStartY = startCoordinate.y - (mapHeight / 2);

	currX = hamsterStartX;
	currY = hamsterStartY;
	currYaw = startCoordinate.yaw;

	Pose initialPose;
	initialPose.setX(hamsterStartX);
	initialPose.setY(hamsterStartY);
	initialPose.setHeading(startCoordinate.yaw);

	sleep(3);
	hamster->setInitialPose(initialPose);

	//localizationManager->InitParticles();

	UpdateCoordinate();
}

double Robot::GetDeltaX() const
{
	return currX - prevX;
}

double Robot::GetDeltaY() const
{
	return currY - prevY;
}

double Robot::GetDeltaYaw() const
{
	return currYaw - prevYaw;
}

Coordinate Robot::GetCurrHamsterCoordinate()
{
	/*Particle * topParticle = localizationManager->GetTopParticle();

	Coordinate currCoordinate;
	currCoordinate = {
			.x = topParticle->x + 2*inflationRadius,
			.y = topParticle->y + 2*inflationRadius,
			.yaw = topParticle->yaw
	};*/


	Pose currPose = hamster->getPose();

	// Convert the coordinates
	double currX = (currPose.getX() - hamsterStartX) * 10;
	double currY = (currPose.getY() - hamsterStartY) * 10;

	double currYaw = currPose.getHeading();

	if (currYaw < 0)
	{
		currYaw += 360;
	}
	else if (currYaw > 360)
	{
		currYaw -= 360;
	}

	Coordinate currCoordinate = { .x = currX, .y = currY, .yaw = currYaw };

	return currCoordinate;
}

void Robot::UpdateCoordinate()
{
	prevX = currX;
	prevY = currY;
	prevYaw = currYaw;

	Coordinate currentCoordinate = GetCurrHamsterCoordinate();

	// Update the current and previous coordinates by the position of the robot
	currX = currentCoordinate.x;
	currY = currentCoordinate.y;
	currYaw = currentCoordinate.yaw;
}

Robot::~Robot() {}
