/**
* Robotics
*
* Particle.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "Particle.h"
#include <math.h>

// Ctor
Particle::Particle()
{}

// Ctor
Particle::Particle(double posX, double posY, double yaw)
{
	this->x = posX;
	this->y = posY;
	this->yaw = yaw;
}

Coordinate Particle::GetCoordinate()
{
	Coordinate coordinate = { .x = x, .y = y, .yaw = yaw };

	return coordinate;
}

Particle::~Particle() {}
