#pragma once
using namespace std;

#include "Zombie.h"

class DamageSource;

class Zombice : public Zombie
{
public:
	eINSTANCEOF GetType() { return eTYPE_ZOMBICE; }
	static Entity *create(Level *level) { return new Zombice(level); }

private:
	void _init();

public:
	Zombice(Level *level);

protected:
	virtual void registerAttributes();
	virtual bool useNewAi();

public:
	virtual void tick();
	virtual bool canSpawn();

protected:
	virtual shared_ptr<Entity> findAttackTarget();

public:
	virtual bool hurt(DamageSource *source, float dmg);
	virtual void aiStep();

private:
	void alert(shared_ptr<Entity> target);

protected:
	virtual int getAmbientSound();
	virtual int getHurtSound();
	virtual int getDeathSound();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual void populateDefaultEquipmentSlots();

public:
	virtual bool mobInteract(shared_ptr<Player> player);

protected:
	virtual void dropRareDeathLoot(int rareLootLevel);
	virtual int getDeathLoot();

	virtual int decreaseAirSupply(int currentSupply);

public:
	virtual bool doHurtTarget(shared_ptr<Entity> target);
	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);
};
