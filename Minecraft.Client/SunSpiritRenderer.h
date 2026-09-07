#pragma once
#include "MobRenderer.h"

class FireMinion;

class SunSpiritRenderer : public MobRenderer
{
private:
	static ResourceLocation SUN_SPIRIT_LOCATION;
	static ResourceLocation SUN_SPIRIT_FROZEN_LOCATION;

public:
	SunSpiritRenderer();

	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
	virtual void scale(shared_ptr<LivingEntity> mob, float a);
};
