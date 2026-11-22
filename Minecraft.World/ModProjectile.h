#pragma once

#include "Throwable.h"

class ModProjectile : public Throwable
{
public:
	eINSTANCEOF GetType() { return eTYPE_MODPROJECTILE; }
	static Entity *create(Level *level) { return new ModProjectile(level); }

	ModProjectile(Level *level);
	ModProjectile(Level *level, shared_ptr<LivingEntity> mob);
	ModProjectile(Level *level, shared_ptr<LivingEntity> mob, int specId);
	ModProjectile(Level *level, double x, double y, double z);

	int getSpecId() const { return specId; }
	void setSpecId(int id) { specId = id; }

protected:
	virtual float getGravity();
	virtual void tick();
	virtual void onHit(HitResult *res);
	virtual void addAdditonalSaveData(CompoundTag *tag);
	virtual void readAdditionalSaveData(CompoundTag *tag);

private:
	int specId;
};
