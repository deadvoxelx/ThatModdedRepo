#pragma once
#include "MobRenderer.h"

class NusaDemonModel;

class NusaDemonRenderer : public MobRenderer
{
private:
	static ResourceLocation NUSA_DEMON_LOCATION;
	static ResourceLocation NUSA_DEMON_OVERLAY_LOCATION;
	int modelVersion;

public:
	NusaDemonRenderer();
	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
	virtual void setupPosition(shared_ptr<LivingEntity> mob, double x, double y, double z);

protected:
	virtual int prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a);
};