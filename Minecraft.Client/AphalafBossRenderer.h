#pragma once
#include "MobRenderer.h"

class AphalafBoss;
class LivingEntity;

class AphalafBossRenderer : public MobRenderer
{
private:
	static ResourceLocation APHALAF_BOSS_LOCATION;
	int modelVersion;

public:
	AphalafBossRenderer();
    virtual void render(shared_ptr<Entity> entity, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> entity);

protected:
    virtual void scale(shared_ptr<LivingEntity> mob, float a);
};