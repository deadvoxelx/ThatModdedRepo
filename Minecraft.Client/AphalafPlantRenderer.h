#pragma once
#include "MobRenderer.h"

class AphalafPlantRenderer : public MobRenderer
{
private:
	static ResourceLocation APHALAF_PLANT_LOCATION;

public:
	AphalafPlantRenderer();
    virtual void render(shared_ptr<Entity> entity, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
    virtual void scale(shared_ptr<LivingEntity> mob, float a);
};