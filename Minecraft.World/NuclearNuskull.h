#pragma once
using namespace std;

#include "FlyingMonster.h"
#include "SharedConstants.h"

class NuclearNuskull : public FlyingMonster
{
public:
	eINSTANCEOF GetType() { return eTYPE_NUCLEAR_NUSKULL; }
	static Entity *create(Level *level) { return new NuclearNuskull(level); }

	NuclearNuskull(Level *level);
	virtual int getFuseTick();

protected:
	virtual void registerAttributes();
	virtual bool useNewAi();

public:
	virtual void aiStep();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual void tick();

private:
	Pos *targetPosition;
	double moveTargetX, moveTargetY, moveTargetZ;

	float flyX, flyY, flyZ;

	int fuse;

protected:
	virtual int getDeathLoot();
	virtual void newServerAiStep();
	virtual bool makeStepSound();
	virtual void causeFallDamage(float distance);
	virtual void checkFallDamage(double ya, bool onGround);
	virtual bool removeWhenFarAway();
};
