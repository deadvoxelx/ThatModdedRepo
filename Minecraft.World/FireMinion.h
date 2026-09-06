#pragma once

using namespace std;

#include "Monster.h"
#include "SharedConstants.h"
#include "MobGroupData.h"

class Player;

class FireMinion : public Monster
{
public:
	eINSTANCEOF GetType() { return eTYPE_FIRE_MINION; }
	static Entity *create(Level *level){ return new FireMinion(level); }

	FireMinion(Level *level);
	virtual bool useNewAi();
	virtual void aiStep();
	virtual bool isPushable();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual bool doHurtTarget(shared_ptr<Entity> target);

protected:
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual int getDeathLoot();
	virtual void causeFallDamage(float distance);
	virtual bool makeStepSound();
};
