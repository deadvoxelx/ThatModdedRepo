#pragma once
#include "EntityRenderer.h"

class Model;
class EntityCrystal;

class CrystalRenderer : public EntityRenderer
{
private:
	Model *model;
	static ResourceLocation FIRE_CRYSTAL_LOCATION;
	static ResourceLocation ICE_CRYSTAL_LOCATION;
	static ResourceLocation CLOUD_CRYSTAL_LOCATION;
	static ResourceLocation THUNDER_CRYSTAL_LOCATION;

	ResourceLocation *getOverlayLocation(shared_ptr<EntityCrystal> crystal);

public:
	CrystalRenderer();

	virtual void render(shared_ptr<Entity> _crystal, double x, double y, double z, float rot, float a);
	virtual ResourceLocation *getTextureLocation(shared_ptr<Entity> mob);
};
