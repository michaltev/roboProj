/*
 * HamsterAPIClientSimpleBehaviourExample.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: ofir
 */

#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
using namespace std;
using namespace HamsterAPI;
HamsterAPI::Hamster * hamster;

void getScansBetween(double min, double max, std::vector<double> & distances) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++) {
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)
			distances.push_back(scan.getDistance(i));
	}
}

bool willCollide(std::vector<double> distances, int angle_from_center, int& collisionCount) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	int collisions = 0;

	for (size_t i = distances.size() / 2 - angle_from_center / 2;
			i < distances.size() / 2 + angle_from_center / 2; i++)
		if (distances[i] < scan.getMaxRange() / 4.0)
			collisions++;
	collisionCount = collisions;

	return collisions >= angle_from_center / 4.0;
}

bool willCollide(std::vector<double> distances, int angle_from_center) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	int collisions = 0;

	for (size_t i = distances.size() / 2 - angle_from_center / 2;
			i < distances.size() / 2 + angle_from_center / 2; i++)
		if (distances[i] < scan.getMaxRange() / 4.0)
			collisions++;

	return collisions >= angle_from_center / 4.0;
}

bool isFrontFree(int &collisionCount) {
	// Degrees : [90, 270]

	std::vector<double> distances;

	getScansBetween(90, 270, distances);

	return !willCollide(distances, 40, collisionCount);
}

bool isLeftFree(int &collisionCount) {
	// Degrees : [180,360]

	std::vector<double> distances;

	getScansBetween(180, 360, distances);

	return !willCollide(distances, 40, collisionCount);
}

bool isRightFree(int &collisionCount) {
	// Degrees : [0, 180]

	std::vector<double> distances;

	getScansBetween(0, 180, distances);

	return !willCollide(distances, 40, collisionCount);
}

bool isBackFree(int &collisionCount) {
	// Degrees : [270,360], [0, 90]

	std::vector<double> distances;

	getScansBetween(270, 360, distances);
	getScansBetween(0, 90, distances);

	return !willCollide(distances, 40, collisionCount);
}

bool isFrontFree() {
	// Degrees : [90, 270]

	std::vector<double> distances;

	getScansBetween(90, 270, distances);

	return !willCollide(distances, 40);
}

bool isLeftFree() {
	// Degrees : [180,360]

	std::vector<double> distances;

	getScansBetween(180, 360, distances);

	return !willCollide(distances, 40);
}

bool isRightFree() {
	// Degrees : [0, 180]

	std::vector<double> distances;

	getScansBetween(0, 180, distances);

	return !willCollide(distances, 40);
}

bool isBackFree() {
	// Degrees : [270,360], [0, 90]

	std::vector<double> distances;

	getScansBetween(270, 360, distances);
	getScansBetween(0, 90, distances);

	return !willCollide(distances, 40);
}


void moveForward() {
	HamsterAPI::Log::i("Client", "Moving Forward");
	hamster->sendSpeed(0.4, 0.0);
}

void turnLeft() {
	HamsterAPI::Log::i("Client", "Turning Left");


	while (!isFrontFree())
		hamster->sendSpeed(0.04, 45.0);
}

void turnRight() {
	HamsterAPI::Log::i("Client", "Turning Right");
	while (!isFrontFree())
		hamster->sendSpeed(0.04, -45.0);
}

void moveBackwards() {
	HamsterAPI::Log::i("Client", "Moving Backwards");
	while (!isLeftFree() && !isRightFree() && isBackFree())
		hamster->sendSpeed(-0.4, 0.0);
	if (isLeftFree())
		turnLeft();
	else
		turnRight();
}

void stopMoving() {
	hamster->sendSpeed(0.0, 0.0);
}

int main(int argc, char ** argv) {
	try {
		hamster = new HamsterAPI::Hamster(1);
		sleep(1);

		while (hamster->isConnected()) {
			try {
				if (isFrontFree())
					moveForward();
				else {
					stopMoving();
					int leftCols = 0;
					int rightCols = 0;

					bool rightFree = isRightFree(rightCols);
					bool leftFree = isLeftFree(leftCols);

					if (rightFree || leftFree)
					{
						if(leftCols >= rightCols)
						{
							turnRight();
						}
						else
						{
							turnLeft();
						}
					}
					else if (isBackFree())
						moveBackwards();
					else
						HamsterAPI::Log::i("Client", "I am stuck!");
				}

				// Speed Getter
				// HamsterAPI::Speed speed = hamster.getSpeed();
			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}
