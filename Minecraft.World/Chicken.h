#pragma once
using namespace std;

#include "Animal.h"
#include "SharedConstants.h"

class Level;
class CompoundTag;

class Chicken : public Animal
{
public:
	eINSTANCEOF GetType() { return eTYPE_CHICKEN; }
	static Entity *create(Level *level) { return new Chicken(level); }
    bool sheared;
    float flap;
    float flapSpeed;
    float oFlapSpeed, oFlap;
    float flapping;
    int eggTime;
	bool isChickenJockey;

	static const int TYPE_DEFAULT = 0;
	static const int TYPE_BLACK = 1;
	static const int TYPE_RED = 2;

private:
	static const int DATA_TYPE_ID = 14;

private:	
	void _init();

public:
	Chicken(Level *level);
	virtual bool useNewAi();

	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);
	virtual int getChickenType();
	virtual void setChickenType(int type);

protected:
	void registerAttributes();

	virtual void defineSynchedData();

public:
	virtual void aiStep();	

protected:
	virtual void causeFallDamage(float distance);
	virtual int getAmbientSound();
    virtual int getHurtSound();
    virtual int getDeathSound();
	virtual void playStepSound(int xt, int yt, int zt, int t);
    virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

	virtual void addAdditonalSaveData(CompoundTag *entityTag, CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);

public:
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);
	virtual bool isFood(shared_ptr<ItemInstance> itemInstance);

};
