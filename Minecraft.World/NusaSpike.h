#pragma once

#include "AmbientCreature.h"

class Level;

class NusaSpike : public AmbientCreature
{
public:
	eINSTANCEOF GetType() { return eTYPE_NUSA_SPIKE; }
	static Entity *create(Level *level) { return new NusaSpike(level); }

	NusaSpike(Level *level);

	virtual bool isPushable();
	virtual void tick();
	virtual bool hurt(DamageSource *source, float dmg);
	virtual void playerTouch(shared_ptr<Player> player);

protected:
	virtual void defineSynchedData(); 
	virtual void doPush(shared_ptr<Entity> e);
	virtual void pushEntities();
	virtual void registerAttributes();
	virtual bool useNewAi();
	virtual void newServerAiStep();
	virtual bool makeStepSound();
	virtual void causeFallDamage(float distance);
	virtual void checkFallDamage(double ya, bool onGround);
	virtual bool isIgnoringTileTriggers();
	virtual bool isDealsDamage();
	virtual int getAttackDamage();

private:
	int spikeLifetime;
};