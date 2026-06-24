#pragma once
using namespace std;

#include "Mob.h"
#include "Enemy.h"
#include "ParticleTypes.h"

class Swet : public Mob, public Enemy
{
public:
	eINSTANCEOF GetType() { return eTYPE_SWET; }
	static Entity *create(Level *level) { return new Swet(level); }

static const int TYPE_BLUE = 0;
	static const int TYPE_GOLD= 1;

private:
	static const int DATA_TYPE_ID = 14;

	static const int ID_SIZE = 16;

public:
	float targetSquish;
	float squish;
	float oSquish;

private:
	int jumpDelay;

	void _init();

public:
	Swet(Level *level);

	virtual double getRideHeight();
	virtual bool mobInteract(shared_ptr<Player> player);
	virtual int getSwetType();
	virtual void setSwetType(int type);

protected: 
	virtual void defineSynchedData();

public:
	using Mob::setSize;

	virtual void setSize(int size);
	virtual int getSize();
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);
	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);

protected:
	virtual ePARTICLE_TYPE getParticleName();
	virtual int getSquishSound();

public:
	virtual void tick();

protected:
	virtual void serverAiStep();
	virtual void decreaseSquish();
	virtual int getJumpDelay();

public:
	virtual void playerTouch(shared_ptr<Player> player);

protected:
	virtual bool isDealsDamage();
	virtual int getAttackDamage();
	virtual int getHurtSound();
	virtual int getDeathSound();

public:
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual bool canSpawn();

protected:
	virtual float getSoundVolume();

public:
	virtual int getMaxHeadXRot();

protected:
	virtual bool doPlayJumpSound();
	virtual bool doPlayLandSound();
};
