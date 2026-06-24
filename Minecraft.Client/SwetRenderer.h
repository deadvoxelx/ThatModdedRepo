#pragma once
#include "MobRenderer.h"

class SwetRenderer : public MobRenderer
{
private:
	Model *armor;
	static ResourceLocation SWET_LOCATION;
	static ResourceLocation SWET_GOLD_LOCATION;

public:
	SwetRenderer(Model *model, Model *armor, float shadow);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
	virtual int prepareArmor(shared_ptr<LivingEntity> _swet, int layer, float a);
    virtual void scale(shared_ptr<LivingEntity> _swet, float a);
};