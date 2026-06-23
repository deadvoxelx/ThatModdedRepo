#pragma once

using namespace std;

#include "Enemy.h"

class Player;
class RangedAttackGoal;

class Cockatrice : public Monster, public RangedAttackMob
{
private:	
	void _init();

public:
	eINSTANCEOF GetType() { return eTYPE_COCKATRICE; }
	static Entity *create(Level *level) { return new Cockatrice(level); }

	float flap;
    float flapSpeed;
    float oFlapSpeed, oFlap;
    float flapping;

	Cockatrice(Level *level);
	virtual bool useNewAi();
	virtual void aiStep();
	virtual void performRangedAttack(shared_ptr<LivingEntity> target, float power);
	virtual int getDeathLoot();
	virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

protected:
	virtual void registerAttributes();
	virtual void newServerAiStep();
	virtual void causeFallDamage(float distance);
};
