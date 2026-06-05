#pragma once
#include "MobRenderer.h"

class FlyingCowRenderer : public MobRenderer
{
private:
	static ResourceLocation FLYINGCOW_LOCATION;

public:
	FlyingCowRenderer(Model *model, float shadow);

	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);

protected:
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
};