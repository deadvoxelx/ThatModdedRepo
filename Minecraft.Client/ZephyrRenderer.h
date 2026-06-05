#pragma once
#include "MobRenderer.h"

class ZephyrRenderer : public MobRenderer
{
private:
	static ResourceLocation ZEPHYR_LOCATION;

public:
	ZephyrRenderer();	

protected:
	virtual void scale(shared_ptr<LivingEntity> mob, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
};