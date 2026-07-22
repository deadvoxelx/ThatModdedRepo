#pragma once
#include "MobRenderer.h"

class Nuskull;
//class NuclearNuskull;
class WatcherModel;

class NuskullRenderer : public MobRenderer
{
private:
	static ResourceLocation NUSKULL_LOCATION;
	static ResourceLocation NUSKULL_OVERLAY_LOCATION;
	static ResourceLocation NUSKULL2_LOCATION;
	static ResourceLocation NUSKULL2_OVERLAY_LOCATION;
	static ResourceLocation NUCLEAR_LOCATION;
	static ResourceLocation NUCLEAR_OVERLAY_LOCATION;

public:
	NuskullRenderer();
	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
 	virtual void scale(shared_ptr<LivingEntity> mob, float a);
	virtual int prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a);
};