#pragma once

using namespace std;

#include "Animal.h",
#include "SharedConstants.h"

class Player;
class Level;

class Cow : public Animal
{
public:
	eINSTANCEOF GetType() { return eTYPE_COW; }
	static Entity *create(Level *level) { return new Cow(level); }

	static const int TYPE_DEFAULT = 0;
	static const int TYPE_BROWN= 1;
	static const int TYPE_BLACK = 2;
	static const int TYPE_INVERT = 3;

private:
	static const int DATA_TYPE_ID = 14;

public:
	Cow(Level *level);
	virtual bool useNewAi();

	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);
	virtual int getCowType();
	virtual void setCowType(int type);

protected:
	virtual void registerAttributes();
	virtual int getAmbientSound();
	virtual int getHurtSound();
	virtual int getDeathSound();
	virtual float getSoundVolume();
	virtual int getDeathLoot();
	virtual void playStepSound(int xt, int yt, int zt, int t);
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

	virtual void defineSynchedData();

public:
	virtual bool mobInteract(shared_ptr<Player> player);
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);
};
