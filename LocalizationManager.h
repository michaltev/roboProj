/**
* Robotics
*
* LocalizationManager.h
*
* Students:
* Shir Zerahia - 308417823
* Michal Teverovsky - 313522039
* Yonatan Berg - 311123327
*
* 22.8.17
*/

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include "Particle.h"
#include <vector>
#include <HamsterAPIClientCPP/Hamster.h>
using namespace std;
using namespace HamsterAPI;

class LocalizationManager
{
	private:
		vector<Particle *> particles;
		const OccupancyGrid & ogrid;
		int ogridHeight, ogridWidth;
		double ogridResolution;
		Hamster * hamster;
		void ConvertFromMapCoordinateToMatIndex(Particle * particle);
		void ConvertFromMatIndexToMapCoordinate(Particle * particle);
		float CalculateBelief(Particle * particle);
		bool InsertParticleIntoMap(Particle * particle);
		void RandomizeParticle(Particle * particleToUpdate, Coordinate currCoordiante);
		void ImproveParticle(Particle * particleToUpdate, Particle * betterParticle);

	public:
		LocalizationManager(
				Hamster * hamster, OccupancyGrid & ogrid,
				double mapHeight, double mapWidth, double mapResolution);
		void InitParticles(Coordinate currCoordinate);
		vector<Particle *> GetParticles() const;
		Particle * GetTopParticle() const;
		void UpdateParticles(double deltaX, double deltaY, double deltaYaw);
		void PrintParticles() const;
		virtual ~LocalizationManager();
};

#endif /* LOCALIZATIONMANAGER_H_ */
