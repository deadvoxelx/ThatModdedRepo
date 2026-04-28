#pragma once
using namespace std;

#include "FlyingMonster.h"
#include "SharedConstants.h"

class EvupulDark : public FlyingMonster
{
public:
	eINSTANCEOF GetType() { return eTYPE_EVUPULDARK; }
	static Entity *create(Level *level) { return new EvupulDark(level); }

	EvupulDark(Level *level);

protected:
	virtual void registerAttributes();

protected:
	virtual bool useNewAi();

public:
	virtual void aiStep();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual void tick();
	virtual bool doHurtTarget(shared_ptr<Entity> target);

private:
	Pos *targetPosition;
	double moveTargetX, moveTargetY, moveTargetZ;

protected:
	virtual int getDeathLoot();
	virtual void newServerAiStep();
	virtual bool makeStepSound();
	virtual void causeFallDamage(float distance);
	virtual void checkFallDamage(double ya, bool onGround);

protected:
	virtual bool removeWhenFarAway();
};
