#pragma once
using namespace std;

#include "FlyingMob.h"
#include "Enemy.h"

class ZephyrClass;
class Level;

class Zephyr : public FlyingMob, public Enemy
{
public:
	eINSTANCEOF GetType() { return eTYPE_ZEPHYR; }
	static Entity *create(Level *level) { return new Zephyr(level); }

private:
	static const int DATA_IS_CHARGING = 16;

public:
	int floatDuration;
    double xTarget, yTarget, zTarget;

private: 
	shared_ptr<Entity> target;
    int retargetTime;

public:
	int oCharge;
    int charge;

private:
	void _init();

public:
	Zephyr(Level *level);

	virtual bool isCharging();
	virtual bool hurt(DamageSource *source, float dmg);

protected:
	virtual void defineSynchedData();
	virtual void registerAttributes();

protected:
	virtual void serverAiStep();

private:
	virtual bool canReach(double xt, double yt, double zt, double dist);

protected:
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
	virtual bool canSpawn();
	virtual int getMaxSpawnClusterSize();
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);
};
