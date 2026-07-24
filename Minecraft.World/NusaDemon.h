#pragma once

using namespace std;

#include "Monster.h"
#include "SharedConstants.h"

class Player;

class NusaDemon : public Monster
{
public:
	eINSTANCEOF GetType() { return eTYPE_NUSA_DEMON; }
	static Entity *create(Level *level){ return new NusaDemon(level); }

	NusaDemon(Level *level);
	virtual bool isPushable();
	virtual bool useNewAi();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual void die(DamageSource *source);

protected:
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual bool removeWhenFarAway();
	virtual int getDeathLoot();
	virtual int decreaseAirSupply(int currentSupply);
	virtual void doPush(shared_ptr<Entity> e);
    virtual void pushEntities();
	virtual int getHurtSound();
	virtual int getDeathSound();
};
