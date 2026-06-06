#pragma once
#include "MobRenderer.h"

class AerwhaleRenderer : public MobRenderer
{
private:
	static ResourceLocation AERWHALE_LOCATION;

public:
	AerwhaleRenderer();

	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
	virtual void scale(shared_ptr<LivingEntity> mob, float a);
};