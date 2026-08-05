#pragma once

using namespace std;

#include "TamableAnimal.h"
#include "SharedConstants.h"

class DamageSource;
class Player;
class Level;

class Moa : public TamableAnimal
{
private:
    static const int DATA_HEALTH_ID = 18;

    static const int START_HEALTH = 30;
    static const int MAX_HEALTH = 30;
    static const int TAME_HEALTH = 30;

	static const int DATA_TYPE_ID = 14;

public:
	float flapM;
    float flapSpeedM;
    float oFlapSpeedM, oFlapM;
    float flappingM;

	static const int TYPE_DEFAULT = 0;
	static const int TYPE_WHITE = 1;
	static const int TYPE_BLACK = 2;

	eINSTANCEOF GetType() { return eTYPE_MOA; }
	static Entity *create(Level *level) { return new Moa(level); }

	Moa(Level *level);

	virtual int getMoaType();
	virtual void setMoaType(int type);

	virtual bool useNewAi();
	virtual void setTarget(shared_ptr<LivingEntity> target);
	virtual bool canSpawn();
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);

protected:
	virtual void causeFallDamage(float distance);
	virtual void registerAttributes();
	virtual void serverAiMobStep();
	virtual void defineSynchedData();

	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
	virtual void aiStep();
	virtual void tick();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual bool doHurtTarget(shared_ptr<Entity> target);
	virtual void setTame(bool value);
	virtual bool mobInteract(shared_ptr<Player> player);
	virtual void handleEntityEvent(byte id);
	virtual bool isFood(shared_ptr<ItemInstance> item);
	bool isAngry();
	void setAngry(bool value);
	void tame(const wstring &wsOwnerUUID, bool bDisplayTamingParticles, bool bSetSitting);

	int GetSynchedHealth();
	virtual bool canMate(shared_ptr<Animal> animal);
	virtual bool wantsToAttack(shared_ptr<LivingEntity> target, shared_ptr<LivingEntity> owner);
	virtual MobGroupData *finalizeMobSpawn(MobGroupData *groupData, int extraData = 0);

protected:
	virtual shared_ptr<AgableMob> getBreedOffspring(shared_ptr<AgableMob> target);
	virtual bool removeWhenFarAway();
};
