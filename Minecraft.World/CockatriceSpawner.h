#pragma once

#include "AmbientCreature.h"

class Level;

class CockatriceSpawner : public AmbientCreature
{
public:
	eINSTANCEOF GetType() { return eTYPE_COCKATRICESPAWNER; }
	static Entity *create(Level *level) { return new CockatriceSpawner(level); }

	CockatriceSpawner(Level *level);

	virtual bool isPushable();
	virtual void tick();
	virtual bool hurt(DamageSource *source, float dmg);

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
};