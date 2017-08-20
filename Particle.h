/**
* Robotics
*
* Particle.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <vector>

#include "Coordinate.h"
using namespace std;

class Particle
{
public:
	int i, j;
	double x, y, yaw, belief;
	Particle();
	Particle(double posX, double posY, double yaw);
	Coordinate GetCoordinate();
	~Particle();
};

#endif /* PARTICLE_H_ */
