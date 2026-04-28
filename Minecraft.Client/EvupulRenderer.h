#pragma once
#include "MobRenderer.h"

class EvupulDark;
class EvupulModel;

class EvupulRenderer : public MobRenderer
{
private:
	static ResourceLocation EVUPULDARK_LOCATION;
	static ResourceLocation EVUPULDARK_OVERLAY_LOCATION;

public:
	EvupulRenderer();
	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
	virtual int prepareArmor(shared_ptr<LivingEntity> _evupulDark, int layer, float a);
};