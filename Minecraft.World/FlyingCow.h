#pragma once

#include "Cow.h"

class FlyingCow : public Cow
{
public:
	eINSTANCEOF GetType() { return eTYPE_FLYING_COW; }
	static Entity *create(Level *level) { return new FlyingCow(level); }

public:
	FlyingCow(Level *level);

	virtual bool mobInteract(shared_ptr<Player> player);
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);

protected:
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
};
