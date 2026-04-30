#pragma once
using namespace std;

#include "FlyingMob.h"
#include "Enemy.h"
#include "SharedConstants.h"

class Evupul : public FlyingMob, public Enemy
{
public:
	eINSTANCEOF GetType() { return eTYPE_EVUPUL; }
	static Entity *create(Level *level) { return new Evupul(level); }

	Evupul(Level *level);

	static const int TYPE_DEFAULT = 0;
	static const int TYPE_GOLD = 1;

private:
	static const int DATA_TYPE_ID = 14;

protected:
	virtual void registerAttributes();
	virtual void defineSynchedData();

protected:
	virtual bool useNewAi();

public:
	virtual void aiStep();
	virtual void tick();
	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);
	virtual int getEvupulType();
	virtual void setEvupulType(int type);

private:
	Pos *targetPosition;
	double moveTargetX, moveTargetY, moveTargetZ;

protected:
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual void newServerAiStep();
	virtual bool makeStepSound();
	virtual void causeFallDamage(float distance);
	virtual void checkFallDamage(double ya, bool onGround);

protected:
	virtual bool removeWhenFarAway();
};
