#pragma once

using namespace std;

#include "Monster.h"
#include "BossMob.h"
#include "SharedConstants.h"

class Player;
class EntityCrystal;

class SunSpirit : public Monster, public BossMob
{
private:
	int originPointX;
	int originPointY;
	int originPointZ;
	int direction;
	int motionTimer;
	int flameCount;
	int ballCount;
	int chatCount;
	double rotary;
	double velocity;

	static const int DATA_CHAT_LINE = 18;
	static const int DATA_FREEZING = 19;
	static const int DATA_BOSS_NAME = 20;

public:
	eINSTANCEOF GetType() { return eTYPE_SUN_SPIRIT; }
	static Entity *create(Level *level){ return new SunSpirit(level); }

	SunSpirit(Level *level);

	virtual bool useNewAi();
	virtual void aiStep();
	virtual void newServerAiStep();
	virtual void tick();
	virtual void travel(float xa, float ya);
	virtual bool isPushable();
	virtual void doPush(shared_ptr<Entity> e);
	virtual void pushEntities();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual bool interact(shared_ptr<Player> player);
	virtual bool canBeAffected(MobEffectInstance *newEffect);
	virtual void die(DamageSource *source);

protected:
	virtual void defineSynchedData();
	virtual void registerAttributes();
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);
	virtual bool removeWhenFarAway();
	virtual void causeFallDamage(float distance);
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);

public:
	void setOriginPosition(int x, int y, int z);
	int getChatLine();
	void setChatLine(int lineNumber);
	bool isFreezing();
	void setFreezing(bool isFreezing);
	void setBossName(const wstring &name);
	wstring getBossName();
	bool chatWithMe(shared_ptr<Player> player);

private:
	void chatLine(shared_ptr<Player> player, int messageId);
	void burnEntities();
	void evapWater();
	void makeFireBall(int count);
	void summonFire();
	void resetAfterPlayerDeath(shared_ptr<Player> dungeonTarget);

public:
	virtual float getMaxHealth() { return Monster::getMaxHealth(); };
	virtual float getHealth() { return Monster::getHealth(); };
	virtual wstring getAName() { return app.GetString(IDS_SUN_SPIRIT); };
	virtual int getDimension() { return Entity::dimension; };
};
