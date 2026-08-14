#pragma once
using namespace std;

#include "Entity.h"
#include "Projectile.h"

class Level;
class CompoundTag;

class DartPoison : public Entity, public Projectile
{
public:
	eINSTANCEOF GetType() {	return eTYPE_DARTPOISON; }
	static Entity *create(Level *level) { return new DartPoison(level); }

	static const int PICKUP_DISALLOWED = 0;
	static const int PICKUP_ALLOWED = 1;
	static const int PICKUP_CREATIVE_ONLY = 2;

private:
	static const double DART_BASE_DAMAGE;
	static const int ID_FLAGS = 16;
	static const int FLAG_CRIT = 1;

	int xTile;
	int yTile;
	int zTile;
	int lastTile;
	int lastData;
	bool inGround;
	double baseDamage;
	int knockback;
	int life;
	int flightTime;

	void _init();

public:
	int pickup;
	int shakeTime;
	shared_ptr<Entity> owner;

	DartPoison(Level *level);
	DartPoison(Level *level, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> target, float power, float uncertainty);
	DartPoison(Level *level, double x, double y, double z);
	DartPoison(Level *level, shared_ptr<LivingEntity> mob, float power);

	void shoot(double xd, double yd, double zd, float pow, float uncertainty);
	virtual void lerpTo(double x, double y, double z, float yRot, float xRot, int steps);
	virtual void lerpMotion(double xd, double yd, double zd);
	virtual void tick();
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);
	virtual void playerTouch(shared_ptr<Player> player);

protected:
	virtual void defineSynchedData();
	virtual bool makeStepSound();

public:
	virtual float getShadowHeightOffs();
	void setBaseDamage(double baseDamage);
	double getBaseDamage();
	void setKnockback(int knockback);
	virtual bool isAttackable();
	void setCritDartPoison(bool critDart);
	bool isCritDartPoison();
};