#pragma once

using namespace std;

#include "Monster.h"
#include "SharedConstants.h"

class Player;

class AphalafPlant : public Monster
{
public:
	eINSTANCEOF GetType() { return eTYPE_APHALAF_PLANT; }
	static Entity *create(Level *level){ return new AphalafPlant(level); }

public:
	AphalafPlant(Level *level);
	virtual bool isPushable();
	virtual bool useNewAi();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual bool doHurtTarget(shared_ptr<Entity> target);
	virtual void tick();
	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);

protected:
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual bool removeWhenFarAway();
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual int decreaseAirSupply(int currentSupply);
	virtual void doPush(shared_ptr<Entity> e);
    virtual void pushEntities();
	virtual int getHurtSound();
	virtual int getDeathSound();

private:
	void explode1();
	void explode2();
};
