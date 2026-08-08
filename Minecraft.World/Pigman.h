#pragma once

using namespace std;

#include "Animal.h"

class Player;
class LightningBolt;

class Pigman : public Animal
{
public:
	eINSTANCEOF GetType() { return eTYPE_PIGMAN; }
	static Entity *create(Level *level){ return new Pigman(level); }

	Pigman(Level *level);

	virtual bool useNewAi();

	virtual bool mobInteract(shared_ptr<Player> player);

protected:
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual bool removeWhenFarAway();
	virtual int getAmbientSound();
	virtual int getHurtSound();
	virtual int getDeathSound();
	virtual void playStepSound(int xt, int yt, int zt, int t);
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
	virtual void thunderHit(const LightningBolt *lightningBolt);
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);

	virtual void readAdditionalSaveData(CompoundTag *tag);

private:
	static const int GOLD_INGOT_HOLD_TICKS = 20 * 4;
	int tradeTimer;
	void finishTradeOffer();

	static const int TREASURE_ITEMS_COUNT = 13;
	static WeighedTreasure *pigmanLoot[TREASURE_ITEMS_COUNT];
};
