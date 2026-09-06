#pragma once
#include "MobRenderer.h"

class NusaSpikeRenderer : public MobRenderer
{
private:
	static ResourceLocation NUSA_SPIKE_LOCATION;

public:
	NusaSpikeRenderer();

	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
};