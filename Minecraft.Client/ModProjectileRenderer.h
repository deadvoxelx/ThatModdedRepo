#pragma once
#include "ItemSpriteRenderer.h"

class ModProjectileRenderer : public ItemSpriteRenderer
{
public:
	ModProjectileRenderer();
	virtual void render(shared_ptr<Entity> e, double x, double y, double z, float rot, float a);
};
