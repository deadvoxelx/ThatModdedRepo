#pragma once

#include "Fireball.h"

class ZephyrFireball : public Fireball
{
public:
	eINSTANCEOF GetType() { return eTYPE_ZEPHYRBALL; }
	static Entity *create(Level *level) { return new ZephyrFireball(level); }

public:
	ZephyrFireball(Level *level);
	ZephyrFireball(Level *level, double x, double y, double z, double xa, double ya, double za);
	ZephyrFireball(Level *level, shared_ptr<LivingEntity> mob, double xa, double ya, double za);
	virtual bool isPickable();

protected:
	void onHit(HitResult *res);
	virtual bool shouldBurn();

public:
	void addAdditonalSaveData(CompoundTag *tag);
	void readAdditionalSaveData(CompoundTag *tag);
};