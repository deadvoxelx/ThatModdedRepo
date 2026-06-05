#pragma once
#include "MobRenderer.h"

class SheepuffRenderer : public MobRenderer
{
private:
	static ResourceLocation SHEEPUFF_LOCATION;
	static ResourceLocation SHEEPUFF_FUR_LOCATION;

public:
	SheepuffRenderer(Model *model, Model *armor, float shadow);

protected:
	virtual int prepareArmor(shared_ptr<LivingEntity> _sheepuff, int layer, float a);

public:
	virtual void render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
};