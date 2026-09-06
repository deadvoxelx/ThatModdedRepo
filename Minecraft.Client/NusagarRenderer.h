#pragma once
#include "MobRenderer.h"

class NusagarModel;

class NusagarRenderer : public MobRenderer
{
private:
	static ResourceLocation NUSAGAR_LOCATION;
	int modelVersion;

public:
	NusagarRenderer();
	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
	virtual void setupPosition(shared_ptr<LivingEntity> mob, double x, double y, double z);
};