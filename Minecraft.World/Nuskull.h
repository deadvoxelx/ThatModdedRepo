#pragma once
using namespace std;

#include "FlyingMonster.h"
#include "SharedConstants.h"

class Nuskull : public FlyingMonster
{
public:
	eINSTANCEOF GetType() { return eTYPE_NUSKULL; }
	static Entity *create(Level *level) { return new Nuskull(level); }

	static const int TYPE_DEFAULT = 0;
	static const int TYPE_VARIANT = 1;

	Nuskull(Level *level);

protected:
	virtual void defineSynchedData();
	virtual void registerAttributes();
	virtual bool useNewAi();

public:
	virtual void aiStep();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual void tick();

private:
	Pos *targetPosition;
	double moveTargetX, moveTargetY, moveTargetZ;

	static const int DATA_TYPE_ID = 14;

protected:
	virtual int getDeathLoot();
	virtual void newServerAiStep();
	virtual bool makeStepSound();
	virtual void causeFallDamage(float distance);
	virtual void checkFallDamage(double ya, bool onGround);
	virtual bool removeWhenFarAway();

public:
	virtual int getNuskullType();
	virtual void setNuskullType(int type);
	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);
	virtual void readAdditionalSaveData(CompoundTag *tag);
	virtual void addAdditonalSaveData(CompoundTag *entityTag);
};
