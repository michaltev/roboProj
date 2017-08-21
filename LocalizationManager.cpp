/**
* Robotics
*
* LocalizationManager.cpp
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#include "LocalizationManager.h"
#include "math.h"
#include <iostream>
#include <algorithm>

#define NUM_OF_PARTICLES 350
#define BAD_BELIEF_PARTICLES 80
#define GOOD_BELIEF_PARTICLES 80
#define ADJACENT_PARTICLE_DESTINATION 1
#define PARTICLE_CORRECTION_MAX_RETRIES 20
#define RADIUS 100

LocalizationManager::LocalizationManager(
	Hamster * hamster, OccupancyGrid & ogrid,
	double mapHeight, double mapWidth, double mapResolution) :
		hamster(hamster), ogrid(ogrid),
		ogridHeight(mapHeight), ogridWidth(mapWidth), ogridResolution(mapResolution)
{}

void LocalizationManager::InitParticles(Coordinate currCoordinate)
{
	particles.resize(NUM_OF_PARTICLES);

	for (size_t i = 0; i < particles.size(); i++)
	{
		particles.at(i) = new Particle();
		RandomizeParticle(particles.at(i), currCoordinate);
	}
}

vector<Particle *> LocalizationManager::GetParticles() const
{
	return particles;
}

// Return rather the belief of x is greater than y's
bool ParticleCompare(Particle * x, Particle * y)
{
	return (x->belief < y->belief);
}

Particle * LocalizationManager::GetTopParticle() const
{
	return *(std::max_element(particles.begin(), particles.end(), ParticleCompare));
}

void LocalizationManager::ConvertFromMapCoordinateToMatIndex(Particle * particle)
{
	particle->i = particle->x / ogridResolution + (double) ogridWidth / 2;
	particle->j = (double) ogridHeight / 2 - particle->y / ogridResolution;
}

void LocalizationManager::ConvertFromMatIndexToMapCoordinate(Particle * particle)
{
	particle->x = (particle->i - (double) ogridWidth / 2) * ogridResolution;
	particle->y = ((double) ogridHeight / 2 - particle->j) * ogridResolution;
}

bool LocalizationManager::InsertParticleIntoMap(Particle * particle)
{
	Particle * copyParticle = new Particle(*particle);
	int distance, count = 0;

	do
	{
		// Finds closest free cell to the out-of-range particle
		distance = 10 - rand() % 20;
		particle->j = copyParticle->j + distance;

		distance = 10 - rand() % 20;
		particle->i = copyParticle->i + distance;

		count++;
	} while (ogrid.getCell(particle->i, particle->j) != CELL_FREE && count < PARTICLE_CORRECTION_MAX_RETRIES);

	// Convert indices to coordinate on map
	ConvertFromMatIndexToMapCoordinate(particle);

	delete copyParticle;

	return count < PARTICLE_CORRECTION_MAX_RETRIES;
}

float LocalizationManager::CalculateBelief(Particle * particle)
{
	LidarScan lidarScan = hamster->getLidarScan();

	int corrects = 0;
	int incorrects = 0;

	for (int i = 0; i < lidarScan.getScanSize(); i++)
	{
		double angle = lidarScan.getScanAngleIncrement() * i * DEG2RAD;

		if (lidarScan.getDistance(i) < lidarScan.getMaxRange() - 0.001)
		{
			// Calculate the obstacle's map coordinate
			double obsX = particle->x + lidarScan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD- 180 * DEG2RAD);
			double obsY = particle->y + lidarScan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD- 180 * DEG2RAD);

			// Convert from map coordinate to matrix cell
			int i = (double) ogridHeight / 2 - obsY / ogridResolution;
			int j = obsX / ogridResolution + ogridWidth / 2;

			// Determine if there was a hit according to the grid
			if (ogrid.getCell(i, j) == CELL_OCCUPIED)
			{
				corrects++;
			}
			else
			{
				incorrects++;
			}
		}
	}

	return (float) corrects / (corrects + incorrects);
}

void LocalizationManager::UpdateParticles(double deltaX, double deltaY, double deltaYaw)
{
	int size = particles.size();

	Coordinate currCoordinate = { .x = deltaX, .y = deltaY, .yaw = deltaYaw };

	for (size_t i = 0; i < particles.size(); i++)
	{
		Particle * currParticle = particles.at(i);

		double r = sqrt(deltaX * deltaX + deltaY * deltaY);
		currParticle->x += r * cos(currParticle->yaw * DEG2RAD);
		currParticle->y += r * sin(currParticle->yaw * DEG2RAD);

		// Modulo 360 calculation
		currParticle->yaw += deltaYaw;

		// Perform the modulu
		if (currParticle->yaw >= 360)
		{
			currParticle->yaw = currParticle->yaw - 360;
		}

		if (currParticle->yaw < 0)
		{
			currParticle->yaw = currParticle->yaw + 360;
		}

		ConvertFromMapCoordinateToMatIndex(currParticle);

		bool isSuccessfullyInserted = false;

		// In case the particle goes outside the free area - try to get it back in
		if (ogrid.getCell(currParticle->i, currParticle->j) != CELL_FREE)
		{
			int indexFromTop = size - rand() % GOOD_BELIEF_PARTICLES - 1;

			if (currParticle->belief > 0.3)
			{
				// Try to get the particle back in
				isSuccessfullyInserted = InsertParticleIntoMap(currParticle);
			}

			if (!isSuccessfullyInserted)
			{
				Particle * betterParticle = particles.at(indexFromTop);

				if (betterParticle->belief > 0.4)
				{
					// Get the current particle closer to another particle which has greater belief
					ImproveParticle(currParticle, betterParticle);
				}
				else
				{
					// If we don't have a better particle, randomize a new one.
					RandomizeParticle(currParticle, currCoordinate);
				}
			}
		}

		currParticle->belief = CalculateBelief(currParticle);
	}

	// Dump low-belief particles and change their coordinate to be close to high belief particles
	std::sort(particles.begin(), particles.end(), ParticleCompare);

	for (int i = 1; i <= BAD_BELIEF_PARTICLES; i++)
	{
		Particle * currParticle = particles.at(i - 1);
		Particle * betterParticle = particles.at(size - i);

		if (betterParticle->belief > 0.3)
		{
			ImproveParticle(currParticle, betterParticle);
			CalculateBelief(currParticle);
		}
		else
		{
			RandomizeParticle(currParticle, currCoordinate);
			CalculateBelief(currParticle);
		}
	}
}

void LocalizationManager::PrintParticles() const
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		Particle * currParticle = particles.at(i);

		cout << "Particle " << i << ": " <<
				currParticle->x << "," <<
				currParticle->y << "," <<
				" yaw: " << currParticle->yaw << "," <<
				" belief: " << currParticle->belief << endl;
	}
}

void LocalizationManager::RandomizeParticle(Particle * particleToUpdate, Coordinate currCoordiante)
{
	do
	{
		particleToUpdate->j = currCoordiante.x + (rand() % RADIUS);
		particleToUpdate->i = currCoordiante.y + (rand() % RADIUS);

	} while (ogrid.getCell(particleToUpdate->i, particleToUpdate->j) != CELL_FREE);

	ConvertFromMatIndexToMapCoordinate(particleToUpdate);

	// Randomize an angle
	particleToUpdate->yaw = rand() % 360;
}

void LocalizationManager::ImproveParticle(Particle * particleToUpdate, Particle * betterParticle)
{
	do
	{
		if (betterParticle->belief < 0.3)
		{
			particleToUpdate->x = betterParticle->x + 0.4 - 0.8*(double)rand()/(double)RAND_MAX;
			particleToUpdate->y = betterParticle->y + 0.4 - 0.8*(double)rand()/(double)RAND_MAX;
		}
		else if (betterParticle->belief < 0.6)
		{
			particleToUpdate->x = betterParticle->x + 0.2 - 0.4*(double)rand()/(double)RAND_MAX;
			particleToUpdate->y = betterParticle->y + 0.2 - 0.4*(double)rand()/(double)RAND_MAX;
		}
		else
		{
			particleToUpdate->x = betterParticle->x + 0.1 - 0.2*(double)rand()/(double)RAND_MAX;
			particleToUpdate->y = betterParticle->y + 0.1 - 0.2*(double)rand()/(double)RAND_MAX;
		}

		ConvertFromMapCoordinateToMatIndex(particleToUpdate);

	} while (ogrid.getCell(particleToUpdate->i, particleToUpdate->j) != CELL_FREE);

	// Randomizing the angle according to the belief of the goodParticle
	if (betterParticle->belief < 0.2)
	{
		particleToUpdate->yaw = (betterParticle->yaw + (180 - rand() % 360));
	}
	else if (betterParticle->belief < 0.4)
	{
		particleToUpdate->yaw = (betterParticle->yaw + (90 - rand() % 180));
	}
	else if (betterParticle->belief < 0.6)
	{
		particleToUpdate->yaw = (betterParticle->yaw + (30 - rand() % 60));
	}
	else if (betterParticle->belief < 0.8)
	{
		particleToUpdate->yaw = (betterParticle->yaw + (10 - rand() % 20));
	}
	else
	{
		particleToUpdate->yaw = (betterParticle->yaw + (5 - rand() % 10));
	}

	//Reducing degrees - modulo 360
	particleToUpdate->yaw = (particleToUpdate->yaw >= 360) ?
			particleToUpdate->yaw - 360 : particleToUpdate->yaw;
	particleToUpdate->yaw = (particleToUpdate->yaw < 0) ?
			particleToUpdate->yaw + 360 : particleToUpdate->yaw;
}

LocalizationManager::~LocalizationManager() {}
