#pragma once
using namespace std;

#include "FlyingMob.h"
#include "Enemy.h"

class AerwhaleClass;
class Level;

class Aerwhale : public FlyingMob, public Enemy
{
public:
	eINSTANCEOF GetType() { return eTYPE_AERWHALE; }
	static Entity *create(Level *level) { return new Aerwhale(level); }
	int floatDuration;
    double xTarget, yTarget, zTarget;
	Aerwhale(Level *level);
	virtual bool hurt(DamageSource *source, float dmg);

private: 
	shared_ptr<Entity> target;
	void _init();
	virtual bool canReach(double xt, double yt, double zt, double dist);

protected:
	virtual int getAmbientSound();
	virtual int getHurtSound();
	virtual int getDeathSound();
	virtual void defineSynchedData();
	virtual void registerAttributes();
	virtual void serverAiStep();

public:
	virtual bool canSpawn();
	virtual int getMaxSpawnClusterSize();
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);
};
