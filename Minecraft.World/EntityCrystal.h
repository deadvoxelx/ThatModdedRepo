#pragma once
using namespace std;

#include "Entity.h"

class Level;
class CompoundTag;
class LivingEntity;
class Player;

enum eCrystalType
{
	eCrystalType_FIRE = 0,
	eCrystalType_ICE = 1,
	eCrystalType_CLOUD = 2,
	eCrystalType_THUNDER = 3,
};

class EntityCrystal : public Entity
{
public:
	eINSTANCEOF GetType() { return eTYPE_CRYSTAL; }
	static Entity *create(Level *level) { return new EntityCrystal(level); }

	EntityCrystal(Level *level);
	EntityCrystal(Level *level, double x, double y, double z, eCrystalType type);
	EntityCrystal(Level *level, double x, double y, double z, shared_ptr<LivingEntity> target);

	virtual void tick();
	virtual bool hurt(DamageSource *source, float damage);
	virtual bool isPickable();
	virtual bool isAttackable();
	virtual bool isShootable();
	virtual void playerTouch(shared_ptr<Player> player);

	eCrystalType getCrystalType();
	bool wasHit();
	void setShootingEntity(shared_ptr<Entity> owner);
	void moveTowardsTarget(shared_ptr<Entity> target, double speed);
	int maxTicksAlive();

	double getSMotionX() { return smotionX; }
	double getSMotionY() { return smotionY; }
	double getSMotionZ() { return smotionZ; }
	void setSMotion(double x, double y, double z);

	virtual void lerpMotion(double xd, double yd, double zd);
	virtual void lerpTo(double x, double y, double z, float yRot, float xRot, int steps);

protected:
	virtual bool makeStepSound();
	virtual void defineSynchedData();
	virtual float getShadowHeightOffs();
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);

private:
	eCrystalType type;
	shared_ptr<Entity> shootingEntity;
	shared_ptr<LivingEntity> targetEntity;
	float sinage[3];
	double smotionX;
	double smotionY;
	double smotionZ;
	bool _wasHit;
	int life;

	double lx, ly, lz;
	float lyr, lxr;
	int lSteps;

	void _init();
	void onHitEntity(shared_ptr<Entity> entity);
	void explode();
	void expire();
};
