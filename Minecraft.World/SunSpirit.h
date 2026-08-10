#pragma once

using namespace std;

#include "Monster.h"

class Player;

class SunSpirit : public Monster
{
public:
	eINSTANCEOF GetType() { return eTYPE_SUN_SPIRIT; }
	static Entity *create(Level *level){ return new SunSpirit(level); }

	SunSpirit(Level *level);
	virtual bool useNewAi();
	virtual void aiStep();
	virtual bool isPushable();
	virtual bool hurt(DamageSource *source, float dmg);

protected:
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual bool removeWhenFarAway();
	virtual void causeFallDamage(float distance);
};
