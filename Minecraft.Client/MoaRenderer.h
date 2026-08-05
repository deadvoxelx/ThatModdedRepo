#pragma once
#include "MobRenderer.h"

class Cockatrice;
class Moa;
class MoaModel;

class MoaRenderer : public MobRenderer
{
private:
	static ResourceLocation COCKATRICE_LOCATION;
	static ResourceLocation MOA_LOCATION;
	static ResourceLocation MOA_WHITE_LOCATION;
	static ResourceLocation MOA_BLACK_LOCATION;

public:
	MoaRenderer();
	virtual void render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);

protected:
	virtual void scale(shared_ptr<LivingEntity> mob, float a);
	virtual float getBob(shared_ptr<LivingEntity> _mob, float a);
	//virtual int prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a);
};
