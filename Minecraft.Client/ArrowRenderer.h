#pragma once
#include "EntityRenderer.h"

class ArrowRenderer : public EntityRenderer
{
private:
	static ResourceLocation ARROW_LOCATION;
	static ResourceLocation DART_LOCATION;
	static ResourceLocation DART_ENCHANTED_LOCATION;

public:
	virtual void render(shared_ptr<Entity> _entity, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> entity);
};
