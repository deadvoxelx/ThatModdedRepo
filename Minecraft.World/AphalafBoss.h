#pragma once

using namespace std;

#include "Monster.h"
#include "BossMob.h"
#include "SharedConstants.h"

class Player;

class AphalafBoss : public Monster, public BossMob
{
public:
	eINSTANCEOF GetType() { return eTYPE_APHALAF_BOSS; }
	static Entity *create(Level *level){ return new AphalafBoss(level); }

public:
	AphalafBoss(Level *level);
	virtual bool isPushable();
	virtual bool useNewAi();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual bool doHurtTarget(shared_ptr<Entity> target);
	virtual void tick();

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

public:
	virtual float getMaxHealth() { return Monster::getMaxHealth(); };
	virtual float getHealth() { return Monster::getHealth(); };
	virtual wstring getAName() { return app.GetString(IDS_APHALAF_BOSS); };
	virtual int getDimension() { return Entity::dimension; };

private:
	void explode1();
	void explode2();
};
