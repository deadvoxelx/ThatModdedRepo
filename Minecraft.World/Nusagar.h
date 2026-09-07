#pragma once

using namespace std;

#include "Monster.h"
#include "FlyingMonster.h"
#include "BossMob.h"
#include "SharedConstants.h"

class Player;

class Nusagar : public FlyingMonster, public BossMob
{
private:
	static const int DATA_BOSS_NAME = 20;
	static const int DATA_SPIN_TICKS = 21;
	static const int SPIN_DURATION_TICKS = 10;

	int originPointX;
	int originPointY;
	int originPointZ;
	bool bOriginSet;

	bool spawnLevel1;
	bool spawnLevel2;
	bool spawnLevel3;

	Pos *targetPosition;

	int checkTicks;			// Distance tracking stuff
	double checkDist;		// Distance tracking stuff

	int spikeTimer;
	int spinTicks;

	static int s_aliveCount;

public:
	eINSTANCEOF GetType() { return eTYPE_NUSAGAR; }
	static Entity *create(Level *level){ return new Nusagar(level); }
	static bool isBossFightActive() { return s_aliveCount > 0; }	// For triggering the bossfight music
	static void resetBossFightState() { s_aliveCount = 0; }			// See Minecraft.cpp line 4649

	Nusagar(Level *level);
	~Nusagar();

	virtual bool useNewAi();
	void setBossName(const wstring &name);
	wstring getBossName();
	void setOriginPosition(int x, int y, int z);
	virtual void aiStep();
	virtual bool hurt(DamageSource *source, float dmg);
	bool isSpinning();
	virtual void playerTouch(shared_ptr<Player> player);
	virtual bool isPushable();
	virtual bool canBeAffected(MobEffectInstance *newEffect);

private:
	bool pickTarget();									// Find, create, and set a valid path
	void clampToOrigin(int &tx, int &ty, int &tz);		// Dont traverse too far from the spawnpoint
	bool pathClear(int tx, int ty, int tz);				// Check for a valid path

	void summonLevel1();		// Nuskulls
	void summonLevel2();		// Nuclear Nuskull + Nusa Demon
	void summonLevel3();		// Nuclear Nuskull x2 + Nusa Demon x2

	void spikeLine();			// Spawns a line of Nusa Spikes in the direction of the nearest player
	void spikeLines();			// 8 Spike lines in 8 directions (4 straight out, 4 diagonal)
	void spikeLineInDirection(shared_ptr<Player> player, double dirX, double dirZ, double lineReach, double spacing);	// Spawns a line of Nusa Spikes

protected:
	virtual void defineSynchedData();
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual bool removeWhenFarAway();
	virtual bool isDealsDamage();
	virtual int getAttackDamage();
	virtual void doPush(shared_ptr<Entity> e);
    virtual void pushEntities();
	virtual int decreaseAirSupply(int currentSupply);
	virtual void causeFallDamage(float distance);
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);
	virtual int getHurtSound();
	virtual int getDeathSound();

public:
	virtual float getMaxHealth() { return FlyingMonster::getMaxHealth(); };
	virtual float getHealth() { return FlyingMonster::getHealth(); };
	virtual wstring getAName() { return app.GetString(IDS_NUSAGAR); };
	virtual int getDimension() { return Entity::dimension; };
};
