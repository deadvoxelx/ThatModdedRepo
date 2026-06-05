#include "stdafx.h"
#include "ModelPart.h"
#include "FlyingCowRenderer.h"
#include "TextureAtlas.h"
#include "QuadrupedModel.h"
#include "FlyingCowModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.h"

ResourceLocation FlyingCowRenderer::FLYINGCOW_LOCATION = ResourceLocation(TN_MOB_FLYINGCOW);

FlyingCowRenderer::FlyingCowRenderer(Model *model, float shadow) : MobRenderer(model, shadow)
{
}

void FlyingCowRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *FlyingCowRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &FLYINGCOW_LOCATION;
}