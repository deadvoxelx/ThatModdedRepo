#pragma once

#include "Cow.h"

class FlyingCow : public Cow
{
public:
	eINSTANCEOF GetType() { return eTYPE_FLYING_COW; }
	static Entity *create(Level *level) { return new FlyingCow(level); }

public:
	FlyingCow(Level *level);

	virtual void aiStep();

	virtual bool mobInteract(shared_ptr<Player> player);
	virtual bool canSpawn();
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);

protected:
	virtual void causeFallDamage(float distance);
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
};
